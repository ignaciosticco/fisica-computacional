#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P     16             // 1/2^P, P=16
#define Z     30000          // iteraciones
#define N     30             // lado de la red simulada

//Declaro las funciones que voy a usar
void llenar(int *red, float prob, int n);
void imprimir(int *red, int n);
void escribir(double z,double a);
double valor_medio(double z,int n);
double dispersion(double z,double x,int n);
int   hoshen(int *red,int n);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red,int n);


//Función principal
int main(int argc,char *argv[]) //Via terminal le tengo que pasar el valor de cada argumento
{
  int    i,j,*red,n,z;
  float  prob,denominador,media,desviacion;
  double prob_acum,prob_cuadr_acum;

  srand(time(NULL));
  n=N;
  z=Z;

  prob_acum=0.0;
  prob_cuadr_acum=0.0;

  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n); //primer argumento: lado de la red
       sscanf(argv[2],"%d",&z); //segundo argumento: número de iteraciones
     }
    
  red=(int *)malloc(n*n*sizeof(int));
   
  for(i=0;i<z;i++) //hago z iteraciones, es decir me "muevo" a través de la red infinita
  {
  
      prob=0.5;
      denominador=2.0;
 
      //srand(time(NULL));

      for(j=0;j<P;j++)
        {
          llenar(red,prob,n); //lleno la matriz
	  //imprimir(red,n);    //imprimo la matriz
	  //printf("\n\n");
      
          hoshen(red,n);      //transformo la matriz
	  //imprimir(red,n);    //imprimo nuevamente la matriz
	  //printf("\n\n");

          denominador=2.0*denominador;

          if (percola(red,n)) prob+=(-1.0/denominador); 
          else prob+=(1.0/denominador);     
	}
	prob_acum=prob_acum+(double)prob;
	prob_cuadr_acum=prob_cuadr_acum+(double)prob*(double)prob;
  }
//  printf("\n\nEl valor de pc para un red cuadrada de lado %i es: %f\n",n,prob); //imprimo el valor de la probabilidad crítica
  media=valor_medio(prob_acum,z);
  desviacion=dispersion(prob_acum,prob_cuadr_acum,z);
  escribir(media,desviacion);


  //imprimir(red,n);	
  free(red);

  return 0;
}


//Subfunciones


double valor_medio(double z,int n)
{
	double m;
	m=(double)z/n;
	return m;
}


double dispersion(double z, double x,int n)
{
	double m,d,k;
	m=z/(double)n; //media de p
	d=x/(double)n; //media de p cuadrado
	k=sqrt(d-m*m);
	return k;
}




int hoshen(int *red,int n)
{
  /*
    Esta funcion implementa en algoritmo de Hoshen-Kopelman.
    Recibe el puntero que apunta a la red y asigna etiquetas 
    a cada fragmento de red. 
  */

  int i,j,k,s1,s2,frag;
  int *clase;

  frag=0;
  
  clase=(int *)malloc(n*n*sizeof(int));

  for(k=0;k<n*n;k++) *(clase+k)=frag; //me voy moviendo de ubicación con *(clase+k)
  
  // primer elemento de la red

  s1=0;
  frag=2; 	
  if (*red) frag=actualizar(red,clase,s1,frag); //if (*red) equivale a if(red[0]=1), es decir el primer elemento de la red
  
  // primera fila de la red

  for(i=1;i<n;i++) 
    {
      if (*(red+i)) //equivale a if(red[i]=1)
         {
           s1=*(red+i-1); //me fijo el valor de la posicion anterior 
           frag=actualizar(red+i,clase,s1,frag);
         }
    }
  

  // el resto de las filas 

  for(i=n;i<n*n;i=i+n)// solamente me ubico es las posiciones iniciales de cada fila, por eso sumo n
    {

      // primer elemento de cada fila

      if (*(red+i)) //me fijo si es distinto de cero. 
         {
           s1=*(red+i-n); //como es distinto de cero, me fijo el valor del que se ubica por encima de este.
           frag=actualizar(red+i,clase,s1,frag);
         }

      for(j=1;j<n;j++) //partiendo desde la segunda posición de cada fila, voy barriendo las posiciones para cada fila
	if (*(red+i+j)) //me fijo si es distinto de cero.
	  {		//como estoy parado en un lugar diferente a la primer ubicacion, me puedo fijar los valores de la anterior ubicación y la de arriba. 
	    s1=*(red+i+j-1); //valor de la anterior posición
            s2=*(red+i+j-n); //valor de la posición superior (fila anterior)

	    if (s1*s2>0) //pregunto si las dos son distintas de cero ("conflicto de etiquetas")
	      {
		etiqueta_falsa(red+i+j,clase,s1,s2); //solamente crea una etiqueta falsa si a la izquierda y por encima de la posicion actual hay etiquetas distintas de cero
	      }
	    else //alguna de ellas vale cero ("actualizo etiqueta")
	      { if (s1!=0) frag=actualizar(red+i+j,clase,s1,frag); //la anterior es distinta de cero (por eso va s1)
	        else       frag=actualizar(red+i+j,clase,s2,frag); //la superior es distinta de cero (por eso va s2)
	      }
	  }
    }


  corregir_etiqueta(red,clase,n);

  free(clase);

  return 0;
}


/*
  // primera fila de la red

  for(i=1;i<n;i++) 
    {
      if (*(red+i)) //equivale a if(red[i]=1)
         {
           s1=*(red+i-1); //me fijo el valor de la posicion anterior 
           frag=actualizar(red+i,clase,s1,frag);
         }
    }
Primera fila: Me paro en una ubicacion. 
1) Si es cero, no hago nada. (no entra al if)
2) Si es distinta de cero, me fijo qué pasa con el valor anterior.
	i) Si es cero, actualizo la etiqueta luego de escribir la anterior en la posicion actual. 
     	ii) Si es distinta de cero, no actualizo la etiqueta y la copio en la posicion actual.
*/



int   actualizar(int *red,int *clase,int s,int frag)
{
	while(clase[s]<0) //hallo la etiqueta verdadera
	{
		s=-clase[s]; 
	}	
	if(s!=0) //el anterior o el de arriba es distinto de cero
	{
		*red=clase[s];
		clase[s]=s;
	}
	else
	{
		*red=frag;
		clase[frag]=frag;
		frag++;
	}	
	
return frag; //devuelve el valor de frag. 
}


//etiqueta_falsa(red+i+j,clase,s1,s2);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2)
{
	while (clase[s1]<0) 
	{
		s1=-clase[s1];//me muevo hasta que encuentro el verdadero valor de s1 (el menos lo coloco porque el argumento de clase es positivo)
	}
	while (clase[s2]<0) 
	{
		s2=-clase[s2];//me muevo hasta que encuentro el verdadero valor de s2
	}

//en este momento, ya encontré el verdadero valor de s1 ó s2
	if(s1==s2)
	{
		*red=s1;
		clase[s1]=s1;	
	}

	if (s2<s1) 
	{
		clase[s1]=-s2; 
		clase[s2]=s2;
		*red=s2;
	}
/*Por ejemplo, si tengo   3
		     	5 1, 
entoces coloco -3 en la posición 5 del vector de clases y al 1 le pongo -3; Quizás el 3 sea falso, es decir que clase[3] sea negativo. Si es así, es que proviene de otro valor. Debo seguir el "caminito" hasta llegar al verdadero valor de s2
*/
	if (s1<s2)
	{
		clase[s2]=-s1;
		clase[s1]=s1;
		*red=s1;
	}

}



void  corregir_etiqueta(int *red,int *clase,int n)
{
	int i,s;
	for (i=0; i<n*n;i++)
	{
		s=*(red+i);
		while(clase[s]<0)
		{
			s=-clase[s];
			*(red+i)=s;
		}
	} 
}


int   percola(int *red,int n)
{
	int i,j,p;
	int f1[n];
	int fn[n];
	p=0;
	for(i=0;i<n;i++) 
	{
		f1[i]=red[i];
		fn[i]=red[i+n*(n-1)+1];
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if (f1[i]==fn[j] && f1[i]!=0)	
			{
				p=1;
				//break
			}
		}
	}
//printf("%i",p);
return p;
}



void llenar(int *red, float prob, int n) 
{
	int i;
	//srand(time(0));
	for (i = 0; i < n*n; i++){
		double e;
		e = (double)rand()/RAND_MAX;
		if (e <= prob){
			//*a = 1; no conviene usarlo porque si uso después a la variable a, por ejemplo, si la imprimo me va a dar a[final+1] entonces voy a obtener cualquier cosa. Conviene que me vaya moviendo yo con los indices pero que no cambie la posición de a. Es decir, si tengo que a=[1 2 4], si al finalizar el for me quedo en la direccion o indice 3, la proxima vez que imprima o use la veriable "a" voy a obtener 4.  
			red[i]=1;
		}
		else {
			//*a = 0; idem arriba
			red[i]=0;
		}
		//a++;
	}
}


void imprimir(int *red, int n) 
{
	int i,j;
	j = 0;
	printf("\n\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (red[j+i*n]<10) printf("%i  ",red[j+i*n]);
			else printf("%i ",red[j+i*n]);
		}
	printf("\n");
	}
	printf("\n\n");
}


void escribir(double z, double a)
{

	FILE *fp;
	fp=fopen("perco_bi.txt","a");
	fprintf(fp,"%g %g\n",z,a);
	fclose(fp);
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P     16         // P=cantidad de veces que hace biseccion -  1/2^P, P=16  
#define Z     27000         // iteraciones
#define N     4          // lado de la red simulada

void  llenar(int *red,int n,float proba);
int   hoshen(int *red,int n);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red, int n);
void  histo_fperco(float max,int *red,int n,float sampleo,float *vector_proba,float *distribucion);
void  escribir(float *vector1,float *vector2,int max);
int   cluster_infinito(int etiqueta, int* vector);
void  calcula_ns(int *red,int n,int* vector);
void  imprimir(int *red, int n);
void  resultado1a (int vector1[],double vector2[],double vector3[], int max);




int main(int argc,char *argv[])
{
  int    i,j,*red,n,z,k;
  float  prob,denominador;
  double  p_med = 0.0;
  double  p_cuadrado = 0.0;
  int    size_max=10;          // cantidad de probabilidades 
  float  distribucion[size_max];
  float  vector_proba[size_max];
  int    L[1]={32};	//tamaño de red
  double  proba_cr[1];		//bisección 1a
  double  varianza[1];		//bisección 1a
    
  
  n=N;
  z=Z;
 
  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%d",&z);
     }
    
  
   //red=(int *)malloc(n*n*sizeof(int));
  
  for (k=0;k<1;k++)
    {
  
    n = L[k];
    p_med=0;
    p_cuadrado=0;

	
    red=(int *)malloc(n*n*sizeof(int));
    srand(time(NULL));

    for(i=0;i<z;i++)
     {
      prob=0.5;
      denominador=2.0;    
      
      for(j=0;j<P;j++)
        {
          printf("%f\n",prob);
          //srand(time(NULL));
          llenar(red,n,prob);
          //imprimir(red,n);
          hoshen(red,n);
          //printf("\n");
          //imprimir(red,n);
          
          denominador=2.0*denominador;
          printf("%i\n",percola(red,n));
          if (percola(red,n)!=0){ 
             prob+=(-1.0/denominador); 
            
             }
          else {
            prob+=(1.0/denominador);
            //printf("%f\n",prob);
          }

        }
        p_med+=(double)prob/(double)z;
        p_cuadrado+=(double)prob*(double)prob/(double)z;
      }


     proba_cr[k] = p_med;
     p_cuadrado = p_cuadrado;
     varianza[k] = p_cuadrado - (p_med*p_med);
     free(red);

 }

  resultado1a(L,proba_cr,varianza,1);

  //histo_fperco(10.0,red,n,30,distribucion,vector_proba);

  /*
  printf("\n");
  //Calculos para el vector ns:
  int* vector;
  vector=(int *)malloc(n*n*sizeof(int));  
  for(i=0;i<n*n;i++){
    vector[i]=0;
  } 
  calcula_ns(red,n,vector);
  // fin de los calculos del vector ns

  */
  //free(red);


  return 0;
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

  for(k=0;k<n*n;k++) *(clase+k)=frag;
  
  // primer elemento de la red

  s1=0;
  frag=2;
  if (*red) frag=actualizar(red,clase,s1,frag);

  
  // primera fila de la red

  for(i=1;i<n;i++) 
    {
      if (*(red+i)) 
         {
           s1=*(red+i-1);  
           frag=actualizar(red+i,clase,s1,frag);

         }
    }
  

  // el resto de las filas 

  for(i=n;i<n*n;i=i+n)
    {

      // primer elemento de cada fila

      if (*(red+i)) 
         {
           s1=*(red+i-n); 
           frag=actualizar(red+i,clase,s1,frag);

         }

      for(j=1;j<n;j++)
	if (*(red+i+j))
	  {
	    s1=*(red+i+j-1); 
      s2=*(red+i+j-n);

	    if (s1*s2>0)   // Si esta en el caso 4 de la clase de guille
	      {
		etiqueta_falsa(red+i+j,clase,s1,s2);
	      }
	    else 
	      { if (s1!=0) frag=actualizar(red+i+j,clase,s1,frag);
	        else       frag=actualizar(red+i+j,clase,s2,frag);
	      }

	  }
    }


  corregir_etiqueta(red,clase,n);
  
  free(clase);

  return 0;
}


void llenar(int* red,int n,float proba){
	int i,j;
	//srand(time(NULL));  
	for(i=0;i<n;i=i+1){
		for(j=0;j<n;j=j+1){
			int r = rand()%10; // random de 0 a 9
			if(r<proba*10){
				red[j+n*i]=1;
			}
			else{
				red[j+n*i]=0;
			}
			
		}
	}
}

void imprimir(int *red, int n){

	// Imprimo en pantalla la matriz///////
	int i,j;
	for(i=0;i<n;i=i+1){
		for(j=0;j<n;j=j+1){
			printf("%d    ",red[j+n*i]);
		}
		printf("\n");
	}
}

int  actualizar(int *red,int *clase,int s,int frag){
  
  while(clase[s]<0){
      s=-clase[s];
    }   

  if(s!=0){
    *red=clase[s];
    clase[s]=s;

  }
  else{
    *red=frag;
    clase[frag]=frag;
    frag++;
  }
  return frag;
}


void  etiqueta_falsa(int *red,int *clase,int s1,int s2){
	int i,n;	
	 
		while(clase[s1]<0){
			s1=-clase[s1];
		}		
		while(clase[s2]<0){
			s2=-clase[s2];
		}
    if(s1==s2){
     *red=s1; 
     clase[s1]=s1;
    }
		if(s1<s2){
			clase[s2]=-s1;
      clase[s1]=s1;
			*red=s1;
		}
    if(s2<s1){
			clase[s1]=-s2;
      clase[s2]=s2;
			*red=s2;
		}
}


void  corregir_etiqueta(int *red,int *clase,int n){
	int i,s;
 
	for(i=0;i<n*n;i++)
  { 
		s=*(red+i);
		while(clase[s]<0){
			s=-clase[s];
      *(red+i)=s;
		}
	}
  
}

int percola (int *red, int n){
	int i,j,s1,s2;
	
	for(i=0;i<n;i++){
		s1=*(red+i);
		if (s1>0){
			for(j=0;j<n;j++){
				s2=*(red+j+(n-1)*n);
		
				if(s1==s2){
					return s1;
				}
								
			}
		}
	}
	return 0;
}	


void histo_fperco(float max,int *red,int n,float sampleo,float *vector_proba,float *distribucion){
  int   iterador,i;
  float prob=0;

  srand(time(NULL));
  for(iterador=0;iterador<max;iterador++){
    
    int contador=0;
    prob = iterador/max;    
    for(i=0;i<sampleo;i++){
      llenar(red,n,prob);
      hoshen(red,n);
      if (percola(red,n)) 
               contador+=1; 
           }
    distribucion[iterador] = contador/sampleo;
    vector_proba[iterador] = prob;
    }
  
  //printf("elemento vector_proba= %f\n",vector_proba[4]);
  escribir(vector_proba,distribucion,10);
  //imprimir(red,4); 	
  }

void escribir(float vector1[],float vector2[],int max){
  int i;
  FILE *fp;
  fp = fopen("histograma.txt","w");
  
  for(i=0;i<max;i++){

    fprintf(fp, "%f\t%f\n",vector1[i],vector2[i] );
  }  

  fclose(fp);
}


void calcula_ns(int *red,int n,int* vector){
  int i=0;
  int* vector_ns;
  vector_ns = (int *)malloc(n*n*sizeof(int));
  //imprimir(red,4); 
  for(i=0;i<n*n;i++){       // Inicializo con ceros al vector_ns
    vector_ns[i]=0;
  }
  i=0;
  while(i<n*n){            // Este vector guarda todos los clusters
    vector[*red] = vector[*red] + 1;  // En la coordeanda i guarda la cantidad de nodos con etiqueta i.
    red=red+1;
    i = i+1;
  }
  printf("Vector: \n");
  //imprimir(vector,n); 
  i=0;
  vector=vector+1;      // Empiezo desde el segundo elemento de vector para no contar los clusters de size cero.
  while(i<n*n){         //vector_ns guarda la cantidad de clusters (n) de tamaño s
    if(*vector!=0){     // en la coordenada i guarda la cantidad de clusters de tamaño i.
      vector_ns[*vector] = vector_ns[*vector]+1;
      }
    vector = vector+1;  
    i = i+1;
  } 
  printf("Vector_ns: \n");
  //imprimir(vector_ns,n); 
}


int cluster_infinito(int etiqueta, int* vector){

  if(etiqueta!=0){
    return vector[etiqueta];
    }
  else{
    return 0;
  }
}

void resultado1a (int vector1[],double vector2[],double vector3[], int max){
  int i;
  FILE *fp;
  fp = fopen("resultado1a.txt","w");
  
  for(i=0;i<max;i++){

    fprintf(fp, "%i\t%g\t%g\n",vector1[i],vector2[i],vector3[i] );
  }  

  fclose(fp);
}

  

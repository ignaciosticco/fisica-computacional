#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P     16        // P=cantidad de veces que hace biseccion -  1/2^P, P=16  
#define Z     10      // iteraciones
#define N     4         // lado de la red simulada

void  llenar(int *red,int n,float proba);
int   hoshen(int *red,int n);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red, int n);
void  histo_fperco(float max,int *red,int n,float sampleo,float *vector_proba,float *distribucion);
void  escribir(float *vector1,float *vector2,int max);
int   cluster_infinito(int etiqueta, int* vector);
void  calcula_ns(int *red,int n,int* vector, int* vector_ns);
void  imprimir(int *red, int n);
void  resultado1a (int vector1[],double vector2[],double vector3[], int max);
void iteracion_ns (int n, int z);
//void resultado1d (int max, int n);
void escribir_varios(float prob,int* vector1,int max,char* str1,int j,int n);
//void nombre (int max);
void resultado4(float prob,int* vector1,int max,char* str1,int j,int n, double zeta,double q0,float tau,float sigma);
void f_de_zeta ( int z,int n);
void calculo_nsc  (int n, int z);
void escribir_nscritico (int* vector1, int max);

int main(int argc,char *argv[])
{
  int     i,j,*red,n,z,k,l,max;
  float   prob,denominador;
  double  p_med = 0.0;
  double  p_cuadrado = 0.0;
  int     size_max=10;          // cantidad de probabilidades 
  float   distribucion[size_max];
  float   vector_proba[size_max];
  int     L[5]={4,16,32,64,128};	//tamaño de red
  double  proba_cr[5];		//bisección 1a
  double  varianza[5];		//bisección 1a
  char str1[30];

  srand(time(NULL)); 
  
  n=N;
  z=Z;
  
  /*for (k=0;k<5;k++)
    {
  
    n = L[k];
    p_med=0.0;
    p_cuadrado=0.0;

    red=(int *)malloc(n*n*sizeof(int));

    for(i=0;i<z;i++)
     {
      prob=0.5;
      denominador=2.0;    
      
      for(j=0;j<P;j++)
        {

          llenar(red,n,prob);
          hoshen(red,n);

          denominador=2.0*denominador;
          if (percola(red,n)!=0){ 
             prob+=(-1.0/denominador); 
            
             }
          else {
            prob+=(1.0/denominador);
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

  resultado1a(L,proba_cr,varianza,5);*/

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
   /*for (k=0;k<1;k++)
   {
      n = L[k];
      iteracion_ns (n,z);
    }
      free(red);
*/

//Punto 4
calculo_nsc(64,500);
f_de_zeta(500,64);

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
  double e;  
  for(i=0;i<n;i=i+1){

    for(j=0;j<n;j=j+1){
      e = (double)rand()/RAND_MAX;
      if(e <= proba){
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
  
  escribir(vector_proba,distribucion,10);
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


void calcula_ns(int *red,int n,int* vector,int* vector_ns){
  int i=0;
  /*int* vector_ns;
  vector_ns = (int *)malloc(n*n*sizeof(int));
  /*for(i=0;i<n*n;i++){       // Inicializo con ceros al vector_ns
    vector_ns[i]=0;
  }*/
  i=0;
  while(i<n*n){            // Este vector guarda todos los clusters
    vector[*red] = vector[*red] + 1;  // En la coordeanda i guarda la cantidad de nodos con etiqueta i.
    red=red+1;
    i = i+1;
  }
  //printf("Vector: \n");
  //imprimir(vector,n);
	//printf("\n");  
  i=0;
  vector=vector+1;      // Empiezo desde el segundo elemento de vector para no contar los clusters de size cero.
  while(i<n*n){         //vector_ns guarda la cantidad de clusters (n) de tamaño s
    if(*vector!=0){     // en la coordenada i guarda la cantidad de clusters de tamaño i.
      vector_ns[*vector] = vector_ns[*vector]+1;
      }
    vector = vector+1;  
    i = i+1;
  } 
  //printf("Vector_ns: \n");
  //imprimir(vector_ns,n);
	//printf("\n"); 
	 
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

void iteracion_ns (int n, int z){
 int m,i,j,max;
 int *red;
 float prob;
 char str1[30];
   
    red=(int *)malloc(n*n*sizeof(int));
    
    max=4;
    
    for(j=0;j<max;j++)  // Voy barriendo proba de llenado alrededor de pc
     {  
	prob=0.57;
	prob+=j/100.0;
        printf("%f \n",prob);
	int* vector_ns;
        int* vector;
        vector=(int *)malloc(n*n*sizeof(int));
        vector_ns = (int *)malloc(n*n*sizeof(int));  
        for(m=0;m<n*n;m++){ //inicializo vectores a 0 en cada proba
        //vector[m]=0;
        vector_ns[m]=0;
	
	
         } 
    	
       
     
	 for(i=0;i<z;i++){

		for(m=0;m<n*n;m++){ //inicializo vectores a 0 en cada proba
                 vector[m]=0;
		   }
		llenar(red,n,prob);
		hoshen(red,n);
	 //imprimir(red,n);
	// printf("\n");  
		calcula_ns(red,n,vector,vector_ns);
	  //imprimir(vector_ns,n);
          //printf("\n");
     
	  }
	//imprimir(vector_ns,n);
	sprintf(str1,"resultado1d_%i",j);
		
	escribir_varios(prob,vector_ns, max, str1, j,n);
	
      }	
}




void escribir_varios(float prob,int* vector1,int max,char* str1,int j,int n){
  int i;
  FILE *fp;
  fp = fopen(str1,"w");
  
  for(i=0;i<n*n;i++){

    fprintf(fp, "%f\t%i\t%i\t%i\n",prob,i,vector1[i],n );
  }  

  fclose(fp);
}

void f_de_zeta ( int z,int n){
	int m,i,j,max;
 	int *red;
 	float prob,pc,tau,sigma,pmin,pmax;
 	char str1[30];
   	double q0,s,zeta;
	float paso;

    	red=(int *)malloc(n*n*sizeof(int));
    	
    	max=100;//desde 0.4 hasta 0.7 con un paso de 0.01
        pc=0.592;
	tau=2.057;	
	sigma=0.3956;
	s=0.0;
	pmin=0.32;
	pmax=0.72;
	paso=(pmax-pmin)/(max-1);
	//printf("%f \n",paso);	

	for(j=1;j<n*n;j++){  //Si divido por 0 se va a infinito!
		s+=pow(j,(1-tau));
	}
	//printf("%f \n",s);
	q0=pc/s;
	j=0;	
	//printf("%f \n",q0);
	prob=pmin;	
	
	while(j<max){  // Voy barriendo proba de llenado alrededor de pc
		  
		prob+=paso;
        	//printf("%f \n",prob);
		int* vector_ns;
        	int* vector;
        	vector=(int *)malloc(n*n*sizeof(int));
        	vector_ns = (int *)malloc(n*n*sizeof(int));  
        	
		for(m=0;m<n*n;m++){ //inicializo vectores a 0 en cada proba
        	//vector[m]=0; no inicializo a cero pq se van acumulando en cada iteracion
        	vector_ns[m]=0;
		
	
         	} 
    	
       
     
	 	for(i=0;i<z;i++){

			for(m=0;m<n*n;m++){ //inicializo vector a 0 en cada iteración
                 	vector[m]=0;
			}
		llenar(red,n,prob);
		hoshen(red,n);
	 //imprimir(red,n);
	// printf("\n");  
		calcula_ns(red,n,vector,vector_ns);
	  //imprimir(vector_ns,n);
          //printf("\n");
        
	  	}
	
	//imprimir(vector_ns,n);
	sprintf(str1,"resultado4_%i.txt",j);
		
	resultado4(prob,vector_ns, max, str1, j,n,z,q0,tau,sigma);
	j++;
      }	
}

void resultado4(float prob,int* vector1,int max,char* str1,int j,int n, double zeta,double q0,float tau,float sigma){
  int i;
  float pc=0.592;
  FILE *fp;
  fp = fopen(str1,"w");
  
  for(i=1;i<440;i++){

    fprintf(fp, "%f\t%i\t%i\t%f\t%f\t%f\n",prob,i,vector1[i],q0,pow(i,-tau),pow(i,sigma)*(prob-pc)/pc );

  }
  fclose(fp);
}

void calculo_nsc  (int n, int z){
	
	int m,i,j;
 	int *red;
 	float prob=0.592;  
   	int* vector_ns;
       	int* vector;

	red=(int *)malloc(n*n*sizeof(int));
        vector=(int *)malloc(n*n*sizeof(int));
        vector_ns = (int *)malloc(n*n*sizeof(int));  
        	
	for(m=0;m<n*n;m++){ //inicializo vectores a 0 en cada proba
        	//vector[m]=0; no inicializo a cero pq se van acumulando en cada iteracion
        	vector_ns[m]=0;
	}  	
       
	for(i=0;i<z;i++){

		for(m=0;m<n*n;m++){ //inicializo vector a 0 en cada iteración
                 	vector[m]=0;
		}
		llenar(red,n,prob);
		hoshen(red,n);
	 //imprimir(red,n);
	// printf("\n");  
		calcula_ns(red,n,vector,vector_ns);
	  //imprimir(vector_ns,n);
          //printf("\n");
     
	}
	escribir_nscritico(vector_ns,440);
}


void escribir_nscritico (int* vector1, int max){
  int i;
  FILE *fp;
  fp = fopen("nscritico.txt","w");
  
  for(i=0;i<max;i++){

    fprintf(fp, "%i\t%i\n",vector1[i],i);
  }  

  fclose(fp);
}


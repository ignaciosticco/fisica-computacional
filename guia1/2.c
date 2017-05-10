#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P     16        // P=cantidad de veces que hace biseccion -  1/2^P, P=16  
#define Z     100      // iteraciones
#define N     4        // lado de la red simulada

void  llenar(int *red,int n,float proba);
int   hoshen(int *red,int n);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red, int n);
void  histo_fperco(int total_proba,int *red,int n,float max_sample);
void  escribir(float vector1[],float vector2[],int total_proba);
int   cluster_infinito(int etiqueta, int* vector);
void  calcula_ns(int *red,int n,int* vector);
void  imprimir(int *red, int n);
void  resultado2(int n,int z,float p_min,float p_max,int cant_p);



int main(int argc,char *argv[])
{

  srand(time(NULL)); 
  
 
  //////// PARA el 2:   /////////////
  int   n = 128;                    // tamaño de la red
  float p_min = 0.5;
  float p_max = 0.9;
  int   cant_p = 100; 
  int   z = 30;     
  resultado2(n,z,p_min,p_max,cant_p);
  //////// //////// /////////////

  
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


void histo_fperco(int total_proba,int *red,int n,float max_sample){
  // n= tamaño de la red. 
  int   itera_proba,sample,contador;
  float prob=0;
  float vector_proba [total_proba];
  float distribucion [total_proba];
  float denominador,primera_proba;
  denominador = total_proba*10.0;
  primera_proba = 0.55;

  for(itera_proba=0;itera_proba < total_proba;itera_proba++){
    contador=0;

    prob = primera_proba + itera_proba/denominador;      
    for(sample=0;sample<max_sample;sample++){
      llenar(red,n,prob);
      hoshen(red,n);
      if (percola(red,n)) 
          contador+=1; 
    }
    vector_proba[itera_proba] = prob;
    distribucion[itera_proba] = contador/max_sample;
  }
  
  escribir(vector_proba,distribucion,10);
}

void escribir(float vector1[],float vector2[],int total_proba){
  int i;
  FILE *fp;
  fp = fopen("resultado2_n16_2.txt","w");
  
  for(i=0;i<total_proba;i++){

    fprintf(fp, "%f\t%f\n",vector1[i],vector2[i] );
  }  

  fclose(fp);
}


void calcula_ns(int *red,int n,int* vector){
  int i;
  int* vector_ns;
  vector_ns = (int *)malloc(n*n*sizeof(int));
  for(i=0;i<n*n;i++){       // Inicializo con ceros al vector_ns
    vector_ns[i]=0;
  }
  for(i=0;i<n*n;i++){       // Inicializo con ceros al VECTOR
      vector[i]=0;
  } 
  i=0;
  while(i<n*n){            // Este vector guarda todos los clusters
    vector[*red] = vector[*red] + 1;  // En la coordeanda i guarda la cantidad de nodos con etiqueta i.
    red=red+1;
    i = i+1;
  }

  i=0;
  vector=vector+1;      // Empiezo desde el segundo elemento de vector para no contar los clusters de sizecero.
  while(i<n*n){         //vector_ns guarda la cantidad de clusters (n) de tamaño s
    if(*vector!=0){     // en la coordenada i guarda la cantidad de clusters de tamaño i.
      vector_ns[*vector] = vector_ns[*vector]+1;
      }
    vector = vector+1;  
    i = i+1;
  } 
  free(vector_ns);
}


int cluster_infinito(int etiqueta, int* vector){

  if(etiqueta!=0){
    return vector[etiqueta];
    }
  else{
    return 0;
  }
}


void resultado2(int n,int z,float p_min,float p_max,int cant_p){
  int   sample,etiqueta,masa,i;
  int   vector[n*n];
  float proba[cant_p];
  float fuerza_cluster_percolante[cant_p];
  int*  red;
  float p,densidad;

  float paso = (p_max-p_min)/(cant_p-1);
  p = p_min;  
  red = (int *)malloc(n*n*sizeof(int)); 
  i = 0;
  while(i<cant_p){
    densidad=0.0;   
    for(sample=0;sample<z;sample++){  
        llenar(red,n,p); 
        hoshen(red,n);
        etiqueta=percola(red,n);
        calcula_ns(red,n,vector);
        masa = cluster_infinito(etiqueta,vector); 
        densidad += masa/((float)n*(float)n);   
    }
    proba[i] = p;
    fuerza_cluster_percolante[i]=densidad/(float)z;
    p +=paso;
    i++;   
  }
  escribir(proba,fuerza_cluster_percolante,cant_p);
  free(red);
} 



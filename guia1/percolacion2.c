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
void  calcula_ns(int *red,int n,int* vector,int* vector_ns);
void  imprimir(int *red, int n);
void  resultado1a (int vector1[],double vector2[],double vector3[], int max);
void  resultado2(int n,int cantidad_probas,int z);
void  resultado3(int z);
int   m2(int* vector_ns,int n);
void  resultado6(int n,float p_min,float p_max,float cant_p,int z);



int main(int argc,char *argv[])
{
  int     i,j,*red,n,z,k;
  float   prob,denominador;
  double  p_med = 0.0;
  double  p_cuadrado = 0.0;
  int     size_max=10;          // cantidad de probabilidades 
  float   distribucion[size_max];
  float   vector_proba[size_max];
  //int     L[5]={4,16,32,64,128};  //tamaño de red
  //double  proba_cr[5];    //bisección 1a
  //double  varianza[5];    //bisección 1a
  
  srand(time(NULL)); 
  
  n=N;
  z=Z;
  /*  
  // PARA el 1a:

  for (k=0;k<5;k++)
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
     varianza[k] = sqrt(p_cuadrado - (p_med*p_med));
     free(red);

 }
  //resultado1a(L,proba_cr,varianza,5);
  */

  //////// PARA el 1b:   /////////////
  /*
  n = 4; //tamaño de la red 
  red = (int *)malloc(n*n*sizeof(int));
  z = 27000;
  int total_proba=10;
  histo_fperco(total_proba,red,n,z);
  
  //////// //////// /////////////

  //////// PARA el 2:   /////////////
  n = 4;                    // tamaño de la red
  int cantidad_probas=9;  
  z = 1000;     
  resultado2(n,cantidad_probas,z);
  //////// //////// /////////////
  

  //////// PARA el 3:   /////////////
  z = 100;            // cantidad de iteraciones   
  resultado3(z);
  //////// //////// /////////////
  */

  //////// PARA el 6:   /////////////
  float p_min,p_max;
  int   cant_p;

  z=2;
  p_min = 0.0;
  p_max = 0.9;
  cant_p = 4;
  n=6;
  resultado6(n,p_min,p_max,cant_p,z);

  
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
  fp = fopen("resultado6_.txt","w");
  
  for(i=0;i<total_proba;i++){

    fprintf(fp, "%f\t%f\n",vector1[i],vector2[i] );
  }  

  fclose(fp);
}


void calcula_ns(int *red,int n,int* vector,int* vector_ns){
  int i;
  //int* vector_ns;
  //vector_ns = (int *)malloc(n*n*sizeof(int));
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
  vector=vector+1;      // Empiezo desde el segundo elemento de vector para no contar los clusters de size cero.
  while(i<n*n){         //vector_ns guarda la cantidad de clusters (n) de tamaño s
    if(*vector!=0){     // en la coordenada i guarda la cantidad de clusters de tamaño i.
      vector_ns[*vector] = vector_ns[*vector]+1;
      }
    vector = vector+1;  
    i = i+1;
  } 
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

/////// IMPORTANTE: si se quieren usar la funcion resultado2 o resultado3,
//                 se debe modificar calcula_ns. Se debe quitar el ultimo 
//                 input de la misma. Esto lo hice para usar el vector_ns
//                 en la funcion del ejercicio 6


/*    
void resultado2(int n,int cantidad_probas,int z){
  int   itera_proba,sample,i,etiqueta,masa;
  int   vector[n*n];
  float proba[cantidad_probas];
  float fuerza_cluster_percolante[cantidad_probas];
  int*  red;
  float p,densidad;

  red = (int *)malloc(n*n*sizeof(int)); 

  for (itera_proba=0;itera_proba<cantidad_probas;itera_proba++){
    p = 0.1+itera_proba/10.0;
    densidad=0.0;
    
    for(sample=0;sample<z;sample++){  
        llenar(red,n,p); 
        hoshen(red,n);
        etiqueta=percola(red,n);
        calcula_ns(red,n,vector);
        masa = cluster_infinito(etiqueta,vector); 
        densidad += masa/((float)n*(float)n);   
    }
    proba[itera_proba] = p;
    fuerza_cluster_percolante[itera_proba]=densidad/(float)z;
    
  }
  escribir(proba,fuerza_cluster_percolante,cantidad_probas);
} 


void resultado3(int z){
  int   iter_size,n,sample,masa,etiqueta;
  int*  red;
  int   iter_size_max=8;
  float masa_percolante[iter_size_max];
  float size[iter_size_max];
  float p = 0.592603;
  double masa_acum;
 
  for (iter_size=0;iter_size<iter_size_max;iter_size++){
    n = pow(2,iter_size+2);
    red = (int *)malloc(n*n*sizeof(int));
    int vector[n*n];
    masa_acum=0.0;
    for(sample=0;sample<z;sample++){  
        llenar(red,n,p); 
        hoshen(red,n);
        etiqueta=percola(red,n);
        calcula_ns(red,n,vector);
        masa = cluster_infinito(etiqueta,vector); 
        masa_acum += (double)masa/((double)z);   
    }
    size[iter_size] = n ;
    masa_percolante[iter_size]=(float)(masa_acum);
  }
  
  escribir(size,masa_percolante,iter_size_max);
} 

*/
int m2(int* vector_ns,int n){
  int i;
  float res=0;
  for(i=0;i<n*n;i++){
    res += (i*i)*(*vector_ns); 
    //printf("%f\n",res);
    vector_ns+=1;
  }
  return res;

}

void  resultado6(int n,float p_min,float p_max,float cant_p,int z){
  int*    red=(int *)malloc(n*n*sizeof(int));
  float*  vector_m = (float *)malloc(cant_p*sizeof(float));
  int*    vector_ns = (int *)malloc(n*n*sizeof(int));
  int*    vector=(int *)malloc(n*n*sizeof(int));
  int     i,valor_m,sample;
  float*  vector_proba = (float *)malloc(cant_p*sizeof(float));
  float   proba;
  float   paso = (p_max-p_min)/(cant_p-1);

  red = (int *)malloc(n*n*sizeof(int));
  i=0;
  sample=0;
  proba = p_min;
  llenar(red,n,1);
  imprimir(red,n);
  hoshen(red,n);
  calcula_ns(red,n,vector,vector_ns);
  imprimir(vector_ns,n);
  printf("%i\n",m2(vector_ns, n));
  while(i<cant_p){          // Loop de probabilidades
    valor_m = 0.0;
    sample = 0;
    while(sample<z){        // Loop de sampleos
        llenar(red,n,proba);
        //imprimir(red,n);
        hoshen(red,n);
        calcula_ns(red,n,vector,vector_ns);
        valor_m += (double)m2(vector_ns, n)/(double)z;
        sample++;
        //printf("%i\t%i\n",i,sample);
        }
    vector_m[i] = (float)valor_m; 
    vector_proba[i] = proba;
    //vector_m = vector_m+1;
    //vector_proba =+1;
    proba +=paso;
    i++;
  }
  escribir(vector_proba,vector_m,cant_p);


}




  

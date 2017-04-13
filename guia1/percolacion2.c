#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void  llenar(int *red,int n,float proba);
int   hoshen(int *red,int n);
int   actualizar(int *red,int *clase,int s,int frag);
void  etiqueta_falsa(int *red,int *clase,int s1,int s2);
void  corregir_etiqueta(int *red,int *clase,int n);
int   percola(int *red, int n);

/*
int main(int argc,char *argv[])
{
  int    i,j,*red;
  float  prob,denominador;

  n=N;
  z=Z;

  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%d",&z);
     }
    
  red=(int *)malloc(n*n*sizeof(int));



  for(i=0;i<z;i++)
    {
      prob=0.5;
      denominador=2.0;
 
      srand(time(NULL));

      for(j=0;j<P;j++)
        {
          llenar(red,n,prob);
      
          hoshen(red,n);
        
          denominador=2.0*denominador;

          if (percola(red,n)) 
             prob+=(-1.0/denominador); 
          else prob+=(1.0/denominador);
        }
    }

  free(red);

  return 0;
}

*/

int main()
{
  // int argc,char *argv[]
  int    i,j,*red,n;
  float proba;

  printf("Ingrese numero de columnas: ");
    if (scanf("%d", &n) != 1){
      printf("No ingreaste un numero entero.\n");};
    printf("Ingrese probabilidad de ocupacion (entre 0 y 1): ");
    if( scanf("%f", &proba) != 1){
      printf("No ingreaste un numero float..\n");};

/*
  if (argc==3) 
     {
       sscanf(argv[1],"%d",&n);
       sscanf(argv[2],"%d",&prob);
     }
  
  */

  red=(int *)malloc(n*n*sizeof(int));
  
  llenar(red,n,proba);
  imprimir(red,n,n);    
  hoshen(red,n);

  puts("\n");

  imprimir(red,n,n);
  percola(red,n);
  if (percola(red,n)){
	printf("PERCOLO \n");
} else {
	printf("No percolo \n");
}
  free(red);

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
	srand(time(NULL));  
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
					printf("%d",s1);
					return 1;
				}
								
			}
		}
	}
	return 0;
}	

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void llenar(int* red,int n_fila, int n_columna, float proba);
void imprimir(int *red, int m, int n);
//Comentario sticco

int main(){
	int* red;
	int n_columna, n_fila;
	float proba;

	printf("Ingrese numero de columnas: ");
   	if (scanf("%d", &n_columna) != 1){
   		printf("No ingreaste un numero entero.\n");};
   	printf("Ingrese numero de filas: ");
   	if (scanf("%d", &n_fila) != 1){
   		printf("No ingreaste un numero entero.\n");};
   	printf("Ingrese probabilidad de ocupacion (entre 0 y 1): ");
   	if( scanf("%f", &proba) != 1){
   		printf("No ingreaste un numero float..\n");};

	red = malloc(n_columna*n_fila*sizeof(int));
	llenar(red,n_fila,n_columna,proba);   	// Corro llenar
	imprimir(red,n_columna,n_fila);			// Corro imprimir
	free(red);
	return 0;
}

void llenar(int* red,int n_fila, int n_columna, float proba){
	int i,j;
	srand(time(NULL));  
	for(i=0;i<n_fila;i=i+1){
		for(j=0;j<n_columna;j=j+1){
			int r = rand()%10; // random de 0 a 9
			if(r<proba*10){
				red[j+n_columna*i]=1;
			}
			else{
				red[j+n_columna*i]=0;
			}
			
		}
	}
}

void imprimir(int *red, int n_fila, int n_columna){

	// Imprimo en pantalla la matriz///////
	int i,j;
	for(i=0;i<n_fila;i=i+1){
		for(j=0;j<n_columna;j=j+1){
			printf("%d    ",red[j+n_columna*i]);
		}
		printf("\n");
	}
}

/*
*
* Resolver la suma dos matrices de la misma dimensión.
* (ambas matrices son randomicas)
*
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){

	int numProcs,
			myRank,
			fila,
			i,
			n,
			dimencion;

	MPI_Status status;
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank (MPI_COMM_WORLD, &myRank);


	// n es la cantidad total de celdas por matriz
	n = numProcs * numProcs;

	// matrices dinamicas.
	int a[n];
	int b[n];
	int c[n];

	// matrices auxiliares.
	int ap[n];
	int bp[n];
	int cp[n];

	if (myRank == 0){
			// cargan las matrices, destribuyen los fragmentos a los demás procesos,
			// recopila y muestra la matriz resultante.
			// duplica el tamaño para tratarlo como una matriz cuadra.

			printf("Mátriz 1: \n");
			for(i=0;i<n;i++){
				a[i] = rand() % 11;
				printf("%d \n", a[i]);
			}

			printf("Mátriz 2: \n");
			for(i=0;i<n;i++){
				b[i] = rand() % 11;
				printf("%d\n", b[i]);
			}

		}

	// Hace conocer a los demás el número a calcular.
	MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// fila es la cantiad de celdas para cada proceso.
	fila = numProcs;
	// MPI_Bcast (&fila, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// envian los fragmentos.
	MPI_Scatter(a, fila, MPI_INT, ap, fila, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, fila, MPI_INT, bp, fila, MPI_INT, 0, MPI_COMM_WORLD);

	for(i=0 ; i<fila ; i++)
		cp[i] = ap[i]+bp[i];

	// tomas los elementos de los procesos y lo reune.
	MPI_Gather(cp, fila, MPI_INT, c, fila, MPI_INT, 0, MPI_COMM_WORLD);

	// Mostramos matriz resultante
	if (myRank == 0){
				printf("Mátriz Resultante: \n");
				for(i=0 ; i<n ; i++){
						printf ("%d \n", c[i]);
				}
	}

	MPI_Finalize();

}

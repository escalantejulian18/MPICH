/*
*
* Modificar el ejercicio anterior de manera que no use comunicaciones colectivas
*
*/

#include <mpi.h>
#include <stdio.h>

int inicio, fin;

//Calcula el rango o intervalo para cada proceso
void calcularRango(int num, int myRank, int numProcs){
	inicio = myRank * (num / numProcs) + 1;
	fin = inicio + (num / numProcs) - 1;

	if (myRank == numProcs - 1){
		fin = num;
	}
};

int main(int argc, char** argv) {

	int numProcs, myRank,constante, num, factorial, i;
	int result_parcial = 1;

	MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Status estado;

	if(myRank == 0){
		printf("Ingrese la constante: ");
		scanf("%d",&constante);
	}
	num = constante * numProcs;

	// enviamos el número al que debemos calcular su factorial
	// MPI_Bcast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(myRank == 0){
		for(int i = 1; i < numProcs; i++){
				MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
  }else{
		MPI_Recv(&num,1, MPI_INT, 0,0,MPI_COMM_WORLD,&estado);
	}



	// Calculamos los intervalos.
	calcularRango(num, myRank, numProcs);

	// cada proceso realiza este calculo con su porsion
	for(int i = inicio; i <= fin ; i ++){
		result_parcial = result_parcial * i;
	}

	// si no son cero, envian a cero su result_parcial
  if(myRank != 0){
      MPI_Send(&result_parcial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }else{

	// sinconización de procesos
  // MPI_Barrier(MPI_COMM_WORLD);


	// Cero recibe los result_parcial de los demás procesos

    factorial = result_parcial;
    for(int i = 1; i < numProcs; i++){
        MPI_Recv(&result_parcial,1, MPI_INT, i,0,MPI_COMM_WORLD,&estado);
        factorial = factorial * result_parcial;
    }
		printf("El factorial de %d es %d. \n", num, factorial);
	}

    MPI_Finalize();
}

/*
*
* Modificar el programa anterior de manera tal que se calcule el num
* del(número de procesos * una constante).
* La constante se ingresa por teclado
*
*/


#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]){
    int myRank,
        numProcs,
        constante,
        num,
        i,
        inicio,
        final,
        result_parcial = 1,
        factorial;


    MPI_Init(&argc,&argv);
    // Determina el rango del proceso que lo llama dentro del comunicador seleccionado.
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    // Determina el tamaño del grupo asociado a un comunicador.
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    if(myRank == 0){
       printf("Ingresa una constante que desee usar: ");
	     scanf("%d",&constante); // obtenemos la constante
    }

    num = constante * numProcs;

    // El proceso root envia msj al resto de los procesos en el grupo.
    MPI_Bcast(&num,1,MPI_INT,0,MPI_COMM_WORLD);

    // Determinamos el intervalo para cada proceso.
    // Inicio para cada proceso.
    if(myRank == 0){
	     inicio = 1;
    }else{
       inicio = myRank * (num / numProcs) + 1;
     }

    // Fin para cada proceso.
    if(myRank == (numProcs - 1)){
      final = num;
    }else{
       final = (myRank + 1) * (num / numProcs);
    }

    for(i = inicio ; i <= final ; i++){
	     result_parcial = result_parcial * (int)i;
    }

    //reduce a todos los procesos en un grupo y guarda el resultado en un proceso.
    MPI_Reduce(&result_parcial, &factorial, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

    if(myRank == 0){
	     printf("El factorial de %d es %d \n", num , factorial);
    }

    MPI_Finalize(); //Finaliza

    return 0;
}

#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char *argv[]){
  // Initialize mpi environment
  MPI_Init(&argc, &argv);

  // declare variable
  MPI_Status status;
  char string[] = "Hello mpi, this is sending message";

  // get mpi rank
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank == 0){
    // do something
    MPI_Send(string, strlen(string) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    printf("Pro 0 send message to Pro 1\n");
  }
  else if(world_rank == 1){
    // do something
    // get mpi status
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

    // create buffer for message
    int amount_count;
    MPI_Get_count(&status, MPI_CHAR, &amount_count);
    char buff[amount_count];

    // get mpi message
    MPI_Recv(buff, amount_count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Pro 1 get mess from Pro 0 has content: %s\n", buff);
  }

  // Finalize mpi
  MPI_Finalize();

  return 0;
}
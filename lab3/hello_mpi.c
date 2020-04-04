#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // print off a hello world message
  // printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);

  // send/receive message
  int number;
  if(world_rank == 0){
    // do something
    number = 20;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  else if(world_rank == 1){
    // do something
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 receive number %d from process 0\n", number);
  }
  else{
    printf("Process %d doesn't do anything\n", world_rank);
  }

  // Finalize the MPI environment
  MPI_Finalize();
}
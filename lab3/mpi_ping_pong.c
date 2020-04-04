#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

#define MAX_PASS 10

int main(int argc, char *argv[]){
  // initialize the MPI environment
  MPI_Init(&argc, &argv);

  // get the rank of process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // get partner rank
  int partner_rank = (world_rank + 1) % 2;

  // add scanf
  int test;

  int pass_ball = 0;
  while(pass_ball < MAX_PASS){
    if(world_rank == (pass_ball % 2)){
      pass_ball++;
      MPI_Send(&pass_ball, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("Process %d pass ball number %d to process %d\n", world_rank, pass_ball, partner_rank);
      if(world_rank == 3){
        scanf("%d", &test);
      }
    }
    else{
      MPI_Recv(&pass_ball, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process %d get ball number %d from process %d\n", world_rank, pass_ball, partner_rank);
    }
    
  }
  
  // finanlize the MPI environment
  MPI_Finalize();
  return 0;
}
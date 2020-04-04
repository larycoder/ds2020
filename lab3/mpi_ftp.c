#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define SRC 2 // path to file in client side
#define DES 3 // path to file in server sile
#define TYPE 1 // download or upload file
#define MAX 101 // number of bits transfer at once

// transfer function
void fileSend(FILE *fp, char *path, char *buff, int des_mpi, int tag_mpi){
  // open file
  fp = fopen(path, "rb");
  
  // trasnfer file
  int n_byte; // number of reading file
  while((n_byte = fread(&(buff[1]), 1, MAX - 1, fp)) != 0){
    buff[0] = 1;
    MPI_Send(buff, n_byte + 1, MPI_CHAR, des_mpi, tag_mpi, MPI_COMM_WORLD);
    bzero(buff, MAX);
  }
  // send end transfer signal
  buff[0] = 0;
  MPI_Send(buff, 1, MPI_CHAR, des_mpi, tag_mpi, MPI_COMM_WORLD);
  fclose(fp);
}

void fileRecv(FILE *fp, char *path, char *buff, int src_mpi, int tag_mpi){
  // open file
  fp = fopen(path, "wb");

  // receive data
  MPI_Status status;
  int n_byte;
  while(1){
    // get mpi status
    MPI_Probe(src_mpi, tag_mpi, MPI_COMM_WORLD, &status);

    // get number of transfer file
    MPI_Get_count(&status, MPI_CHAR, &n_byte);

    // get mpi message
    MPI_Recv(buff, n_byte, MPI_CHAR, src_mpi, tag_mpi, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // close process if end signal sent
    if(buff[0] == 0){
      fclose(fp);
      break;
    }
    
    // push data to file
    fwrite(&(buff[1]), 1, MAX - 1, fp);
  }
  
}

int main(int argc, char *argv[]){
  // Initialize mpi environment
  MPI_Init(NULL, NULL);

  // get mpi rank
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // File pointer and buffer
  FILE *fp;
  char buff[MAX]; // first byte is checksum byte
  bzero(buff, MAX);

  if(world_rank == 0){
    // this is client side
    if(strcmp(argv[TYPE], "-u") == 0){
      // send file
      fileSend(fp, argv[SRC], buff, 1, 0);
    }
    else if(strcmp(argv[TYPE], "-d") == 0){
      // recv file
      fileRecv(fp, argv[SRC], buff, 1, 0);
    }
    else{
      // do notthing
    }
  }
  else if(world_rank == 1){
    // this is server side
    if(strcmp(argv[TYPE], "-u") == 0){
      // recv file
      fileRecv(fp, argv[DES], buff, 0, 0);
    }
    else if(strcmp(argv[TYPE], "-d") == 0){
      // send file
      fileSend(fp, argv[DES], buff, 0, 0);
    }
    else{
      // do notthing
    }
  }

  // Finalize mpi
  MPI_Finalize();
}
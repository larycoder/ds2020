MPICC = mpicc
TARGET = hello_mpi.o
OBJECT = hello_mpi.c mpi_ping_pong.c mpi_message.c

hello_mpi: $(OBJECT)
	$(MPICC) -o $(TARGET) $(OBJECT)

pingpong_mpi: $(OBJECT)
	$(MPICC) -o pingpong.o mpi_ping_pong.c

message_mpi: $(OBJECT)
	$(MPICC) -o message.o mpi_message.c

clean:
	rm *.o
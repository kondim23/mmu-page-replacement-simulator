OBJS	= simulator.o pageTable.o memoryStructure.o
CC		= gcc
EXEC	= simulator

$(EXEC):$(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

clean:
	rm -f $(OBJS) $(EXEC)
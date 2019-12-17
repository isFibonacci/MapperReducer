CC = gcc
CFLAGS = -g -std=c99 -Wall
CLINK = -lpthread
CINCL = -Iinclude
IDIR =  ./include
ODIR = ./src

client: $(ODIR)/client.o $(ODIR)/phase1.o $(ODIR)/phase2.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLINK) $(CINCL)

$(ODIR)/client.o : $(ODIR)/client.c $(IDIR)/protocol.h
	$(CC) -o $@ -c $(ODIR)/client.c $(CFLAGS) $(CLINK) $(CINCL)

$(ODIR)/phase1.o: $(ODIR)/phase1.c $(IDIR)/phase1.h $(IDIR)/protocol.h
	$(CC) -o $@ -c $(ODIR)/phase1.c $(CFLAGS) $(CLINK) $(CINCL)

$(ODIR)/phase2.o: $(ODIR)/phase2.c $(IDIR)/phase2.h $(IDIR)/protocol.h
	$(CC) -o $@ -c $(ODIR)/phase2.c $(CFLAGS) $(CLINK) $(CINCL)

clean:
	rm client
	rm -rf log
	rm -rf MapperInput
	rm -f $(ODIR)/*.o

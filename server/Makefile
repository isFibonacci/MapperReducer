CC = gcc
CFLAGS = -g -std=c99 -Wall
CLINK = -lpthread
CINCL = -Iinclude

IDIR =  ./include
ODIR = ./src

server: $(ODIR)/server.o $(ODIR)/processReq.o
	$(CC) -o $@ $^ $(CFLAGS) $(CLINK) $(CINCL)

$(ODIR)/server.o : $(ODIR)/server.c $(IDIR)/protocol.h $(IDIR)/processReq.h
	$(CC) -o $@ -c $(ODIR)/server.c $(CFLAGS) $(CLINK) $(CINCL)

$(ODIR)/processReq.o : $(ODIR)/processReq.c $(IDIR)/protocol.h $(IDIR)/processReq.h
	$(CC) -o $@ -c $(ODIR)/processReq.c $(CFLAGS) $(CLINK) $(CINCL)


clean:
	rm server
	rm -f $(ODIR)/*.o
	rm -f  $(ODIR)/server


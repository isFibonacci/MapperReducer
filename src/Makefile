IDIR = ../include

CC = gcc
CFLAGS = -g -I$(IDIR)

ODIR = obj

mapreduce: main.c $(IDIR)/main.h $(ODIR)/phase1.o $(ODIR)/phase2.o $(ODIR)/phase3.o $(ODIR)/phase4.o
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/phase1.o: phase1.c $(IDIR)/phase1.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/phase2.o: phase2.c $(IDIR)/phase2.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/phase3.o: phase3.c $(IDIR)/phase3.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/phase4.o: phase4.c $(IDIR)/phase4.h
	$(CC) -c -o $@ $< $(CFLAGS)

TST_DIR = ../Testcases
test-mapreduce:
	@printf "===Testing mapreduce===\n"
	@printf "\nTestCase1: Empty directory\n"	
	./mapreduce ${TST_DIR}/TestCase1 5
	@printf "\n"
	@printf "\nTestCase2_1: Normal Case - One mapper\n"	
	./mapreduce ${TST_DIR}/TestCase2 1
	@printf "\n"
	@printf "\nTestCase2_2: Normal Case - 32 max mapper\n"	
	./mapreduce ${TST_DIR}/TestCase2 32
	@printf "\n"
	@printf "\nTestCase2_3: Normal Case - Single level folder hierarchy with same number of files in each folder\n"	
	./mapreduce ${TST_DIR}/TestCase2 5
	@printf "\n"
	@printf "\nTestCase3: Normal Case - Single level folder hierarchy with carying number of files in each folder\n"	
	./mapreduce ${TST_DIR}/TestCase3 5
	@printf "\n"
	@printf "\nTestCase4: Normal case - Multi level folder hierarchy\n"	
	./mapreduce ${TST_DIR}/TestCase4 5 
	@printf "\n"

test-extracredits:
	@printf "===Testing extra credits===\n"	
	./mapreduce ${TST_DIR}/TestCase5 5
	@printf "\n"

.PHONY: clean

clean:
	rm mapreduce
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~
	rm -rf MapperInput
	rm ReducerResult.txt
	rm FinalResult.txt

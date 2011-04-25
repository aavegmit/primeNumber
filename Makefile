# Macros

CC = g++
CFLAGS = -g -Wall
OBJ = main.o functions.o
LIBS = -lcrypto
#LIBS = -L/home.scf-22/csci551b/openssl/lib -lcrypto -lnsl -lsocket -lresolv
#INC = -I/home/scf-22/csci551b/openssl/include
INC = 

# Explicit rule
all: hw7 

hw7: $(OBJ)
	$(CC) $(CFLAGS) -o hw7 $(OBJ) $(INC) $(LIBS) 

clean:
	rm -rf *.o hw7

main.o: main.cc
	$(CC) $(CFLAGS) -c main.cc $(INC)
functions.o: functions.cc
	$(CC) $(CFLAGS) -c functions.cc $(INC)

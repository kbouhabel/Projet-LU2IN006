CC = gcc
CFLAGS = -Wall
TARGET = test_tableHachage
OBJS = tableHachage.o test_tableHachage.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

tableHachage.o: tableHachage.c tableHachage.h
	$(CC) $(CFLAGS) -c tableHachage.c

test_tableHachage.o: test_tableHachage.c tableHachage.h
	$(CC) $(CFLAGS) -c test_tableHachage.c

test: gestionMemoire.o test_gestionMemoire.o tableHachage.o
	gcc -o test gestionMemoire.o test_gestionMemoire.o tableHachage.o -lm

gestionMemoire.o: gestionMemoire.c gestionMemoire.h
	gcc -c gestionMemoire.c

test_gestionMemoire.o: test_gestionMemoire.c gestionMemoire.h
	gcc -c test_gestionMemoire.c

clean:
	rm -f $(OBJS) $(TARGET) *.o test

run: all
	./$(TARGET)

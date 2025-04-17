CC = gcc
CFLAGS = -Wall
TARGET = test_tableHachage
TARGET2= test_gestionMemoire
OBJS = tableHachage.o test_tableHachage.o
OBJS2 = gestionMemoire.o test_gestionMemoire.o tableHachage.o


all: $(TARGET) $(TARGET2)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJS2)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

tableHachage.o: tableHachage.c tableHachage.h
	$(CC) $(CFLAGS) -c tableHachage.c

test_tableHachage.o: test_tableHachage.c tableHachage.h
	$(CC) $(CFLAGS) -c test_tableHachage.c

gestionMemoire.o: gestionMemoire.c gestionMemoire.h
	gcc -c gestionMemoire.c

test_gestionMemoire.o: test_gestionMemoire.c gestionMemoire.h
	gcc -c test_gestionMemoire.c

clean:
	rm -f $(OBJS) $(TARGET) *.o test

run: all
	./$(TARGET)

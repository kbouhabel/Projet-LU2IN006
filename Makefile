CC = gcc
CFLAGS = -Wall -g
TARGET = test_tableHachage
TARGET2= test_gestionMemoire
TARGET3= test_parser
OBJS = tableHachage.o test_tableHachage.o
OBJS2 = gestionMemoire.o test_gestionMemoire.o tableHachage.o
OBJS3 = parser.o test_parser.o gestionMemoire.o tableHachage.o

all: $(TARGET) $(TARGET2) $(TARGET3)

$(TARGET3): $(OBJS3)
	$(CC) $(CFLAGS) -o $(TARGET3) $(OBJS3)
$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJS2)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

tableHachage.o: tableHachage.c tableHachage.h
	$(CC) $(CFLAGS) -c tableHachage.c

test_tableHachage.o: test_tableHachage.c tableHachage.h
	$(CC) $(CFLAGS) -c test_tableHachage.c

gestionMemoire.o: gestionMemoire.c gestionMemoire.h
	gcc $(CFLAGS) -c gestionMemoire.c

test_gestionMemoire.o: test_gestionMemoire.c gestionMemoire.h
	gcc $(CFLAGS) -c test_gestionMemoire.c

parser.o: parser.c parser.h
	gcc $(CFLAGS) -c parser.c

test_parser.o: test_parser.c parser.h
	gcc $(CFLAGS) -c test_parser.c

clean:
	rm -f $(OBJS) $(TARGET) *.o test

run: all
	./$(TARGET)

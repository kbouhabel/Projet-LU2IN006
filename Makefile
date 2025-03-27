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

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)

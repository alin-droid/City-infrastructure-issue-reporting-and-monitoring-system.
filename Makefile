CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = main.c operations.c fileHeandling.c permissions.c processArguments.c 
OBJ = $(SRC:.c=.o)

all: city_manager

city_manager: $(OBJ)
	$(CC) $(CFLAGS) -o city_manager $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o city_manager
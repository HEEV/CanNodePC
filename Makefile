SRC:= can.c CanNode.c
LOGGER:= canLogger.c
SENDER:= sender.c
OBJ:=$(SRC:.c=.o)
LOGGER_OBJ=$(LOGGER:.c=.o)
SENDER_OBJ=$(SENDER:.c=.o)

.PHONY: clean

all: $(LOGGER_OBJ) $(SENDER_OBJ) $(OBJ)
	gcc -o CanLogger $(LOGGER_OBJ) $(OBJ)
	gcc -o sender $(SENDER_OBJ) $(OBJ)
	

clean:
	rm $(OBJ) CanLogger

.c.o:
	gcc -c -g $< -o $@

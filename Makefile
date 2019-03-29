SRC:= CanNode/can.cpp CanNode/CanNode.cpp
LOGGER:= canLogger.cpp
SENDER:= sender.cpp
OBJ:=$(SRC:.cpp=.o)
LOGGER_OBJ=$(LOGGER:.cpp=.o)
SENDER_OBJ=$(SENDER:.cpp=.o)


.PHONY: clean

all: $(LOGGER_OBJ) $(SENDER_OBJ) $(OBJ)
#	g++ -o canLogger $(LOGGER_OBJ) $(OBJ)
	g++ -o sender $(SENDER_OBJ) $(OBJ)
	

clean:
	rm $(OBJ) canLogger sender

.cpp.o:
	g++ -g3 -c $< -o $@

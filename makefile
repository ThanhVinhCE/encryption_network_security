
TARGET = app

INC = -Iinclude

SRC = src
INCLUDE = include
OBJ = obj

CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

vpath %.cpp $(SRC)
vpath %.h $(INCLUDE)

MAKE = $(CC) $(INC) 

OBJ_FILE = $(addprefix $(OBJ)/,rsa.o primeGenerator.o utils.o main.o)
HEADER = $(wildcard $(INCLUDE)/*.h)

all: $(OBJ_FILE)
	$(MAKE) $(LFLAGS) $(OBJ_FILE) -o $(TARGET) -lntl -lgmp -lgmpxx

$(OBJ)/%.o: %.cpp ${HEADER}
	@mkdir -p $(OBJ) # create folder obj if not exist
	$(MAKE) $(CFLAGS) $< -o $@

clean:
	rm -r obj/
	rm decryptFile.txt
	rm encryptFile.txt
	rm privatekey.txt
	rm publickey.txt
	rm -f obj/*.o $(TARGET)
	@echo "Clean successfully"

run:
	./$(TARGET)
	
#Compiler single file for test cases
#g++ -Wall PrimeGenerator.cpp -o test -lntl -lgmp -lgmpxx

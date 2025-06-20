@"
# Output file name
OUTPUT = komponen_app

# Source file
SRC = src/main.c

# Compiler
CC = gcc
CFLAGS = -Wall -g

# Compile rule
all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

# Run program
run: all
	./$(OUTPUT)

# Clean output file
clean:
	del $(OUTPUT) 2>nul || rm -f $(OUTPUT)
"@ > Makefile

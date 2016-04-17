CC = g++
CFLAGS = -Wall -Werror

INCDIR = include
SRCDIR = src
BINDIR = bin
OBJDIR = obj
LIB = -lbcm2835

TARGET = $(BINDIR)/pipad

$(TARGET): compile
	@echo "Building"
	@mkdir -p $(BINDIR)
	$(CC) -shared -o $(BINDIR)/libpipad.so $(OBJDIR)/pipad.o $(CFLAGS)
	$(CC) -o $@ $(OBJDIR)/main.o $(BINDIR)/libpipad.so $(CFLAGS) $(LIB)

.PHONY: compile
compile:
	@echo "Compiling"
	@mkdir -p $(OBJDIR)
	$(CC) -c -o $(OBJDIR)/main.o $(SRCDIR)/main.cpp $(CFLAGS)
	$(CC) -c -o $(OBJDIR)/pipad.o $(SRCDIR)/pipad.cpp $(CFLAGS) -fpic

.PHONY: clean
clean:
	@echo "Cleaning"
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)

.PHONY: install
install:
	@mkdir -p ~/.pipad
	@cp -f gpio.ini ~/.pipad/
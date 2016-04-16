CC = g++
CFLAGS = -Wall -Werror

INCDIR = include
SRCDIR = src
BINDIR = bin
OBJDIR = obj
INC = $(wildcard $(INCDIR)/*.*)
SRC = $(wildcard $(SRCDIR)/*.*)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
LIB = -lbcm2835

TARGET = $(BINDIR)/pi-gamepad

$(TARGET): compile
	@echo "Building"
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIB)

.PHONY: compile
compile:
	@echo "Compiling"
	@mkdir -p $(OBJDIR)
	@$(foreach target,$(SRC),echo $(target);$(CC) -c -o $(subst $(SRCDIR)/,$(OBJDIR)/,$(subst .cpp,.o,$(target))) $(target) $(CFLAGS);)

.PHONY: clean
clean:
	@echo "Cleaning"
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)
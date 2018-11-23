# currently directory of each file
SRCDIR 		= src
INCLUDEDIR 	= include
OBJDIR 		= obj
LIBDIR 		= lib
BINDIR 		= bin

# project name (generate executable with this name)
TARGET = sudoku_game

# compiler in use
CC = gcc -std=c99

# linker's arguments
EXELINKER = $(CC) -o
LIBLINKER = ar -crs

# compiling flags here
CFLAGS = -ggdb -Wall -Wunused -Wformat=0 -Werror\
-Ofast -fstack-protector-all -I./$(INCLUDEDIR)

# list of libraries used
LIBS = -lm -lncurses

# general definition of the files
STATICLIB 	= $(LIBDIR)/lib$(TARGET).a
SOURCES 	:= $(wildcard $(SRCDIR)/*.c)
INCLUDES 	:= $(wildcard $(INCLUDEDIR)/*.h)
OBJECTS 	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# clean process
rm = rm -f

# directorie's creation process
mkdir = mkdir -p

# find process
find = find -maxdepth 1 -name

# move process
mv = mv -i

# links the objects creating the static library and the executable 
$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LIBLINKER) $(STATICLIB) $(OBJECTS)
	@$(EXELINKER) $@ $(STATICLIB) $(LIBS)
	@echo "Linking complete!"

# compilates the sources creating the objects
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES) Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

# executes the target
exe: $(BINDIR)/$(TARGET)
	@$(BINDIR)/$(TARGET)

# executes the target in God Mode
exe_god: $(BINDIR)/$(TARGET)
	@$(BINDIR)/$(TARGET) GOD

# cleanup
PURGE: clean_bin clean_lib clean_obj

# cleans the executable only
clean_bin:
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

# cleans the static library only
clean_lib:
	@$(rm) $(STATICLIB)
	@echo "Static library removed!"

# cleans the objects only
clean_obj:
	@$(rm) $(OBJECTS)
	@echo "Objects removed!"

# creates the folders if needed
# and moves the archives to its proper folder
organize:
	@$(mkdir) $(SRCDIR) $(INCLUDEDIR) $(OBJDIR) $(LIBDIR) $(BINDIR)
	@$(find) "*.a" -exec $(mv) "{}" "./$(LIBDIR)/{}" \;
	@$(find) "*.c" -exec $(mv) "{}" "./$(SRCDIR)/{}" \;
	@$(find) "*.h" -exec $(mv) "{}" "./$(INCLUDEDIR)/{}" \;
	@$(find) "*.o" -exec $(mv) "{}" "./$(OBJDIR)/{}" \;
	@$(find) "$(TARGET)" -exec $(mv) "{}" "./$(BINDIR)/{}" \;
	@sleep 1.5
	@echo "The files have been organized!"

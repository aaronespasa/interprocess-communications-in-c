CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall -Wextra -Werror

LDFLAGS = -L$(INSTALL_PATH)/lib/

LDLIBS = -lpthread -lrt

# Adding ./lib directory to the LD_LIBRARY_PATH environment variable and exporting it
LD_LIBRARY_PATH = $LD_LIBRARY_PATH:./lib
export LD_LIBRARY_PATH

# Default target
all: dir information libclaves cliente proxy

# Print output files information 
information:
	@echo ''
	@echo "Output files:"
	@echo "  - servidor.out"
	@echo "  - cliente.out (using libclaves.so)"
	@echo ''

# Create the ./lib directory if it doesn't exist
dir:
	$(shell mkdir -p lib)

# proxy.c compilation
proxy: proxy.c servidor.c LinkedList.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o servidor.out

# Generate dynamic library (FPIC flag generates position independent code; -shared flag generates a shared object)
libclaves: claves.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) -fPIC -shared $^ -o ./lib/$@.so

# Client compilation
cliente: cliente.c
	@$(CC) $(CPPFLAGS) $(LDLIBS) $^ -o $@.out -L./lib -lclaves -Wl,-rpath=./lib

# Clean all files
clean:
	@rm -f *.o *.out *.so ./lib/*.so -d ./lib
	@if [ ! -z "$(shell ls -A /dev/mqueue)" ]; then rm /dev/mqueue/*; fi
	@echo '\n'"All files removed"'\n'

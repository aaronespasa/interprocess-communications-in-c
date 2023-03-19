CPPFLAGS = -I$(INSTALL_PATH)/include -Wall -Wextra -Werror

LDFLAGS = -L$(INSTALL_PATH)/lib/

LDLIBS = -lpthread -lrt

# Adding ./lib directory to the LD_LIBRARY_PATH environment variable and exporting it
LD_LIBRARY_PATH = $LD_LIBRARY_PATH:./lib
export LD_LIBRARY_PATH

# check if hostname == guernika using a function called get_compiler
get_compiler = $(if $(findstring guernika,$1),/opt/gcc-12.1.0/bin/gcc,gcc)

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
	@$(call get_compiler, $(shell hostname)) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o servidor.out

# Generate dynamic library (FPIC flag generates position independent code; -shared flag generates a shared object)
libclaves: claves.c
	@$(call get_compiler, $(shell hostname)) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) -fPIC -shared $^ -o ./lib/$@.so

# Client compilation
cliente: cliente.c
	@$(call get_compiler, $(shell hostname)) $(CPPFLAGS) $(LDLIBS) $^ -o $@.out -L./lib -lclaves -Wl,-rpath=./lib

# Clean all files
clean:
	@rm -f *.o *.out *.so ./lib/*.so -d ./lib
	@if [ ! -z "$(shell ls -A /dev/mqueue)" ]; then rm /dev/mqueue/*; fi
	@echo -e '\n'"All files removed"'\n'

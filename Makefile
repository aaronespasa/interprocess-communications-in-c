CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall -Wextra -Werror

LDFLAGS = -L$(INSTALL_PATH)/lib/

LDLIBS = -lpthread -lrt

LD_LIBRARY_PATH = /lib

all: information libclaves cliente proxy

information:
	@echo ''
	@echo "Output files:"
	@echo "  - proxy.out"
	@echo "  - cliente.out"
	@echo "  - libclaves.so"
	@echo ''

# proxy.c compilation
proxy: proxy.c servidor.c LinkedList.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o $@.out

# Generate dynamic library (FPIC flag generates position independent code; -shared flag generates a shared object)
# @$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) -fPIC -shared $^ -o $@.so
libclaves: claves.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) -fPIC -shared $^ -o /lib/$@.so

# Client compilation
cliente: cliente.c
	@$(CC) $(CPPFLAGS) $(LDLIBS) $^ -o $@.out -lclaves -L/lib

clean:
	@rm -f *.o *.out *.so /lib/libclaves.so
	@if [ ! -z "$(shell ls -A /dev/mqueue)" ]; then rm /dev/mqueue/*; fi
	@echo -e '\n'"All files removed"'\n'

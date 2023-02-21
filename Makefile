CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall -Wextra -Werror

LDFLAGS = -L$(INSTALL_PATH)/lib/

LDLIBS = -lpthread -lrt

all: information cliente 

information:
	@echo "Output files:"
	@echo "  - servidor.out"
	@echo "  - cliente.out (uses libclaves.so)"

# servidor.c compilation
servidor: servidor.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o $@.out

# Generate dynamic library (FPIC flag generates position independent code; -shared flag generates a shared object)
libclaves: claves.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) -fPIC -shared $^ -o $@.so

# Client compilation
cliente: cliente.c claves.c
	@$(CC) $(LDFLAGS) $(CPPFLAGS) $(LDLIBS) $^ -o $@.out

clean:
	@rm -f *.o *.out *.so
	@if [ ! -z "$(shell ls -A /dev/mqueue)" ]; then rm /dev/mqueue/*; fi
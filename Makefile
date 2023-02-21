BIN_FILES  = p1

CC = gcc

CPPFLAGS = -I$(INSTALL_PATH)/include -Wall

LDFLAGS = -L$(INSTALL_PATH)/lib/
#LDLIBS = -lpthread -lm

all: $(BIN_FILES)
.PHONY : all

# servidor.c compilation
servidor.out: servidor.c
	$(CC) $(LDFLAGS) -lrt -o $@ $^ $(LDLIBS)

# Client compilation
cliente.out: cliente.c
	$(CC) $(LDFLAGS) -lrt -o $@ $^ $(LDLIBS)

# Claves compilation
claves.out: claves.c
	$(CC) $(LDFLAGS) -lrt -o $@ $^ $(LDLIBS)

# Generate dynamic library
libclaves.so: libclaves.c
	$(CC) -shared -fPIC -o $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<


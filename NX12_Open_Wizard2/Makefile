CC=/usr/bin/gcc
ARGS=-DIPLIB=none -c -fPIC -m64 -DPOSIX -DNO_DOT_H -g -c 
DBINCLUDE=-I//siemens/tc13/tc/include
DBLIB=-L//siemens/tc13/tc/lib

OBJS=cust_sign.o libsign_custom_methods.o libsign_register_callbacks.o

init:
	$(CC) $(ARGS) -I. $(DBINCLUDE) $(DBLIB) cust_sign.c
	$(CC) $(ARGS) -I. $(DBINCLUDE) $(DBLIB) libsign_custom_methods.c
	$(CC) $(ARGS) -I. $(DBINCLUDE) $(DBLIB) libsign_register_callbacks.c

all: 
	/usr/bin/ld -G -o NANCAL_libsign.so $(OBJS) $(DBLIB)

purge:
	-rm -f *.o

clean: purge
	-rm -f *.so


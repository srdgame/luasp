CC	= gcc
CFLAGS	= -DLUA_MODULE `pkg-config lua5.1 --cflags` -shared -fPIC -fno-rtti -fno-exceptions
LIBS	= 

all:
	$(CC) $(CFLAGS) -o luaredis.so luaredis.cpp $(LIBS)

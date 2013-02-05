CC	= gcc
CFLAGS	= -DLUA_MODULE `pkg-config lua5.1 --cflags` -shared -fPIC -fno-rtti -fno-exceptions -DLUA_MODULE_VERSION=\"0.1.1\"
LIBS	= 

all:
	$(CC) $(CFLAGS) -o luaredis.so luaredis.cpp $(LIBS)
	lua test.lua

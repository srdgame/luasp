CC	= gcc
CFLAGS	= -DLUA_MODULE `pkg-config lua5.1 --cflags` -shared -fPIC -fno-rtti -fno-exceptions -DLUA_MODULE_VERSION=\"0.1.1\"
LIBS	= 

all:
	$(CC) $(CFLAGS) -o luamcache.so luamcache.cpp $(LIBS)
	lua test.lua

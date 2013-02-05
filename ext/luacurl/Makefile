CC	= gcc
CFLAGS	= -DLUA_MODULE `pkg-config libcurl lua5.1 --cflags` -shared -fPIC -fno-rtti -fno-exceptions -DLUA_MODULE_VERSION=\"0.1.1\"
LIBS	= `pkg-config libcurl --libs`

all:
	$(CC) $(CFLAGS) -o luacurl.so luacurl.cpp $(LIBS)

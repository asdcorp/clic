OPT=-Os -Wall
CFLAGS=-fno-ident -nostdlib
LDFLAGS=-L. -lucrt -lkernel32 -Wl,--exclude-all-symbols,--enable-stdcall-fixup,--dynamicbase,--nxcompat,--subsystem,console:6.0

PREFIX32=C:/mingw32/bin/
PREFIX64=C:/mingw64/bin/

CC32=gcc -Wl,-e_wWinMain -municode
CC64=gcc -Wl,-ewWinMain -municode

all: clic32.exe clic64.exe

clic32.exe: clic.c libclipc32.a libslc32.a
	$(PREFIX32)$(CC32) $(OPT) $< -o $@ $(CFLAGS) $(LDFLAGS) -lslc32 -lclipc32
	strip -s $@

clic64.exe: clic.c libclipc64.a libslc64.a
	$(PREFIX64)$(CC64) $(OPT) $< -o $@ $(CFLAGS) $(LDFLAGS) -lslc64 -lclipc64
	strip -s $@

libslc32.a:
	$(PREFIX32)dlltool -k -dslc32.def -llibslc32.a

libslc64.a:
	$(PREFIX64)dlltool -k -dslc64.def -llibslc64.a

libclipc32.a:
	$(PREFIX32)dlltool -k -dclipc32.def -llibclipc32.a

libclipc64.a:
	$(PREFIX64)dlltool -k -dclipc64.def -llibclipc64.a

clean:
	rm -f *.exe *.a

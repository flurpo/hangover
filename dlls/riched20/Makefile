SRCDIR?=build_from_build_dllsXX_YYY
DESTDIR?=../..
CURRENT_DLL=DLL_RICHED20
CFLAGS+=-I$(SRCDIR)/../include -I$(SRCDIR)/../../qemu/include -DQEMU_CURRENT_DLL=$(CURRENT_DLL) -O2 -DCONST_VTABLE -D_WIN32_WINNT=0x0603 -DSECURITY_WIN32 -DGUEST_BIT=$(GUEST_BIT) -DHOST_BIT=$(HOST_BIT)
WINEGCC=$(DESTDIR)/build/wine-host/tools/winegcc/winegcc --winebuild $(DESTDIR)/build/wine-host/tools/winebuild/winebuild -I$(DESTDIR)/build/wine-host/include -I$(DESTDIR)/wine/include

VPATH=$(SRCDIR)

all: riched20.dll qemu_riched20.dll.so

riched20.dll: editor_g.o main_g.o riched20.spec txtsrv_g.o
	$(WINEGCC) --wine-objdir $(DESTDIR)/build/$(WINE_DIR) -shared -b $(GUEST_CC) $(SRCDIR)/riched20.spec *_g.o -lwinecrt0 -o riched20.dll -nodefaultlibs -nostdlib -Wl,--image-base,0x110000000

qemu_riched20.dll.so: editor_h.o main_h.o txtsrv_h.o
	$(WINEGCC) --wine-objdir $(DESTDIR)/build/wine-host -shared $(HOST_CC) $(SRCDIR)/qemu_riched20.spec *_h.o -o qemu_riched20.dll.so -lriched20 -luser32 -luuid

%_g.o: %.c qemu_riched20.h Makefile $(SRCDIR)/Makefile
	$(GUEST_CC)-gcc $< -c -o $@ $(CFLAGS) -DQEMU_DLL_GUEST

%_h.o: %.c qemu_riched20.h Makefile $(SRCDIR)/Makefile
	$(WINEGCC) $(HOST_CC) $< -c -o $@ $(CFLAGS) -DUSE_WS_PREFIX

clean:
	rm -rf *o *.dll *.so

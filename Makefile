# See LICENSE file for copyright and license details
# slslstatus - suckless status monitor
.POSIX:

include config.mk

REQ = util
COM =\
	components/battery\
	components/cpu\
	components/datetime\
	components/disk\
	components/entropy\
	components/hostname\
	components/ip\
	components/kernel_release\
	components/keyboard_indicators\
	components/keymap\
	components/load_avg\
	components/netspeeds\
	components/num_files\
	components/ram\
	components/run_command\
	components/swap\
	components/temperature\
	components/uptime\
	components/user\
	components/volume\
	components/wifi

all: slslstatus

$(COM:=.o): config.mk $(REQ:=.h)
slslstatus.o: slslstatus.c slslstatus.h arg.h config.h config.mk $(REQ:=.h)

.c.o:
	$(CC) -o $@ -c $(CPPFLAGS) $(CFLAGS) $<

config.h:
	cp config.def.h $@

slslstatus: slslstatus.o $(COM:=.o) $(REQ:=.o)
	$(CC) -o $@ $(LDFLAGS) $(COM:=.o) $(REQ:=.o) slslstatus.o $(LDLIBS)

clean:
	rm -f slslstatus slslstatus.o $(COM:=.o) $(REQ:=.o)

dist:
	rm -rf "slslstatus-$(VERSION)"
	mkdir -p "slslstatus-$(VERSION)/components"
	cp -R LICENSE Makefile README config.mk config.def.h \
	      arg.h slslstatus.c $(COM:=.c) $(REQ:=.c) $(REQ:=.h) \
	      slslstatus.1 "slslstatus-$(VERSION)"
	tar -cf - "slslstatus-$(VERSION)" | gzip -c > "slslstatus-$(VERSION).tar.gz"
	rm -rf "slslstatus-$(VERSION)"

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f slslstatus "$(DESTDIR)$(PREFIX)/bin"
	chmod 755 "$(DESTDIR)$(PREFIX)/bin/slslstatus"
	mkdir -p "$(DESTDIR)$(MANPREFIX)/man1"
	cp -f slslstatus.1 "$(DESTDIR)$(MANPREFIX)/man1"
	chmod 644 "$(DESTDIR)$(MANPREFIX)/man1/slslstatus.1"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/slslstatus"
	rm -f "$(DESTDIR)$(MANPREFIX)/man1/slslstatus.1"

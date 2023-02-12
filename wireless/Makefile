include config.mak
wireles	: wireless.o kgWhich.o dlink.o runjob.o wirelessmain.o
	  cc  -fPIE -o  wireless wireless.o kgWhich.o dlink.o runjob.o wirelessmain.o
	  cp wireless $(KULINA)/TARBALL
install	: wireless
	  mv wireless /usr/bin/
clean	:
	 rm -f wireless *.o
	 rm -f $(KULINA)TARBALL/wireless

include config.mak
wireles	: wireless.o kgWhich.o dlink.o runjob.o wirelessmain.o
	  cc  -fPIE -o  wireless wireless.o kgWhich.o dlink.o runjob.o wirelessmain.o
	  cp wireless $(KULINA)/TARBALL

wireless.o	: wireless.c
	  cc -c wireless.c

wirelessmain.o	: wirelessmain.c
	  cc -c wirelessmain.c

install	: wireless
	  mv wireless /usr/sbin/
clean	:
	 rm -f wireless *.o
	 rm -f $(KULINA)TARBALL/wireless

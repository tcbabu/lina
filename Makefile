# lina Makefile
#
#
# This file is part of lina
#
###### variable declarations #######

# local fallbacks for missing operating system features
SHELL	:= /bin/bash
PREFIX=/usr
export CFLAGS=-I$(PWD)/include
export LDFLAGS=-L$(PWD)/lib
X11_CFLAGS	="-I$(PWD)/include $(shell pkg-config --cflags x11)"
X11_LIBS 	="$(shell pkg-config --libs x11)"
PKG_CONFIG_PATH_OLD	:=$(PKG_CONFIG_PATH)
PKG_CONFIG_PATH	:=$(PWD)/lib/pkgconfig:/usr/X11R76/lib/pkgconfig
PATH_OLD	:=$(PATH)
PATHNEW	:=$(PWD)/bin:$(PATH_OLD)
PKG_CONFIG_PATH_NEW	:=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD):/usr/X11R76/lib/pkgconfig


GMFILES = $(foreach part, OpenSource, $(wildcard $(addprefix $(part)/,*.gz)))
GMFILES += $(foreach part, OpenSource, $(wildcard $(addprefix $(part)/,*.bz2)))
GMFILES += $(foreach part, OpenSource, $(wildcard $(addprefix $(part)/,*.xz)))

KGLIBFILES = $(foreach part, kglib, $(wildcard $(addprefix $(part)/,*.[ch])))
LINAFILES = $(foreach part, lina, $(wildcard $(addprefix $(part)/,*.[ch])))
CONFIGFILES = $(foreach part, Config, $(wildcard $(addprefix $(part)/,*.[ch])))
SETPHOTOFILES = $(foreach part, SetPhoto, $(wildcard $(addprefix $(part)/,*.[ch])))

all	: lina/lina SetPhoto/SetPhoto Config/configlina wireless/wireless lib/libcrypt.a

lib/libkulina.a	: lib/libgm.a $(KGLIBFILES) 
	echo "PREFIX=$(PWD)">kglib/config.mak
	echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>kglib/config.mak
	echo "export X11_LIBS=\"$(X11_LIBS)\"">>kglib/config.mak
	$(MAKE) -C kglib 
	$(MAKE) -C kglib  install

lib/libgm.a	: $(GMFILES)
	echo "PREFIX=$(PWD)">OpenSource/config.mak
	echo "KULINA=$(PWD)">>OpenSource/config.mak
	echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>OpenSource/config.mak
	echo "export X11_LIBS=\"$(X11_LIBS)\"">>OpenSource/config.mak
	echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:/usr/X11R76/lib/pkgconfig">>OpenSource/config.mak
	$(MAKE) -C OpenSource 
	$(MAKE) -C OpenSource install

lina/lina	: lib/libgm.a lib/libkulina.a lib/libcrypt.a $(LINAFILES)
	echo "PREFIX=$(PREFIX)">lina/config.mak
	echo "KULINA=$(PWD)">>lina/config.mak
	$(MAKE) -C lina
SetPhoto/SetPhoto	: lib/libgm.a lib/libkulina.a $(SETPHOTOFILES)
	echo "PREFIX=$(PREFIX)">SetPhoto/config.mak
	echo "KULINA=$(PWD)">>SetPhoto/config.mak
	$(MAKE) -C SetPhoto
Config/configlina	: lib/libgm.a lib/libkulina.a $(CONFIGFILES)
	echo "PREFIX=$(PREFIX)">Config/config.mak
	echo "KULINA=$(PWD)">>Config/config.mak
	$(MAKE) -C Config
wireless/wireless	:
	echo "PREFIX=$(PREFIX)">wireless/config.mak
	echo "KULINA=$(PWD)">>wireless/config.mak
	$(MAKE) -C wireless
install	: lina/lina SetPhoto/SetPhoto Config/configlina wireless/wireless
	  $(MAKE) -C lina install
	  $(MAKE) -C Config install
	  $(MAKE) -C SetPhoto install
	  $(MAKE) -C wireless install

tarball : TARBALL/lina TARBALL/SetPhoto TARBALL/configlina TARBALL/wireless
	  mv TARBALL lina-1.2
	  tar czf lina-1.2.tgz lina-1.2
	  mv lina-1.2 TARBALL
clean	:
	 rm -rf lib/* share/* bin/* man/*
	$(MAKE) -C OpenSource clean
	$(MAKE) -C kglib clean
	$(MAKE) -C lina clean
	$(MAKE) -C Config clean
	$(MAKE) -C SetPhoto  clean
	$(MAKE) -C wireless  clean
	$(MAKE) -C libcrypt clean
	rm -f TARBALL/configlina TARBALL/SetPhoto TARBALL/lina TARBALL/wireless lina-1.2.tgz

#!/bin/bash
install -m 755 SetPhoto /usr/bin/SetPhoto
mkdir -p /usr/share/lina
install  -m 644 userphoto.png /usr/share/lina/
install  -m 644 userphoto.desktop  /usr/share/applications/
install -m 755 configlina /usr/sbin/configlina
install -m 644 configlina.png /usr/share/lina
install -m 644 ninjagirl.jpg  /usr/share/lina
install -m 644 configlina.desktop /usr/share/applications/
install -m 755 lina /usr/sbin/lina
install -m 644 lina.init  /usr/share/lina
install -m 755 wireless  /usr/sbin/
tar xzvf fonts.tar -C /usr/share/fonts

#!/bin/bash
install -m 755 -s SetPhoto /usr/bin/SetPhoto
mkdir -p /usr/share/lina
install  -m 644 userphoto.png /usr/share/lina/
install  -m 644 userphoto.desktop  /usr/share/applications/
install -m 755 -s configlina /usr/sbin/configlina
install -m 644 configlina.png /usr/share/lina
install -m 644 ninjagirl.jpg  /usr/share/lina
install -m 644 configlina.desktop /usr/share/applications/
install -m 755 -s lina /usr/sbin/lina
install -m 644 lina.init  /usr/share/lina
install -m 755 wireless  /usr/sbin/

#!/bin/sh
#script to detect new dhcp lease

# this will be called by dnsmasq everytime a new device is connected
# with the following arguments
# $1 = add | old
# $2 = mac address
# $3 = ip address
# $4 = device name

if [ "$1" == "add" ]; then
  msg="Device $*"
  echo `date +'%y-%m-%d %H:%M:%S'` $msg >> /tmp/dhcpmasq.log
fi


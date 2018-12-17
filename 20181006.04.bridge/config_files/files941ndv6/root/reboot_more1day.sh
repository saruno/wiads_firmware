#!/bin/sh
echo $( cat /proc/uptime | cut -d '.' -f 1 )
if [ $( cat /proc/uptime | cut -d '.' -f 1 ) -gt 90000 ]; then
        reboot
fi
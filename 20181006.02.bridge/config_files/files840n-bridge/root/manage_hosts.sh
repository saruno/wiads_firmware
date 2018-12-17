#!/bin/sh              

# PATH TO YOUR HOSTS FILE
ETC_HOSTS=/etc/hosts

DEFAULT_IP=$(uci get network.lan.ipaddr)
IP=${3:-$DEFAULT_IP}
HOSTNAME=$2

case "$1" in
  add)
        if [ -n "$(grep "$IP $HOSTNAME" /etc/hosts)" ]
        then
                echo "$IP $HOSTNAME exist"
        else
                sed -ie "\|$HOSTNAME\$|d" /etc/hosts
                echo "$IP $HOSTNAME"  >> /etc/hosts
                killall dnsmasq
                /etc/init.d/dnsmasq start
        fi
        ;;
  remove)
        sed -ie "\|$HOSTNAME\$|d" /etc/hosts
        ;;                                            
                                                      
  *)                                                  
        echo "Usage: "                                
        echo "hosts.sh [add|remove] [hostname] [ip]"
        echo                                        
        echo "Ip defaults to 127.0.0.1"             
        echo "Examples:"                            
        echo "hosts.sh add testing.com"             
        echo "hosts.sh remove testing.com 192.168.1.1"
        exit 1                                        
        ;;                                            
esac                                                  
                                                      
exit 0

#!/bin/sh
ssid=$(uci get wireless.@wifi-iface[0].ssid)
wireless_diabled=$(uci get wireless.@wifi-device[0].disabled)

case "$ssid" in
    *OpenWrt*)
        uci set wireless.@wifi-device[0].disabled=0
        uci set wireless.@wifi-iface[0].ssid='Wifi Free'
        uci set wireless.@wifi-iface[0].encryption='none'
        uci set wireless.@wifi-iface[0].key='12345678'
	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
	;;
    *Openwrt*)
        uci set wireless.@wifi-device[0].disabled=0
        uci set wireless.@wifi-iface[0].ssid='Wifi Free'
        uci set wireless.@wifi-iface[0].encryption='none'
        uci set wireless.@wifi-iface[0].key='12345678'
	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
        ;;
    *)
	    echo "ssid new"
	    ;;
esac

chmod 777 /etc/config/network
chmod 777 /etc/config/wireless

#ipaddr=$(uci get network.lan.ipaddr)

#case "$ipaddr" in
#     192.168.1.1)
#        uci set network.lan.ipaddr=172.16.16.1
#        uci commit network
#		;;
#     *)
#		echo "ok fine"
#		;;
#esac

#exit 0

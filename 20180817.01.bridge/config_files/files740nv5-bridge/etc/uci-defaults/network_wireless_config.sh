#!/bin/sh
ssid=$(uci get wireless.@wifi-iface[0].ssid)
wireless_diabled=$(uci get wireless.@wifi-device[0].disabled)

case "$ssid" in
    *OpenWrt*)
	#setup network bridge
	uci set network.wan.ifname="$(uci get network.lan.ifname) $(uci get network.wan.ifname)"
	uci set network.wan.type='bridge'
	uci delete network.lan.type
	uci set network.lan.ifname='wlan0'
	uci set wireless.@wifi-iface[0].ifname='wlan0'
	#setup wifi free
        uci set wireless.@wifi-device[0].disabled=0
        uci set wireless.@wifi-iface[0].ssid='Wifi Free'
        uci set wireless.@wifi-iface[0].encryption='none'
        uci set wireless.@wifi-iface[0].key='12345678'
	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
	uci commit network
	;;
    *Openwrt*)
	#setup network bridge
	uci set network.wan.ifname="$(uci get network.lan.ifname) $(uci get network.wan.ifname)"
	uci set network.wan.type='bridge'
	uci delete network.lan.type
	uci set network.lan.ifname='wlan0'
	uci set wireless.@wifi-iface[0].ifname='wlan0'
	#setup wifi free
        uci set wireless.@wifi-device[0].disabled=0
        uci set wireless.@wifi-iface[0].ssid='Wifi Free'
        uci set wireless.@wifi-iface[0].encryption='none'
        uci set wireless.@wifi-iface[0].key='12345678'
	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
	uci commit network
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

exit 0

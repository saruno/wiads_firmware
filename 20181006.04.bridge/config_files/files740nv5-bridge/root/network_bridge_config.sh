#!/bin/sh
wantype=$(uci get network.wan.type)

case "$wantype" in
    *bridge*)
	echo "It is bridge. Do nothing"
	;;
    *)
	echo "ssid new"
	uci set network.wan.ifname="$(uci get network.lan.ifname) $(uci get network.wan.ifname)"
	uci set network.wan.type='bridge'
	uci delete network.lan.type
	uci set network.lan.ifname='wlan0'
	uci set wireless.@wifi-iface[0].ifname='wlan0'
	uci commit wireless
	uci commit network
	/etc/init.d/network reload
	wifi
	;;
esac

exit 0

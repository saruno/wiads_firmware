#!/bin/sh
ssid=$(uci get wireless.@wifi-iface[0].ssid)
wireless_diabled=$(uci get wireless.@wifi-device[0].disabled)
sqm_config="/etc/config/sqm";

case "$ssid" in
    *OpenWrt*)
	#setup sqm
	if [ -f "$sqm_config" ]
	then
		uci set sqm.eth1.interface="eth0"
		uci set sqm.eth1.enabled='1'
		uci set sqm.eth1.download=30000
		uci set sqm.eth1.upload=30000
		uci commit sqm
	fi
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

	#uci set wireless.radio0.hwmode='11n'
	#uci set wireless.radio0.htmode='HT20'
	#uci set wireless.radio0.noscan='1'
	#uci set wireless.radio0.bursting='1'
	#uci set wireless.radio0.ff='1'
	#uci set wireless.radio0.compression='1'
	#uci set wireless.radio0.turbo='1'
	uci set wireless.@wifi-iface[0].disassoc_low_ack=0

	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
	uci commit network
	echo "dhcp-script=/root/detect_new_device.sh" >> /etc/dnsmasq.conf
	;;
    *Openwrt*)
	#setup sqm
	if [ -f "$sqm_config" ]
	then
		uci set sqm.eth1.interface="eth0"
		uci set sqm.eth1.enabled='1'
		uci set sqm.eth1.download=30000
		uci set sqm.eth1.upload=30000
		uci commit sqm
	fi
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

	#uci set wireless.radio0.hwmode='11n'
	#uci set wireless.radio0.htmode='HT20'
	#uci set wireless.radio0.noscan='1'
	#uci set wireless.radio0.bursting='1'
	#uci set wireless.radio0.ff='1'
	#uci set wireless.radio0.compression='1'
	#uci set wireless.radio0.turbo='1'
	uci set wireless.@wifi-iface[0].disassoc_low_ack=0

	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
	uci commit network
	echo "dhcp-script=/root/detect_new_device.sh" >> /etc/dnsmasq.conf
        ;;
    *)
	    echo "ssid new"
	    ;;
esac

#add cronjob to run wiads_daemon random time
start_min=$(awk 'BEGIN{srand();print int(rand()*6)}')
cron_job="$start_min-59/5 * * * * /usr/bin/wiads_daemon"
echo "$cron_job" >> /etc/crontabs/root

chmod 777 /etc/config/network
chmod 777 /etc/config/wireless

ipaddr=$(uci get network.lan.ipaddr)

case "$ipaddr" in
     192.168.1.1)
        uci set network.lan.ipaddr=172.17.17.1
        uci commit network
		;;
     *)
		echo "ok fine"
		;;
esac

exit 0

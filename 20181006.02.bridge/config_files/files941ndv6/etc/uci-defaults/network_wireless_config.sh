#!/bin/sh
ssid=$(uci get wireless.@wifi-iface[0].ssid)
wireless_diabled=$(uci get wireless.@wifi-device[0].disabled)
sqm_config="/etc/config/sqm";

case "$ssid" in
    *OpenWrt*)
	#setup sqm
	if [ -f "$sqm_config" ]
	then
		uci set sqm.eth1.interface="$(uci get network.wan.ifname)"
		uci set sqm.eth1.enabled='1'
		uci set sqm.eth1.download=20000
		uci set sqm.eth1.upload=20000
		uci commit sqm
	fi
        uci set wireless.@wifi-device[0].disabled=0
        uci set wireless.@wifi-iface[0].ssid='Wifi Free'
        uci set wireless.@wifi-iface[0].encryption='none'
        uci set wireless.@wifi-iface[0].key='12345678'
	
	uci set wireless.radio0.hwmode='11n'
	uci set wireless.radio0.htmode='HT40'
	uci set wireless.radio0.noscan='1'
	uci set wireless.radio0.bursting='1'
	uci set wireless.radio0.ff='1'
	uci set wireless.radio0.compression='1'
	uci set wireless.radio0.turbo='1'

	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
	echo "dhcp-script=/root/detect_new_device.sh" >> /etc/dnsmasq.conf
	;;
    *Openwrt*)
	#setup sqm
	if [ -f "$sqm_config" ]
	then
		uci set sqm.eth1.interface="$(uci get network.wan.ifname)"
		uci set sqm.eth1.enabled='1'
		uci set sqm.eth1.download=20000
		uci set sqm.eth1.upload=20000
		uci commit sqm
	fi
        uci set wireless.@wifi-device[0].disabled=0
        uci set wireless.@wifi-iface[0].ssid='Wifi Free'
        uci set wireless.@wifi-iface[0].encryption='none'
        uci set wireless.@wifi-iface[0].key='12345678'
	
	uci set wireless.radio0.hwmode='11n'
	uci set wireless.radio0.htmode='HT40'
	uci set wireless.radio0.noscan='1'
	uci set wireless.radio0.bursting='1'
	uci set wireless.radio0.ff='1'
	uci set wireless.radio0.compression='1'
	uci set wireless.radio0.turbo='1'

	uci set system.@system[0].timezone='ICT-7'
        uci commit wireless
	uci commit system
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

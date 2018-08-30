//
//  temp_str.h
//  TestC
//
//  Created by Mac One on 6/20/18.
//  Copyright © 2018 Mac One. All rights reserved.
//

#ifndef temp_str_h
#define temp_str_h

/* Result ssid.html
 challenge:urygffj$30e3dby36gzk
 hash:8cc29d0ccfc04302ed7c6f462acec039
 option fw_upgrade 0
 option ssid_update 0
 option ssid ' Cao Quán T2 -WIADS'
 option safe_sleep 0
 option reset_to_default 0
 option update_uamdomains 0
 HS_UAMDOMAINS='.wiads.vn,.hotspotwifisystem.com,meganet.com.vn,.meganet.com.vn,.valuepotion.com,valuepotion.com,daumcdn.net,.daumcdn.net,bs.serving-sys.com,junoteam.com,.junoteam.com,.facebook.com,.facebook.net,.akamaitechnologies.com,.akamaihd.net,.akamaiedge.net,.akamaitechnologies.com,.fbcdn.net,.gstatic.com,.android.com,.google-analytics.com'
 option update_uamformat 0
 HS_UAMFORMAT=http://enter.wiads.vn/ap
 option update_uamhomepage 0
 HS_UAMHOMEPAGE=
 option update_macauth 0
 HS_MACAUTH=off
 option htmode_update 0
 
 option hwmode_update 0
 
 option noscan_update 0
 
 option encryption_update 0
 option encryption 'psk2'
 option disabled 0
 option key_update 0
 option network_lan_update 0
 option network_lan '172.16.16.1'
 option hosts_update 0
 125.212.233.15 enter.wiads.vn
 option key '88888888'
 option need_reboot 0
 option normal_mode 0
 
 0
*/

/*test html
http://enter.wiads.vn/ap/ssid.html?app_mac=EC-08-6B-6E-E1-0D&mac=EC-08-6B-6E-E1-0D&mac=EC-08-6B-6E-E1-0D&v=20160625.02&hw=machine : TP-LINK TL-WR741ND v4&ssid=option ssid "Cafe Phố Đà Thành 4-WiAds"&justboot=0&uamdomains=HS_UAMDOMAINS=".yimg.com,.googlesyndication.com,.doubleclick.net,.google-analytics.com,.facebook.com,.facebook.net,.hotspotwifisystem.com,.wiads.vn,.cococ.com,.akamaitechnologies.com,.akamaihd.net,.akamaiedge.net,.akamaitechnologies.com,.fbcdn.net"&uamformat=HS_UAMFORMAT=http://http://125.212.233.15/ap&uamhomepage=HS_UAMHOMEPAGE= #HS_UAMHOMEPAGE=http://$HS_UAMLISTEN:$HS_UAMPORT/www/connect.html&macauth=HS_MACAUTH=off&htmode= option htmode HT20&hwmode= option hwmode '11n'&channel=option channel 4&noscan= option noscan '0'&encryption=option encryption psk2&key=option key "20112014"&wifi_status=&iwinfo=wlan0 ESSID: "Cafe Phố Đà Thành 4-WiAds" Access Point: EC:08:6B:6E:E1:0E Mode: Master Channel: 4 (2.427 GHz) Tx-Power: 18 dBm Link Quality: unknown/70 Signal: unknown Noise: -93 dBm Bit Rate: unknown Encryption: WPA2 PSK (CCMP) Type: nl80211 HW Mode(s): 802.11bgn Hardware: unknown [Generic MAC80211] TX power offset: unknown Frequency offset: unknown Supports VAPs: yes PHY name: phy0 Cell 01 - Address: 9C:D6:43:CF:6D:64 ESSID: "Cafe Pho Da Thanh 3" Mode: Master Channel: 1 Signal: -10 dBm Quality: 70/70 Encryption: mixed WPA/WPA2 PSK (TKIP, CCMP) Cell 02 - Address: E8:94:F6:CA:40:00 ESSID: "Cafe Pho Da Thanh 1" Mode: Master Channel: 2 Signal: -17 dBm Quality: 70/70 Encryption: WPA2 PSK (CCMP) Cell 03 - Address: 5C:0E:8B:80:C7:13 ESSID: unknown Mode: Master Channel: 6 Signal: -69 dBm Quality: 41/70 Encryption: mixed WPA/WPA2 PSK (TKIP, CCMP) Cell 04 - Address: F4:F2:6D:9A:2A:68 ESSID: "Cafe Pho Da Thanh 2" Mode: Master Channel: 11 Signal: -256 dBm Quality: 70/70 Encryption: WPA2 PSK (CCMP)&challenge=challenge&hash=&secret_hash_key=&
*/

char* build_url_query_wiads_ssid() {
    char * url = (char*)malloc(4096);
    char * base_url = "http://enter.wiads.vn/ap/ssid.html?";
    //Params Value
    char * ap_mac = "EC-08-6B-6E-E1-0D";
    char * mac = "EC-08-6B-6E-E1-0D";
    char * v= "20160625.02";//Firmware version
    char * hw="machine : TP-LINK TL-WR741ND v4";//Hardware info machine:TP-LINK TL-W941N/ND v6
    char * ssid="option ssid \"Cafe Phố Đà Thành 4-WiAds\"";
    char * justboot="0";
    char * uamdomains="HS_UAMDOMAINS=\".yimg.com,.googlesyndication.com,.doubleclick.net,.google-analytics.com,.facebook.com,.facebook.net,.hotspotwifisystem.com,.wiads.vn,.cococ.com,.akamaitechnologies.com,.akamaihd.net,.akamaiedge.net,.akamaitechnologies.com,.fbcdn.net\"";
    char * uamformat="HS_UAMFORMAT=http://http://125.212.233.15/ap";
    char * uamhomepage="HS_UAMHOMEPAGE= HS_UAMHOMEPAGE=http://\$HS_UAMLISTEN:\$HS_UAMPORT/www/connect.html";
    char * macauth="HS_MACAUTH=off";//HS_MACAUTH=off cua coovachilli
    char * htmode=" option htmode HT20";//HTMODE cua wireless
    char * hwmode=" option hwmode '11n'";//HWMODE cua wireless
    char * channel="option channel 4";
    char * noscan=" option noscan '0'";//noscan cua wireless
    char * encryption="option encryption psk2";
    char * key="option key \"20112014\"";
    char * wifi_status="";
    char * iwinfo="wlan0 ESSID: \"Cafe Phố Đà Thành 4-WiAds\" Access Point: EC:08:6B:6E:E1:0E Mode: Master Channel: 4 (2.427 GHz) Tx-Power: 18 dBm Link Quality: unknown/70 Signal: unknown Noise: -93 dBm Bit Rate: unknown Encryption: WPA2 PSK (CCMP) Type: nl80211 HW Mode(s): 802.11bgn Hardware: unknown [Generic MAC80211] TX power offset: unknown Frequency offset: unknown Supports VAPs: yes PHY name: phy0 Cell 01 - Address: 9C:D6:43:CF:6D:64 ESSID: \"Cafe Pho Da Thanh 3\" Mode: Master Channel: 1 Signal: -10 dBm Quality: 70/70 Encryption: mixed WPA/WPA2 PSK (TKIP, CCMP) Cell 02 - Address: E8:94:F6:CA:40:00 ESSID: \"Cafe Pho Da Thanh 1\" Mode: Master Channel: 2 Signal: -17 dBm Quality: 70/70 Encryption: WPA2 PSK (CCMP) Cell 03 - Address: 5C:0E:8B:80:C7:13 ESSID: unknown Mode: Master Channel: 6 Signal: -69 dBm Quality: 41/70 Encryption: mixed WPA/WPA2 PSK (TKIP, CCMP) Cell 04 - Address: F4:F2:6D:9A:2A:68 ESSID: \"Cafe Pho Da Thanh 2\" Mode: Master Channel: 11 Signal: -256 dBm Quality: 70/70 Encryption: WPA2 PSK (CCMP)";//Lay ra bang command iwinfo
    char * challenge="challenge";
    char * hash="";
    char * secretkey="";
    /*
     char * key_ap_mac = "ap_mac";
     char * key_mac = "mac";
     char * key_v="v";//Firmware version
     char * key_hw="hw";//Hardware info machine:TP-LINK TL-W941N/ND v6
     char * key_ssid="ssid";
     char * key_justboot="justboot";
     char * key_uamdomains="uamdomains";
     char * key_uamformat="uamformat";
     char * key_uamhomepage="uamhomepage";
     char * key_macauth="macauth";//HS_MACAUTH=off cua coovachilli
     char * key_htmode="htmode";//HTMODE cua wireless
     char * key_hwmode="hwmode";
     char * key_channel="channel";
     char * key_noscan="noscan";//noscan cua wireless
     char * key_encryption="encryption";
     char * key_key="key";
     char * key_wifi_status="wifi_status";
     char * key_iwinfo="iwinfo";//Lay ra bang command iwinfo
     char * key_challenge="challenge";
     char * key_hash="hash";
     char * key_secretkey="secret_hash_key";
     */
    sprintf(url, "%s", base_url);
    sprintf(url, "%s%s=%s&", url, "app_mac", ap_mac);
    sprintf(url, "%s%s=%s&", url, "mac", mac);
    sprintf(url, "%s%s=%s&", url, "v", v);
    /*
     sprintf(url, "%s%s=%s&", url, "hw", hw);
     sprintf(url, "%s%s=%s&", url, "ssid", ssid);
     sprintf(url, "%s%s=%s&", url, "justboot", justboot);
     sprintf(url, "%s%s=%s&", url, "uamdomains", uamdomains);
     sprintf(url, "%s%s=%s&", url, "uamformat", uamformat);
     sprintf(url, "%s%s=%s&", url, "uamhomepage", uamhomepage);
     sprintf(url, "%s%s=%s&", url, "macauth", macauth);
     sprintf(url, "%s%s=%s&", url, "htmode", htmode);
     sprintf(url, "%s%s=%s&", url, "hwmode", hwmode);
     sprintf(url, "%s%s=%s&", url, "channel", channel);
     sprintf(url, "%s%s=%s&", url, "noscan", noscan);
     sprintf(url, "%s%s=%s&", url, "encryption", encryption);
     sprintf(url, "%s%s=%s&", url, "key", key);
     sprintf(url, "%s%s=%s&", url, "wifi_status", wifi_status);
     sprintf(url, "%s%s=%s&", url, "iwinfo", iwinfo);
     sprintf(url, "%s%s=%s&", url, "challenge", challenge);
     sprintf(url, "%s%s=%s&", url, "hash", hash);
     sprintf(url, "%s%s=%s&", url, "secret_hash_key", secretkey);
     */
    url = str_replace(" ", "%20", url);
    return url;
}

#endif /* temp_str_h */

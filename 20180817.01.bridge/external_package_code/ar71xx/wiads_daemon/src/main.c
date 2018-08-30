//
//  main.c
//  TestC
//
//  Created by Mac One on 6/14/18.
//  Copyright © 2018 Mac One. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "http-client-c.h"
#include "common_utils.h"
#include "main.h"
#include <unistd.h>

/*=====Begin Variables==============*/
int reboot_after_update = 0;
int reset_after_update = 0;
char * mac_address = NULL;
char * arg_justboot = "0";
/*=====End Variables======================================================*/

/*=====Begin Define function========*/
//char * run_command(char * cmd);
char * build_url_query_wiads_ssid();
char * build_params_wiads_ssid();
/*=====End Define function=================================================*/

/*=====Begin Function test==========*/
int test() {
//    file_replace_str("/Users/macone/Documents/data/appinasia/wiads/ProjectC/TestC/data/test.txt", "172.16.116.157", "127.0.0.1");
//    char * test = "test strstr xem no la gi";
//    char * p = strstr(test, "strstr");
//    printf("%s\n", p);
//    file_change_key_value("/Users/macone/Documents/data/appinasia/wiads/ProjectC/TestC/data/test.txt", "option channel", "option channel '13'");
    
//    printf("Start With %d\n", str_start_with("HS_UAMFORMAT=http://enter.wiads.vn/ap", "HS_UAMFORMAT"));
    
//    file_change_hosts("/Users/macone/Documents/data/appinasia/wiads/ProjectC/TestC/data/etc/hosts", "enter.wiads.vn", "125.212.233.15 enter.wiads.vn");
    printf("Hwmachine : %s\n", get_hw_machine());
//    printf("Mac Address : %s\n", get_mac_address());
//    printf("SSID : %s\n", get_ssid());
//    printf("Htmode : %s\n", get_htmode());
//    printf("Hwmode : %s\n", get_hwmode());
//    printf("Encryption : %s\n", get_encryption());
//    printf("Key : %s\n", get_key());
//    printf("Channel : %s\n", get_channel());
//	file_change_key_value(FILE_WIRELESS_CONFIG, "option ssid", "option ssid 'PiPoGame 3Arena1'");
//	char * params = build_params_wiads_ssid();
//	struct http_response *hresp = http_post("http://testapi.wiads.net/ws/app/ap_config", NULL, params);
//	char * http_response = hresp->body;
    return 0;
}

/*=====End Function test========================================================*/

/*=====BEGIN MAIN FUNCTION========*/
/*
int main(int argc, const char * argv[]) {
//    test();
	testHttpResponse();
}
*/

int main(int argc, const char * argv[]) {
	printf("Wiads start load config .....\n");
	mac_address = get_mac_address();
	if (argc >= 2) {
		arg_justboot = argv[1];
	}
	if (str_equal(arg_justboot, "1")) {
		printf("-First Run /usr/bin/wiads_daemon\n");
		system("/etc/init.d/chilli stop");
		change_default_network_lan_ipaddr();
		sleep(5);
	}
    char * params = build_params_wiads_ssid();
    struct http_response *hresp = http_post("http://enter.wiads.vn/ap/ssid.html", NULL, params);
//    struct http_response *hresp = http_get("http://52.74.27.142/wiads_test_app/ws/app/ap_config", NULL);
//    struct http_response *hresp = http_post("http://testapi.wiads.net/ws/app/ap_config", NULL, params);
    char * http_response = hresp->body;
    if (_DEBUG_) {
    	printf("==================================== \n");
    }
    struct splits_array * options = str_splits(http_response, "\r\n");
    for (int i = 0; i < options->arr_len; i++) {
    	char* content_option = options->arr[i];
    	if (_DEBUG_) {
			printf("\n");
			printf("===Option %d : %s\n", i, content_option);
    	}
    	int is_str_start_with_option = str_start_with(content_option, "option");
        if (is_str_start_with_option > 0) {
            struct option_config * opt_config = parse_string_to_option_config(content_option);
            if (opt_config != NULL) {
            	if (_DEBUG_) {
            		printf("Option key %s, update %d\n", opt_config->key, opt_config->update);
            	}
                if (str_equal(opt_config->key, "fw_upgrade")) {
                    update_firmware(opt_config);
                }
                else if (str_equal(opt_config->key, "ssid_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option ssid", "ssid_update");
                    opt_config->update_content = update_content;
                    update_ssid(opt_config);
                }
                else if (str_equal(opt_config->key, "reset_to_default")) {
                    update_reset_to_default(opt_config);
                }
                else if (str_equal(opt_config->key, "update_uamdomains")) {
                    char * update_content = get_string_startwith_in_array(options->arr, options->arr_len, "HS_UAMDOMAINS");
                    if (update_content != NULL) {
						opt_config->update_content = update_content;
						update_uamdomains(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "update_uamformat")) {
                    char * update_content = get_string_startwith_in_array(options->arr, options->arr_len, "HS_UAMFORMAT");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_uamformat(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "update_uamhomepage")) {
                    char * update_content = get_string_startwith_in_array(options->arr, options->arr_len, "HS_UAMHOMEPAGE");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_uamhomepage(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "update_macauth")) {
                    char * update_content = get_string_startwith_in_array(options->arr, options->arr_len, "HS_MACAUTH");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_macauth(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "htmode_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option htmode", "htmode_update");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_htmode(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "hwmode_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option hwmode", "hwmode_update");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_hwmode(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "channel_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option channel", "channel_update");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_channel(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "noscan_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option noscan", "noscan_update");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_noscan(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "encryption_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option encryption", "encryption_update");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_encryption(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "disabled")) {
                    update_disabled(opt_config);
                }
                else if (str_equal(opt_config->key, "key_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option key", "key_update");
                    if (update_content != NULL) {
                    	opt_config->update_content = update_content;
                    	update_key(opt_config);
                    }
                }
                else if (str_equal(opt_config->key, "network_lan_update")) {
                    char * update_content = get_string_contain_a_not_b(options->arr, options->arr_len, "option network_lan", "network_lan_update");
                    if (update_content != NULL) {
                        char* update_str = str_dup(update_content);
                        struct splits_array * splits = str_splits(update_str, " ");
                        opt_config->update_content = str_cat("option ipaddr ", splits->arr[2]);
                        update_network_lan(opt_config);
                        free(update_str);
                    }
                }
                else if (str_equal(opt_config->key, "hosts_update")) {
                    char * update_content = options->arr[i+1];
                    opt_config->update_content = update_content;
                    update_hosts(opt_config);
                }
                else if (str_equal(opt_config->key, "need_reboot")) {
                    update_reboot(opt_config);
                }
                else if (str_equal(opt_config->key, "normal_mode")) {
                    update_normal_mode(opt_config);
                }
                option_config_free(opt_config);
            }
        }
    }

    if (reboot_after_update) {
        run_command("reboot");
        reboot_after_update = 0;
    }
    if (reset_after_update) {
    	run_command("firstboot");
    	reset_after_update = 0;
    }
    printf("Wiads finish load config .....\n");
    //Confirm finish updated
//    char* api_finish_updated_url = (char*)malloc(128);
//    strcat(api_finish_updated_url, "http://testapi.wiads.net/ws/app/ap_config/finish_update?ap_mac=");
//    strcat(api_finish_updated_url, mac_address);
//    api_finish_updated_url = strstr(api_finish_updated_url, "http");
//    struct http_response *hresp_get = http_get(api_finish_updated_url, NULL);
//    printf("%s\n", hresp_get->body);
    //Free Memory
    if (params != NULL) {
        free(params);
        params = NULL;
    }
    if (hresp != NULL) {
    	http_response_free(hresp);
        hresp = NULL;
    }
    if (options != NULL) {
    	splits_array_free(options);
        options = NULL;
    }

    return 0;
}

/*=====END MAIN FUNCTION========================================================*/

//=========================================
//BEGIN FUNCTION
//=========================================

char* build_params_wiads_ssid() {
    char * url = (char*)malloc(4096);
    //Params Value
    char * ap_mac = mac_address/*get_mac_address()*//*"EC-08-6B-6E-E1-74"*/;
    char * mac = mac_address/*get_mac_address()*//*"EC-08-6B-6E-E1-74"*/;
    char * v= get_version()/*"20170924.01.bridge"*/;//Firmware version
    char * hw= get_hw_machine();
    printf("HW : %s\n", hw);
    char * ssid= get_ssid();
    char * justboot=arg_justboot;
    char * uamdomains= get_uamdomains();/*"HS_UAMDOMAINS='.wiads.vn,.hotspotwifisystem.com,meganet.com.vn,.meganet.com.vn,.valuepotion.com,valuepotion.com,daumcdn.net,.daumcdn.net,bs.serving-sys.com,junoteam.com,.junoteam.com,.facebook.com,.facebook.net,.akamaitechnologies.com,.akamaihd.net,.akamaiedge.net,.akamaitechnologies.com,.fbcdn.net,.gstatic.com,.android.com,.google-analytics.com'";*/
    char * uamformat=get_uamformat();/*"HS_UAMFORMAT=http://enter.wiads.vn/ap";*/
    char * uamhomepage=get_uamhomepage();/*"HS_UAMHOMEPAGE= HS_UAMHOMEPAGE=http://\$HS_UAMLISTEN:\$HS_UAMPORT/www/connect.html";*/
    char * macauth=get_macauth();/*"HS_MACAUTH=off";*///HS_MACAUTH=off cua coovachilli
    char * htmode= get_htmode();//HTMODE cua wireless
    char * hwmode= get_hwmode();//HWMODE cua wireless
    char * channel= get_channel();
    char * noscan=" option noscan '0'";//noscan cua wireless
    char * encryption= get_encryption();
    char * key= get_key();
    char * wifi_status="";
    char * iwinfo= get_iwinfo();//Lay ra bang command iwinfo
    char * network_lan = get_network_lan();
    char * challenge="challenge";
    char * hash="";
    char * secretkey="";

//    http_params_append(url, "ap_mac", ap_mac);
    strcpy(url, "ap_mac");
    strcat(url, "=");
    strcat(url, ap_mac);
    strcat(url, "&");
    http_params_append(url, "mac", mac);
    http_params_append(url, "v", v);
    http_params_append(url, "hw", hw);
    http_params_append(url, "ssid", ssid);
    http_params_append(url, "justboot", justboot);
    http_params_append(url, "uamdomains", uamdomains);
    http_params_append(url, "uamformat", uamformat);
    http_params_append(url, "uamhomepage", uamhomepage);
    http_params_append(url, "macauth", macauth);
    http_params_append(url, "htmode", htmode);
    http_params_append(url, "hwmode", hwmode);
    http_params_append(url, "channel", channel);
    http_params_append(url, "noscan", noscan);
    http_params_append(url, "encryption", encryption);
    http_params_append(url, "key", key);
    http_params_append(url, "wifi_status", wifi_status);
    http_params_append(url, "iwinfo", iwinfo);
    http_params_append(url, "network_lan", network_lan);
    http_params_append(url, "challenge", challenge);
    http_params_append(url, "hash", hash);
    http_params_append(url, "secret_hash_key", secretkey);
    return url;
}

void update_firmware(struct option_config* opt_config) {
	if (opt_config->update >= 1) {
	        //link get firmware
	        char * firmware_url = (char*)malloc(128);
	        strcpy(firmware_url, "http://enter.wiads.vn/ap/fw.html?mac=");
	        strcat(firmware_url, mac_address);
	        char * wget_firmware_url = http_get_302_redirecturl(firmware_url, NULL);
	        //wget and save file to /tmp/firmware.bin
	        char * wget_cmd = (char*)malloc(256);
	        strcpy(wget_cmd, "wget ");
	        strcat(wget_cmd, wget_firmware_url);
	        strcat(wget_cmd, " -O /tmp/firmware.bin");
	        printf("wget_cmd %s\n", wget_cmd);
	        system(wget_cmd);
	        //upgrade firmware
	        char * upgrade_cmd = (char*)malloc(256);
	        if (opt_config->update == 1) {
	            strcpy(upgrade_cmd, "/sbin/sysupgrade");
	            strcat(upgrade_cmd, " /tmp/firmware.bin");
	        } else if (opt_config->update == 2) {
	            strcpy(upgrade_cmd, "/sbin/sysupgrade");
	            strcat(upgrade_cmd, " -n /tmp/firmware.bin");
	        }
	        system(upgrade_cmd);
	    }
}

void update_ssid(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
    	if (_DEBUG_) {
    		printf("Change File %s : %s\n", FILE_WIRELESS_CONFIG, opt_config->update_content);
    	}
    	file_change_key_value(FILE_WIRELESS_CONFIG, "option ssid", opt_config->update_content);
    }
}

void update_reset_to_default(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reset_after_update = 1;
//        run_command("firstboot");
    }
}

void update_uamdomains(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_CHILLI_CONFIG, "HS_UAMDOMAINS", opt_config->update_content);
    }
}

void update_uamformat(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_CHILLI_CONFIG, "HS_UAMFORMAT", opt_config->update_content);
    }
}

void update_uamhomepage(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_CHILLI_CONFIG, "HS_UAMHOMEPAGE", opt_config->update_content);
    }
}

void update_macauth(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_CHILLI_CONFIG, "HS_MACAUTH", opt_config->update_content);
    }
}

void update_htmode(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_WIRELESS_CONFIG, "option htmode", opt_config->update_content);
    }
}

void update_hwmode(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_WIRELESS_CONFIG, "option hwmode", opt_config->update_content);
    }
}

void update_channel(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_WIRELESS_CONFIG, "option channel", opt_config->update_content);
    }
}

void update_noscan(struct option_config* opt_config) {
    
}

void update_encryption(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_WIRELESS_CONFIG, "option encryption", opt_config->update_content);
    }
}

void update_key(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value(FILE_WIRELESS_CONFIG, "option key", opt_config->update_content);
    }
}

void update_network_lan(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        file_change_key_value_after_segment(FILE_NETWORK_CONFIG, "option ipaddr", opt_config->update_content, "config interface 'lan'");
    }
}

void update_hosts(struct option_config* opt_config) {
    if (opt_config->update == 1) {
    	reboot_after_update = 1;
        char* update_str = strdup(opt_config->update_content);
        struct splits_array * splits = str_splits(update_str, " ");
        char* host_name = splits->arr[1];
        file_change_hosts(FILE_HOSTS, host_name, opt_config->update_content);
        free(update_str);
    }
}

void update_reboot(struct option_config* opt_config) {
    if (opt_config->update == 1) {
//        run_command("reboot");
    	reboot_after_update = 1;
    	if (_DEBUG_) {
    		printf("========Update Reboot==========");
    	}
    }
}

void update_disabled(struct option_config* opt_config) {
    
}

void update_normal_mode(struct option_config* opt_config) {
	if (opt_config->update == 1) {
		run_command(CMD_CHILLI_STOP);
	} else if (opt_config->update == 0) {
		if (atoi(arg_justboot) == 0) {
			run_command(CMD_CHILLI_START);
		}
	}
}

//================================================================
//END FUNCTION
//================================================================


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
char * api_url = "http://enter.wiads.vn/ap/ssid.html";
/*=====End Variables======================================================*/

/*=====Begin Define function========*/
//char * run_command(char * cmd);
char * build_url_query_wiads_ssid();
char * build_params_wiads_ssid();
char * build_params_wiads_newdevice(char* dhcpmasq_log);
/*=====End Define function=================================================*/

/*=====Begin Function test==========*/
int test() {
    printf("Hwmachine : %s\n", get_hw_machine());
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
	mac_address = get_mac_address();

	//
	if (argc >= 2) {
		arg_justboot = argv[1];
	}
	if (str_equal(arg_justboot, "1")) {
		printf("-First Run /usr/bin/wiads_daemon\n");
		system("/etc/init.d/chilli stop");
		/*system("/root/network_bridge_config.sh");*/
		change_default_network_lan_ipaddr();
		update_daemon_api_status("1", "-1");
		update_daemon_api_status("2", "-1");
		sleep(5);
	}

	//Xac dinh api_url. status = -1 : Chua ket noi, status = 0 : Dang ket noi, status = 1 : Ket noi success
	printf("Begin get get_api_url %s\n",mac_address);
	char * api1_url = get_api_url1();
	char * api2_url = get_api_url2();
	char * api1_status = get_api1_status();
	char * api2_status = get_api2_status();
	int indexApi = 1;
	if (str_equal(api1_status, "0")) {
		if(str_equal(api2_status, "0")) {
			//Neu ca 2 cung fail thi van dung cai 1, reset cai 2 de thu co hoi lan sau
			indexApi = 1;
			api_url = api1_url;
			update_daemon_api_status("2", "-1");
		} else {
			//Neu cai 2 dung thi dung cai 2
			indexApi = 2;
			api_url = api2_url;
		}
	} else {
		indexApi = 1;
		api_url = api1_url;
	}
	if (indexApi == 1) {
		update_daemon_api_status("1", "0");
	} else {
		update_daemon_api_status("2", "0");
	}

	//
	printf("Wiads start load config .....%s\n",api_url);
    char * params = build_params_wiads_ssid();
    struct http_response *hresp = http_post(api_url/*"http://enter.wiads.vn/ap/ssid.html"*/
    		, NULL, params);
    if (hresp == NULL) {
    	if (arg_justboot == 0) {
    		return 0;
    	} else {
    		api_url = api2_url;
    		hresp = http_post(api_url, NULL, params);
    		if (hresp == NULL) {
    			return 0;
    		}
    	}
    }

    //
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
                else if (str_equal(opt_config->key, "bw_profile")) {
                	char * update_content = get_string_startwith_in_array(options->arr, options->arr_len, "option bw");
                	printf("update_content : %s\n", update_content);
                	if (update_content != NULL) {
                		opt_config->update_content = update_content;
                		update_bw_profile(opt_config);
                	}
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

    if (indexApi == 1) {
    	update_daemon_api_status("1", "1");
    } else {
    	update_daemon_api_status("2", "1");
    }
    printf("Wiads finish load config .....\n");
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
    //
    printf("Wiads start load log dhcpmasq_log\n");
    char * log_device_url = NULL;
    if (indexApi == 1) {
    	log_device_url = "http://coreapi.wiads.net/ap/new_device_connect_wifi";
    } else {
    	log_device_url = "http://enter2.wiads.vn/ap/new_device_connect_wifi";
    }
    char * cur_time = current_time();
    int index_line = get_line_dhcplog_before_time(FILE_DHCPLOG, cur_time);
    char * content = get_content_dhcplog_before_line(FILE_DHCPLOG, index_line);
    char * params_dhcpmasq_log = build_params_wiads_newdevice(content);
    if (strlen(content) > 0) {
    	struct http_response *hresp_log = http_post(log_device_url, NULL, params_dhcpmasq_log);
    	if (hresp_log != NULL) {
    		delete_dhcplog_before_line(FILE_DHCPLOG, index_line);
    		free(hresp_log);
    		hresp_log = NULL;
    	}
    }
    if (params_dhcpmasq_log != NULL) {
    	free(params_dhcpmasq_log);
    	params_dhcpmasq_log = NULL;
    }
    if (content != NULL) {
    	free(content);
    	content = NULL;
    }
    printf("Wiads finish load log dhcpmasq_log\n");

    return 0;
}

/*=====END MAIN FUNCTION========================================================*/

//=========================================
//BEGIN FUNCTION
//=========================================

char* build_params_wiads_newdevice(char* dhcpmasq_log) {
	char * url = (char*)malloc(4096);
	strcpy(url, "mac");
	strcat(url, "=");
	strcat(url, mac_address);
	strcat(url, "&");
	http_params_append(url, "dhcpmasq_log", dhcpmasq_log);
	return url;
}

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
	        strcpy(firmware_url, "http://coreapi.wiads.net/ap/fw?mac=");
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
	        /*
	        if (opt_config->update == 1) {
	            strcpy(upgrade_cmd, "/sbin/sysupgrade");
	            strcat(upgrade_cmd, " /tmp/firmware.bin");
	        } else if (opt_config->update == 2) {
	            strcpy(upgrade_cmd, "/sbin/sysupgrade");
	            strcat(upgrade_cmd, " -n /tmp/firmware.bin");
	        }
	        */
	        strcpy(upgrade_cmd, "/sbin/sysupgrade");
	        strcat(upgrade_cmd, " -n /tmp/firmware.bin");
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
//		if (atoi(arg_justboot) == 0) {
			run_command(CMD_CHILLI_START);
//		}
	}
}

void update_daemon_api_status(char * indexApi, char * status) {
	if (str_equal(indexApi, "1")) {
		char * key = "option api1_status ";
		char * content_status = str_cat(key, status);
		file_change_key_value(FILE_WIADS_DAEMON_CONFIG, key, content_status);
	} else {
		char * key = "option api2_status ";
		char * content_status = str_cat(key, status);
		file_change_key_value(FILE_WIADS_DAEMON_CONFIG, key, content_status);
	}
}

void update_bw_profile(struct option_config* opt_config) {
	char * bw_profile_str = get_bw_profile();
	if (bw_profile_str == NULL) {
		return;
	}
	int current_bw_profile = atoi(bw_profile_str);
	int next_bw_profile = opt_config->update;
	if (next_bw_profile != current_bw_profile) {
		char * download_limit = "20000";
		char * upload_limit = "20000";
		if (next_bw_profile == 1 || next_bw_profile == 2 || next_bw_profile == 3 ||
				next_bw_profile == 4) {
			download_limit = "10000";
			upload_limit = "10000";
		} else if (next_bw_profile == 5 || next_bw_profile == 9) {
			download_limit = "5000";
			upload_limit = "5000";
		} else if (next_bw_profile == 6) {
			download_limit = "3000";
			upload_limit = "3000";
		} else if (next_bw_profile == 10) {
			download_limit = "2000";
			upload_limit = "2000";
		}
		char * set_sqm_down_cmd = (char*)malloc(128);
		char * set_sqm_up_cmd = (char*)malloc(128);
		strcpy(set_sqm_down_cmd, "uci set sqm.eth1.download=");
		strcat(set_sqm_down_cmd, download_limit);
		strcpy(set_sqm_up_cmd, "uci set sqm.eth1.upload=");
		strcat(set_sqm_up_cmd, upload_limit);
		system(set_sqm_down_cmd);
		system(set_sqm_up_cmd);
		system("uci commit sqm");
		system("/etc/init.d/sqm restart");

		file_change_key_value(FILE_WIADS_DAEMON_CONFIG, "option bw_profile", opt_config->update_content);
	}
}

//================================================================
//END FUNCTION
//================================================================


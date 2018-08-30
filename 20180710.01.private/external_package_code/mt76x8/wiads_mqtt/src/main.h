//
//  main.h
//  TestC
//
//  Created by Mac One on 6/21/18.
//  Copyright © 2018 Mac One. All rights reserved.
//

#ifndef main_h
#define main_h

#include "common_utils.h"

/*CONSTANT*/
#define BUFSIZE_RUN_COMMAND 128

const char * FILE_CHILLI_CONFIG = "/etc/chilli/defaults";
const char * FILE_WIRELESS_CONFIG = "/etc/config/wireless";
const char * FILE_NETWORK_CONFIG = "/etc/config/network";
const char * FILE_HOSTS = "/etc/hosts";
const char * FILE_ADDRESS = "/sys/class/net/wlan0/address";

const char CMD_GETMACADDR[] = "/root/get_macaddr.sh";
const char CMD_GET_MACHINE[] = "cat /proc/cpuinfo | grep 'machine'";
const char CMD_IWINFO[] = "iwinfo wlan0 info";
const char CMD_IWINFO_SCAN[] = "iwinfo wlan0 scan";

/*Config format 'option key value'*/
struct option_config {
    char * option_content;
    char * key;
    int update;
    char * update_content;
};

struct properties_config {
    char * key;
    char * value;
};

void option_config_free(struct option_config* opt_cfg);
void update_firmware(struct option_config*  opt_config);
void update_ssid(struct option_config* opt_config);
void update_reset_to_default(struct option_config* opt_config);
void update_uamdomains(struct option_config* opt_config);
void update_uamformat(struct option_config* opt_config);
void update_uamhomepage(struct option_config* opt_config);
void update_macauth(struct option_config* opt_config);
void update_htmode(struct option_config* opt_config);
void update_hwmode(struct option_config* opt_config);
void update_channel(struct option_config* opt_config);
void update_noscan(struct option_config* opt_config);
void update_encryption(struct option_config* opt_config);
void update_key(struct option_config* opt_config);
void update_network_lan(struct option_config* opt_config);
void update_hosts(struct option_config* opt_config);
void update_reboot(struct option_config* opt_config);
void update_disabled(struct option_config* opt_config);
void update_normal_mode(struct option_config* opt_config);

void option_config_free(struct option_config* opt_cfg) {
	if (opt_cfg != NULL) {
		free(opt_cfg);
	}
}

/* Run Command and Print Output*/
char * run_command(char * cmd) {
    char *output = (char*)malloc(4096);
    char buf[BUFSIZE_RUN_COMMAND];
    FILE *fp;
    
    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return "-1";
    }
    while (fgets(buf, BUFSIZE_RUN_COMMAND, fp) != NULL) {
/*
        if (output == NULL) {
            output = (char *) malloc((strlen(buf) + 1) * sizeof(char));
        } else {
            output = (char *) realloc(output, (strlen(buf) + strlen(output) + 1) * sizeof(char));
        }
*/
        strcat(output, buf);
    }
    if(pclose(fp))  {
        printf("Command not found or exited with error status\n");
        return "-1";
    }
    //printf("Run Command %s\n", cmd);
    //printf("Run Command Output %s\n", output);
    
    return output;
}

/*Parse option normal_mode 0 to struct option_config*/
struct option_config * parse_string_to_option_config(char * op_str) {
	trim(op_str);
    if (!str_start_with(op_str, "option")) {
        return NULL;
    }
    struct splits_array * splits = str_splits(op_str, " ");
    struct option_config * opc = (struct option_config*)malloc(sizeof(struct option_config));
    if (opc == NULL) {
    	return NULL;
    }
    opc->option_content = op_str;
    opc->key = NULL;
    opc->update = 0;
    opc->update_content = NULL;
    if (splits->arr_len >= 3) {
        opc->key = splits->arr[1];
        opc->update = atoi(splits->arr[2]);
    }
    return opc;
}

struct properties_config * parse_string_to_properties_config(char * str) {
    trim(str);
    if (!strstr(str, "=")) {
        return NULL;
    }
    struct splits_array * splits = str_splits(str, "=");
    struct properties_config * pcfg = (struct properties_config*)malloc(sizeof(struct properties_config));
    pcfg->key = NULL;
    pcfg->value = NULL;
    if (splits->arr_len >= 2) {
        pcfg->key = splits->arr[0];
        pcfg->value = splits->arr[1];
    }
    return pcfg;
}

/*127.0.0.1 enter.wiads.vn*/
struct properties_config * parse_hoststring_to_prop_config(char * str) {
    trim(str);
    if (!strstr(str, " ")) {
        return NULL;
    }
    struct splits_array * splits = str_splits(str, " ");
    struct properties_config * pcfg = (struct properties_config*)malloc(sizeof(struct properties_config));
    pcfg->key = NULL;
    pcfg->value = NULL;
    if (splits->arr_len >= 2) {
        pcfg->key = splits->arr[1]; //Host
        pcfg->value = splits->arr[0]; //IP
    }
    return pcfg;
}

char * get_hw_machine() {
    char * hw_machine = NULL;
    char * cmd_content = run_command(CMD_GET_MACHINE);
    struct splits_array * splits = str_splits(cmd_content, ":");
    if (splits->arr_len >= 2) {
        hw_machine = splits->arr[1];
        trim(hw_machine);
    }
    return hw_machine;
}

char * get_mac_address() {
    char * output = run_command(CMD_GETMACADDR);
    char * first_2dot = strstr(output, ":");
    int position = first_2dot - output - 1;
    char * mac_addr = str_substring(output, position, 17);
    return mac_addr;
}

char * get_ssid() {
    char * ssid = file_read_line_contain_key(FILE_WIRELESS_CONFIG, "option ssid");
    trim(ssid);
    return ssid;
}

char * get_htmode() {
    char * htmode = file_read_line_contain_key(FILE_WIRELESS_CONFIG, "option htmode");
    trim(htmode);
    return htmode;
}

char * get_hwmode() {
    char * hwmode = file_read_line_contain_key(FILE_WIRELESS_CONFIG, "option hwmode");
    trim(hwmode);
    return hwmode;
}

char * get_encryption() {
    char * encryption = file_read_line_contain_key(FILE_WIRELESS_CONFIG, "option encryption");
    trim(encryption);
    return encryption;
}

char * get_key() {
    char * key = file_read_line_contain_key(FILE_WIRELESS_CONFIG, "option key");
    trim(key);
    return key;
}

char * get_channel() {
    char * channel = file_read_line_contain_key(FILE_WIRELESS_CONFIG, "option channel");
    trim(channel);
    return channel;
}

char * get_iwinfo() {
    char * iwinfo = (char*)malloc(0);
    char * space = " ";
    //Info
    char * info_output = run_command(CMD_IWINFO);
    //char * info_output = strstr(output1, "wlan0");
    struct splits_array * splits = str_splits(info_output, "\n");
    for (int i = 0; i < splits->arr_len; i++) {
        char * s = splits->arr[i];
        trim(s);
        iwinfo = (char *)realloc(iwinfo, strlen(iwinfo) + strlen(s) + strlen(space) + 1);
        strcat(iwinfo, s);
        strcat(iwinfo, space);
    }
    /*
    //Scan Cell 0, Cell 1, ...
    char * output2 = run_command(CMD_IWINFO_SCAN);
    char * scan_output = strstr(output2, "Cell");
    struct splits_array * splits_scan = str_splits(scan_output, "\n");
    for (int i = 0; i < splits_scan->arr_len; i++) {
        char * s = splits_scan->arr[i];
        trim(s);
        iwinfo = (char *)realloc(iwinfo, strlen(iwinfo) + strlen(s) + strlen(space) + 1);
        strcat(iwinfo, s);
        strcat(iwinfo, space);
    }
	*/
    return strstr(iwinfo, "wlan0");
}

#endif /* main_h */

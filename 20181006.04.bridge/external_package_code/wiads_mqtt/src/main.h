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

char * get_mac_address() {
    char * output = run_command(CMD_GETMACADDR);
    char * first_2dot = strstr(output, ":");
    int position = first_2dot - output - 1;
    char * mac_addr = str_substring(output, position, 17);
    return mac_addr;
}

#endif /* main_h */

//
//  main.c
//  TestMqtt
//
//  Created by Mac One on 7/6/18.
//  Copyright © 2018 Mac One. All rights reserved.
//

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <err.h>
#include <signal.h>

#include "mosquitto.h"
#include "main.h"
#include "common_utils.h"
#include "stringx.h"

/* The linked code creates a client that connects to a broker at
 * localhost:1883, subscribes to the topics "tick", "control/#{PID}",
 * and "control/all", and publishes its process ID and uptime (in
 * seconds) on "tock/#{PID}" every time it gets a "tick" message. If the
 * message "halt" is sent to either "control" endpoint, it will
 * disconnect, free its resources, and exit. */

#ifdef DEBUG
#define LOG(...) do { printf(__VA_ARGS__); } while (0)
#else
#define LOG(...)
#endif

/* How many seconds the broker should wait between sending out
 * keep-alive messages. */
#define KEEPALIVE_SECONDS 60
/*CONSTANT*/
#define BUFSIZE_RUN_COMMAND 128

/* Hostname and port for the MQTT broker. */
#define BROKER_HOSTNAME /*"localhost"*/"iot.wiads.vn"
#define BROKER_PORT 1883
/*
#define mqtt_host "iot.wiads.vn"
#define mqtt_port 1883
 */
#define username "wiads"
#define password "cnec!#)kJHVWDq#!"
#define secretkey='((_(@&(*#%(&KJBC(&OP{SC:FVGP)(!{HCJLHCF!)(:KGP(!GF`9`70198212y-192ye12hjvcshvdc$#@'

char * macaddr;

struct client_info {
    struct mosquitto *m;
    pid_t pid;
    uint32_t tick_ct;
};

static void die(const char *msg);
static struct mosquitto *init(struct client_info *info);
static bool set_callbacks(struct mosquitto *m);
static bool connect(struct mosquitto *m);
static int run_loop(struct client_info *info);
char * run_command(char * cmd);

static int run = 1;

void handle_signal(int s)
{
    run = 0;
}

int main(int argc, char **argv) {
    int secs = 5;
    if (argc == 2) {
    	secs = atoi(argv[1]);
    }
    /*
    sleep(secs);
    system("/usr/bin/wiads_daemon 1");
    */
    /*Get Mac Address to topic*/
    char * str_mac = get_mac_address();
    macaddr = str_replace(":", "-", str_mac);
    str_toupper(macaddr);
    printf("Mac Addr %s\n", macaddr);

    sleep(secs);
    //
    mosquitto_lib_init();
    pid_t pid = getpid();

    struct client_info info;
    memset(&info, 0, sizeof(info));
    info.pid = pid;
    
    struct mosquitto *m = init(&info);
    if (m == NULL) { die("init() failure\n"); }
    info.m = m;
    
    mosquitto_username_pw_set(m, username, password);
    
    if (!set_callbacks(m)) { die("set_callbacks() failure\n"); }
    
    if (!connect(m)) { die("connect() failure\n"); }
    
    return run_loop(&info);
}

/* Fail with an error message. */
static void die(const char *msg) {
    fprintf(stderr, "%s", msg);
    exit(1);
}

/* Initialize a mosquitto client. */
static struct mosquitto *init(struct client_info *info) {
    void *udata = (void *)info;
    size_t buf_sz = 32;
    char buf[buf_sz];
    if (buf_sz < snprintf(buf, buf_sz, "client_%d", info->pid)) {
        return NULL;            /* snprintf buffer failure */
    }
    /* Create a new mosquitto client, with the name "client_#{PID}". */
    struct mosquitto *m = mosquitto_new(buf, true, udata);
    
    return m;
}

/* Callback for successful connection: add subscriptions. */
static void on_connect(struct mosquitto *m, void *udata, int res) {  
    if (res == 0) {             /* success */
        struct client_info *info = (struct client_info *)udata;
        /*
        mosquitto_subscribe(m, NULL, "tick", 0);
        mosquitto_subscribe(m, NULL, "control/all", 0);
        size_t sz = 32;
        char control_pid[sz];
        if (sz < snprintf(control_pid, sz, "control/%d", info->pid)) {
            die("snprintf\n");
        }
        mosquitto_subscribe(m, NULL, control_pid, 0);
        mosquitto_subscribe(m, NULL, "tick", 0);
         */
        mosquitto_subscribe(m, NULL, macaddr, 0);
    } else {
        die("connection refused\n");
    }
}

/* A message was successfully published. */
static void on_publish(struct mosquitto *m, void *udata, int m_id) {
    LOG("-- published successfully\n");
}

static void on_disconnect(struct mosquitto *m, void *obj, int res) {
    LOG("-- disconnect reason %d, Stop loop....\n", res);
    run = 0;
}

static bool match(const char *topic, const char *key) {
    return 0 == strncmp(topic, key, strlen(key));
}

/* Handle a message that just arrived via one of the subscriptions. */
static void on_message(struct mosquitto *m, void *udata,
                       const struct mosquitto_message *msg) {
    if (msg == NULL) { return; }
    LOG("-- got message @ %s: (%d, QoS %d, %s) '%s'\n",
        msg->topic, msg->payloadlen, msg->qos, msg->retain ? "R" : "!r",
        msg->payload);
    
    struct client_info *info = (struct client_info *)udata;
    
    if (match(msg->topic, "tick")) {
        if (0 == strncmp(msg->payload, "tick", msg->payloadlen)) {
            LOG("tock %d\n", info->pid);
            size_t sz = 32;
            char tock_pid[sz];
            info->tick_ct++;
            if (sz < snprintf(tock_pid, sz, "tock/%d", info->pid)) {
                die("snprintf\n");
            }
            
            size_t payload_sz = 32;
            char payload[payload_sz];
            size_t payloadlen = 0;
            payloadlen = snprintf(payload, payload_sz, "tock %d %d",
                                  info->pid, info->tick_ct);
            if (payload_sz < payloadlen) {
                die("snprintf\n");
            }
            int res = mosquitto_publish(m, NULL, tock_pid,
                                        payloadlen, payload, 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                die("publish\n");
            }
        } else {
            LOG("invalid 'tick' message\n");
        }
    } else if (match(msg->topic, "control/")) {
        /* This will cover both "control/all" and "control/$(PID)".
         * We won'st see "control/$(OTHER_PID)" because we won't be
         * subscribed to them.*/
        if (0 == strncmp(msg->payload, "halt", msg->payloadlen)) {
            LOG("*** halt\n");
            (void)mosquitto_disconnect(m);
        }
    } else if (match(msg->topic, macaddr)) {
        if (0 == strncmp(msg->payload, "request", msg->payloadlen)) {
            LOG("Run /usr/bin/wiads_daemon\n");
            system("/usr/bin/wiads_daemon");
        } else {
        	char * cmd_content = msg->payload;
        	if (str_start_with(cmd_content, "wiads_run_command")) {
        		char *cmd = str_replace("wiads_run_command", " ", cmd_content);
        		trim(cmd);
        		system(cmd);
        	}
        }
    }
}

/* Successful subscription hook. */
static void on_subscribe(struct mosquitto *m, void *udata, int mid,
                         int qos_count, const int *granted_qos) {
    LOG("-- subscribed successfully\n");
}

/* Register the callbacks that the mosquitto connection will use. */
static bool set_callbacks(struct mosquitto *m) {
    mosquitto_connect_callback_set(m, on_connect);
    mosquitto_publish_callback_set(m, on_publish);
    mosquitto_subscribe_callback_set(m, on_subscribe);
    mosquitto_message_callback_set(m, on_message);
    mosquitto_disconnect_callback_set(m, on_disconnect);
    return true;
}

/* Connect to the network. */
static bool connect(struct mosquitto *m) {
    int res = mosquitto_connect(m, BROKER_HOSTNAME, BROKER_PORT, KEEPALIVE_SECONDS);
    return res == MOSQ_ERR_SUCCESS;
}

/* Loop until it is explicitly halted or the network is lost, then clean up. */
static int run_loop_forever(struct client_info *info) {
    int res = mosquitto_loop_forever(info->m, 10, 10 /* unused */);
    mosquitto_destroy(info->m);
    (void)mosquitto_lib_cleanup();
    
    if (res == MOSQ_ERR_SUCCESS) {
        return 0;
    } else {
        return 1;
    }
}

static int run_loop(struct client_info *info) {
    uint8_t reconnect = true;
    int rc = 0;
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    while(run){
        rc = mosquitto_loop(info->m, -1, 1);
        if(run && rc){
            printf("connection error!\n");
            sleep(10);
            mosquitto_reconnect(info->m);
        }
    }
    mosquitto_destroy(info->m);
    (void)mosquitto_lib_cleanup();
    return rc;
}

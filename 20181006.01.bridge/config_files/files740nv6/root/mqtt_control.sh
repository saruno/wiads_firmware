#!/bin/sh              

# PATH TO YOUR HOSTS FILE
case "$1" in
  start)
        check=`/bin/pidof wiads_mqtt`
		if [ -z $check ]
		then
			/etc/init.d/wiads_mqtt start
		fi
        ;;
  stop)
        check=`/bin/pidof wiads_mqtt`
		if [ -z $check ]
		then
			echo "wiads_mqtt not running"
		else
			/etc/init.d/wiads_mqtt stop
			kill -9 $check
		fi
        ;;                                            
                                                      
  *)                                                  
        echo "Usage: "                                
        echo "mqtt_control.sh [start|stop]"
        echo                                        
        exit 1                                        
        ;;                                            
esac                                                  
                                                      
exit 0

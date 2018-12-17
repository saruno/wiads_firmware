#!/bin/sh              

# PATH TO YOUR HOSTS FILE
case "$1" in
  start)
        check=`/bin/pidof chilli`
	if [ -z $check ]
	then
		/root/chilli start
	fi	
        ;;
  stop)
        check=`/bin/pidof chilli`
	if [ -z $check ]
	then
		echo "===chilli_control.sh Chilli not running"
	else
		/root/chilli stop
		kill -9 $check
	fi

	check=`/bin/pidof /usr/sbin/chilli`
	if [ -z $check ]
	then
		echo "===chilli_control.sh Chilli not running"
	else
		/root/chilli stop
		kill -9 $check
	fi
        ;;                                            
                                                      
  *)                                                  
        echo "Usage: "                                
        echo "chilli_control.sh [start|stop]"
        echo                                        
        exit 1                                        
        ;;                                            
esac                                                  
                                                      
exit 0

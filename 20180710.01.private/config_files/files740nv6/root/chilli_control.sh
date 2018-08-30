#!/bin/sh              

# PATH TO YOUR HOSTS FILE
case "$1" in
  start)
        check=`/bin/pidof chilli`
		if [ -z $check ]
		then
			/etc/init.d/chilli start
		fi
        ;;
  stop)
        check=`/bin/pidof chilli`
		if [ -z $check ]
		then
			echo "Chilli not running"
		else
			/etc/init.d/chilli stop
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

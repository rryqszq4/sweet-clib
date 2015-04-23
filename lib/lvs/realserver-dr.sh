#! /bin/bash

WEB_VIP=192.168.80.100
. /etc/rc.d/init.d/functions

case "$1" in
start)
	ifconfig lo:0 $WEB_VIP netmask 255.255.255.255 broadcast $WEB_VIP up
	/sbin/route add -host $WEB_VIP dev lo:0
	#echo "1" >/proc/sys/net/ipv4/conf/lo/arp_ignore
	#echo "2" >/proc/sys/net/ipv4/conf/lo/arp_announce
	#echo "1" >/proc/sys/net/ipv4/conf/all/arp_ignore
	#echo "2" >/proc/sys/net/ipv4/conf/all/arp_announce
	#sysctl -p >/dev/null 2>&1
	echo "realserver start OK."
	;;
stop)
	ifconfig lo:0 down
	route del $WEB_VIP 
	#echo "0" >/proc/sys/net/ipv4/conf/lo/arp_ignore
        #echo "0" >/proc/sys/net/ipv4/conf/lo/arp_announce
        #echo "0" >/proc/sys/net/ipv4/conf/all/arp_ignore
        #echo "0" >/proc/sys/net/ipv4/conf/all/arp_announce
	echo "realserver stoped."
	;;
*)
	echo "Usage: $0 {start|stop}"
	exit 1;
	;;
	
esac
exit 0





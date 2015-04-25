#! /bin/bash

GW=192.168.80.2

WEB_VIP=192.168.80.100
WEB_RIP1=192.168.80.140
WEB_RIP2=192.168.80.141

. /etc/rc.d/init.d/functions

logger $0 called with $1
case "$1" in

start)
	#set lvs vip for dr
	/sbin/ifconfig eth0:0 $WEB_VIP broadcast $WEB_VIP netmask 255.255.255.255 up
	/sbin/route add -host $WEB_VIP dev eth0:0
	echo "1" > /proc/sys/net/ipv4/ip_forward
	/sbin/ipvsadm -C
	/sbin/ipvsadm --set 30 5 60
	/sbin/ipvsadm -A -t $WEB_VIP:80 -s wrr -p 30
	/sbin/ipvsadm -a -t $WEB_VIP:80 -r $WEB_RIP1:80 -g -w 1
	/sbin/ipvsadm -a -t $WEB_VIP:80 -r $WEB_RIP2:80 -g -w 1
	
	/sbin/ipvsadm
	;;

stop)
	echo "0" > /proc/sys/net/ipv4/ip_forward
	/sbin/ipvsadm -C
	ifconfig eth0:0 down
	#route del $WEB_VIP
	echo "ipvsadm stoped"

	;;

	
*)
	echo "Usage: $0 {start|stop}"
	exit 1

esac
exit 0






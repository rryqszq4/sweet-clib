#! /bin/bash

GW=192.168.80.2

WEB_VIP=192.168.80.100
WEB_RIP1=192.168.80.138
WEB_RIP2=192.168.80.140

. /etc/rc.d/init.d/functions

logger $0 called with $1
case "$1" in

start)
	#set lvs vip for dr
	/sbin/ifconfig eth0:0 $WEB_VIP broadcast $WEB_VIP netmask 255.255.255.255 up
	/sbin/route add -host $WEB_VIP dev eth0:0
	echo "1" >/proc/sys/net/ipv4/ip_forward
	/sbin/ipvsadm -C
	/sbin/ipvsadm -A -t 192.168.80.100:80 -s rr
	#/sbin/ipvsadm -a -t 192.168.80.100:80 -r 192.168.80.138:80 -g
	/sbin/ipvsadm -a -t 192.168.80.100:80 -r 192.168.80.140:80 -g
	
	/sbin/ipvsadm
	;;

stop)
	echo "0" >/proc/sys/net/ipv4/ip_forward
	/sbin/ipvsadm -C
	ifconfig eth0:0 down
	route del $WEB_VIP
	echo "ipvsadm stoped"

	;;

	
*)
	echo "Usage: $0 {start|stop}"
	exit 1

esac
exit 0






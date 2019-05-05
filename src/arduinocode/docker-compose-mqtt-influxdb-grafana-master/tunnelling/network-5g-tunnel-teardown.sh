#/bin/sh

FOU_REMOTE_NETWORK="10.200.1.1/24"
FOU_LOCAL_PORT=1883

ip addr del $FOU_REMOTE_NETWORK dev fou0
ip link set down dev fou0
ip link set down dev tunl0
ip link del name fou0
ip fou del port $FOU_LOCAL_PORT

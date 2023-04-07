#!/bin/sh

args_array="$@"
args=""
for i in ${args_array}
do :
   if [ "$i" = "$1" ]; then
      continue
   fi
   args="$args $i"
done

echo "Command: $1"
echo "Arguments: $args"

if [ -z $1 ]; then
    echo "Entry point is NOT set! Select one:"
    echo "   HttpBankingClient HttpBankingServer"
    echo "   TcpBankingClient TcpBankingServer"
    echo "   UdpBankingClient UdpBankingServer"
    exit 
else
    case $1 in
    'HttpBankingClient')
        ./HttpBankingClient $args
    ;;
    'HttpBankingServer')
        ./HttpBankingServer
    ;;
    'TcpBankingClient')
        ./HttpBankingClient $args
    ;;
    'TcpBankingServer')
        ./HttpBankingServer
    ;;
    'UdpBankingClient')
        ./HttpBankingClient $args
    ;;
    'UdpBankingServer')
        ./UdpBankingServer
    ;;
    esac
fi

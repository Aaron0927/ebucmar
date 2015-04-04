#! /bin/bash

ramcube=/opt/bin/memcached


$ramcube -u root -z Config-19-8.txt &

sleep 2s

$ramcube -u root -z Config-19-9.txt &


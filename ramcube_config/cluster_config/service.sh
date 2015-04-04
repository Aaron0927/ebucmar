#! /bin/bash

ramcube=/opt/bin/memcached


$ramcube -u root -z config_19_8.txt &
$ramcube -u root -z config_19_9.txt &

sleep 2s

$ramcube -u root -z config_21_21.txt &
$ramcube -u root -z config_21_22.txt &


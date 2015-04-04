#! /bin/bash

mcbenchmark=/root/mc-benchmark-linux/mc-benchmark

$mcbenchmark -n 10000 -c 50 -h 127.0.0.1 -p 11121 > Result_8.txt &
$mcbenchmark -n 10000 -c 50 -h 127.0.0.1 -p 11124 > Result_9.txt &


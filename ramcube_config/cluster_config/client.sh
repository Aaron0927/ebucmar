#! /bin/bash

mcbenchmark=/root/mc-benchmark-linux/mc-benchmark

$mcbenchmark -n 100000 -c 50 -h 10.107.19.8 -p 11111 > result_8.txt &
$mcbenchmark -n 100000 -c 50 -h 10.107.19.9 -p 11121 > result_9.txt &
$mcbenchmark -n 100000 -c 50 -h 10.107.21.21 -p 11131 > result_21.txt &
$mcbenchmark -n 100000 -c 50 -h 10.107.21.22 -p 11141 > result_22.txt &



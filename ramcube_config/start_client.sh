#! /bin/bash

mcbenchmark=/home/zhangchengfei/workspace/mc-benchmark-linux/mc-benchmark

$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11111 > result_11111.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11121 > result_11121.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11131 > result_11131.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11141 > result_11141.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11151 > result_11151.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11161 > result_11161.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11171 > result_11171.txt &
#$mcbenchmark -n 100000 -c 50 -h 127.0.0.1 -p 11181 > result_11181.txt &



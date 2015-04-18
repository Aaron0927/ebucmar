#!/bin/bash
ps -ef|grep memcached|grep -v grep|awk '{print $2}'|xargs kill -9
echo 'clean memcached'
ps -ef|grep mc-benchmark|grep -v grep|awk '{print $2}'|xargs kill -9
echo 'clean benchmark'

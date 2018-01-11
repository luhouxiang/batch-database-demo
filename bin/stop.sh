#!/bin/sh

killall -9 batch-database-demo

if [ `ps -fC batch-database-demo | wc -l` -gt 1 ] 
then echo "batch-database-demo: 模块停止失败"
else echo "batch-database-demo: 模块已停止"
fi

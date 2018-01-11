#!/bin/bash
#source /etc/profile
source /opt/kds/.bash_profile
export DEMO_HOME=/opt/kds/mobile-stock/batch-database-demo
cd $DEMO_HOME/bin
$DEMO_HOME/bin/batch-database-demo&

if [ `ps -fC batch-database-demo | wc -l` -gt 1 ]
then echo "batch-database-demo: 模块已启动"
else echo "batch-database-demo: 模块启动失败"
fi

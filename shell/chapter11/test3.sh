#!/bin/bash
#shell脚本中最有用的特性之一就是可以从命令输出中提取信息，并将其赋给变量。把输出赋
#给变量之后，就可以随意在脚本中使用了。这个特性在处理脚本数据时尤为方便。
#有两种方法可以将命令输出赋给变量： 反
#引号字符（`）
# $()格式`
testing=$(date)
echo "The date and time are: " $testing
testing=`date`
echo "The date and time are " $testing
today=$(date +%y%m%d)
ls /usr/bin -al > log.$today

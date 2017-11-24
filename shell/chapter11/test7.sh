#!/bin/bash
var1=100
var2=50
var3=45
# 用方括号执行shell数学运算比用 expr 命令方便很多.
# 同样，注意在使用方括号来计算公式时，不用担心shell会误解乘号或其他符号。shell知道它不是通配符，因为它在方括号内
var4=$[$var1*($var2+$var3)]
echo "the var4 is $var4"

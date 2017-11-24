#!/bin/bash
#echo -n "Enter a temperature in Celsius : "
#read celsius 
#if [[ $celsius =~ ^([-+]?[0-9]+)(\.[0-9]*)?$  ]]  #匹配变量全是由数字组成
#then
#		fah=$(echo "scale=2; $celsius * 9 / 5 + 32" | bc)
#		echo $fah
#else
#	echo "Expecting a number,so I don'n understand $celsius"
#fi
echo -n "继续输入 : "
read celsius2
if [[ $celsius2 =~ ^[-+]?([1-9][0-9]*|([0]))(\.[0-9]*)?([cfCF])?$   ]] 
then
	echo "匹配正确：$celsius2"
else
	echo "匹配不正确"
fi
	

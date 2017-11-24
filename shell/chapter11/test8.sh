#!/bin/bash
var1=$(echo "scale=4; 6.44 / 5" | bc)
echo The answer is $var1

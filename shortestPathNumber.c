/*======================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   Programmer :Linyuanpeng
* 	Date       :2018/01/23
*
========================================================================*/

#include<stdio.h>
int shortPathNumber(int row,int col)
{	
	int g_num = 0;
	if(row == 8 && col == 8)
	{
		return 1;
	}
	else if (row < 8 && col == 8)
	{
		g_num+=shortPathNumber(row+1,8);

	}else if (row == 8 && col < 8)
	{
		g_num+=shortPathNumber(8,col+1);
	}else{
		g_num+=shortPathNumber(row+1,col);
		g_num+=shortPathNumber(row,col+1);
	}

	return g_num;
}
int main(){
	int g_num = 0;
	g_num = shortPathNumber(0,0);
	printf("%d \n",g_num);
}

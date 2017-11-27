#include <stdio.h>
//指针的算术运算
void test2();
int main(){
	//char a[20]={'a','b','c','d','e','f','1','2','3','4','5','6','7','8','9','A','B','C','D','E'};
	//int i = 0;
	//for(;i<20;i++){
	//	printf("-------%c------  %p\n",a[i],a[i]);
	//}
	//int *ptr = (int *)a;//强制类型转换并不会改变a的类型
	//ptr++;
	//printf("ptr++ value %p \n",ptr);
	test2();

	return 0;
}
void test2(){
	int array[20] = {0};
	int *ptr = array;
	for(int i=0;i<20;i++){
		(*ptr)++;
		ptr++;
	}
	for(int i = 0;i<20;i++){
		printf("array value :%d\n",array[i]);
	}
}
void test3(){
	//，r ptr 被 加 上了 了 5 5 ， 编 译 器 是 这 样 处 理 的 ： 将 指针 针 r ptr 的
	//值 加上 上 5 5 乘 乘 sizeof(int) ，在 在 2 32 位 程 序 中 就 是 加 上了 了 5 5 乘 乘 4=20 。 由 于 地 址
	//的 单 位 是 字 节 ， 故 现 在 的 r ptr 所 指 向 的 地 址 比 起加 加 5 5 后的 的 r ptr 所 指 向 的 地 址
	//来 说 ， 向 高 地 址 方 向 移 动了 了 0 20 个 字 节 。 在 这 个 例 子 中 ， 没加 加 5 5 前的 的 r ptr 指
	//向 数组 组 a a 的第 第 0 0 号 单 元 开 始 的 四 个 字 节 ，加 加 5 5 后 ，r ptr 已 经 指 向 了 数组 组 a a 的
	//合 法 范 围 之 外 了。 。 虽 然 这 种 情 况 在 应 用 上 会 出 问 题， ， 但 在 语 法 上 却 是 可 以 
	char a[20] = "You_are_a_girl";
	int *ptr = (int *a);
	ptr += 5;
}
void test4(){
	//ptr 的 类 型是 是 char **, 指 向 的 类 型 是 一个 个 char * 类 型, , 该 指 向 的
	//地 址 就 是p p 的 地 址 (&p), 当 执 行 ptr++; 时, , 会 使 指 针 加 一 个 sizeof(char
	//*), , 即 &p+4; 那 *(&p+4) 指 向 哪 呢, , 这 个 你 去 问 上 帝 吧, , 或 者 他 会 告 诉 你 在
	//哪? ? 所 以 最 后 的 输 出 会 是 一 个 随 机 的 值, , 或 许 是 一 个 非 法 操 作. 
	char a[20] = "You_are_a_girl";
	char *p = a;
	char **ptr = &p;
	prinf("**ptr = %c\n",**ptr);
	ptr++;
	printf("**ptr=%c\ \ n",**ptr);
}
/*
 *一 个 指针 针 d ptrold 加( ( 减 )  ) 一 个 整数 数 n n 后 ， 结 果 是 一 个 新 的 指 针 ptrnew ，
 w ptrnew 的 类 型和 和 d ptrold 的 类 型 相 同 ，w ptrnew 所 指 向 的 类 型和 和 ptrold
 所 指 向 的 类 型 也 相 同。 。w ptrnew 的 值 将比 比 d ptrold 的 值 增 加( ( 减 少 )  )了 了 n n 乘
 d sizeof(ptrold 所 指 向 的 类 型) ) 个 字 节 。 就 是 说 ，w ptrnew 所 指 向 的 内 存
 区 将比 比 d ptrold 所 指 向 的 内 存 区 向 高( ( 低 )  ) 地 址 方 向 移 动了 了 n n 乘
 d sizeof(ptrold 所 指 向 的 类 型) ) 个 字 节
 */

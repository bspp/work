#define QUIT 5
#define HOTEL1 180.00
#define HOTEL2 255.00
#define HOTEL3 255.00
#define HOTEL4 355.00
#define HOTEL5 332.00
#define DISCOUNT 0.95
#define START "**********************************************************"
//显示选择列表
int menu(void);
//返回预定天数
int getnights(void);
void showprice(double rate,int nights);
//根据费率、入住天数计算费用

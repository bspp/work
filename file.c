#include <sys/file.h>  
#include <stdio.h>  
  
int main (void)  
{  
    FILE *fp = NULL;  
    FILE *fp1 = NULL;  
  
    int i = 20;  
  
    if ((fp = fopen("./time.c", "r+b")) == NULL) //打开文件  
  
        printf("file open error!\n");  
  
    if (flock(fp->_fileno, LOCK_EX) != 0) //给该文件加锁  
  
        printf("file lock by others\n");  
  
		    if ((fp1 = fopen("./time.c", "r+b")) == NULL) //打开文件  
					  
					        printf("file open error!\n");  
				else
					printf("sucess\n");
				  

    flock(fp->_fileno, LOCK_UN); //文件解锁  
    fclose(fp); //20秒后退出,关闭文件  
    fclose(fp1); //20秒后退出,关闭文件  
  
  
    return 0;  
  
}  

/*
整数划分问题
编程序求某一个正整数的所有划分数。如输入6，则输出：（要求输出不能重复）
6 = 5+1
6 = 4+2
6 = 4+1+1
6 = 3+3
6 = 3+2+1
6 = 3+1+1+1
6 = 2+2+2
6 = 2+2+1+1
6 = 2+1+1+1+1
6 = 1+1+1+1+1+1
*/ 


#include <stdio.h>

#define MIN(x,y) (x<y?x:y)
#define NUM_MAX 10000 
int n;		/*待划分数*/
int no = 0;	/*num数组当前下标*/

void print_divide(int p, int last, int num[]);

void main()
{
	int num[NUM_MAX];	/*从大到小记录划分数*/
	
	printf("Please input a number to be divided: ");
	scanf("%d", &n);	/*接受待划分数*/
	print_divide(n, n-1, num);	/*打印划分，把待划分数-1作为上一划分数，保证第一个划分数能够遍历所有情况，且不为待划分数*/
} 

/*
打印划分
参数：p为剩余待划分数、last为上一个划分数、num是记录划分数的数组 
*/
void print_divide(int p, int last, int num[])
{
	int i;	/*计数器*/
	
	if(p == 0)	/*如果都划分完了，打印划分情况*/
	{
		printf("%d=", n);	
		for(i = 0; i < no-1; i++)
			printf("%d+", num[i]);
		printf("%d\n", num[i]);
		return;
	}
	
	for(i = MIN(p, last); i > 0; i--)	/*从大到小依次取一个数来划分，同时保证划分数比上一个划分数小*/
	{
		num[no++] = i;	/*记录划分数，数组下标+1*/
		print_divide(p - i, i, num);	/*获取下一个划分数*/
		no--;	/*当前划分数的情况遍历完毕，回溯到上一划分数*/
	}
}

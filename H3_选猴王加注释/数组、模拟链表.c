#include "stdio.h"
#define M 11
int find_next(int start, int monkey[], int n);
int main( )                                  
{   int monkey[ ]={1,2,3,4,5,6,7,8,9,10,0};	/*初始化*/
    int pre, i, count, n = M;	/*pre保存报数为2的猴子的编号，i和count为计数器，n为猴子总数*/	
    int start=0;	/*报数起点*/
	
	/*重复筛选猴子，直到只剩下最后一只*/
    for( count=n; count>1; count-- )
    {   
		i = find_next(start,monkey,n);	/*找到下一只有效的猴子，报数为2*/
        pre = i;	/*记录报数为2的猴子的编号，便于删除报数为3的猴子，重新指向*/
        i = find_next(i,monkey,n);	/*找到下一只有效的猴子，报数为3*/
        printf("monkey[%d] is deleted;\n", i+1);	/*打印删除信息*/
        monkey[pre] = monkey[i];   /* delete this monkey */
        start = find_next(i,monkey,n);	/*把下一只有效的猴子作为报数起点*/
    }
    printf("The king is monkey[%d].\n",start+1);	/*打印结果*/
    return 0;
} 

/*由于数组存储了下一只有效猴子的编号，所以直接返回数组内容即可*/
int  find_next( int start, int monkey[], int n )
{   
	return monkey[start];
}


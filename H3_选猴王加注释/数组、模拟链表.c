#include "stdio.h"
#define M 11
int find_next(int start, int monkey[], int n);
int main( )                                  
{   int monkey[ ]={1,2,3,4,5,6,7,8,9,10,0};	/*��ʼ��*/
    int pre, i, count, n = M;	/*pre���汨��Ϊ2�ĺ��ӵı�ţ�i��countΪ��������nΪ��������*/	
    int start=0;	/*�������*/
	
	/*�ظ�ɸѡ���ӣ�ֱ��ֻʣ�����һֻ*/
    for( count=n; count>1; count-- )
    {   
		i = find_next(start,monkey,n);	/*�ҵ���һֻ��Ч�ĺ��ӣ�����Ϊ2*/
        pre = i;	/*��¼����Ϊ2�ĺ��ӵı�ţ�����ɾ������Ϊ3�ĺ��ӣ�����ָ��*/
        i = find_next(i,monkey,n);	/*�ҵ���һֻ��Ч�ĺ��ӣ�����Ϊ3*/
        printf("monkey[%d] is deleted;\n", i+1);	/*��ӡɾ����Ϣ*/
        monkey[pre] = monkey[i];   /* delete this monkey */
        start = find_next(i,monkey,n);	/*����һֻ��Ч�ĺ�����Ϊ�������*/
    }
    printf("The king is monkey[%d].\n",start+1);	/*��ӡ���*/
    return 0;
} 

/*��������洢����һֻ��Ч���ӵı�ţ�����ֱ�ӷ����������ݼ���*/
int  find_next( int start, int monkey[], int n )
{   
	return monkey[start];
}


/*
������������
�������ĳһ�������������л�������������6�����������Ҫ����������ظ���
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
int n;		/*��������*/
int no = 0;	/*num���鵱ǰ�±�*/

void print_divide(int p, int last, int num[]);

void main()
{
	int num[NUM_MAX];	/*�Ӵ�С��¼������*/
	
	printf("Please input a number to be divided: ");
	scanf("%d", &n);	/*���ܴ�������*/
	print_divide(n, n-1, num);	/*��ӡ���֣��Ѵ�������-1��Ϊ��һ����������֤��һ���������ܹ���������������Ҳ�Ϊ��������*/
} 

/*
��ӡ����
������pΪʣ�����������lastΪ��һ����������num�Ǽ�¼������������ 
*/
void print_divide(int p, int last, int num[])
{
	int i;	/*������*/
	
	if(p == 0)	/*������������ˣ���ӡ�������*/
	{
		printf("%d=", n);	
		for(i = 0; i < no-1; i++)
			printf("%d+", num[i]);
		printf("%d\n", num[i]);
		return;
	}
	
	for(i = MIN(p, last); i > 0; i--)	/*�Ӵ�С����ȡһ���������֣�ͬʱ��֤����������һ��������С*/
	{
		num[no++] = i;	/*��¼�������������±�+1*/
		print_divide(p - i, i, num);	/*��ȡ��һ��������*/
		no--;	/*��ǰ�����������������ϣ����ݵ���һ������*/
	}
}

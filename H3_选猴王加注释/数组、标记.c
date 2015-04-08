/*
�ҵ���һ����Ȧ�ڵĺ���
��������
	start��	�������
	monkey����¼����״̬�Ķ�λ����
	n��		��ʼ�涨�ĺ������� 
������һֻȦ�ڵĺ��ӵı��-1 
*/
int find_next( int start, int monkey[ ][M+1], int n)
{	
	int i=(start+1)%n;	/*iΪ��ʼ��Ϊ��һֻ���ӵı��-1��ʹ���ӵı���ڶ�����*/
	while( monkey[1][i]==0 )  i=(i+1)%n;	/*ȷ����һֻ�������ڶ����ڵġ�����������n-1*/
	return i;	/*������һֻȦ�ڵĺ��ӵı��-1*/
}

int main( )
{  
   int monkey[2][M+1];  /* monkey[0] �洢���,monkey[1] ��ʾ�Ƿ�����*/
   int i, count, n, start=0;     /*i��countΪ��������nΪ����������startΪ�������*/
   scanf("%d",&n);	/*������ӵ�����n*/
   
   /*��ʼ��������һ������n�����ӵ�Ȧ*/
   for( i=0; i<n; i++ )
   {    
   		monkey[0][i] = i+1;		/*���ӱ�ţ���1����*/
     	monkey[1][i] = 1;    /* all the monkeies are in the circle */
   }

	/*�ظ�ɸѡ���ӣ�ֱ��ֻʣ�����һֻ*/   
   for( count=n; count>1; count-- )
   {	
   		i = find_next(start,monkey,n);	/*�ҵ���һֻȦ�ڵĺ��ӣ�������Ϊ2*/
		i = find_next(i,monkey,n);	/*�ҵ���һֻȦ�ڵĺ��ӣ�������Ϊ3*/
		monkey[1][i] = 0;   /* delete this monkey */
		/* printf("monkey[%d] is deleted;\n",monkey[0][i]); */
		start = find_next(i,monkey,n);	/*����һֻȦ�ڵĺ�����Ϊ�������*/
  	}
    printf("The king is monkey[%d].\n",monkey[0][start]);	/*��ӡ���ս��*/
    return 0;
}


/*
找到下一个在圈内的猴子
参数——
	start：	报数起点
	monkey：记录猴子状态的二位数组
	n：		起始规定的猴子总数 
返回下一只圈内的猴子的编号-1 
*/
int find_next( int start, int monkey[ ][M+1], int n)
{	
	int i=(start+1)%n;	/*i为初始化为下一只猴子的编号-1，使猴子的编号在定义内*/
	while( monkey[1][i]==0 )  i=(i+1)%n;	/*确保下一只猴子是在定义内的——即不超过n-1*/
	return i;	/*返回下一只圈内的猴子的编号-1*/
}

int main( )
{  
   int monkey[2][M+1];  /* monkey[0] 存储编号,monkey[1] 表示是否在列*/
   int i, count, n, start=0;     /*i和count为计数器，n为猴子数量，start为报数起点*/
   scanf("%d",&n);	/*输入猴子的数量n*/
   
   /*初始化，生成一个含有n个猴子的圈*/
   for( i=0; i<n; i++ )
   {    
   		monkey[0][i] = i+1;		/*猴子编号，从1记起*/
     	monkey[1][i] = 1;    /* all the monkeies are in the circle */
   }

	/*重复筛选猴子，直到只剩下最后一只*/   
   for( count=n; count>1; count-- )
   {	
   		i = find_next(start,monkey,n);	/*找到下一只圈内的猴子，即报数为2*/
		i = find_next(i,monkey,n);	/*找到下一只圈内的猴子，即报数为3*/
		monkey[1][i] = 0;   /* delete this monkey */
		/* printf("monkey[%d] is deleted;\n",monkey[0][i]); */
		start = find_next(i,monkey,n);	/*把下一只圈内的猴子作为报数起点*/
  	}
    printf("The king is monkey[%d].\n",monkey[0][start]);	/*打印最终结果*/
    return 0;
}


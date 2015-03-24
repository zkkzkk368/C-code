void PrintPath(int x)
{
	if(x==MAX_INT)
		printf("No Solution!\n");
	else
	{
		while(x>0){
			/*此时路径是倒着存放的*/
			printf("(%d,%d)->",ans_x[min-x],ans_y[min-x]);
			x--;
		}
		printf("(%d,%d)",ans_x[min-x],ans_y[min-x]);	/*最后一个点*/
	}
}

void PrintMap(int x)
{
	if(x==MAX_INT)
		printf("No Solution!\n");
	else
	{
		/*路径标记*/
		while(x>=0){
			map[ ans_x[min-x] ][ ans_y[min-x] ] = 2;
			x--;
		}
		map[end_x][end_y] = 3;

		int i, j;

		for(i = 0; i < n; i++)
		{	
			for(j = 0; j < n; j++)	
				switch(map[i][j])
				{ 
					case 0: printf(" %c", '+');   break;	/*墙*/
					case 1: printf(" %c", '_');   break;	/*路*/
					case 2: printf(" %c", '@');   break;	/*路径*/
					case 3: printf(" %c", '$');				/*终点*/
				} 
			printf("\n");
		}
	}
}

#define NX x+nx[i]
#define NY y+ny[i]

/*移动，k参数为当前步数*/
void go(int x,int y,int k)
{
	int i;

	/*四个方向的坐标增量*/
	int nx[]={1,-1,0,0};
	int ny[]={0,0,1,-1};

	if(x==end_x&&y==end_y)	/*是终点？*/
	{
		if(k<min)	/*步数比已知最短路径还短？*/
		{
			/*将当前路径复制到最短路径*/
			step_x[k]=x;step_y[k]=y;
			min=k;
			for(i=0;i<=k;i++)
			{
				ans_x[i]=step_x[i];ans_y[i]=step_y[i];
			}
		}
	}
	else	/*非终点？*/
	{
		step_x[k]=x;step_y[k]=y;	/*写入当前路径*/
		map[x][y]=0;				/*路径占位，防止“进退进”和“田”字型的死循环*/
		
		/*尝试四个方向*/
		for(i=0;i<4;i++)
		{
			if( (NX>=0 && NX<n) && (NY>=0 && NY<n) && map[ x+nx[i] ][ y+ny[i] ] )
			/*在边界内？下一步可行？*/
			{
				go(x+nx[i],y+ny[i],k+1);
			}
		}

		/*回溯*/
		map[x][y]=1;
		step_x[k]=0;step_y[k]=0;
	}
}
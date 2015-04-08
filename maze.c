/*
用一个二维数组记录迷宫状态，代号如下 —— 
	0：墙
	1：路
	2：路径
	3：终点

在迷宫上记录路径的作用：
1、便于输出
2、占位
	表示这里已经走过，防止“进退进”和田字形的死循环 
*/

#include <stdio.h>
#define NEXT_X x+next_x[i]
#define	NEXT_Y y+next_y[i]
#define INIT_BY_FILE

int move(int x, int y);
void pretty_print(); 

/*
终点和迷宫在递归中多次调用，设置为全局变量
简化函数的参数传递，避免重复赋值 
*/
int maze[10][10];
int xx, yy; 	/*记录终点*/

/*下一步的增量*/ 
int next_x[] = {1,0,-1,0};
int next_y[] = {0,1,0,-1};

int main()
{
	#ifdef INIT_BY_FILE
	    int i, j;	/*计数器*/
	    int x, y;	/*记录起点*/
	    
	    /*初始化，通过txt文件获取迷宫和起止点*/ 
	    FILE *fp;
	    fp = fopen("maze.txt", "r");

	    for(i = 0; i < 10; i++)
	          for(j = 0; j < 10; j++)
	                fscanf(fp, "%d", &maze[i][j]);
	                
	    fscanf(fp, "%d %d", &x, &y);
		fscanf(fp, "%d %d", &xx, &yy);            
	    maze[x][y] = 2;	/*将起点纳入路径*/ 
	    fclose(fp);
	#else
		int i,j;
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				scanf("%d",&maze[i][j]); //读入地图 其中0表示不能通行，1表示可以通行
			}
		}
		printf("Please input the coordinate of the start and the end:\n");
		printf("start:");
		scanf("%d%d",&x,&y); //读入起点坐标
		while(maze[x][y]==0){ //判断起点坐标是否输入正确
			printf("Wrong Start!Please check the input and try again:\n");
			scanf("%d%d",&x,&y);
		}
		printf("end:");
		scanf("%d%d",&xx,&yy); //读入终点坐标
		while(map[xx][yy]==0){ //判断终点坐标是否输入正确
			printf("Wrong End!Please check the input and try again:\n");
			scanf("%d%d",&xx,&yy);
		}    
    #endif
    
    if(move(x, y))
    {
    	maze[xx][yy] = 3;
        pretty_print(); 
	}
    else
    	printf("No solution!");
}


/*
递归、回溯，依次尝试向右、下、左、上移动；
移动的过程中在迷宫上记录路径，如果失败，清除路径记录； 
*/
int move(int x, int y)
{
	int i;
	
    if(x == xx && y == yy)
    	return 1;
    
    /*
	#define NEXT_X x+next_x[i]
	#define NEXT_Y y+next_y[i]
	*/ 
    for(i = 0; i < 4; i++)
	    if(maze[NEXT_X][NEXT_Y] == 1)
	    {
	        maze[NEXT_X][NEXT_Y] = 2;
	        if(move(NEXT_X, NEXT_Y))
	        	return 1;
	        else
	            maze[NEXT_X][NEXT_Y] = 1;                
	    }
    
    /*没有合适的路径*/
    return 0;
}

void pretty_print()
{
	int i, j;
	printf("+ + + + + + + + + +\n");	/*上墙*/
	for(i = 1; i < 9; i++)
	{
		putchar('+');	/*左墙*/
		for(j = 1; j < 9; j++)
			switch(maze[i][j])
			{ 
				case 0: printf(" %c", '+');   break;	/*墙*/
				case 1: printf(" %c", '_');   break;	/*路*/
				case 2: printf(" %c", '@');   break;	/*路径*/
				case 3: printf(" %c", '$');				/*终点*/
			} 
		printf(" %c\n", '+');	/*右墙*/
	}
	printf("+ + + + + + + + + +\n");
}

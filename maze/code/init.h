#define MAX_INT 32766

int min;							/*最短路径步数*/
int start_x,start_y,end_x,end_y;	/*起止位置*/
int map[100][100];					/*迷宫*/
int step_x[100],step_y[100];		/*当前路径*/
int ans_x[100],ans_y[100];			/*目前最短路径*/
int n;

void InitByFile()		/*迷宫初始化——通过文件*/
{
	int i,j; 
	/*初始化，通过txt文件获取迷宫和起止点*/ 
	FILE *fp;
	fp = fopen("maze.txt", "r");

    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            fscanf(fp, "%d", &map[i][j]);
	min=MAX_INT;
	    
    fscanf(fp, "%d %d", &start_x, &start_y);
	fscanf(fp, "%d %d", &end_x, &end_y); 
    fclose(fp);
}

void InitByScan()        /*迷宫初始化——通过输入*/
{
	int i,j;
	printf("Print the maze: \n");
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			scanf("%d",&map[i][j]);
	min=MAX_INT;

	printf("Print the start point: ");
	scanf("%d%d",&start_x,&start_y);

	printf("Print the end point: ");
	scanf("%d%d",&end_x,&end_y);	
}
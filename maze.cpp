#include<stdio.h>
#define MAX_INT 32767 //宏定义整形的最大值：
#define INIT_FILE    //初始化方式为读取文件，否则为手动输入
/*
	本体解题思路：通过递归调用go()函数来进行路线的模拟，在每一步递归的过程中保留当前的路线状况，并将走
	过的路视为不可通的路。在递归调用结束后，将走过的点的不可用状态重新置换为可用状态
*/

int start_x,start_y,end_x,end_y; //分别记录起点终点坐标，采用全局变量
int map[100][100]; //记录地图上各个点的通行状况
int min; //记录最小通路的长度
int step_x[100],step_y[100]; //保存最小通路所经过的点

void init(int n){ //初始化函数
	#ifdef INIT_FILE
		int i,j; 
		/*初始化，通过txt文件获取迷宫和起止点*/ 
	    FILE *fp;
	    fp = fopen("maze.txt", "r");

	    for(i = 0; i < n; i++)
	          for(j = 0; j < n; j++)
	                fscanf(fp, "%d", &map[i][j]);
	    
	   
	    fscanf(fp, "%d %d", &start_x, &start_y);
		fscanf(fp, "%d %d", &end_x, &end_y); 
	    fclose(fp);
	#else
		int i,j;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				scanf("%d",&map[i][j]); //读入地图 其中0表示不能通行，1表示可以通行
			}
		}
		printf("Please input the coordinate of the start and the end:\n");
		printf("start:");
		scanf("%d%d",&start_x,&start_y); //读入起点坐标
		while(map[start_x][start_y]==0){ //判断起点坐标是否输入正确
			printf("Wrong Start!Please check the input and try again:\n");
			scanf("%d%d",&start_x,&start_y);
		}
		printf("end:");
		scanf("%d%d",&end_x,&end_y); //读入终点坐标
		while(map[end_x][end_y]==0){ //判断终点坐标是否输入正确
			printf("Wrong End!Please check the input and try again:\n");
			scanf("%d%d",&end_x,&end_y);
		}
	#endif
}

void go(int now_x,int now_y,int k){ //操作函数
	int i; 
	int next_x[]={1,-1,0,0}; //初始化步伐变化数组
	int next_y[]={0,0,1,-1};
	if(now_x==end_x&&now_y==end_y){ //判断是否到达终点
		if(k<min){ //保存最小路径状态
			min=k;
			step_x[k]=now_x;step_y[k]=now_y;
		}
	}
	else{
		map[now_x][now_y]=0; //改变路径
		step_x[k]=now_x;step_y[k]=now_y; //记录路径
		for(i=0;i<4;i++){ //进行递归
			if(map[ now_x+next_x[i] ][ now_y+next_y[i] ]==1){
				go(now_x+next_x[i],now_y+next_y[i],k+1); //递归
			}
		}
		map[now_x][now_y]=1; //回溯，状态重置
	}
}

void print(int n){
	int i;
	
	if(min==MAX_INT){ //判断是否找到通路
		printf("No Solution!");
	}
	else{ //打印路径
		for(i=1;i<n;i++){
			printf("(%d,%d)->",step_x[i],step_y[i]);
		}
		printf("(%d,%d)",step_x[i],step_y[i]);
	}
}

int main(){
	int n;
	printf("Please input the size of the maze:");
	scanf("%d",&n);
	init(n);
	min=MAX_INT;
	go(start_x,start_y,1);
	print(min);	
}

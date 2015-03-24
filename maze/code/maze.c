/*
git@github.com/zkkzkk368/C-code/maze
*/

#include <stdio.h>
#include "init.h"
#include "go.h"
#include "print.h"


int main()
{
	int InitWay, PrintWay; 	/*迷宫的接受方式，输出方式*/

	/*迷宫规模*/
	printf("Print the size of the maze: ");
	scanf("%d",&n);

	/*迷宫输入方式*/
	do
	{	
		printf("Select a way to init(1:file, 2:scan): ");
		scanf("%d", &InitWay);
	}while(InitWay == 1 && InitWay == 2);

	/*迷宫输出方式*/
	do
	{
		printf("Select a way to print(1:path, 2:map): ");
		scanf("%d", &PrintWay);
	}while(PrintWay == 1 && PrintWay == 2);

	/*初始化*/
	if(InitWay == 1)
		InitByFile();
	else
		InitByScan();

	go(start_x,start_y,0);
	
	/*输出*/
	if(PrintWay == 1)
		PrintPath(min);
	else 
		PrintMap(min);
}

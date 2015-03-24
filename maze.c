#include <stdio.h>

int move(int x, int y, int maze[][10], int xx, int yy);
void pretty_print(int maze[][10]); 

int main()
{
    int i, j;
    int maze[10][10];
    int x, y, xx, yy;
    FILE *fp;
    
    fp = fopen("maze.txt", "r");

    for(i = 0; i < 10; i++)
          for(j = 0; j < 10; j++)
                fscanf(fp, "%d", &maze[i][j]);
                
    fscanf(fp, "%d %d", &x, &y);
	fscanf(fp, "%d %d", &xx, &yy);            
    
    maze[x][y] = 2;
    
    if(move(x, y, maze, xx, yy))
    {
		maze[xx][yy] = 3; 
        pretty_print(maze);
    } 
    else
    	printf("There are no ways!");
    
    
    fclose(fp);
}
 
int move(int x, int y, int maze[][10], int xx, int yy)
{
    int next = 1;
    
    if(x == xx && y == yy)
         return 1;
    
    if(maze[x+1][y] == 1)
    {
        maze[x+1][y] = 2;
        next = move(x+1, y, maze, xx, yy);
        if(next != 0)
            return 1;   
        else
            maze[x+1][y] = 1;                
    }
    if(maze[x][y+1] == 1)
    {
        maze[x][y+1] = 2;
        next = move(x, y+1, maze, xx, yy);
        if(next != 0)
            return 1;
        else
            maze[x][y+1] = 1;                   
    }
    if(maze[x-1][y] == 1)
    {
        maze[x-1][y] = 2;
        next = move(x-1, y, maze, xx, yy);
        if(next != 0)
            return 1;
        else
            maze[x-1][y] = 1;                 
    }           
    if(maze[x][y-1] == 1)
    {
        maze[x][y-1] = 2;
        next = move(x, y-1, maze, xx, yy);
        if(next != 0)
            return 1;
        else
            maze[x][y-1] = 1;                   
    }
    return 0;
}

void pretty_print(int maze[][10])
{
	int i, j;
	printf("+++++++++++++++++++\n");
	for(i = 1; i < 9; i++)
	{
		putchar('+');
		for(j = 1; j < 9; j++)
			switch(maze[i][j])
			{ 
				case 0: printf(" %c", '+');   break;
				case 1: printf(" %c", '_');   break;
				case 2: printf(" %c", '@');   break;
				case 3: printf(" %c", '$');
			} 
		printf(" %c\n", '+');
	}
	printf("+++++++++++++++++++\n");
}

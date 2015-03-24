/*
��һ����ά�����¼�Թ�״̬���������� ���� 
	0��ǽ
	1��·
	2��·��
	3���յ�

���Թ��ϼ�¼·�������ã�
1���������
2��ռλ
	��ʾ�����Ѿ��߹�����ֹ�����˽����������ε���ѭ�� 
*/

#include <stdio.h>
#define NEXT_X x+next_x[i]
#define	NEXT_Y y+next_y[i]

int move(int x, int y);
void pretty_print(); 

/*
�յ���Թ��ڵݹ��ж�ε��ã�����Ϊȫ�ֱ���
�򻯺����Ĳ������ݣ������ظ���ֵ 
*/
int maze[10][10];
int xx, yy; 	/*��¼�յ�*/

/*��һ��������*/ 
int next_x[] = {1,0,-1,0};
int next_y[] = {0,1,0,-1};

int main()
{
    int i, j;	/*������*/
    int x, y;	/*��¼���*/
    
    /*��ʼ����ͨ��txt�ļ���ȡ�Թ�����ֹ��*/ 
    FILE *fp;
    fp = fopen("maze.txt", "r");

    for(i = 0; i < 10; i++)
          for(j = 0; j < 10; j++)
                fscanf(fp, "%d", &maze[i][j]);
                
    fscanf(fp, "%d %d", &x, &y);
	fscanf(fp, "%d %d", &xx, &yy);            
    maze[x][y] = 2;	/*���������·��*/ 
    fclose(fp);
    
    
    if(move(x, y))
    {
    	maze[xx][yy] = 3;
        pretty_print(); 
	}
    else
    	printf("There is no way!");
}


/*
�ݹ顢���ݣ����γ������ҡ��¡������ƶ���
�ƶ��Ĺ��������Թ��ϼ�¼·�������ʧ�ܣ����·����¼�� 
*/
int move(int x, int y)
{
	int i;
	
    if(x == xx && y == yy)
    	return 1;
    
    /*
	define NEXT_X x+next_x[i]
	define NEXT_Y y+next_y[i]
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
    
    /*û�к��ʵ�·��*/
    return 0;
}

void pretty_print()
{
	int i, j;
	printf("+ + + + + + + + + +\n");	/*��ǽ*/
	for(i = 1; i < 9; i++)
	{
		putchar('+');	/*��ǽ*/
		for(j = 1; j < 9; j++)
			switch(maze[i][j])
			{ 
				case 0: printf(" %c", '+');   break;	/*ǽ*/
				case 1: printf(" %c", '_');   break;	/*·*/
				case 2: printf(" %c", '@');   break;	/*·��*/
				case 3: printf(" %c", '$');				/*�յ�*/
			} 
		printf(" %c\n", '+');	/*��ǽ*/
	}
	printf("+ + + + + + + + + +\n");
}

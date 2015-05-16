#include <graphics.h>
#include <time.h>

#define PTR(x,y) (_vp+(_width*y+x)*(_color_bits/8))		//相应坐标的指针

#define BLOCKSIZE 30		//每格方块大小
#define PLAYAREA_WIDTH  15	
#define PLAYAREA_HEIGHT 18	//游戏区域大小（单位：格）

#define ACTIVECOLOR RED		//活动方块的颜色
#define STATICCOLOR	GREEN	//静态方块的颜色
#define OTHERCOLOR WHITE	//其他部件的颜色

#define BOARD_T 100 		//键盘检测周期

//方向键扫描码
#define TOP    0x4800
#define LEFT   0x4b00
#define BOTTOM 0x5000
#define RIGHT  0x4d00

void Init(void);
void PrintBlock(int x, int y);
void PrintBlocks(int x, int y);
void ShowBlocks(void);
void MoveBlocks(int x, int y);
void NewBlocks(void);


struct
{
	char style;		//方块样式，'RL','LL','T','RZ','LZ','I'六种
	int x;
	int y;			//旋转中心的坐标
	int dire;		//当前方向，'L'类和'T'为0-3，'Z'类和'I'为0-1
	int top;	//该方块占用的区域，分别为上左下右占用的格数（以旋转中心为中心）
	int right;
	int bottom;
	int left;
}NextBlocks, ActiveBlocks;	//下一方块和当前方块

void main()
{
	int gdriver = 0, gmode=VESA_800x600x8bit;
	int MoveSpeed = 1000;	//方框移动速度，毫秒
	int DelayTime;			//延迟累计时间
	int Board_k;			//从键盘缓冲队列获取的按键信息

	initgraph(&gdriver, &gmode, "");
	Init();

	srand((int)time(0));	//随机数种子

	while(1)
	{
		for(DelayTime = 0; DelayTime < MoveSpeed; DelayTime += BOARD_T)
		{
			delay(BOARD_T);
			while(bioskey(1))
			{
				Board_k = bioskey(0);
				switch(Board_k)
				{
					case LEFT:
						if(ActiveBlocks.x - ActiveBlocks.left > 1)
							MoveBlocks(ActiveBlocks.x-1, ActiveBlocks.y);
						break;
					case RIGHT:
						if(ActiveBlocks.x + ActiveBlocks.right < PLAYAREA_WIDTH)
							MoveBlocks(ActiveBlocks.x+1, ActiveBlocks.y);
						break;
				}
			}
		}
		MoveBlocks(ActiveBlocks.x, ActiveBlocks.y+1);
	}

	closegraph();
}

/*
初始化
打印界面
给生成新的方块信息
*/
void Init(void)
{
	setcolor(OTHERCOLOR);
	line(BLOCKSIZE*(PLAYAREA_WIDTH+2), 0, BLOCKSIZE*(PLAYAREA_WIDTH+2), _height);		//分界线x=BLOCKSIZE*(PLAYAREA_WIDTH+2)
	rectangle(BLOCKSIZE, BLOCKSIZE, BLOCKSIZE*(PLAYAREA_WIDTH+1), BLOCKSIZE*(PLAYAREA_HEIGHT+1));	//打印游戏区域边框

	NewBlocks();
	ShowBlocks();
}

/*
输出新方块
将next作为active
*/
void ShowBlocks(void)
{
	ActiveBlocks = NextBlocks;

	MoveBlocks(PLAYAREA_WIDTH / 2 + 1, 2);
}

/*
生成新的方块信息
*/
void NewBlocks(void)
{
	NextBlocks.style = rand() % 6;
	NextBlocks.x = PLAYAREA_WIDTH / 2 + 1;
	NextBlocks.y = 2;
	NextBlocks.dire = 0;

	switch(NextBlocks.style)
	{
		case 0:
			//RL方块
			NextBlocks.top    = 1;		
			NextBlocks.left   = 0;		
			NextBlocks.bottom = 1;		
			NextBlocks.right  = 1;		
			break;
		case 1:
			//LL方块
			NextBlocks.top    = 1;			
			NextBlocks.left   = 1;		
			NextBlocks.bottom = 1;		
			NextBlocks.right  = 0;		
			break;
		case 2:
			//T方块
			NextBlocks.top    = 1;			
			NextBlocks.left   = 1;		
			NextBlocks.bottom = 0;		
			NextBlocks.right  = 1;		
			break;
		case 3:
			//RZ方块
			NextBlocks.top    = 1;			
			NextBlocks.left   = 0;			
			NextBlocks.bottom = 1;			
			NextBlocks.right  = 1;		
			break;
		case 4:
			//LZ方块
			NextBlocks.top    = 1;			
			NextBlocks.left   = 1;		
			NextBlocks.bottom = 1;		
			NextBlocks.right  = 0;		
			break;
		case 5:
			//I方块
			NextBlocks.top    = 1;		
			NextBlocks.left   = 0;		
			NextBlocks.bottom = 2;	
			NextBlocks.right  = 0;		
			break;
	}
}


/*
移动方块
包括抹除旧方块和输出新方块
*/
void MoveBlocks(int x, int y)
{
	//抹除原来的方块
	setcolor(_back_color);
	PrintBlocks(ActiveBlocks.x, ActiveBlocks.y);
	
	//打印新的方块
	setcolor(ACTIVECOLOR);
	PrintBlocks(x, y);

	//修改方块的位置信息
	ActiveBlocks.x = x;
	ActiveBlocks.y = y;

	//重打印边框（移动过程中方框有可能会压到边框）
	setcolor(OTHERCOLOR);
	rectangle(BLOCKSIZE, BLOCKSIZE, BLOCKSIZE*(PLAYAREA_WIDTH+1), BLOCKSIZE*(PLAYAREA_HEIGHT+1));
}


/*
【辅助函数】
打印对应方块坐标的单一方块
*/
void PrintBlock(int x, int y)
{
	rectangle(x * BLOCKSIZE, y * BLOCKSIZE, (x+1) * BLOCKSIZE, (y+1) * BLOCKSIZE);
}

/*
打印完整的方块
*/
void PrintBlocks(int x, int y)
{
	switch(ActiveBlocks.style)
	{
		case 0:
			//RL方块
			PrintBlock(x, y-1);
			PrintBlock(x, y);
			PrintBlock(x, y+1);
			PrintBlock(x+1, y+1);
			break;
		case 1:
			//LL方块
			PrintBlock(x, y-1);
			PrintBlock(x, y);
			PrintBlock(x, y+1);
			PrintBlock(x-1, y+1);
			break;
		case 2:
			//T方块
			PrintBlock(x, y-1);
			PrintBlock(x, y);
			PrintBlock(x-1, y);
			PrintBlock(x+1, y);
			break;
		case 3:
			//RZ方块
			PrintBlock(x, y-1);
			PrintBlock(x, y);
			PrintBlock(x+1, y);
			PrintBlock(x+1, y+1);
			break;
		case 4:
			//LZ方块
			PrintBlock(x, y-1);
			PrintBlock(x, y);
			PrintBlock(x-1, y);
			PrintBlock(x-1, y+1);
			break;
		case 5:
			//I方块
			PrintBlock(x, y-1);
			PrintBlock(x, y);
			PrintBlock(x, y+1);
			PrintBlock(x, y+2);
			break;
	}
}
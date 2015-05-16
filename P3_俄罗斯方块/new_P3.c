/*
方块状态表示方法——
除了特殊的长条形方块，
每个方块的状态都可以用一个以旋转中心为中心的3x3的正方格表示

如：
0 1 0
0 1 0
0 1 1  表示L形的方块

这种情况下，不考虑中心，共有八个0或1的元素，将矩阵按行摊开
即 010 010 011 ，为八进制数0223

而对于长条形方块，
由于每个方块都占有四个，所以该值必定大于等于000 001 111即十进制数15
我们不妨定义1为横条形（以左2格为中心），2为竖条形（以上2格为中心）
就像这样，我们可以直接用一个字节表示出方块的状态；
*/

#include <graphics.h>
#include <time.h>

#define PTR(x,y)         (_vp+(_width*y+x)*(_color_bits/8))		//相应像素坐标的指针

#define BLOCKSIZE       30	//每格方块大小
#define PLAYAREA_WIDTH  15	
#define PLAYAREA_HEIGHT 18	//游戏区域大小（单位：格）

#define ACTIVECOLOR RED		//活动方块的颜色
#define STATICCOLOR	GREEN	//静态方块的颜色
#define OTHERCOLOR  WHITE	//其他部件的颜色

#define BOARD_T 100 		//键盘检测周期

//方向键扫描码
#define UP     0x4800
#define LEFT   0x4b00
#define DOWN   0x5000
#define RIGHT  0x4d00

struct
{
	int x, y;		//旋转中心坐标
	int status;	//方块状态
}NextBlocks, ActiveBlocks;	//下一方块和当前方块
int status[PLAYAREA_HEIGHT+1];	//记录各行状态，bit string，多出一行作为地板

void Init(void);
void ShowBlocks(void);
void NewBlocks(void);
void MoveBlocks(int x, int y);
void PrintBlock(int x, int y);
void PrintBlocks(int x, int y);
int MoveCheck(int dire_k);

void main()
{
	int gdriver = 0, gmode=VESA_800x600x8bit;
	int MoveSpeed = 1000;	//方框移动速度，毫秒
	int DelayTime;			//延迟累计时间
	int Board_k;			//从键盘缓冲队列获取的按键信息

	srand((int)time(0));	//随机数种子
	initgraph(&gdriver, &gmode, "");
	Init();

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
						if( MoveCheck(LEFT) )
							MoveBlocks(ActiveBlocks.x-1, ActiveBlocks.y);
						break;
					case RIGHT:
						if( MoveCheck(RIGHT) )
							MoveBlocks(ActiveBlocks.x+1, ActiveBlocks.y);
						break;
					case DOWN:
						if( MoveCheck(DOWN) )
							MoveBlocks(ActiveBlocks.x, ActiveBlocks.y+1);
						break;
				}
			}
		}

		if( MoveCheck(DOWN) )
			MoveBlocks(ActiveBlocks.x, ActiveBlocks.y+1);
		else
			getchar();
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
	status[PLAYAREA_HEIGHT] = (1<<(PLAYAREA_WIDTH)) -1;		//设置地板

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
	int type;

	NextBlocks.x = PLAYAREA_WIDTH / 2 + 1;
	NextBlocks.y = 2;

	type = rand() % 7;
	switch(type)
	{
		case 0:	
			/*RL
			0 1 0
			0 1 0
			0 1 1   ==>  010 010 011  ==> 0223
			*/
			NextBlocks.status = 0223;	break;
		case 1:
			/*LL
			0 1 0
			0 1 0
			1 1 0   ==>  010 010 110  ==> 0226
			*/
			NextBlocks.status = 0226;	break;
		case 2:
			/*T
			0 1 0
			1 1 1
			0 0 0   ==>  010 111 000  ==> 0270
			*/
			NextBlocks.status = 0226;	break;
		case 3:
			/*RZ
			0 1 0
			0 1 1
			0 0 1   ==>  010 011 001  ==> 0231
			*/
			NextBlocks.status = 0231;	break;
		case 4:
			/*LZ
			0 1 0
			1 1 0
			1 0 0   ==>  010 110 100  ==> 0264
			*/
			NextBlocks.status = 0264;	break;
		case 5:
			/*横条形*/
			NextBlocks.status = 1;		break;
		case 6:
			/*田字形
			1 1 0
			1 1 0
			0 0 0   ==>  110 110 000  ==> 0660
			*/
			NextBlocks.status = 0660;	break;
	}
}

/*
平移方块
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

	//重打印边框（移动过程中方块有可能会压到边框）
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
	int i,j;
	int StatusCk=1;	//状态探针

	if(ActiveBlocks.status == 1)
		//横长条
		for(i=x-1; i<=x+2; i++)
			PrintBlock(i, y);
	else if(ActiveBlocks.status == 2)
		//竖长条
		for(i=y-1; i<y+2; i++)
			PrintBlock(x, i);
	else
		//其他
	{
		//从右下角到左上角
		for(i=1; i>=-1; i--)
		{	
			for(j=1; j>=-1; j--)
			{	
				if(ActiveBlocks.status & StatusCk)
					PrintBlock(x+j, y+i);
				StatusCk <<= 1;		//探针偏移
			}
		}
	}
}

int MoveCheck(int dire_k)
{
	int i, j;
	int rel[3] = {-2,-2,-2};	//从上到下（从左到右）对应边界相对旋转中心的坐标，-2表示该行（列）无占用空间
	int StatusCk;				//探针
	int ckx, cky;				//检查点的方块坐标

	switch(dire_k)
	{
		case DOWN:
			StatusCk = 0400;	//从左上角开始
			//获取方块底部
			for(i=-1; i<=1; i++)
			//从上到下
			{	
				for(j=-1; j<=1; j++)
				//从左到右
				{	
					if(ActiveBlocks.status & StatusCk)
						rel[j+1] = i;
					StatusCk >>= 1;		//探针右移
				}
			}
			//此时rel依次保存着从左到右的最低点的y坐标，或为缺省值-2

			//检测方块底部是否有障碍
			for(i=0; i<3; i++)
			{
				if(rel[i] != -2)
				{
					cky = ActiveBlocks.y + rel[i] + 1;
					ckx = ActiveBlocks.x + i - 1;			//方块下方的一格的坐标

					//如果有方块障碍
					if(status[cky-1] & ((1<<PLAYAREA_WIDTH)>>ckx))		//此处cky-1是因为第一行是无效的
						return 0;
				}
			}
			return 1;	//如果没有障碍
		case LEFT:
			StatusCk = 1;	//从右下角开始
			//获取方块左
			for(i=1; i>=-1; i--)
			//从下到上
			{	
				for(j=1; j>=-1; j--)
				//从右到左
				{	
					if(ActiveBlocks.status & StatusCk)
						rel[i+1] = j;
					StatusCk <<= 1;		//探针左移
				}
			}
			//此时rel依次保存着从下到上的最左点的x坐标，或为缺省值-2

			//检测方块左部是否有障碍
			for(i=0; i<3; i++)
			{
				if(rel[i] != -2)
				{
					cky = ActiveBlocks.y + i - 1;
					ckx = ActiveBlocks.x + rel[i] - 1;			//方格左方的一格的坐标

					//如果检测点越界
					if(ckx < 1)	return 0;

					//如果有方块障碍
					if(status[cky-1] & ((1<<PLAYAREA_WIDTH)>>ckx))		//此处cky-1是因为第一行是无效的
						return 0;
				}
			}
			return 1;	//如果没有障碍
		case RIGHT:
			StatusCk = 0400;	//从左上角开始
			//获取方块右部
			for(i=-1; i<=1; i++)
			//从上到下
			{	
				for(j=-1; j<=1; j++)
				//从左到右
				{	
					if(ActiveBlocks.status & StatusCk)
						rel[i+1] = j;
					StatusCk >>= 1;		//探针右移
				}
			}
			//此时rel依次保存着从上到下的最右点的x坐标，或为缺省值-2

			//检测方块右部是否有障碍
			for(i=0; i<3; i++)
			{
				if(rel[i] != -2)
				{
					cky = ActiveBlocks.y + i - 1;
					ckx = ActiveBlocks.x + rel[i] + 1;			//方格右方的一格的坐标

					//如果检测点越界
					if(ckx > PLAYAREA_WIDTH) return 0;

					//如果有方块障碍
					if(status[cky-1] & ((1<<PLAYAREA_WIDTH)>>ckx))			//此处cky-1是因为第一行是无效的
						return 0;
				}
			}
			return 1;	//如果没有障碍
	}
}
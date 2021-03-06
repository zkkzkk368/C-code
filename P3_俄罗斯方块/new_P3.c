/*
============坐标说明=============
1、像素坐标
	即系统本身的坐标
2、方格坐标
	将原坐标划分为若干大小为BLOCKSIZE的方格
	以左上角为(0,0)
	游戏区域（PLAYAREA）的左上角为(1,1)

==========方块状态表示方法=================
除了特殊的长条形方块，
每个方块的状态都可以用一个以旋转中心为中心的3x3的正方格表示

如：
0 1 0
0 1 0
0 1 1  表示L形的方块

这种情况下，不考虑中心，共有八个0或1的元素，将矩阵压缩，按行摊开
即 010 010 011 ，为八进制数0223

#########
=。=如果忽略旋转中心的1， 我们可以直接使用char类型存储姿态
但是第一三行、第二行的信息需要分别获取
造成代码冗长，也不易维护
#########

而对于长条形方块，
由于每个方块都占有四个，所以该值必定大于等于000 001 111即十进制数15
我们不妨定义1为横条形（以左2格为中心），2为竖条形（以上2格为中心）

=============游戏状态记录==================
使用一个整型数组
用一个整型代表一行的状态（bit string），1为占用，0为空

=============方块平移、旋转的判定方法================
假定能够平移、旋转
根据方块的位置与姿态生成一个“行状态”
将该“行状态”与对应行的游戏状态记录的相应行的值进行比较（做位与&运算）即可

=============方块平移===================
直接修改旋转中心的位置信息

==============方块旋转==================
长条形、竖条形互换
一般形对姿态信息，做3x3矩阵的转置

==============版本更新==================
Ver1.0:
1、游戏区域、辅助区域的规划
2、方块的生成（方块信息没有结构化，直接分类生成）
3、方块的打印（分类打印）

Ver1.1:
1、添加“左右移动”功能
2、将方块视为矩形，记录矩形四条边的信息
3、实现“左右边界限制”，使方块不会超出游戏区域

Ver2.0
1、修改方块状态表示方法
	原：将方块简单地视为矩形块
		（局限很大，难以据此判断方块冲突）
	现：采用3x3矩阵记录方块信息
		（信息压缩后存储所需空间小，完整记录方块姿态信息）
2、根据新的方块状态表示重写Ver1.x的功能
3、实现“左右下边界限制”

Ver2.1
1、添加“方块移动可行性判定”
	（直接判断方格左侧、右侧、下方是否被占用）
2、添加“旋转”功能（但未实现判定）
3、添加“方块消除”和“动静态方块颜色区分”

Ver3.0
1、重写“方块变化可行性判定”算法
	原：直接判断方格左侧、右侧、下方的方格是否被占用
		（三个方向需要分别写代码判断，导致代码冗长，且该方法不适用于旋转判断）
	现：模拟变化后进行方格冲突判定
		（代码更易理解，并且可以统一“平移判定”和“旋转判定”）
=========>【基础功能完成】

Ver3.1
1、添加“下一方块提示”的辅助功能
2、修复BUG——
	I. 竖条形方块右边界、下边界判定错误
	II.方块消除后边框重画出现延迟

Ver3.2
1、添加“成绩统计”
2、修复BUG：部分方块在游戏区域最右端时冲突方块判定错误

Ver3.3
1、添加“计分板”、“退出”、“暂停”功能
2、完善注释
3、添加“游戏结束”

Ver3.4
1、改变delay的时机，解决持续按住 ↓键 松开后方块依旧下移的问题
2、修复BUG：
	同时消除两层以上时，高层的方块没有沉下来

*/

#include <graphics.h>
#include <time.h>

#define BLOCKSIZE       30	//每格方块大小
#define PLAYAREA_WIDTH  15	//游戏区域宽度（单位：格）
#define PLAYAREA_HEIGHT 18	//游戏区域高度（单位：格）
#define SCOREAREA_X		(_width - (_width - PLAYAREA_WIDTH*BLOCKSIZE) / 2)	//计分板位置x
#define SCOREAREA_Y		300		//计分板位置y

#define ACTIVECOLOR 	RED		//活动方块的颜色
#define STATICCOLOR		GREEN	//静态方块的颜色
#define OTHERCOLOR  	WHITE	//其他部件的颜色

#define BOARD_T 100 		//键盘检测周期

//方向键扫描码
#define UP     0x4800
#define LEFT   0x4b00
#define DOWN   0x5000
#define RIGHT  0x4d00

//方块信息存储结构
struct Block
{
	int x, y;		//旋转中心坐标
	int status;		//方块状态
}NextBlocks, ActiveBlocks;	//下一方块和当前方块

int status[PLAYAREA_HEIGHT];	//记录各行状态，bit string
								//除去上左边框，游戏区域内的坐标应从1记起
								//但该数组索引从0记起，即与游戏区域方格坐标相差1,

int score=0;	//分数统计

/*
旋转映射
8 → 2   7 → 5
↑   ↓   ↑   ↓
6 ← 0   3 ← 1  4 → 4
*/
int Rev[]={6,3,0,7,4,1,8,5,2};

void Init(void);								//初始化
void ShowBlocks(void);							//打印新的动态方块
void NewBlocks(void);							//生成下一方块的信息
void MoveBlocks(int x, int y);					//移动方块
void PrintBlock(int x, int y, char *style);		//打印方格
void PrintBlocks(struct Block pblock);			//打印方块
void SetStatus(void);							//设置状态（动态方块静态化）
void PrintFinished(void);						//打印静态方块
void revolve(void);								//旋转方块
int  TryClear(void);							//清除方块（仅信息层面）
int  Check(int x, int y, int BlockStatus);		//方块平移、旋转的模拟检测

void main()
{
	int gdriver = 0, gmode=VESA_800x600x8bit;
	int MoveSpeed = 1000;	//方框移动速度，毫秒
	int DelayTime;			//延迟累计时间
	int Board_k;			//从键盘缓冲队列获取的按键信息
	char pscore[4];			//成绩字符串

	srand((int)time(0));	//随机数种子
	initgraph(&gdriver, &gmode, "");
	Init();					//初始化
	delay(BOARD_T);

	while(1)
	{
		for(DelayTime = 0; DelayTime < MoveSpeed; DelayTime += BOARD_T)
		{
			while(bioskey(1))
			{
				Board_k = bioskey(0);
				switch(Board_k)			//特殊按键扫描码
				{
					case LEFT:
						if( Check(ActiveBlocks.x-1, ActiveBlocks.y,   ActiveBlocks.status) )
							MoveBlocks(ActiveBlocks.x-1, ActiveBlocks.y);
						break;
					case RIGHT:
						if( Check(ActiveBlocks.x+1, ActiveBlocks.y,   ActiveBlocks.status) )
							MoveBlocks(ActiveBlocks.x+1, ActiveBlocks.y);
						break;
					case DOWN:			//发生常规下移工作，直接跳转，结束键盘检测
						goto Down;
						break;
					case UP:
						revolve();
						break;
				}

				switch((char) Board_k)	//截取ASICC码
				{
					case 'e':
						closegraph();
						exit(0);
						break;
					case 'p':
						bioskey(0);
						break;
				}
			}
			delay(BOARD_T);
		}

   Down:if( Check(ActiveBlocks.x, ActiveBlocks.y+1,   ActiveBlocks.status) )
			MoveBlocks(ActiveBlocks.x, ActiveBlocks.y+1);
		else
		{
			SetStatus();

			//Game Over
			if(status[0] != 0)
			{
				initgraph(&gdriver, &gmode, "");
				load_8bit_bmp(0,0,"GameOver.bmp");
				setcolor(RED);
				outtextxy(_width / 2 - 80, _height / 2 + 40, "Press any key to exit");
				bioskey(0);
				closegraph();
				exit(0);
			}

			//清除方块
			if( TryClear() )
			{
				//抹除整个游戏区域
				setfillstyle(SOLID_FILL, _back_color);
				bar(BLOCKSIZE, BLOCKSIZE, BLOCKSIZE * (PLAYAREA_WIDTH+1), BLOCKSIZE * (PLAYAREA_HEIGHT+1));	

				//重打印边框（抹除游戏区域时，边框也被抹除了）
				setcolor(OTHERCOLOR);
				rectangle(BLOCKSIZE, BLOCKSIZE, BLOCKSIZE*(PLAYAREA_WIDTH+1), BLOCKSIZE*(PLAYAREA_HEIGHT+1));

				//抹除分数区
				setcolor(_back_color);
				bar(SCOREAREA_X + 48, SCOREAREA_Y, SCOREAREA_X+8*9, SCOREAREA_Y+16);

				//打印分数
				setcolor(RED);
				sprintf(pscore, "%03d", score);
				outtextxy( SCOREAREA_X + 48 , SCOREAREA_Y, pscore );
			}
			PrintFinished();
			ShowBlocks();
			NewBlocks();
		}
	}

	closegraph();
}

/*
【初始化】
==============功能=============
打印界面
产生首个方块
再生成新的方块信息

==============参数=============
无

=============返回值=============
无
*/
void Init(void)
{
	int NextMargin;		//显示下一方块的区域的边距

	setcolor(OTHERCOLOR);
	line(BLOCKSIZE*(PLAYAREA_WIDTH+2), 0, BLOCKSIZE*(PLAYAREA_WIDTH+2), _height);					//分界线x=BLOCKSIZE*(PLAYAREA_WIDTH+2)
	rectangle(BLOCKSIZE, BLOCKSIZE, BLOCKSIZE*(PLAYAREA_WIDTH+1), BLOCKSIZE*(PLAYAREA_HEIGHT+1));	//打印游戏区域边框

	//打印显示下一方块的区域
	NextMargin = ( _width - (PLAYAREA_WIDTH + 2 + 6)*BLOCKSIZE ) / 2;
	rectangle(_width - 6*BLOCKSIZE - NextMargin, NextMargin, _width - NextMargin, NextMargin + 6*BLOCKSIZE);

	//打印提示语
	outtextxy( SCOREAREA_X - 40, SCOREAREA_Y + 70, "Press [P] to pause" );
	outtextxy( SCOREAREA_X - 90, SCOREAREA_Y + 90, "Then press any key to continue" );
	outtextxy( SCOREAREA_X - 40, SCOREAREA_Y + 150, "Press [E] to exit" );

	//打印分数
	outtextxy( SCOREAREA_X , SCOREAREA_Y, "Score:" );
	setcolor(RED);
	outtextxy( SCOREAREA_X + 48 , SCOREAREA_Y, "000" );
	
	//生成并打印第一个动态方块
	NewBlocks();
	ShowBlocks();

	//生成下一个方块
	NewBlocks();
}

/*
【输出新方块】
=============功能===================
将NextBlocks作为ActiveBlocks
修改位置信息后输出方块

=============参数===============
无

=============返回值===============
无
*/
void ShowBlocks(void)
{
	ActiveBlocks = NextBlocks;

	//坐标变换
	ActiveBlocks.x = PLAYAREA_WIDTH / 2 + 1;
	ActiveBlocks.y = 2;

	PrintBlocks(ActiveBlocks);

}

/*
【生成新的方块信息】
==================功能====================
抹除原来的“下一方块”
生成随机类型的方块
并赋予初始信息后打印新的“下一方块”

==================参数===================
无

==================返回值================
无
*/
void NewBlocks(void)
{
	int type;	//方块类型（0-6）

	//抹除原来的“下一方块”区域的内容
	setcolor(_back_color);
	PrintBlocks(NextBlocks);

	//给予新的方块的位置信息
	NextBlocks.x = PLAYAREA_WIDTH + 6;
	NextBlocks.y = 4;

	//产生随机类型的方块
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
			NextBlocks.status = 0270;	break;
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

	/**********LZ、LL、田字形的“下一个方块”打印位置修正************/
	if(type == 1)
		NextBlocks.x++;

	if(type == 4)
		NextBlocks.x++;

	if(type == 6)
	{
		NextBlocks.x++;
		NextBlocks.y++;
	}
	/**********位置修正结束************/

	//在对应区域打印下一方块
	setcolor(RED);
	PrintBlocks(NextBlocks);
}

/*
【平移方块】
==================功能===============
按照原来的位置信息抹除旧方块
按照新的位置信息输出新方块
由于移动过程中方块可能会抹除游戏区域边框，所以边框最后需要重画
由于移动过程中方块肯呢过会抹除静态方块的边框，静态方块需要重画

=================参数====================
x,y 为新的方块的旋转中心坐标

=================返回值===================
无
*/
void MoveBlocks(int x, int y)
{
	//抹除原来的方块
	setcolor(_back_color);
	PrintBlocks(ActiveBlocks);

	//修改方块的位置信息
	ActiveBlocks.x = x;
	ActiveBlocks.y = y;

	//打印新的方块
	setcolor(ACTIVECOLOR);
	PrintBlocks(ActiveBlocks);

	//重打印边框
	setcolor(OTHERCOLOR);
	rectangle(BLOCKSIZE, BLOCKSIZE, BLOCKSIZE*(PLAYAREA_WIDTH+1), BLOCKSIZE*(PLAYAREA_HEIGHT+1));

	//重画静态方块
	PrintFinished();
}

/*
（辅助函数）【打印方格】
===============功能==============
在对应方格坐标上打印方格（仅边框 或 填充）

===============参数===============
x,y 为方格坐标
style 为打印方式，仅边框('line')或填充('fill')

===============返回值==============
无
*/
void PrintBlock(int x, int y, char *style)
{
	//由于line和fill第一个字母不同，所以直接判断首字母即可
	switch(style[0])
	{
		case 'l':	//line
			rectangle(x * BLOCKSIZE, y * BLOCKSIZE, (x+1) * BLOCKSIZE, (y+1) * BLOCKSIZE);
			break;
		case 'f':	//fill
			bar(x * BLOCKSIZE, y * BLOCKSIZE, (x+1) * BLOCKSIZE, (y+1) * BLOCKSIZE);
			break;
	}
}

/*
【打印方块】
=================功能==============
打印完整的方块，利用辅助函数PrintBlock

=================参数================
pblock 为保存方块信息的结构

=================返回值===============
无
*/
void PrintBlocks(struct Block pblock)
{
	int i,j;
	int StatusCk=1;	//状态探针

	if(pblock.status == 1)			//横长条
		for(i=pblock.x-1; i<=pblock.x+2; i++)
			PrintBlock(i, pblock.y, "line");
	else if(pblock.status == 2)		//竖长条
		for(i=pblock.y-1; i<=pblock.y+2; i++)
			PrintBlock(pblock.x, i, "line");
	else		//一般形
	{
		//从右下角到左上角
		for(i=1; i>=-1; i--)
		{	
			for(j=1; j>=-1; j--)
			{	
				if(pblock.status & StatusCk)
					PrintBlock(pblock.x+j, pblock.y+i, "line");
				StatusCk <<= 1;		//探针偏移
			}
		}
	}
}


/*
【插入游戏状态记录】
=============功能===============
当动态方块要转换成静态方块时调用该函数
将方块当前的位置信息插入到游戏状态中

=============参数==============
无

=============返回值===========
无
*/
void SetStatus(void)
{
	int GetStatus = 07;		//07 => 000 000 111，状态探针，一次获取三位
	int InsertStatus;		//要插入的状态信息
	int i;
	int offset;				//对于获取的状态信息所要做为偏移量

	//横条形，只需在一行填入四个空白
	if(ActiveBlocks.status == 1)	
	{
		InsertStatus = 017 << (PLAYAREA_WIDTH - ActiveBlocks.x - 2);	//017 => 000 001 111
		status[ActiveBlocks.y-1] |= InsertStatus;						//插入信息
		return;
	}

	//竖条形，需要在四行分别填入一个空白
	if(ActiveBlocks.status == 2)
	{
		for(i=-2; i<=1; i++)
		{
			status[ActiveBlocks.y+i] |= 1 << PLAYAREA_WIDTH - ActiveBlocks.x;	//逐行插入位置信息
		}
		return;
	}

	//一般形，分别检测三行信息
	for(i=0; i<3; i++)
	{
		offset = PLAYAREA_WIDTH - ActiveBlocks.x - 3*i - 1;					//插入信息的偏移量
		if(offset >= 0)		//左偏移
			InsertStatus = (GetStatus & ActiveBlocks.status) << offset;		//信息偏移
		else				//右偏移
			InsertStatus = (GetStatus & ActiveBlocks.status) >> -offset;	//信息偏移
		status[ActiveBlocks.y-i] |= InsertStatus;		//插入信息
		GetStatus <<= 3;			//探针偏移，获取上一行的信息
	}
}

/*
【打印静态方块】
==============功能================
打印静态方块
填充+边框

==============参数================
无

==============返回值==============
无
*/
void PrintFinished(void)
{
	int i, j;
	int StatusCk;	//状态探针

	//设置填充、边框样式
	setfillstyle(SOLID_FILL, STATICCOLOR);
	setcolor(RED);

	//逐一扫描各行状态
	for(i=0; i<PLAYAREA_HEIGHT; i++)
	{
		StatusCk = 1;	//探针初始化

		//逐一获取当前行各列的状态信息（自右向左）
		for(j=0; j<PLAYAREA_WIDTH; j++)
		{
			if(status[i] & StatusCk)	//如果该方格被占用
			{
				PrintBlock(PLAYAREA_WIDTH-j, i+1, "fill");
				PrintBlock(PLAYAREA_WIDTH-j, i+1, "line");
			}

			StatusCk <<= 1;		//探针偏移
		}
	}
}

/*
【旋转】
==================功能===============
借助Check()函数判断旋转可行性后
对方块进行旋转（重写姿态信息、重打印方块）

==================参数===================
无

==================返回值===================
无
*/
void revolve(void)
{
	int StatusCk = 1;		//状态探针，每次获取一位
	int NewStatus = 0;		//新的状态信息
	int i;

	//横条形直接转换为竖条形
	if(ActiveBlocks.status == 1)
		NewStatus = 2;

	//竖条形直接转换为横条形
	else if(ActiveBlocks.status == 2)
		NewStatus = 1;

	//一般形，直接做矩阵转置
	else
	{
		//逐个获取矩阵元素，做转置
		for(i=0; i<9; i++)
		{	
			if(StatusCk & ActiveBlocks.status)	//如果元素为1
			{
				NewStatus |= ( 1 << Rev[i] );	
			}
			StatusCk <<= 1;		//探针偏移
		}
	}

	//判断新的姿态是否与其他方格冲突，如不冲突则写入新的信息并重画
	if(Check(ActiveBlocks.x, ActiveBlocks.y, NewStatus))
	{
		//抹除原来的方块
		setcolor(_back_color);
		PrintBlocks(ActiveBlocks);

		//写入新的状态信息
		ActiveBlocks.status = NewStatus;

		//重画方块
		setcolor(ACTIVECOLOR);
		PrintBlocks(ActiveBlocks);	
	}
}


/*
【尝试消除】
================功能================
扫描各行状态
在信息方面尝试做行消除（并没有重画，仅在信息层面）

================参数================
无

================返回值==============
布尔，可消除返回1，不可消除返回0
据此返回值判断是否重画静态方块
*/
int TryClear(void)
{
	int i, j;
	int flag = 0;	//“消除”标志

	for(i=0; i<PLAYAREA_HEIGHT; i++)
	{//32767	65535
		// printf("%d | ", status[PLAYAREA_HEIGHT]);	
		if(status[i] == 32767)	//32767为行占满时的值
		{	
			score++;		//计分板增加
			status[i] = 0;	//重置行状态
			flag = 1;		//设置“消除”标志
		}
	}
	if(flag == 0)	//如果没有发生消除，直接返回0
		return 0;	
	
	//填补被消除的空行
	for(i=PLAYAREA_HEIGHT-1; i>0; i--)
	{
		if(status[i] == 0)
		{
			for(j=i-1; j>=0; j--)	//往上找到首个非空行
			{
				if(status[j] != 0)
				{
					status[i] = status[j];
					status[j] = 0;
					break;
				}	
			}

			if(j==0)	//如果找到了首行，说明i行以上都是空行，无需再找
				break;
		}
	}
	return 1;
}


/*
【模拟检测】
======功能======
接受测试方块的旋转中心坐标与姿态
判断该处的方块是否越界、是否与其他方块发生冲突

======参数======
x,y为检测点旋转中心坐标（方格坐标）
BlockStatus为检测点方块姿态

======返回值======
布尔，1为可行，0为不可行

======注意======
本函数的if用了大量“多余”的花括号
是为了else和if对齐，避免混乱
因为内嵌的if没有else，直接用else表示的是内嵌if的else
*/
int Check(int x, int y, int BlockStatus)
{
	int LineStatus;		//根据方块信息生成的行信息，用来了游戏状态信息比对
	int i;

	/*
	（左边界）
	0444 => 1 0 0
			1 0 0
			1 0 0
	*/
	if(BlockStatus == 1)			//横条形
	{
		if(x-1<1)
			return 0;
	}
	else if(BlockStatus == 2)		//竖条形
	{
		if(x<1)
			return 0;
	}
	else if(BlockStatus & 0444)		//一般形，方块最左端在中心左边
	{
		if(x-1<1)
			return 0;
	}
	else if(x<1)					//一般形，方块最左端即为中心
		return 0;

	/*
	（右边界）
	0111 => 0 0 1
			0 0 1
			0 0 1
	*/
	if(BlockStatus == 1)				//长条形
	{
		if(x+2>PLAYAREA_WIDTH)	
			return 0;
	}
	else if(BlockStatus == 2)			//竖条形
	{
		if(x>PLAYAREA_WIDTH)
			return 0;
	}
	else if(BlockStatus & 0111)			//一般形，方块最右端在中心右边
	{
		if(x+1>PLAYAREA_WIDTH)
			return 0;
	}
	else if(x>PLAYAREA_WIDTH)			//一般形，方块最右端即为中心
		return 0;
	
	/*
	（下边界）
	0007 => 0 0 0
			0 0 0
			1 1 1
	*/
	if(BlockStatus == 1)				//横条形	
	{
		if(y>PLAYAREA_HEIGHT)	
			return 0;
	}
	else if(BlockStatus == 2 )			//竖条形
	{
		if(y+2>PLAYAREA_HEIGHT)
			return 0;
	}
	else if(BlockStatus & 0007)			//一般形，方块最下端为中心下边
	{
		if(y+1>PLAYAREA_HEIGHT)	
			return 0;
	}
	else if(y>PLAYAREA_HEIGHT)			//一般形，方块最下端即为中心
		return 0;

	/*
	（方块冲突）
	*/
	if(BlockStatus == 1)									//长条形
	{
		LineStatus = 017 << (PLAYAREA_WIDTH - x - 2);		//017 => 000 001 111，方块在区域最右边时，中心右侧还有2格
		if(LineStatus & status[y-1])
			return 0;
	}
	else if(BlockStatus == 2)								//竖条形
	{
		LineStatus = 1 << (PLAYAREA_WIDTH - x);				//方块在区域最右边时，中心右侧没有方格
		for(i=-1; i<=2; i++)								//从上往下扫描各方格是否冲突
			if(LineStatus & status[y+i-1])
				return 0;
	}
	else
	{
		for(i=-1; i<=1; i++)								//从上往下分别扫描方块的三行是否冲突
		{
			LineStatus = ( BlockStatus >> 3*(1-i) ) & 07; 	//07 => 000 000 111，获取方块当前行的姿态
			if(x+1>PLAYAREA_WIDTH)							//如果旋转中心在最右侧，则行信息需要右移
				LineStatus >>= 1;
			else 
				LineStatus <<= (PLAYAREA_WIDTH - x - 1);	//以三格为一行，方块在区域最右边时，中心右侧还有一格
			if(LineStatus & status[y+i-1])
				return 0;
		}
	}

	return 1;	
}

/*
单词信息存放在循环链表中
==============================================================================
程序分为以下模块：
（程序都在文本模式下运行，并没有必要使用图形模式）
1、开始界面，选择游戏难度，共三档
2、循环链表初始化：根据屏幕高度，初始化相应节点数的链表
3、单词文本随机读取
4、打印新行
5、行滚动（只修改单词结构信息）
6、屏幕刷新
7、状态栏打印，包括提示、作者、成绩
8、报告打印，按下ESC键后显示报告，之后按下任意键退出程序
==============================================================================
版本更新：(2015/05/12~14)
Ver0.0: 
	测试graphics库和watcom平台
	实现单词滚动
	但由于单词信息混乱，程序无法继续扩展，重新规划结构、模块后重写程序
====>【失败品】╮(╯▽╰)╭

Ver1.0:
	实现基本功能：单词生成、滚动
Ver1.1:
	实现基本功能：键盘读取，匹配字母（但延迟明显），单词消除
Ver1.2:
	使用毫秒级的delay取代秒级的sleep
	提高键盘缓冲区读取频率
	并修复部分小问题
====>【基本功能完成】O(∩_∩)O

Ver2.0:
	添加“退出”和“成绩统计”功能，添加“状态栏”显示
	修改程序结束条件：
		原：文件读取结束
		现：文件读取结束  且  页面滚动结束（即单词滚动结束） 或  用户按下ESC
Ver2.1:
	修复BUG：
		清除单词后没有重置所有单词状态
		导致类似清除absolute后再键入ly即可清除absolutely的情况
	添加“游戏难度选择”功能
Ver2.2:
	添加“随机读取单词”功能
	修改程序条件：
		原：文件读取结束  且  页面滚动结束（即单词滚动结束）
		现：用户按下ESC（由于随机读取文件，文件读取不会结束）
	完善注释内容
	优化代码，除去部分无用变量
====>【附加功能完成】O(∩_∩)O
*/

#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>


#define ColorBytes (_color_bits/8)						//每个元素占用的字节数
#define Ptr(x,y) (_vp+(_width*(y)+(x))*ColorBytes)		//指向坐标(x,y)的指针
#define ESC 0x011b										//ESC键的扫描码
#define BOARD_T 100 									//键盘检测周期（毫秒/次）


//单词存储结构
struct line
{
	char word[20];			//单词内容
	char *stptr;			//指向本行输出头部的指针
	int len;				//单词长度
	struct line *next;		//指向下一节链表的指针
	long status;			//单词状态，bit string，默认为0
};


//模块函数
struct line * InitList(int n);							//链表初始化
void NewLine(struct line *head, char *word);			//生成新行
void StatusCheck(struct line *foot, char c);			//状态检查和修改
void Refresh(struct line *foot, struct line *head);		//刷新
void CallReport(FILE *fp);								//打印报告（即结束页面）
char SpeedOption(void);									//速度选项（即开始页面）
void RandomChoice(FILE *fp, char *word);				//随机读取文件中的单词


//辅助函数
void PrintWord(struct line *wp);						//单词打印，主要在Refresh函数中调用
void CleanStatus(struct line *foot);					//状态清空，主要在StatusCheck函数中调用


//全局变量
int grade=0;	//记录成绩
int count=0;	//记录单词总数
int fbytes;		//文件大小


void main()
{
	struct line *head, *next, *foot;		//因为一开始屏幕上有首末行之分，所以要区分head和foot
	FILE *fp;
	char word[20];
	// int result;
	int Board_k;		//记录键盘按键
	int MoveSpeed;		//滚动速度，即每行MS毫秒
	int SleepTime;		//累计已delay的时间

	//获取用户选择的速度档，并转换成各档对应的滚动速度
	MoveSpeed = SpeedOption();
	switch(MoveSpeed)
	{
		case 'e': MoveSpeed = 1000; break;
		case 'n': MoveSpeed = 750; break;
		case 'd': MoveSpeed = 500; break;
	}

	fp = fopen("words.txt", "r");	//文件在CallReport函数中关闭

	//获取文件大小，方便随机读取单词
	fseek(fp, 0, SEEK_END);
	fbytes = ftell(fp);
	rewind(fp);

	//设置时间为随机种子
	srand((int)time(0));	

	//初始化
	text_mode();	//清屏
	foot = head = InitList(_height);		//初始化空链表，用来储存各行信息
	
	//生成首行
	RandomChoice(fp, word);
	count++;
	NewLine(head, word);

	while(1)	//程序不自动停止
	{	
		//生成新的首行
		head = head->next;
		RandomChoice(fp, word);
		count++;

		NewLine(head, word);
		
		//其余各行下移
		if(head->next->len==0)	//len=0为空节点的标记
			//如果文本末行不是屏幕末行，则以文本末行为打印起始行
			next = foot;
		else					
			//如果文本末行是屏幕末行，则重设文本倒数第二行为末行（即滚动）并以其为打印起始行
			foot = next = foot->next;
		while(next != head)
		{
			next->stptr += _width * ColorBytes;
			next = next->next;
		}

		// 刷新（重打印）
		Refresh(foot, head);


		// 按照一定频率扫描键盘缓冲区
		// 可是sleep只支持秒不支持毫秒，不好操作
		// 但是delay单位为毫秒
		for(SleepTime = 0; SleepTime < MoveSpeed; SleepTime += BOARD_T)
		{
			while(bioskey(1))	//读完所有键盘缓冲队列
			{
				Board_k = bioskey(0);
				if(Board_k == ESC)	//先判断是否退出
					CallReport(fp);
				else  	//根据按键匹配各个单词，修改状态后刷新
				{
					StatusCheck(foot, Board_k);
					Refresh(foot, head);
				}
				
			}
			delay(BOARD_T);
		}
	}
}


/*
该函数初始化一个空循环链表
返回任意一节链表的指针
*/
struct line * InitList(int n)
{
	struct line *head, *p1, *p2;
	int i;

	//创建头节点
	head = (struct line *) malloc(sizeof(struct line));
	head->len = head->status = 0;	//len=0作为空节点的标记

	//依次创建剩余的n-1个节点
	p1 = head;
	for(i=0; i<n-1; i++)
	{
		p2 = (struct line *) malloc(sizeof(struct line));
		p1->next = p2;
		p2->len = p2->status = 0;	//len=0作为空节点的标记
		p1 = p2;
	}

	//将尾节点指向头节点
	p1->next = head;

	return head;	//此处返回任一节点都行
}


/*
写入新行的信息
*/
void NewLine(struct line *head, char *word)
{
	int x;

	head->len = strlen(word);				//单词长度
	strcpy(head->word, word);				//单词内容
	x = rand() % ( _width - head->len );	//产生随机x坐标
	head->stptr = Ptr(x,0); 				//单词头部的指针
	head->status = 0;						//状态清零
}


/*
状态检查，根据按键判断是否有匹配字母
第二个参数c由于是char，会截断Board_k的后八位，即按键的ASSIC码
*/
void StatusCheck(struct line *foot, char c)
{
	struct line *p=foot;	//从文本底部开始检查
	int i;
	int isTyped;			//作为布尔值，指示当前位置是否已被匹配过（即该位为1）
	int FirstLoop=1;		//作为布尔值，指示变量，表示是否为第一次循环

	//由于是循环链表，需要借助指示变量来遍历链表
	//从foot开始查询，又以foot为结束
	while(p != foot || FirstLoop)
	{
		//跳过已打出的字母
		i = 0;
		do
		{
			isTyped = p->status & (1 << i);		//逐位判断
			i++;								//记录已匹配的字母个数，便于判断是否要“清除”单词
		}while(isTyped);		//此时i为所要判断的字母的索引+1

		//比较字符
		if(p->word[i-1] != c || i > p->len)	//字符不匹配 或 索引超出范围？
			p->status = 0;
		else if(i == p->len)	//匹配且恰好为最后一个字符？
		{
			grade++;						//【Global】成绩增加
			p->word[0] = '\0';				//“清除”单词
			CleanStatus(foot);				//重置所有单词的状态，主要为了防止部分相同单词的存在，如消除absolute后只需键入ly即可清除absolutely的情况
			return;							//其他单词无需再检查，直接返回
		}
		else	//字符匹配且未完成	
			p->status = ( p->status << 1 ) + 1;		//添加状态

		p = p->next;
		FirstLoop = 0;
	}
}


/*
【辅助函数】
清除所有单词的状态
*/
void CleanStatus(struct line *foot)
{
	struct line *p=foot;
	int FirstLoop=1;

	//借助指示变量来遍历链表
	while(p != foot || FirstLoop)
	{
		p->status = 0;
		FirstLoop = 0;	
		p = p->next;
	}
}


/*
刷新，清屏后，逐个调用PrintWord()来输出
*/
void Refresh(struct line *wp, struct line *head)
{
	char *p;					//用于输出状态栏文字的字符指针
	char *tip = "<ESC>:EXIT";	//提示文字的字符指针
	char result[10];			//记录结果记录的字符串
	char *cp_result=result;		//指向结果记录的字符指针
	char *cp_name = "--BY YaHei(ZK)--";

	text_mode();	//重载文本模式，清屏

	//遍历链表（并不是完全遍历，因为一开始有部分节点没有信息，所以是从foot遍历到head），逐个打印
	while(wp != head)
	{	
		PrintWord(wp);
		wp = wp->next;
	}
	PrintWord(head);

	/*以下为状态栏********/

	//打印退出按键的提示文字
	p = Ptr(0, _height-1);
	while(*tip != '\0')
	{
		*p = *tip++;
		*(p+1) = BLUE << 4;
		p += 2;
	}

	//打印成绩记录
	sprintf(result, "Grade:%03d", grade);	//格式化成绩记录字符串
	p = Ptr(_width - 9, _height-1);
	while(*cp_result != '\0')
	{
		*p = *cp_result++;
		*(p+1) = BLUE <<4;
		p += 2;
	}

	//打印作者信息
	p = Ptr(_width / 2 - 6, _height-1);
	while(*cp_name != '\0')
	{
		*p = *cp_name++;
		*(p+1) = BLUE <<4;
		p += 2;
	}

	/********以上为状态栏*/

}


/*
【辅助函数】
打印单词
*/
void PrintWord(struct line *wp)
{
	char *p, *c=wp->word;
	int statusCk=1;	//特定位置字符的状态探针

	p = wp->stptr;
	while(*c != '\0')
	{
		*p = *c++;
		if(wp->status & statusCk)	//检查特定字符是否已被匹配
			*(p+1) = RED << 4;
		statusCk <<= 1;				//探针偏移
		p += ColorBytes;
	}
}


/*
打印报告
按任意键后可退出游戏
*/
void CallReport(FILE *fp)
{
	int i, middle, center;
	char result[20];
	char *cp_report = "Report", *cp_result = result;
	char *cp_tip = "Press any key to exit";

	sprintf(result, "Result:%03d/%03d(%02d%%)", grade, count, grade*100/count);

	text_mode();

	middle = _height / 2;
	center = _width / 2;

	//打印上下边框
	for(i=-12; i<12; i++)
		*Ptr(center+i, middle-2) = *Ptr(center+i, middle+2) = '-';

	//打印左右边框
	for(i=-2; i<3; i++)
		*Ptr(center-13, middle+i) = *Ptr(center+12, middle+i) = '|';

	//打印“Report”
	for(i=-3; i<3; i++)
		*Ptr(center+i, middle-1) = *cp_report++;

	//打印“Result:***/***(**%)”
	for(i=-9; i<10; i++)
		*Ptr(center+i, middle) = *cp_result++;

	//打印“Press any key to exit”
	for(i=-10; i<11; i++)
		*Ptr(center+i, middle+1) = *cp_tip++;
	
	//获取任意键后关闭文件、清屏、退出游戏
	bioskey(0);
	fclose(fp);		//文件关闭
	text_mode();
	exit(1);
}


/*
打印速度选择界面
返回所选的速度档次
*/
char SpeedOption(void)
{
	char *cp_diff="Difficulty", *cp_easy="[E]asy", *cp_normal="[N]ormal", *cp_difficult="[D]ifficult", *cp_tip="Press E/N/D to select";
	int i, middle, center;
	char Board_k;

	text_mode();	//清屏

	middle = _height / 2;
	center = _width / 2;

	//打印上下边框
	for(i=-11; i<12; i++)
		*Ptr(center+i, middle-3) = *Ptr(center+i, middle+3) = '-';

	//打印左右边框
	for(i=-3; i<4; i++)
		*Ptr(center-12, middle+i) = *Ptr(center+12, middle+i) = '|';

	//打印“Difficulty”
	for(i=-5; i<5; i++)
		*Ptr(center+i, middle-2) = *cp_diff++;

	//打印"[E]asy"
	for(i=-5; i<1; i++)
		*Ptr(center+i, middle-1) = *cp_easy++;

	//打印“[N]ormal”
	for(i=-5; i<3; i++)
		*Ptr(center+i, middle) = *cp_normal++;

	//打印“[D]ifficult”
	for(i=-5; i<6; i++)
		*Ptr(center+i, middle+1) = *cp_difficult++;

	//打印“Press E/N/D to select”
	for(i=-10; i<11; i++)
		*Ptr(center+i, middle+2) = *cp_tip++;

	//过滤无效按键
	do
	{
		Board_k = bioskey(0);
	}while(Board_k != 'e' && Board_k != 'n' && Board_k != 'd');

	return Board_k;
}


/*
随机从文件中读取一个单词，放入word字符串中
*/
void RandomChoice(FILE *fp, char *word)
{
	char c;

	fseek(fp, rand() % fbytes, SEEK_SET);	//fbytes为全局变量
	
	//由于位置随机，一般都在单词中间，所以要跳过该单词
	while( ( c=fgetc(fp) ) != '\n' );	
	
	//如果恰好跳过最后一个单词，那么输出第一个单词（不要重新随机定位，否则第一个单词没有输出的机会）
	if(feof(fp))	
		rewind(fp);

	//读取单词，放入word中
	fscanf(fp, "%s", word);
}
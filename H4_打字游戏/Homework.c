#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>

#define ColorBytes (_color_bits/8)						//每个元素占用的字节数
#define Ptr(x,y) (_vp+(_width*(y)+(x))*ColorBytes)		//指向坐标(x,y)的指针
#define ESC 0x011b
#define BOARD_T 100 	//键盘检测周期（毫秒/次）

struct line
{
	char word[20];			//单词内容
	char *stptr;			//指向本行输出头部的指针
	int len;				//单词长度
	struct line *next;		//指向下一节链表的指针
	long status;			//单词状态，bit string，默认为0
};

struct line * InitList(int n);
void NewLine(struct line *head, char *word);
void PrintWord(struct line *wp);
// void CleanScreen(struct line *head);
// int fgetline(FILE *fp, char *str);
void StatusCheck(struct line *foot, char c);
void Refresh(struct line *foot, struct line *head);
void CallReport(void);
void CleanStatus(struct line *foot);
char SpeedOption(void);

int grade=0;	//记录成绩
int count=0;	//记录单词总数

void main()
{
	struct line *head, *next, *foot;		//指向第一行
	FILE *fp;
	char word[20];
	// int result;
	int Board_k;		//记录键盘按键
	int MoveSpeed;	//滚动速度，即每行MS毫秒
	int SleepTime;		

	//获取用户选择的速度档，并转换成各档对应的滚动速度
	MoveSpeed = SpeedOption();
	switch(MoveSpeed)
	{
		case 'e': MoveSpeed = 1000; break;
		case 'n': MoveSpeed = 750; break;
		case 'd': MoveSpeed = 500; break;
	}

	fp = fopen("words.txt", "r");

	srand((int)time(0));	//设置时间为随机种子

	text_mode();	//清屏
	foot = head = InitList(_height);		//初始化空链表，用来储存各行信息
	
	//生成首行
	// fgetline(fp, word);
	fscanf(fp, "%s", word);
	count++;
	NewLine(head, word);

	while(!feof(fp) || foot->word[0]!='\0')
	{	
		// CleanScreen(foot);
		// text_mode();	//重载文本模式，清屏

		//生成新的首行
		head = head->next;
		// result = fgetline(fp, word);
		// if(result == 0)	break;
		if(!feof(fp))
		{
			fscanf(fp, "%s", word);
			count++;
		}
		else
			word[0]='\0';

		NewLine(head, word);
		
		//其余各行下移
		if(head->next->len==0)	//文本末行不是屏幕末行？
			next = foot;
		else
			foot = next = foot->next;

		while(next != head)
		{
			next->stptr += _width * ColorBytes;
			next = next->next;
		}

		// 刷新（重打印）
		// next = foot;
		// while(next != head)
		// {	
		// 	PrintWord(next);
		// 	next = next->next;
		// }
		// PrintWord(head);
		Refresh(foot, head);


		// 按照一定频率扫描键盘缓冲区
		// 可是sleep只支持秒不支持毫秒，不好操作
		// 但是delay单位为毫秒
		for(SleepTime = 0; SleepTime < MoveSpeed; SleepTime += BOARD_T)
		{
			while(bioskey(1))
			{
				if(Board_k = bioskey(0))
					if(Board_k == ESC)
						CallReport();
					else  //根据按键匹配各个单词，修改状态后刷新
					{
						StatusCheck(foot, Board_k);
						Refresh(foot, head);
					}
				
			}
			delay(BOARD_T);
		}

		// sleep(1);
		// while(bioskey(1))
		// 	if(Board_k =bioskey(0))
		// 		StatusCheck(foot, Board_k);
	}

	fclose(fp);
	CallReport();
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
	head->len = head->status = 0;

	//依次创建剩余的n-1个节点
	p1 = head;
	for(i=0; i<n-1; i++)
	{
		p2 = (struct line *) malloc(sizeof(struct line));
		p1->next = p2;
		p2->len = p2->status = 0;
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
	// char *p=head->word;

	head->len = strlen(word);				//单词长度
	strcpy(head->word, word);				//单词内容
	
	// while(*word != '\0')
	// 	*p++ = *word++;
	// *p = '\0';

	x = rand() % ( _width - head->len );	//产生随机x坐标
	head->stptr = Ptr(x,0); 				//单词头部的指针
	// x = 10;
	// head->stptr = _vp + x * (_color_bits / 8);
	head->status = 0;						//状态清零
}

/*
清屏
好奇怪！！好像屏幕上每个元素只能写入两次，用空格清屏后该点变得不可写入？？？
难道只能重新载入text_mode()来清屏？？
*/
/*
void CleanScreen(struct line *head)
{
	char *p;
	int i;
	struct line *wp;

	//清空首行内容
	p = head->stptr;
	for(i=0; i<head->len; i++)
	{
		*p = ' ';
		*(p+1) = 0;
		p += ColorBytes;
	}
	
	//清空剩余行内容
	wp = head->next;
	while(wp != head && wp->len != 0)
	{	
		p = wp->stptr;
		for(i=0; i<wp->len; i++)
		{
			*p = ' ';
			*(p+1) = 0;
			p += ColorBytes;
		}
		wp = wp->next;
	}
}
*/

/*文件读取测试，手动读取*/
/*
int fgetline(FILE *fp, char *str)
{
	int c;

	while(( c=fgetc(fp) ) != '|')
	{
		if(c == EOF)
			return 0;
		*str++ = c;
	}
	*str = '\0';
	return 1;
}
*/

/*
状态检查，根据按键判断是否有匹配字母
第二个参数c由于是char，会截断Board_k的后八位，即按键的ASSIC码
*/
void StatusCheck(struct line *foot, char c)
{
	struct line *p=foot;
	int isTyped, i, FirstLoop=1;

	while(p != foot || FirstLoop)
	{
		//跳过已打出的字母
		i = 0;
		do
		{
			isTyped = p->status & (1 << i);
			i++;
		}while(isTyped);	//此时i为所要判断的字母的索引+1

		//比较字符
		if(p->word[i-1] != c || i > p->len)	//字符不匹配 或 索引超出范围？
			p->status = 0;
		else if(i == p->len)	//匹配且恰好为最后一个字符？
		{
			grade++;		//【Global】成绩增加
			p->word[0] = '\0';	//“清除”单词
			CleanStatus(foot);	//重置所有单词的状态，主要为了防止部分相同单词的存在，如absolute和absolutely，消除absolute后只需键入ly即可清除absolutely的情况
			return;		//不存在相同的单词，没有必要继续检查其他单词
		}
		else	//字符匹配且未完成	
			p->status = ( p->status << 1 ) + 1;	//添加状态

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
	char *p;
	char *tip = "<ESC>:EXIT";
	char result[10];
	char *cp_result=result;
	sprintf(result, "Grade:%03d", grade);

	text_mode();	//重载文本模式，清屏
	while(wp != head)
	{	
		PrintWord(wp);
		wp = wp->next;
	}
	PrintWord(head);

	//打印状态栏
	p = Ptr(0, _height-1);
	while(*tip != '\0')
	{
		*p = *tip++;
		*(p+1) = BLUE << 4;
		p += 2;
	}

	p = Ptr(_width - 9, _height-1);
	while(*cp_result != '\0')
	{
		*p = *cp_result++;
		*(p+1) = BLUE <<4;
		p += 2;
	}
}

/*
【辅助函数】
打印单词
*/
void PrintWord(struct line *wp)
{
	char *p, *c=wp->word;
	// int i;
	int statusCk=1;	//特定位置字符的状态探针

	p = wp->stptr;
	while(*c != '\0')
	// for(i=0; i<wp->len; i++)
	{
		// *p = wp->word[i];
		*p = *c++;
		if(wp->status & statusCk)	//检查特定字符是否已被匹配
			*(p+1) = RED << 4;
		statusCk <<= 1;
		p += ColorBytes;
	}
}

void CallReport(void)
{
	int i, middle, center;
	char result[20];
	char *cp_report = "Report", *cp_result = result;
	char *cp_tip = "Press any key to exit.";

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

	//打印“Press any key to exit.”
	for(i=-11; i<11; i++)
		*Ptr(center+i, middle+1) = *cp_tip++;
	
	bioskey(0);
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
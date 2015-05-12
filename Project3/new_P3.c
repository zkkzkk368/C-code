#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>

#define ColorBytes (_color_bits/8)
#define Ptr(x,y) (_vp+(_width*(y)+(x))*ColorBytes)		//指向坐标(x,y)的指针

struct line
{
	char word[20];				//单词内容
	char *stptr;				//指向本行输出头部的指针
	int len;				//单词长度
	struct line *next;		//指向下一节链表的指针
	int status;				//单词状态，bit string，默认为0
};

struct line * InitList(int n);
void NewLine(struct line *head, char *word);
void PrintWord(struct line *wp);
// void CleanScreen(struct line *head);
// int fgetline(FILE *fp, char *str);

void main()
{
	struct line *head, *next, *foot;		//指向第一行
	FILE *fp;
	char word[20];
	int result;

	fp = fopen("words.txt", "r");

	srand((int)time(0));	//设置时间为随机种子

	text_mode();	//清屏
	foot = head = InitList(_height);		//初始化空链表，用来储存各行信息
	
	//生成首行
	// fgetline(fp, word);
	fscanf(fp, "%s", word);
	NewLine(head, word);

	while(!feof(fp))
	{	
		// CleanScreen(foot);
		text_mode();	//重载文本模式，清屏

		//生成新的首行
		head = head->next;
		// result = fgetline(fp, word);
		// if(result == 0)	break;
		fscanf(fp, "%s", word);
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

		//刷新（重打印）
		next = foot;
		while(next != head)
		{	
			PrintWord(next);
			next = next->next;
		}
		PrintWord(head);

		sleep(1);
	}

	fclose(fp);
	getchar();
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
打印单词
*/
void PrintWord(struct line *wp)
{
	char *p;
	int i;

	p = wp->stptr;
	for(i=0; i<wp->len; i++)
	{
		*p = wp->word[i];
		p += ColorBytes;
	}

	//缺少字体样式输出
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
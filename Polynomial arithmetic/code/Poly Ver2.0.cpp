/********************************头文件包含**************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <process.h>
/*********************************常量声明**************************/
#define MAX_INT 32000
#define STACK_INIT_SIZE 100
#define ERROR -1
#define STACKINCREMENT 10 
#define TRUE 1
#define FALSE 0
#define OK 1
/******************************结构声明*****************************/
typedef struct
{ 
//运算符栈
char *base;//栈底指针
char *top; //栈顶指针
int stacksize; //栈规模
}stack1;

typedef struct
{ 
//运算数栈
int *base;//栈底指针
int *top;//栈顶指针
int stacksize;//栈规模
}stack2; 

struct li
{/*指针链表*/
	struct p* px;//存储各个多项式的头指针
	int num;//存储多项式序号
	struct li *next;//形成链表存储结构
};

struct p
{/*多项式结构 coef为常数 expon为指数*/
	int coef;//多项式常数
	int expon;//多项式系数
	struct p *next;//链表多项式存储结构
};
/**************************函数声明*********************************/
void init(); //初始化函数
void creatlist(int n); //建立多项式存储列表
struct li *add(int num); //添加多项式
struct p* jia(struct p* p1,struct p* p2); //加法操作并返回结果
struct p* jian(struct p* p1,struct p* p2); //减法操作并返回结果
struct p* cheng(struct p* p1,struct p*p2); //乘法操作并返回结果
int evaluate(); //计算表达式，返回是否计算成功
struct p* find(int num); //通过序号找到对应多项式
void print(int n); //输出函数
int InitStack(stack1 &S); //初始化运算符栈
int InitStack(stack2 &S); //初始化运算数栈
char GetTop(stack1 S); //获取运算符栈栈顶元素，返回运算符
int GetTop(stack2 S); //获取运算数栈栈顶元素，返回序号
int InOP(char c); //判断输入是否为运算符
int push(stack1 &S,char e); //运算符入栈
int push(stack2 &S,int e); //运算数入栈
char precede(char A,char B); //判断运算符优先级
int pop(stack1 &S,char &e); //运算符栈栈顶元素出栈
int pop(stack2 &S,int &e); //运算数栈栈顶元素出栈
int operate(int a,char theta,int b); //计算两个表达式和运算符的结果，返回是否成功
/*******************************全局变量声明***************************/
struct li *head,*end;
int sizep=sizeof(struct p); //规模初始化
int sizeli=sizeof(struct li);
int n;
/************************************主函数*************************/
int main()
{
	init();
	evaluate();
	system("pause");
	return 0;
}
/*********************************初始化函数*************************/
void init()
{
	printf("please input the number of the polymerization:");
	scanf("%d",&n);
	creatlist(n);
}
/***************************************输出函数********************/
void print(int n)
{
	struct p * temp,*t;
	temp=find(n);
		/*忽略前段被已被消去的项*/
	while(temp->coef == 0 && temp!=NULL)
		temp=temp->next;
	
	/*如果每一项都被消去了*/
	if(temp == NULL)
	{
		printf("0\n");
		return;
	}

	/*输出第一项——判断第一项是否为常数项*/
	if(temp->expon == 0)
		printf("%d\n",temp->coef);
	else
		printf("%dx^%d",temp->coef,temp->expon);
	
	/*输出中间项*/
	for(t=temp->next; t->next != NULL; t=t->next)
		if(t->coef != 0)
			/*有效项？*/
			if(t->expon == 1)
				/*一次项？*/
				printf("%+dx",t->coef);
			else
				printf("%+dx^%d",t->coef,t->expon);

	/*输出最后一项*/
	if(t->coef == 0)
		/*被消去？*/
		printf("\n");
	else if(t->expon == 0)
		/*常数项？*/
		printf("%+d\n",t->coef);
	else if(t->expon == 1)
		/*一次项？*/
		printf("%+dx",t->coef);
	else
		/*其余情况*/
		printf("%+dx^%d\n",t->coef,t->expon);

	// printf("%dx^%d",temp->coef,temp->expon);


	// for(t=temp->next;t!=NULL;t=t->next)
	// {
	// 	if(t->coef>0) printf("+");
	// 	printf("%dx^%d",t->coef,t->expon);
	// }
}
/*********************************搜寻函数**************************/
struct p* find(int num)
{
	struct li* temp;
	for(temp=head;temp->num!=num;temp=temp->next); //判断是否找到对应序号多项式
	return temp->px; //这里少了一个无法找到的返回值
}
/**************************************表达式计算*********************/
int evaluate()
{
	char c,x,theta,prec;
	//c储存读入字符，x读取多余括号，theta储存进行操作的运算符，prec储存前一个字符
	int a,b,result;
	//a，b储存进行计算的多项式序号，result储存结果的多项式序号
	int cc,flag,minus=1;
	//cc储存序号，flag储存符号，minus储存符号
	stack1 OPTR;//运算符栈
	stack2 OPND;//运算数栈
	InitStack(OPTR);InitStack(OPND);
	push(OPTR,'=');//先将'='压入运算符栈，为输出结果准备
	prec='=';
	getchar();
	scanf("%c",&c);
	while(c!='='||GetTop(OPTR)!='=')//判断是否读到算式的结尾
	{
		cc=0;flag=0;
		if(c=='-'&&(prec=='='||prec=='(')) //读取项之前的'-'
		{
			minus=-1;prec=c;
			scanf("%c",&c);
		}
		else if(!InOP(c)) //读入的不是运算符
		{
			while(!InOP(c))
			{
				if(c>='0'&&c<='9')
				{
					cc=cc*10+c-'0';
				}
				prec=c;
				scanf("%c",&c);
			}
			cc*=minus;minus=1;
			push(OPND,cc);//运算数入栈
		}
		else //读入运算符，做计算
			switch(precede(GetTop(OPTR),c)) //判断栈顶元素和当前运算符的优先级
			{
				case '<':push(OPTR,c);prec=c;scanf("%c",&c);break; //当前运算符优先级高则当前运算符入栈
				case '=':pop(OPTR,x);prec=c;scanf("%c",&c);break; //读取多余的'='
				case '>':pop(OPTR,theta);pop(OPND,b);pop(OPND,a);push(OPND,operate(a,theta,b));break; //当前运算符优先级低则去运算数栈栈顶元素进行进行计算
			}
	}
	result=GetTop(OPND); //最后剩下的运算数栈中元素就是答案
	print(result); //输出
	return OK;
}
/***************************************************************/
//运算符栈
int InitStack(stack1 &S)
{
	S.base=(char *)malloc(STACK_INIT_SIZE*sizeof(char)); //栈底指针初始化
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE; //分配栈容量
	return OK;
}
char GetTop(stack1 S) //返回栈顶元素
{
	if(S.top==S.base) return ERROR;
	return *(S.top-1);
}
int push(stack1 &S,char e)
{
	if(S.top-S.base>=S.stacksize) //判断入栈元素是否超过栈容量
	{
		S.base=(char *)realloc(S.base,(S.stacksize+STACKINCREMENT)*sizeof(char)); //重新分配栈元素数量
		if(!S.base)exit(OVERFLOW);
		S.top=S.base+S.stacksize;
		S.stacksize+=STACKINCREMENT;
	}
	*S.top++=e;
	return OK;
}
int pop(stack1 &S,char &e) //栈顶元素出栈，并将元素返回给e地址所对应变量
{
	if(S.top==S.base) return ERROR;
	e=*(--S.top);
	return OK;
}
/***************************************************************/
//运算数栈
int InitStack(stack2 &S)
{
	S.base=(int *)malloc(STACK_INIT_SIZE*sizeof(int)); //栈底指针初始化
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE; //分配栈容量
	return OK;
}
int GetTop(stack2 S) //返回栈顶元素
{
	if(S.top==S.base)return ERROR;
	return *(S.top-1);
}
int push(stack2 &S,int e)
{
	if(S.top-S.base>=S.stacksize)
	{
		S.base=(int *)realloc(S.base,(S.stacksize+STACKINCREMENT)*sizeof(float));
		if(!S.base)exit(OVERFLOW);
		S.top=S.base+S.stacksize;
		S.stacksize+=STACKINCREMENT;
	}
	*S.top++=e;
	return OK;
}
int pop(stack2 &S,int &e)
{
	if(S.top==S.base) return ERROR;
	e=*(--S.top);
	return OK;
}
/***************************************************************/
char precede(char A,char B) //判断两个运算符的优先级
{
	switch(A)
	{
		case '+':
			switch(B)
			{
				case '+':return '>';
				case '-':return '>';
				case '*':return '<';
				case '(':return '<';
				case ')':return '>';
				case '=':return '>';
				default:printf("wron expression!\n");exit(0);
			}
		case '-':
			switch(B)
			{
				case '+':return '>';
				case '-':return '>';
				case '*':return '<';
				case '(':return '<';
				case ')':return '>';
				case '=':return '>';
				default:printf("wrong expression!\n");exit(0);
			}
		case '*':
			switch(B)
			{
				case '+':return '>';
				case '-':return '>';
				case '*':return '>';
				case '(':return '<';
				case ')':return '>';
				case '=':return '>';
				default:printf("wrong expression\n");exit(0);
			}
		case '(':
			switch(B)
			{
				case '+':return'<';
				case '-':return'<';
				case '*':return'<';
				case '(':return'<';
				case ')':return'=';
				case '=':printf("wrong expression\n");exit(0);
				default:printf("wrong expression\n");exit(0);
			}
		case')':
			switch(B)
			{
				case '+':return'>';
				case '-':return'>';
				case '*':return'>';
				case '(':printf("wrong expression\n");exit(0);
				case ')':return'>';
				case '=':return'>';
				default:printf("wrong expression\n");exit(0);
			}
		case '=':
			switch(B)
			{
				case '+':return'<';
				case '-':return'<';
				case '*':return'<';
				case '(':return'<';
				case ')':printf("wrong expression\n");exit(0);
				case '=':return'=';
				default:printf("wrong expression\n");exit(0);
			}
		default:printf("wrong expression\n");exit(0);
	}
}
int InOP(char c) //判断读入是否为运算符
{
	switch(c)
	{
		case '+':return TRUE;
		case '-':return TRUE;
		case '*':return TRUE;
		case '(':return TRUE;
		case ')':return TRUE;
		case '=':return TRUE;
		default:return FALSE;
	}
}
/***************************************************************/
int operate(int a,char theta,int b) //计算运算数和运算符
{
	struct li* tail;
	switch(theta)
	{
		case '+':
			n++; //序号容量增加
			tail=(struct li*)malloc(sizeli);
			tail->px=jia( find(a),find(b) );//加法运算并将结果挂载到多项式链表尾部
			tail->num=n;
			tail->next=NULL;
			end->next=tail;
			end=end->next;
			return n;
		case '-':
			n++; //序号容量增加
			tail=(struct li*)malloc(sizeli);
			tail->px=jian( find(a),find(b) );//减法运算并将结果挂载到多项式链表的尾部
			tail->num=n;
			tail->next=NULL;
			end->next=tail;
			end=end->next;
			return n;
		case '*':
			n++;//序号容量增加
			tail=(struct li*)malloc(sizeli);
			tail->px=cheng( find(a),find(b) );//乘法运算并将结果挂载到多项式链表的尾部
			tail->num=n;
			tail->next=NULL;
			end->next=tail;
			end=end->next;
			return n;
		default:printf("wrong expression\n");
	}
}
/***************************************************************/
void creatlist(int n) //创建多项式存储链表
{
	struct li *tail;
	int i;
	
	head=add(1);
	tail=head;
	for(i=2;i<=n;i++)
	{
		tail->next=add(i);
		tail=tail->next;
	}
	end=tail;
}
/***************************************************************/
struct li *add(int num) //挂载新指针
{
	struct p * p1,*p2,*p3;//p1为多项式头指针,p2为多项式为指针,p3为多项式临时指针
	struct li* tail;
	char ch;
	int flag,tmpa,tmp;//flag正负判断，tmp临时变量，tmpa多项式系数的临时变量

	p1=(struct p*)malloc(sizep);
	p1->next=NULL;
	p2=p1;
	tail=(struct li*)malloc(sizeli);
	ch=getchar();
	ch=getchar();
	flag=1;
	if(ch=='-')//读入初始'-'号
	{
		flag=-1;
		ch=getchar();
	}
	tmp=0;
	tmpa=0;
	while( ch != '#' )//判断多项式是否读到尾部
	{
		if(ch>='0'&&ch<='9') tmp=tmp*10+ch-'0';
		if(ch=='x') //读入'x'后存储ai的值
		{
			tmpa=tmp*flag;
			if(tmp==0) tmpa=1;
			tmp=0;
		}
		if(ch=='+'||ch=='-')//存入上一项的系数和次数，并将尾指针后移
		{
			if(ch=='+') flag=1;
			else flag=-1;
			p3=(struct p*)malloc(sizep);
			if(tmp==0) //对于次数是1的项需要特殊处理
				{
					p3->expon=1;
					p3->coef=tmpa;
				}
			else 
				{
					p3->expon=tmp;
					p3->coef=tmpa;
				}
			tmp=0;tmpa=0;
			p3->next=NULL;
			p2->next=p3;
			p2=p2->next;
		} 
		ch=getchar();
	}
	//读入最后一项：1.次数为0；2.次数为其他；3.次数为0。
	p3=(struct p*)malloc(sizep);
	if(tmp!=0&&tmpa==0)
		{
			p3->coef=tmp;
			p3->expon=0;
		}
	else if(tmp!=0) 
		{
			p3->coef=tmpa;
			p3->expon=tmp;
		}
	else 
		{
			p3->coef=tmpa;
			p3->expon=1;
		}
	p3->next=NULL;
	p2->next=p3;
	p1=p1->next;
	tail->px=p1;//将新指针挂到尾指针上
	tail->num=num;
	tail->next=NULL;
	return tail;//返回新的指针
}
/***************************************************************/
struct p *jia(struct p* p1,struct p* p2)
{
//做加法运算时，将加数从高到低分别加到ans指针尾部
	struct p *ans,*tail,*temp;
	//ans为结果多项式头指针，tail为结果多项式尾指针，temp为临时指针
	ans=(struct p*)malloc(sizep);
	ans->next=NULL;
	tail=ans;
	while(p1!=NULL&&p2!=NULL)//两个多项式都没有到尾部
	{
		//做加法时优先挂载次数高的项，次数相同时需要合并
		temp=(struct p*)malloc(sizep);
		if( p1->expon > p2->expon )
		{
			temp->expon=p1->expon;
			temp->coef=p1->coef;
			temp->next=NULL;
			p1=p1->next;
		}
		else if( p1->expon==p2->expon )
		{
			temp->expon=p1->expon;
			temp->coef=p1->coef+p2->coef;
			temp->next=NULL;
			p1=p1->next;
			p2=p2->next;
		}
		else if( p1->expon < p2->expon )
		{
			temp->expon=p2->expon;
			temp->coef=p2->coef;
			temp->next=NULL;
			p2=p2->next;
		}
		tail->next=temp;
		tail=tail->next;
	}
	//读入多余的项
	while(p1!=NULL)
	{
		temp=(struct p*)malloc(sizep);
		temp->expon=p1->expon;
		temp->coef=p1->coef;
		temp->next=NULL;
		p1=p1->next;
		tail->next=temp;
		tail=tail->next;
	}
	while(p2!=NULL)
	{
		temp=(struct p*)malloc(sizep);
		temp->expon=p2->expon;
		temp->coef=p2->coef;
		temp->next=NULL;
		p2=p2->next;
		tail->next=temp;
		tail=tail->next;
	}
	ans=ans->next;
	return ans;
}
/***************************************************************/
struct p *jian(struct p* p1,struct p* p2)
{
//做加法运算时，将加数从高到低分别加到ans指针尾部，方法同加法
	struct p *ans,*tail,*temp;
	ans=(struct p*)malloc(sizep);
	ans->next=NULL;
	tail=ans;
	while(p1!=NULL&&p2!=NULL)
	{
		temp=(struct p*)malloc(sizep);
		if( p1->expon > p2->expon )
		{
			temp->expon=p1->expon;
			temp->coef=p1->coef;
			temp->next=NULL;
			p1=p1->next;
		}
		else if( p1->expon==p2->expon )
		{
			temp->expon=p1->expon;
			temp->coef=p1->coef-p2->coef;
			temp->next=NULL;
			p1=p1->next;
			p2=p2->next;
		}
		else if( p1->expon < p2->expon )
		{
			temp->expon=p2->expon;
			temp->coef=-p2->coef;
			temp->next=NULL;
			p2=p2->next;
		}
		tail->next=temp;
		tail=tail->next;
	}
	while(p1!=NULL)
	{
		temp=(struct p*)malloc(sizep);
		temp->expon=p1->expon;
		temp->coef=p1->coef;
		temp->next=NULL;
		p1=p1->next;
		tail->next=temp;
		tail=tail->next;
	}
	while(p2!=NULL)
	{
		temp=(struct p*)malloc(sizep);
		temp->expon=p2->expon;
		temp->coef=-p2->coef;
		temp->next=NULL;
		p2=p2->next;
		tail->next=temp;
		tail=tail->next;
	}
	ans=ans->next;
	return ans;
}
/***************************************************************/
struct p *cheng(struct p* p1,struct p* p2)
{
//乘法运算时，乘数各项先相乘，然后根据次数用插入排序的方式加到ans指针上
	struct p *ans,*pp1,*pp2,*t1,*t2,*temp;
	//ans为结果的头指针，pp1,pp2分别为p1,p2的临时指针，t1,t2为挂载到ans是插入排序的临时判断变量
	ans=(struct p*)malloc(sizep);
	ans->expon=MAX_INT;
	ans->next=NULL;
	for(pp1=p1;pp1!=NULL;pp1=pp1->next)
		for(pp2=p2;pp2!=NULL;pp2=pp2->next)//将p1,p2各项相乘
		{
			temp=(struct p*)malloc(sizep);
			temp->expon=pp1->expon+pp2->expon;
			temp->coef=pp1->coef*pp2->coef;
			temp->next=NULL;
			for(t1=ans,t2=t1->next; t2!=NULL && t2->expon>temp->expon ;t1=t1->next,t2=t2->next);//插入排序
			if(t2==NULL)
			{
				t1->next=temp;
			}
			else if(t2->expon==temp->expon)
			{
				t2->coef=t2->coef+temp->coef;
			}
			else
			{
				temp->next=t2;
				t1->next=temp;
			}
		}
	ans=ans->next;
	return ans;
}
/***************************************************************/


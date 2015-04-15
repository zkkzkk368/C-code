/*
      / \`\          __
      |  \ `\      /`/ \
      \_/`\  \-"-/` /\  \
           |       |  \  |
           (d     b)   \_/
           /       \ 
       ,".|.'.\_/.'.|.",
      /   /\' _|_ '/\   \
      |  /  '-`"`-'  \  |
      | |             | |
      | \    \   /    / |
       \ \    \ /    / /
        `"`\   :   /'"`
            `""`""`    
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INT 32000
#define TWO    	/*只满足于两个多项式*/

struct li
{/*指针链表*/
	struct p* px;	/*指向多项式的某一项*/
	int num;	/*项数编号*/
	struct li *next;	/*下个多项式*/
};

struct p
{/*多项式结构*/
	int coef;	/*系数*/
	int expon;	/*指数*/
	struct p *next;	/*指向下一项*/
};

void init(); //初始化函数
void creatlist(int n); //建立多项式存储列表
struct li *add(int num); //添加多项式
struct p* jia(struct p* p1,struct p* p2); //加法操作并返回结果
struct p* cheng(struct p* p1,struct p*p2); //乘法操作并返回结果
void print(struct p *ans);	//打印结果

#ifdef TWO
char operator;
#endif	

struct li *head;
int sizep=sizeof(struct p); //规模初始化
int sizeli=sizeof(struct li);

int main()
{
	struct p *ans;

	init();
	#ifdef TWO
	if(operator == '+')
		ans = jia(head->px,head->next->px);
	else if(operator == '*')
		ans = cheng(head->px,head->next->px);
	else
	{	
		printf("The operator is invalid!"); 
		return 0;
	}
	#endif

	print(ans);
	/*system("pause");*/
	return 0;
}

/*
初始化
*/
void init()
{
	#ifdef TWO
	creatlist(2);
	#else
	int n;
	printf("please input the number of the polymerization:");
	scanf("%d",&n);
	creatlist(n);
	#endif
}

/*
建立多项式的存储表，用于初始化
*/
void creatlist(int n)
{
	struct li *tail;
	int i;
	
	/*第一个多项式的建立*/
	head=add(1);
	tail=head;

	/*其余多项式的建立*/
	#ifdef TWO
	printf("Please input the operator:");
	getchar();
	operator = getchar();
	tail->next=add(2);
	tail=tail->next;
	#else
	for(i=2;i<=n;i++)
	{
		tail->next=add(i);
		tail=tail->next;
	}
	#endif

}


struct li *add(int num) //挂载新指针
{
	struct p * p1,*p2,*p3;
	struct li* tail;
	char ch;
	int flag,tmpa,tmp;//flag正负判断，tmp临时变量，tmpa多项式系数的临时变量

	p1=(struct p*)malloc(sizep);
	p1->next=NULL;
	p2=p1;
	tail=(struct li*)malloc(sizeli);

	
	/*skip the '\n' from input*/
	#ifdef TWO
	if(num != 1)
		getchar();
	#else
	ch=getchar();
	#endif

	/*判断第一个字符是否为负号*/
	ch=getchar();
	flag=1;
	if(ch=='-')//读入初始'-'号
	{
		flag=-1;
		ch=getchar();
	}

	/*初始化临时变量*/
	tmp=0;
	tmpa=0;

	/*获取并分析处理剩余字符*/
	while( ch != '#' )
	{
		/*获取数字*/
		if(ch>='0'&&ch<='9') tmp=tmp*10+ch-'0';
		
		/*将x之前的数字作为系数*/
		if(ch=='x')
		{
			tmpa=tmp*flag;
			if(tmp==0) tmpa=1;
			tmp=0;
		}

		/*将符号前的数字作为指数*/
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

			tmp=0;tmpa=0;	/*重新初始化临时变量*/
			
			/*将该项存入链表中*/
			p3->next=NULL;
			p2->next=p3;
			p2=p2->next;
		} 

		ch=getchar();
	}

	//读入最后一项：次数为1，次数为0，次数为其他。
	p3=(struct p*)malloc(sizep);
	if(tmp!=0&&tmpa==0)
	{
		p3->coef=tmp*flag;
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

	#ifdef TEST
	for(p3=p1;p3!=NULL;p3=p3->next){printf("%dx^%d\n",p3->coef,p3->expon);}
	#endif
		
	return tail;//返回新的指针
}

/*
加法运算
*/
struct p *jia(struct p* p1,struct p* p2)
{//做加法运算时，将加数从高到低分别加到ans指针尾部
	struct p *ans,*tail,*temp;
	ans=(struct p*)malloc(sizep);
	ans->next=NULL;
	tail=ans;

	/*将两个多项式按指数从高到低复制到以ans为首节点的链表上，指数相同则合并*/
	while(p1!=NULL&&p2!=NULL)
	{
		temp=(struct p*)malloc(sizep);

		/*指数比较*/
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

		/*对应项入栈*/
		tail->next=temp;
		tail=tail->next;
	}

	/*读取p1余下的项*/
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

	/*读取p2余下的项*/
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

	#ifdef TEST
	for(temp=ans;temp!=NULL;temp=temp->next){printf("%dx^%d\n",temp->coef,temp->expon);}
	#endif

	return ans;
}

/*
乘法运算
*/
struct p *cheng(struct p* p1,struct p* p2)
{//乘法运算时，乘数各项先相乘，然后根据次数用插入排序的方式加到ans指针上
	struct p *ans,*pp1,*pp2,*t1,*t2,*temp;
	ans=(struct p*)malloc(sizep);
	ans->expon=MAX_INT;
	ans->next=NULL;
	for(pp1=p1;pp1!=NULL;pp1=pp1->next)
		for(pp2=p2;pp2!=NULL;pp2=pp2->next)
		{
			/*两项间的乘法运算*/
			temp=(struct p*)malloc(sizep);
			temp->expon=pp1->expon+pp2->expon;
			temp->coef=pp1->coef*pp2->coef;
			temp->next=NULL;

			/*插入排序*/
			for(t1=ans,t2=t1->next; t2!=NULL && t2->expon>temp->expon ;t1=t1->next,t2=t2->next);
			if(t2==NULL)
				t1->next=temp;
			else if(t2->expon==temp->expon)
				t2->coef=t2->coef+temp->coef;
			else
			{
				temp->next=t2;
				t1->next=temp;
			}
		}
	ans=ans->next;
	
	#ifdef TEST
	for(temp=ans;temp!=NULL;temp=temp->next){printf("%dx^%d\n",temp->coef,temp->expon);}
	#endif

	return ans;
}

/*
打印结果
*/
void print(struct p *ans)
{
	struct p *p;

	/*忽略前段被已被消去的项*/
	while(ans->coef == 0 && ans!=NULL)
		ans=ans->next;
	
	/*如果每一项都被消去了*/
	if(ans == NULL)
	{
		printf("0\n");
		return;
	}

	/*输出第一项——判断第一项是否为常数项*/
	if(ans->expon == 0)
		printf("%d\n",p->coef);
	else
		printf("%dx^%d",ans->coef,ans->expon);

	/*输出中间项*/
	for(p=ans->next; p->next != NULL; p=p->next)
		if(p->coef != 0)
			/*有效项？*/
			if(p->expon == 1)
				/*一次项？*/
				printf("%+dx",p->coef);
			else
				printf("%+dx^%d",p->coef,p->expon);

	/*输出最后一项*/
	if(p->coef == 0)
		/*被消去？*/
		printf("\n");
	else if(p->expon == 0)
		/*常数项？*/
		printf("%+d\n",p->coef);
	else if(p->expon == 1)
		/*一次项？*/
		printf("%+dx",p->coef);
	else
		/*其余情况*/
		printf("%+dx^%d\n",p->coef,p->expon);
}

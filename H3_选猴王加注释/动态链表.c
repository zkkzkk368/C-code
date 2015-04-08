typedef struct node        /*这个结构类型包括三个域 */
{	int number;        /*猴子的编号*/
	int mydata;        /* 猴子的定数 */
	struct node *next; /* 指向下一只猴子的指针 */
} linklist;

/*接收猴子总数，初始化猴子圈的链表*/
linklist *CreateCircle( int n ){
    linklist *head,*p,*s;	/*head为链表头，p为上一结点，s为当前结点*/
    int i;	/*计数器*/
    
    /*首结点的创建与初始化*/
    head =(linklist*)malloc(sizeof(linklist)); /*首节点创建*/
    p = head;   p->number = 1;	/*将首结点作为链表头，并且首结点编号为1*/
    scanf("%d",&p->mydata); /* 定数(正整数)，确定下一只出局的猴子*/
    p->next = NULL;	/*下一结点的指向初始化为空*/
    
    for( i=2; i<=n; i++ ){  /* 链表创建*/
		s = (linklist*)malloc(sizeof(linklist) );	/*结点创建*/
		s->number = i;	/*猴子编号*/
		scanf("%d",&s->mydata); /* 报数(正整数)，确定下一只出局的猴子*/
		p->next = s;	/*将上一结点next指向到当前节点*/
		p = s;	/*保存当前节点的指针，作为下一结点的“上一结点”*/
    }	
    
    p->next=head;   /*链表首尾相接构成循环链表*/
    return p;  /* 返回最后一只猴子的指针，因为它指向第一只猴子 */
}

/*接收猴子总数和链表头*/
int KingOfMonkey(int n,linklist *head){
    linklist *p = head;       /* head指向最后一只猴子结点 */  
    int i, j, steps = p->mydata;       /*用最后一只猴子的定数开始 */ 
    for( j=1; j<=n-1; j++ ) {   /*重复该过程n-1次*/ 
        for( i=1; i<steps; i++ )    p=p->next;	/*依据定数寻找下一个出局的猴子*/
	    
		/* 将p所指的下一个节点删除, 删除前取其定数 */
        steps = p->next->mydata;	/*取下即将删除的结点的定数*/ 
        printf(" %d ", p->next->number);	/*打印出局的猴子的编号*/
        DeleteNext(p);     /*删除结点p*/
    }   
    return p->number;	/*返回被删除的结点的编号*/
 } 
int main(){
    linklist *head;	/*链表头*/
    int n;	/*猴子总数*/
    
    printf("请输入猴子的总数和每只猴子的定数(必须是正整数)：");
    scanf("%d",&n);
	 
    head = CreateCircle(n); /*创建单循环链表，返回最后一个结点的指针 */
    printf("\nThe king is monkey[%d].\n", KingOfMonkey(n,head));	/*选出猴王，并且将其打印*/
    return 0;
}


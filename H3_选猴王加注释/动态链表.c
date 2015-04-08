typedef struct node        /*����ṹ���Ͱ��������� */
{	int number;        /*���ӵı��*/
	int mydata;        /* ���ӵĶ��� */
	struct node *next; /* ָ����һֻ���ӵ�ָ�� */
} linklist;

/*���պ�����������ʼ������Ȧ������*/
linklist *CreateCircle( int n ){
    linklist *head,*p,*s;	/*headΪ����ͷ��pΪ��һ��㣬sΪ��ǰ���*/
    int i;	/*������*/
    
    /*�׽��Ĵ������ʼ��*/
    head =(linklist*)malloc(sizeof(linklist)); /*�׽ڵ㴴��*/
    p = head;   p->number = 1;	/*���׽����Ϊ����ͷ�������׽����Ϊ1*/
    scanf("%d",&p->mydata); /* ����(������)��ȷ����һֻ���ֵĺ���*/
    p->next = NULL;	/*��һ����ָ���ʼ��Ϊ��*/
    
    for( i=2; i<=n; i++ ){  /* ������*/
		s = (linklist*)malloc(sizeof(linklist) );	/*��㴴��*/
		s->number = i;	/*���ӱ��*/
		scanf("%d",&s->mydata); /* ����(������)��ȷ����һֻ���ֵĺ���*/
		p->next = s;	/*����һ���nextָ�򵽵�ǰ�ڵ�*/
		p = s;	/*���浱ǰ�ڵ��ָ�룬��Ϊ��һ���ġ���һ��㡱*/
    }	
    
    p->next=head;   /*������β��ӹ���ѭ������*/
    return p;  /* �������һֻ���ӵ�ָ�룬��Ϊ��ָ���һֻ���� */
}

/*���պ�������������ͷ*/
int KingOfMonkey(int n,linklist *head){
    linklist *p = head;       /* headָ�����һֻ���ӽ�� */  
    int i, j, steps = p->mydata;       /*�����һֻ���ӵĶ�����ʼ */ 
    for( j=1; j<=n-1; j++ ) {   /*�ظ��ù���n-1��*/ 
        for( i=1; i<steps; i++ )    p=p->next;	/*���ݶ���Ѱ����һ�����ֵĺ���*/
	    
		/* ��p��ָ����һ���ڵ�ɾ��, ɾ��ǰȡ�䶨�� */
        steps = p->next->mydata;	/*ȡ�¼���ɾ���Ľ��Ķ���*/ 
        printf(" %d ", p->next->number);	/*��ӡ���ֵĺ��ӵı��*/
        DeleteNext(p);     /*ɾ�����p*/
    }   
    return p->number;	/*���ر�ɾ���Ľ��ı��*/
 } 
int main(){
    linklist *head;	/*����ͷ*/
    int n;	/*��������*/
    
    printf("��������ӵ�������ÿֻ���ӵĶ���(������������)��");
    scanf("%d",&n);
	 
    head = CreateCircle(n); /*������ѭ�������������һ������ָ�� */
    printf("\nThe king is monkey[%d].\n", KingOfMonkey(n,head));	/*ѡ�����������ҽ����ӡ*/
    return 0;
}


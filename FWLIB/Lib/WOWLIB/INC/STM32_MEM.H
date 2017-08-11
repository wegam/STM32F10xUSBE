/******************************** User_library *********************************
* 文件名 	: STM32_MEM.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* 数据管理
* 
* 
* 
* 
* 
*******************************************************************************/


#ifndef __STM32_MEM_H
#define __STM32_MEM_H

#include	"stdio.h"				//用于printf
#include	"string.h"			//用于printf
#include	"stdarg.h"			//用于获取不确定个数的参数
#include	"stdlib.h"			//malloc动态申请内存空间




////结点结构体定义
//typedef struct ListNode
//{	
//	u16 Lenth;
////	const char*	Buffer=NULL;
//	ListNode*	pNext;
//}ListNode;

//链表的操作，以有头节点为例，无头节点类似  
//ListNode* head = NULL;
/************************************************************************************** 
*函数名称	：Create 
*函数功能	：创建链表. 
*输入			：各节点的data 
*返回值		：表头节点指针head 
*************************************************************************************/
//创建链表，头结点data=0，pNext=NULL;
//ListNode *Create()
//{
//	ListNode *NewNode	=	NULL;		//p1保存创建的新节点的地址
//	ListNode *next 		= NULL;		//p2保存原链表最后一个节点的地址

////	n = 0;						//创建前链表的节点总数为0：空链表
//	head = (ListNode*) malloc(sizeof(ListNode));
//	if(head==NULL)			//节点开辟不成功
//	{
//		//------------节点创建不成功处理
//		return NULL;
//	}
//	else							//节点开辟成功
//	{
//		//------------节点创建成功处理
////		next = NewNode;					//如果节点开辟成功，则p2先把它的指针保存下来以备后用
////		next->next = NewNode;		//使第一个节点的next成员指向第二个节点，即将第二个节点与第一个节点连接
////		NewNode = NULL;					//特别不要忘记把释放的变量清空置为NULL,否则就变成"野指针"，即地址不确定的指针
//		head->Lenth=0;
//		head->Buffer=NULL;
//		head->pNext=NULL;	
//		return head;	    	//返回创建链表的头指针
//	}
//	
//	return head;	    	//返回创建链表的头指针 

//}














//#include <stdio.h>
//#include <malloc.h>
//#define LEN sizeof(struct student)

///*----------------数据定义----------------------*/ 

////定义一个学生信息的结构体,包括学号,姓名和结构体类型的指针 
//typedef struct student
//{
//    long num;                //学号 
//    char name[128];            //姓名 
//    struct student *next;    //结构体指针 
//};


//student * stuNode;

//int n=0;                    //全局变量,记录链表的长度 
// 
///*---------------函数声明---------------------*/
// 
//stuNode Create();            //创建一个新的链表                     

//void Print(stuNode head);    //通过传入的链表头指针打印整个链表 

//stuNode Delete(stuNode head,int num);    //通过传入的链表头指针和学生学号删除节点 

//stuNode Insert(stuNode head,stuNode newStu);    //依照学生学号的顺序向链表中插入新元素 


///*---------------函数定义----------------------*/
///************************************************************************************** 
//*函数名称	：Create 
//*函数功能	：创建链表. 
//*输入			：各节点的data 
//*返回值		：表头节点指针head 
//*************************************************************************************/
//struct student *Create()
//{
//    struct student *head,*p1,*p2;
//    
//    //开辟一个LEN大小的空间,并让p1,p2指针指向它 
//    p2=p1=(struct student *)malloc(LEN);
//    //将头指针置为NULL 
//    head=NULL;
//    
//    //创建链表节点并给节点的元素赋值 
//    printf("请输入学生的学号和姓名:");
//    scanf("%ld %s",&p1->num,p1->name);
//    while(p1->num!=0)
//    {
//        n=n+1;
//        if(NULL==head)
//        {
//            head=p1;
//        }
//        else
//        {
//            p2->next=p1;
//        }
//        p2=p1;
//        p1=(struct student *)malloc(LEN);
//        printf("请输入学生的学号和姓名:");
//        scanf("%ld %s",&p1->num,p1->name);
//    }
//    //将尾节点的指针置为NULL 
//    p2->next=NULL;
//    return head;
//}


//void Print(struct student *head)
//{
//    struct student * p;
//    p=head;
//    
//    //判断链表是否为空 
//    if(NULL==head)
//    {
//        printf("链表为空!\n");
//        return head;
//    }
//    else
//    {
//        //循环打印链表中的元素 
//        printf("%d 个记录分别为:\n",n);
//        while(p!=NULL)
//        {
//            printf("%ld %s\n",p->num,p->name);
//            //指针指向下一个节点 
//            p=p->next;
//        }
//    }
//}

///************************************************************************************** 
//*函数名称：del 
//*函数功能：删除链表中的元素 
//*输入：head 链表头指针，p 被删除元素位置 
//*返回值：被删除元素中的数据域.如果删除失败返回-1 
//**************************************************************************************/
//struct student *Delete(struct student * head,int num)
//{
//    struct student *p1;
//    struct student *p2;
//    p1=head;
//    //判断链表是否为空 
//    if(NULL==head)
//    {
//        printf("链表为空!\n");
//        return head;
//    }
//    //遍历节点,判断当前节点是不是需要删除的节点及是否为尾节点
//    //如果找到相应节点,或者已经遍历到尾节点就跳出循环 
//    while(p1->num!=num&&p1->next!=NULL)
//    {
//        p2=p1;
//        p1=p1->next;
//    }
//    //判断是否找到相应节点 
//    if(p1->num==num)
//    {
//        //要删除的节点是不是链表的第一个节点
//        //如果是,就将头指针指向该节点的后一个节点
//        //如果不是,就将该节点的前一个节点的指针指向该节点的后一个节点 
//        if(head==p1)
//        {
//            head=p1->next;
//        }
//        else
//        {
//            p2->next=p1->next;
//        }
//        n=n-1;
//        printf("%ld 节点已删除.\n",num);
//    }
//    else
//    {
//        printf("链表中没有要删除的元素.\n");
//    }
//    return head;
//}

///************************************************************************************** 
//*函数名称：insert 
//*函数功能：在链表中插入元素. 
//*输入：head 链表头指针，p新元素插入位置，x 新元素中的数据域内容 
//*返回值：无 
//*************************************************************************************/
//struct student *Insert(struct student * head,struct student * newStu)
//{
//    struct student *p0;
//    struct student *p1;
//    struct student *p2;
//    p0=newStu;
//    p1=head;
//    //判断链表是否为空,如果是空链表,就将新节点作为第一个节点 
//    if(NULL==head)
//    {
//        head=p0;
//        p0->next=NULL;
//    }
//    else
//    {
//        //遍历每一个节点中的学号,与新学号比较大小
//        //如果找到一个学号比新学号大,就将新学号的节点插入它之前 
//        //如果尾节点的学号仍比新学号小,就将新节点插入到链表尾部 
//        while((p0->num > p1->num)&&(p1->next!=NULL))
//        {
//            p2=p1;
//            p1=p1->next;
//        }
//        //找到一个比新学号大的节点 
//        if(p0->num <= p1->num)
//        {
//            //判断该节点是否为头节点,如果是,则将新节点设置为头节点 
//            if(p1==head)
//            {
//                head=p0;
//            }
//            else
//            {
//                p2->next=p0;
//            }
//              p0->next=p1;
//        }
//        else
//        {
//            p1->next=p0;
//            p0->next=NULL;
//        }
//    }
//    //链表长度加1 
//    n=n+1;
//    printf("%ld 插入成功!\n",newStu->num);
//    return head;
//}

//void main()
//{
//    struct student *head;
//    struct student *stu;
//    int num;
//    head=Create();
//    Print(head);
//    printf("请输入要删除的学号:");
//    scanf("%ld",&num);
//    while(num!=0)
//    {
//        head=Delete(head,num);
//        Print(head);
//        printf("请输入要删除的学号:");
//        scanf("%ld",&num);
//    }
//    printf("请输入要插入的节点:");
//    stu=(struct student *)malloc(LEN);
//    scanf("%ld %s",&stu->num,stu->name);
//    while(stu->num!=0)
//    {
//        head=Insert(head,stu);
//        printf("请输入要插入的节点:");
//        stu=(struct student *)malloc(LEN);
//        scanf("%ld %s",&stu->num,stu->name);
//    }
//    Print(head);
//}




#endif



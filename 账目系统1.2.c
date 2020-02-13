/************************各种头和声明************************/
#include<stdio.h>
#include<stdlib.h> 
#include<windows.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include<conio.h>
#include<time.h>

#define Length 50
#define MAX 100
#define DATA_FILE_NAME "datainfo.txt"
#define income 0;
#define outcome 1;

typedef struct {//数据结构 
	int id;//账目id 
	char date[11];//日期 
	int source;//收入支出 
	float amount;//账目 
	char reason[Length + 1];//来源 
} Info;

typedef struct node {//链表结构 
	Info data;
	struct node* next;
} List;

//全局变量，统计余额
float total = 0;
List* last;
//全局变量，存储当前数据的数量
int CurrentCnt = 0;
/************************以下是函数声明************************/
//初始化数据库。
void initialized_database(List **L);
//主菜单中的函数。
void menu_income();
void menu_outcome();
void query_all();
void sort_by_time(List* L);     //这个函数用于在结束程序时更新数据库。
//收入管理中的函数。
bool income_add(List* L, Info dataInfo);
void income_inquire(List* L);
void income_delete(List* L);
void income_modificate(List* L);
int income_print_txt(List* L);
//支出管理中的函数。
bool outcome_add(List* L, Info dataInfo);
void outcome_inquire(List* L);
void outcome_delete(List*L);
void outcome_modificate(List* L);
int outcome_print_txt(List* L);
//整体查询中的函数。
void print_all_of_datas(List *L);
void search(List* L);
float surplus(List* L);
void qs(void);
void line(Info sortlist[], const int low, int high);
int quilt(Info sortlist[], int low, int high);
void sort_2();
void sort_3(List* L);
int save_to_file(List* L);
//通用函数
int read_line(char str[], int n);//行输入函数
Info read_item_info();//读取一个data
void output_one_item(List* p);//输出一个data
void destory_list_and_file(List** L);
void destory_list(List** L);

/************************以下是主函数************************/
//主函数，兼具菜单功能。
int main(void)
{
	int menu_1 = 1;
	
	initialized_database(&last);
	printf("\n");
	system("pause");
	system("cls");
	Info item;
	for (menu_1 = 1; menu_1 != 0; ) {
		system("cls");
		printf(	"*************这个程序是个小管家，目的是管理账务*************\n"
				"*                 输入1：收入管理                          *\n"
				"*                 输入2：支出管理                          *\n"
				"*                 输入3：查询账目                          *\n"
				"*                 输入4：输出流水账到txt                   *\n"
				"*                 输入5：warning！！全盘删除               *\n"
				"*                                                          *\n"
				"*                 输入9：按时间顺序输出流水账到TXT         *\n"
				"*                 输入0：退出程序                          *\n"
			    "************************************************************\n"
			    "\n\n输入一个数字：");
		scanf("%d", &menu_1);
		printf("\n");
		switch (menu_1)
		{
		case 1:
			system("cls");
			menu_income();
			break;
		case 2:
			system("cls");
			menu_outcome();
			break;
		case 3:
			system("cls");
			query_all();
			break;

		case 4:
			sort_by_time(last);
			printf("以下这些数据将被写入TXT文件\n");
			print_all_of_datas(last);
			save_to_file(last);
			system("pause");
			system("cls");
			break;
		case 5:
			destory_list_and_file(&last);
			exit(0);
		case 9:
			sort_by_time(last);
			printf("以下这些数据将被写入TXT文件\n");
			print_all_of_datas(last);
			save_to_file(last);
			exit(0);
		case 0:
			system("cls");
			break;
		default:
			printf("你输入的不是任何命令，请重新输入······");
			system("pause");
			system("cls");
			break;
		}
	}
	return 0;
}
/************************以下是函数************************/
//初始化数据库
void initialized_database(List **L)
{
	FILE* fp;
	Info oneInfo;
	List* p, * r;

	(*L) = (List*)malloc(sizeof(List));
	r = (*L);
	if ((fp = fopen(DATA_FILE_NAME, "r")) == NULL)
	{
		if ((fp = fopen(DATA_FILE_NAME, "w")) == NULL)
			printf("提示：不能创建商品文件\n");
	}
	else {
		while (!feof(fp))
		{
			
			fscanf(fp, " %d", &oneInfo.id);
			fscanf(fp, "\t%s", oneInfo.date);
			fscanf(fp, "\t%d", &oneInfo.source);
			fscanf(fp, "\t%f", &oneInfo.amount);
			fscanf(fp, "\t%s", oneInfo.reason);
			p = (List*)malloc(sizeof(List));
			p->data = oneInfo;
			r->next = p;
			r = p;
			CurrentCnt++;
		}
	}
	r->next = NULL;
	printf("正在初始化······\n");
	Sleep(1000);
	printf("链表文件已建立，有%d个信息记录\n", CurrentCnt);
	Sleep(1000);
	printf("启动成功！！");
	CurrentCnt++;
}

//收入处理菜单
void menu_income()
{
	int menu_2 = 1;

	for (menu_2 = 1; menu_2 != 0; ) {
		printf("**************在这里你可以管理收入**************\n"
		  	   "*            输入1：添加收入记录               *\n"
			   "*            输入2：查询收入记录               *\n"
			   "*            输入3：删除收入记录               *\n"
			   "*            输入4：修改收入记录               *\n"
			   "*            输入5：按照时间打印收入记录至TXT  *\n"
			   "*                                              *\n"
			   "*            输入0：返回                       *\n"
			   "************************************************\n"
			   "\n\n输入一个数字：");
		scanf("%d", &menu_2);
		printf("\n");
		switch (menu_2)
		{
		case 1:
			system("cls");
			printf("在这里你可以添加一个收入项目:\n");
			income_add(last, read_item_info());
			system("pause");
			system("cls");
			break;
		case 2:
			system("cls");
			printf("在这里你可以查找一个收入项目:\n");
			income_inquire(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 3:
			system("cls");
			printf("在这里你可以按照ID删除一个收入项目:\n");
			income_delete(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 4:
			system("cls");
			printf("在这里你可以按照ID修改一个收入项目:\n");
			income_modificate(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 5:
			system("cls");
			sort_by_time(last);
			printf("************************\n*已将%d个账目打印至TXT！*\n************************\n", income_print_txt(last));
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 0:
			system("cls");
			break;
		default:
			printf("你输入的不是0~5，请重新输入······");
			printf("\n");
			system("pause");
			system("cls");
			break;
		}
	}
}
//支出处理菜单
void menu_outcome()
{
	int menu_2 = 1;


	for (menu_2 = 1; menu_2 != 0; ) {
		printf("**************在这里你可以管理支出**************\n"
		  	   "*            输入1：添加支出记录               *\n"
			   "*            输入2：查询支出记录               *\n"
			   "*            输入3：删除支出记录               *\n"
			   "*            输入4：修改支出记录               *\n"
			   "*            输入5：按照时间打印支出记录至TXT  *\n"
			   "*                                              *\n"
			   "*            输入0：返回                       *\n"
			   "************************************************\n"
			"\n\n输入一个数字：");
		scanf("%d", &menu_2);
		printf("\n");
		switch (menu_2)
		{
		case 1:
			system("cls");
			printf("在这里你可以输入一个支出项目:\n");
			outcome_add(last, read_item_info());
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 2:
			system("cls");
			printf("在这里你可以查找一个支出项目:\n");
			outcome_inquire(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 3:
			system("cls");
			printf("在这里你可以按照ID删除一个支出项目:\n");
			outcome_delete(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 4:
			system("cls");
			printf("在这里你可以按照ID修改一个支出项目:\n");
			outcome_modificate(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 5:
			system("cls");
			sort_by_time(last);
			printf("************************\n*已将%d个账目打印至TXT！*\n************************\n", outcome_print_txt(last));
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 0:
			system("cls");
			break;
		default:
			printf("你输入的不是0~5，请重新输入······");
			printf("\n");
			system("pause");
			system("cls");
			break;
		}
	}
}

//整体查询菜单
void query_all()
{
	int menu_3 = 1;


	for (menu_3 = 1; menu_3 != 0; ) {
		printf("/*************在这里你可以对所有数据进行查询*************/\n"
			"输入1：流水帐查看\n输入2：查找特定账目\n输入3：按照账目大小排序（快速排序算法）\n输入4：按照账目大小排序（选择排序）\n输入5：按照账目大小排序（冒泡排序算法）\n输入0：返回\n\n输入一个数字：");
		scanf("%d", &menu_3);
		printf("\n");
		switch (menu_3)
		{
		case 1:
			system("cls");
			print_all_of_datas(last);
			printf("\n你的余额是：%8.2f", surplus(last));
			if (surplus(last) <= 0) {
				printf("\n*****你个穷鬼，搞什么灰机！！！*****");
			}
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 2:
			system("cls");
			search(last);
			printf("\n");
			system("cls");
			break;
		case 3:
			system("cls");
			qs();
			print_all_of_datas(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 4:
			system("cls");
			sort_2(last);
			print_all_of_datas(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 5:
			system("cls");
			sort_3(last);
			print_all_of_datas(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 0:
			system("cls");
			break;
		default:
			printf("你输入的不是0~5，请重新输入······");
			printf("\n");
			system("pause");
			system("cls");
			break;
		}
	}
}

//按照日期排序并且给予新的ID
void sort_by_time(List* L)
{
	List* p ;
	Info temp;
	int n = CurrentCnt;
	int i, j, k;
	//冒泡排序法
	if (L == NULL || L->next == NULL)
		printf("当前链表中没有商品\n");
	for (j = 0; j < n-1; j++) {
		p = L->next;
		for (i = 0; i < n - j - 2; i++) {
			if (strcmp((p->data.date),(p->next->data.date))>0) {
				temp = p->data;
				p->data = p->next->data;
				p->next->data = temp;
			}
			p = p->next;
		}
	}
	//修改ID
	for (p = L->next,k=1; p != NULL; p = p->next,k++) 
	{
		p->data.id = k;
	}
}

//收入管理中的函数。
bool income_add(List* L, Info dataInfo)
{
	dataInfo.source = 0;
	List* temp;
	List* pre = L, * p = L->next;

	if (CurrentCnt >= 100) {
		printf("信息库已满，要插入请先删除一定量的商品数据!\n");
		return false;
	}
	//尾部插入
	while (p != NULL)
	{
		pre = p;
		p = p->next;
	}
	temp = (List*)malloc(sizeof(List));
	temp->data = dataInfo;
	pre->next = temp;
	temp->next = NULL;
	printf("Tips:添加ID为%d的账目成功\n", dataInfo.id);
	CurrentCnt++;
	return true;
}
void income_inquire(List * L)
{
	int choice = 0, id = 0;
	char date[11];
	bool flag = false;
	List* p;

	p = L;
	printf("*收入查询界面：\n输入1：按照ID搜索\n输入2：按照日期搜索\n\n输入0：返回\n\n*请输入你的选择：");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("输入ID：");
		scanf("%d", &id);
		for (p = L; p != NULL; p = p->next) {
			if ((p->data.source) == 1)
				continue;
			if (p->data.id == id) {
				printf("找到了\n");
				flag = true;
				output_one_item(p);
				printf("\n");
				continue;
			}
		}
		if (flag == false)
			printf("莫得你要找的\n");
		else
			printf("全都打印了！\n");
		break;
	case 2:
		printf("输入日期(xxxx/xx/xx)：");
		scanf("%s", date);
		for (p = L; p != NULL; p = p->next) {
			if ((p->data.source) == 0)
				continue;
			if (!strcmp(p->data.date, date)) {
				flag = true;
				output_one_item(p);
				continue;
			}
		}
		if (flag == false)
			printf("莫得你要找的\n");
		else
			printf("找到了而且我全都打印了！>_<\n");
		break;
	case 0:
		system("cls");
		break;
	default:
		printf("你输入的数字不是1或2，即将返回菜单");
		printf("\n");
		break;
	}
}
void income_delete(List* L)
{
	List* prev = NULL, * cur = L;
	int id;
	bool flag = false;

	printf("输入ID：");
	scanf("%d", &id);
	for (cur = L; cur != NULL; cur = cur->next) {
		if (cur->data.id == id) {
			printf("找到了!以下将被删除\n");
			output_one_item(cur);
			flag = true;
			break;
		}
		prev = cur;
	}
	if (flag == false)
		printf("莫得你要找的\n");
	else {
		prev->next = cur->next;
		free(cur);
		printf("删除成功！\n");
		CurrentCnt--;
	}
}
void income_modificate(List* L)
{
	printf("OK\n");
	List* p = L;
	int id, choice;
	float amount;
	bool flag = false;
	char date[11], reason[Length];

	printf("输入ID：");
	scanf("%d", &id);
	for (p = L; p != NULL; p = p->next) {
		if (p->data.id == id) {
			flag = true;
			break;
		}
	}
	if (flag == false)
		printf("莫得你要找的\n");
	else {
		printf("找到了,请选择你要修改的内容：\n1.日期\n2.账目\n3.原因\n你的选择是：");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("输入新的日期：");
			read_line(date, 11);
			strcpy(p->data.date, date);
			break;
		case 2:
			printf("输入新的支出额度：");
			scanf("%f", &amount);
			p->data.amount = amount;
			break;
		case 3:
			printf("输入新的原因：");
			read_line(reason, Length);
			strcpy(p->data.reason, reason);
			break;
		default:
			printf("你输入的数字不在1~3之间！\n");
			break;
		}
	}
	if (flag == true)
		printf("修改成功！");
	else
		printf("修改失败！");
}
int income_print_txt(List* L)
{
	if (L == NULL)
		return 0;
	List* p = L->next;
	FILE* fp;
	if ((fp = fopen("intcome.txt", "w")) == NULL)
	{
		printf("提示：不能打开商品文件\n");
		return 0;
	}
	int save_count = 0, id = 1;
	fprintf(fp, "这里是打印收入情况（按照日期排序）:\n");
	fprintf(fp, "序号\t日期\t\t额度\t原因\n");
	while (p != NULL)
	{
		if (p->data.source == 1) {
			p = p->next;
			continue;
		}

		fprintf(fp, "\n%d\t", id);
		fprintf(fp, "%s\t", p->data.date);
		fprintf(fp, "%.2f\t", p->data.amount);
		fprintf(fp, "%s", p->data.reason);
		p = p->next;
		save_count++;
		id++;
	}
	fclose(fp);
	return save_count;
}

//支出管理中的函数。
bool outcome_add(List *L,Info dataInfo)
{
	dataInfo.source = 1;
	List* temp;
	List* pre = L, * p = L->next;

	if (CurrentCnt >= 100) {
		printf("信息库已满，要插入请先删除一定量的商品数据!\n");
		return false;
	}
	//尾部插入
	while (p != NULL)
	{
		pre = p;
		p = p->next;
	}
	temp = (List*)malloc(sizeof(List));
	temp->data = dataInfo;
	pre->next = temp;
	temp->next = NULL;
	printf("Tips:添加ID为%d的账目成功\n",dataInfo.id);
	CurrentCnt++;
	return true;
}
void outcome_inquire(List *L)
{
	int choice = 0,id = 0;
	char date[11];
	bool flag = false;
	List *p;

	p = L;
	printf("*支出查询界面：\n输入1：按照ID搜索\n输入2：按照日期搜索\n\n输入0：返回\n\n*请输入你的选择：");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("输入ID：");
		scanf("%d", &id);
		for (p = L; p != NULL; p = p->next){
			if ((p->data.source) == 0)
				continue;
			if (p->data.id == id) {
				printf("找到了\n");
				flag = true;
				output_one_item(p);
				printf("\n");
				continue;
			}
		}
		if (flag == false)
			printf("莫得你要找的\n");
		else 
			printf("全都打印了！\n");
		break;
	case 2:
		printf("输入日期(xxxx/xx/xx)：");
		scanf("%s", date);
		for (p = L; p != NULL; p = p->next) {
			if ((p->data.source) == 0)
				continue;
			if (!strcmp(p->data.date,date)) {
				flag = true;
				output_one_item(p);
				continue;
			}
		}
		if (flag == false)
			printf("莫得你要找的\n");
		else
			printf("找到了而且我全都打印了！>_<\n");
		break;
	case 0:
		system("cls");
		break;
	default:
		printf("你输入的数字不是1或2，即将返回菜单");
		printf("\n");
		break;
	}
}

void outcome_delete(List *L)
{
	List* prev = NULL, * cur = L;
	int id;
	bool flag = false;

	printf("输入ID：");
	scanf("%d", &id);
	for (cur = L; cur != NULL; cur = cur->next) {
		if (cur->data.id == id) {
			printf("找到了!以下将被删除\n");
			output_one_item(cur);
			flag = true;
			break;
		}
		prev = cur;
	}
	if (flag == false)
		printf("莫得你要找的\n");
	else {
		prev->next = cur->next;
		free(cur);
		printf("删除成功！\n");
		CurrentCnt--;
	}

}

void outcome_modificate(List *L)
{
	List* p=L;
	int id, choice;
	float amount;
	bool flag = false;
	char date[11], reason[Length];

	printf("输入ID：");
	scanf("%d", &id);
	for (p = L; p != NULL; p = p->next) {
		if (p->data.id == id) {
			flag = true;
			break;
		}
	}
	if (flag == false)
		printf("莫得你要找的\n");
	else {
		printf("找到了,请选择你要修改的内容：\n1.日期\n2.账目\n3.原因\n你的选择是：");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("输入新的日期：");
			read_line(date,11);
			strcpy(p->data.date,date);
			break;
		case 2:
			printf("输入新的支出额度：");
			scanf("%f", &amount);
			p->data.amount = amount;
			break;
		case 3:
			printf("输入新的原因：");
			read_line(reason, Length);
			strcpy(p->data.reason, reason);
			break;
		default:
			printf("你输入的数字不在1~3之间！\n");
			break;
		}
	}
	if (flag == true)
		printf("修改成功！");
	else
		printf("修改失败！");
}

int outcome_print_txt(List *L)
{
	if (L == NULL)
		return 0;
	List* p = L->next;
	FILE* fp;
	if ((fp = fopen("outcome.txt", "w")) == NULL)
	{
		printf("提示：不能打开商品文件\n");
		return 0;
	}
	int save_count = 0, id = 1;
	fprintf(fp, "这里是打印支出情况（按照日期排序）:\n");
	fprintf(fp, "序号\t日期\t\t额度\t原因\n");
	while (p != NULL)
	{
		if (p->data.source == 0) {
			p = p->next;
			continue;
		}
		
		fprintf(fp, "\n%d\t", id);
		fprintf(fp, "%s\t", p->data.date);
		fprintf(fp, "%.2f\t", p->data.amount);
		fprintf(fp, "%s", p->data.reason);
		p = p->next;
		save_count++;
		id++;
	}
	fclose(fp);
	return save_count;
}

//整体查询中的函数。
void print_all_of_datas(List *L)
{
	List* p = L->next;
	printf("当前有%d个项目\n", CurrentCnt);
	while (p != NULL) {
		output_one_item(p);
		p = p->next;
	}
}

void search(List* L)
{
	int choice = 0, id = 0;
	char date[11];
	bool flag = false;
	List* p;

	p = L;
	printf("*在这里你可以通过ID找到某一项或按照日期找到多项\n输入1：按照ID搜索\n输入2：按照日期搜索\n\n输入0：返回\n\n*请输入你的选择：");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("输入ID：");
		scanf("%d", &id);
		for (p = L; p != NULL; p = p->next) {
			if (p->data.id == id) {
				printf("找到了\n");
				flag = true;
				output_one_item(p);
				printf("\n");
				continue;
			}
		}
		if (flag == false)
			printf("莫得你要找的\n");
		else
			printf("已经打印了！\n");
		system("pause");
		break;
	case 2:
		printf("输入日期(xxxx/xx/xx)：");
		scanf("%s", date);
		for (p = L; p != NULL; p = p->next) {
			if (!strcmp(p->data.date, date)) {
				flag = true;
				output_one_item(p);
				continue;
			}
		}
		if (flag == false)
			printf("莫得你要找的\n");
		else
			printf("找到了而且我全都打印了！>_<\n");
		system("pause");
		break;
	case 0:
		system("cls");
		break;
	default:
		printf("你输入的数字不是1或2，即将返回菜单");
		printf("\n");
		system("pause");
		break;
	}
	system("cls");
}

void qs(void)                                 //by 陈俊杨
{
	Info sortlist[ MAX ]; int i; List* r;
	r = last->next;
	for (i = 0; i < CurrentCnt - 1; i++) {
		sortlist[i] = r->data;
		r = r->next;
	}
	int low = 0, high = CurrentCnt - 2;
	line(sortlist, low, high);
	r = last->next;
	for (i = 0; i < CurrentCnt - 1; i++) {
		r->data = sortlist[i];
		r = r->next;
	}
	printf("快速排序成功（这是从小到大的）\n");

}


void line(Info sortlist[], const int low, const int high)
{
	int middle;
	if (low >= high) return;
	middle = quilt(sortlist, low, high);
	line(sortlist, low, middle - 1);
	line(sortlist, middle + 1, high);
}

int quilt(Info sortlist[], int low, int high)
{
	Info part_element = sortlist[low];
	for (;;) {
		while (low < high && part_element.amount <= sortlist[high].amount)
			high--;
		if (low >= high) break;
		sortlist[low++] = sortlist[high];
		while (low < high && sortlist[low].amount <= part_element.amount)
			low++;
		if (low >= high) break;
		sortlist[high--] = sortlist[low];
	}
	sortlist[high] = part_element;
	return high;
}

void sort_2(List* head)
{

	List* p1, * p2, * p3;
	Info temp;
	p1 = head;

	for (p2 = p1->next; p2 != NULL; p2 = p2->next)
	{
		for (p3 = p2->next; p3 != NULL; p3 = p3->next)
		{

			if (p2->data.amount < p3->data.amount)
			{
				temp = p3->data;
				p3->data = p2->data;
				p2->data = temp;
			}
		}
	}

}
void sort_3(List *L)
{
	List* p;
	Info temp;
	int n = CurrentCnt;
	int i, j;
	//冒泡排序法
	if (L == NULL || L->next == NULL)
		printf("当前链表中没有商品\n");
	for (j = 0; j < n - 1; j++) {
		p = L->next;
		for (i = 0; i < n - j - 2; i++) {
			if ((p->data.amount) < (p->next->data.amount)) {
				temp = p->data;
				p->data = p->next->data;
				p->next->data = temp;
			}
			p = p->next;
		}
	}
}

//通用函数
/*读取一行*/
int read_line(char str[], int n)
{
	int ch, i = 0;

	while (isspace(ch = getchar()))
		;
	while (ch != '\n' && ch != EOF) {
		if (i < n)
			str[i++] = ch;
		ch = getchar();
	}
	str[i] = '\0';
	return i;
}

/*读取结构体*/
Info read_item_info() {
	Info item_info;

	item_info.id = CurrentCnt;
	printf("输入日期（xxxx/xx/xx)：");
	read_line(item_info.date, 11);
	item_info.source = -1;
	printf("账目：");
	scanf("%f", &item_info.amount);
	printf("解释：");
	read_line(item_info.reason, Length);
	
		return item_info;
}

/*打印单个节点信息*/
void output_one_item(List* p) {
	if (p == NULL)
		return;
	printf("ID:%d\t|日期:%s||",p->data.id, p->data.date);
	if ((p->data.source) == 1)
		printf("支出：%8.2f", -p->data.amount);
	else
		printf("收入：%8.2f", +p->data.amount);
	printf("|原因:%s\n",p->data.reason);
}
/*输出流水账到txt*/
int save_to_file(List* L) {
	if (L == NULL)
		return 0;
	List* p = L->next;
	FILE* fp;
	if ((fp = fopen("datainfo.txt", "w")) == NULL)
	{
		printf("提示：不能打开商品文件\n");
		return 0;
	}
	int save_count = 0;
	while (p != NULL)
	{
		fprintf(fp, "\n%d\t", p->data.id);
		fprintf(fp, "%s\t", p->data.date);
		fprintf(fp, "%d\t", p->data.source);
		fprintf(fp, "%.2f\t", p->data.amount);
		fprintf(fp, "%s", p->data.reason);
		p = p->next;
		save_count++;
	}
	fclose(fp);
	return save_count;
}
/*删除链表及文件*///暂时还有问题,它文件没删掉？？
void destory_list_and_file(List** L) {
	destory_list(L);
	remove(DATA_FILE_NAME);
}
void destory_list(List** L) {
	if (L == NULL || *L == NULL)
		return;
	List* pre = *L, * p = (*L)->next;
	while (p != NULL)
	{
		free(pre);
		pre = p;
		p = p->next;
	}
	free(pre);
	CurrentCnt = 0;
	*L = NULL;
}

/*统计余额*/
float surplus(List* L)
{
	List* p = L->next;
	float surplus = 0;

	for (p ; p != NULL; p = p->next) {
		if ((p->data.source) == 1)
			surplus = surplus - (p->data.amount);
		else
			surplus = surplus + (p->data.amount);
	}
	return surplus;
}

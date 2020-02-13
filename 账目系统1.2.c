/************************����ͷ������************************/
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

typedef struct {//���ݽṹ 
	int id;//��Ŀid 
	char date[11];//���� 
	int source;//����֧�� 
	float amount;//��Ŀ 
	char reason[Length + 1];//��Դ 
} Info;

typedef struct node {//����ṹ 
	Info data;
	struct node* next;
} List;

//ȫ�ֱ�����ͳ�����
float total = 0;
List* last;
//ȫ�ֱ������洢��ǰ���ݵ�����
int CurrentCnt = 0;
/************************�����Ǻ�������************************/
//��ʼ�����ݿ⡣
void initialized_database(List **L);
//���˵��еĺ�����
void menu_income();
void menu_outcome();
void query_all();
void sort_by_time(List* L);     //������������ڽ�������ʱ�������ݿ⡣
//��������еĺ�����
bool income_add(List* L, Info dataInfo);
void income_inquire(List* L);
void income_delete(List* L);
void income_modificate(List* L);
int income_print_txt(List* L);
//֧�������еĺ�����
bool outcome_add(List* L, Info dataInfo);
void outcome_inquire(List* L);
void outcome_delete(List*L);
void outcome_modificate(List* L);
int outcome_print_txt(List* L);
//�����ѯ�еĺ�����
void print_all_of_datas(List *L);
void search(List* L);
float surplus(List* L);
void qs(void);
void line(Info sortlist[], const int low, int high);
int quilt(Info sortlist[], int low, int high);
void sort_2();
void sort_3(List* L);
int save_to_file(List* L);
//ͨ�ú���
int read_line(char str[], int n);//�����뺯��
Info read_item_info();//��ȡһ��data
void output_one_item(List* p);//���һ��data
void destory_list_and_file(List** L);
void destory_list(List** L);

/************************������������************************/
//����������߲˵����ܡ�
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
		printf(	"*************��������Ǹ�С�ܼң�Ŀ���ǹ�������*************\n"
				"*                 ����1���������                          *\n"
				"*                 ����2��֧������                          *\n"
				"*                 ����3����ѯ��Ŀ                          *\n"
				"*                 ����4�������ˮ�˵�txt                   *\n"
				"*                 ����5��warning����ȫ��ɾ��               *\n"
				"*                                                          *\n"
				"*                 ����9����ʱ��˳�������ˮ�˵�TXT         *\n"
				"*                 ����0���˳�����                          *\n"
			    "************************************************************\n"
			    "\n\n����һ�����֣�");
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
			printf("������Щ���ݽ���д��TXT�ļ�\n");
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
			printf("������Щ���ݽ���д��TXT�ļ�\n");
			print_all_of_datas(last);
			save_to_file(last);
			exit(0);
		case 0:
			system("cls");
			break;
		default:
			printf("������Ĳ����κ�������������롤����������");
			system("pause");
			system("cls");
			break;
		}
	}
	return 0;
}
/************************�����Ǻ���************************/
//��ʼ�����ݿ�
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
			printf("��ʾ�����ܴ�����Ʒ�ļ�\n");
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
	printf("���ڳ�ʼ��������������\n");
	Sleep(1000);
	printf("�����ļ��ѽ�������%d����Ϣ��¼\n", CurrentCnt);
	Sleep(1000);
	printf("�����ɹ�����");
	CurrentCnt++;
}

//���봦��˵�
void menu_income()
{
	int menu_2 = 1;

	for (menu_2 = 1; menu_2 != 0; ) {
		printf("**************����������Թ�������**************\n"
		  	   "*            ����1����������¼               *\n"
			   "*            ����2����ѯ�����¼               *\n"
			   "*            ����3��ɾ�������¼               *\n"
			   "*            ����4���޸������¼               *\n"
			   "*            ����5������ʱ���ӡ�����¼��TXT  *\n"
			   "*                                              *\n"
			   "*            ����0������                       *\n"
			   "************************************************\n"
			   "\n\n����һ�����֣�");
		scanf("%d", &menu_2);
		printf("\n");
		switch (menu_2)
		{
		case 1:
			system("cls");
			printf("��������������һ��������Ŀ:\n");
			income_add(last, read_item_info());
			system("pause");
			system("cls");
			break;
		case 2:
			system("cls");
			printf("����������Բ���һ��������Ŀ:\n");
			income_inquire(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 3:
			system("cls");
			printf("����������԰���IDɾ��һ��������Ŀ:\n");
			income_delete(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 4:
			system("cls");
			printf("����������԰���ID�޸�һ��������Ŀ:\n");
			income_modificate(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 5:
			system("cls");
			sort_by_time(last);
			printf("************************\n*�ѽ�%d����Ŀ��ӡ��TXT��*\n************************\n", income_print_txt(last));
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 0:
			system("cls");
			break;
		default:
			printf("������Ĳ���0~5�����������롤����������");
			printf("\n");
			system("pause");
			system("cls");
			break;
		}
	}
}
//֧������˵�
void menu_outcome()
{
	int menu_2 = 1;


	for (menu_2 = 1; menu_2 != 0; ) {
		printf("**************����������Թ���֧��**************\n"
		  	   "*            ����1�����֧����¼               *\n"
			   "*            ����2����ѯ֧����¼               *\n"
			   "*            ����3��ɾ��֧����¼               *\n"
			   "*            ����4���޸�֧����¼               *\n"
			   "*            ����5������ʱ���ӡ֧����¼��TXT  *\n"
			   "*                                              *\n"
			   "*            ����0������                       *\n"
			   "************************************************\n"
			"\n\n����һ�����֣�");
		scanf("%d", &menu_2);
		printf("\n");
		switch (menu_2)
		{
		case 1:
			system("cls");
			printf("���������������һ��֧����Ŀ:\n");
			outcome_add(last, read_item_info());
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 2:
			system("cls");
			printf("����������Բ���һ��֧����Ŀ:\n");
			outcome_inquire(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 3:
			system("cls");
			printf("����������԰���IDɾ��һ��֧����Ŀ:\n");
			outcome_delete(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 4:
			system("cls");
			printf("����������԰���ID�޸�һ��֧����Ŀ:\n");
			outcome_modificate(last);
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 5:
			system("cls");
			sort_by_time(last);
			printf("************************\n*�ѽ�%d����Ŀ��ӡ��TXT��*\n************************\n", outcome_print_txt(last));
			printf("\n");
			system("pause");
			system("cls");
			break;
		case 0:
			system("cls");
			break;
		default:
			printf("������Ĳ���0~5�����������롤����������");
			printf("\n");
			system("pause");
			system("cls");
			break;
		}
	}
}

//�����ѯ�˵�
void query_all()
{
	int menu_3 = 1;


	for (menu_3 = 1; menu_3 != 0; ) {
		printf("/*************����������Զ��������ݽ��в�ѯ*************/\n"
			"����1����ˮ�ʲ鿴\n����2�������ض���Ŀ\n����3��������Ŀ��С���򣨿��������㷨��\n����4��������Ŀ��С����ѡ������\n����5��������Ŀ��С����ð�������㷨��\n����0������\n\n����һ�����֣�");
		scanf("%d", &menu_3);
		printf("\n");
		switch (menu_3)
		{
		case 1:
			system("cls");
			print_all_of_datas(last);
			printf("\n�������ǣ�%8.2f", surplus(last));
			if (surplus(last) <= 0) {
				printf("\n*****��������ʲô�һ�������*****");
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
			printf("������Ĳ���0~5�����������롤����������");
			printf("\n");
			system("pause");
			system("cls");
			break;
		}
	}
}

//�������������Ҹ����µ�ID
void sort_by_time(List* L)
{
	List* p ;
	Info temp;
	int n = CurrentCnt;
	int i, j, k;
	//ð������
	if (L == NULL || L->next == NULL)
		printf("��ǰ������û����Ʒ\n");
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
	//�޸�ID
	for (p = L->next,k=1; p != NULL; p = p->next,k++) 
	{
		p->data.id = k;
	}
}

//��������еĺ�����
bool income_add(List* L, Info dataInfo)
{
	dataInfo.source = 0;
	List* temp;
	List* pre = L, * p = L->next;

	if (CurrentCnt >= 100) {
		printf("��Ϣ��������Ҫ��������ɾ��һ��������Ʒ����!\n");
		return false;
	}
	//β������
	while (p != NULL)
	{
		pre = p;
		p = p->next;
	}
	temp = (List*)malloc(sizeof(List));
	temp->data = dataInfo;
	pre->next = temp;
	temp->next = NULL;
	printf("Tips:���IDΪ%d����Ŀ�ɹ�\n", dataInfo.id);
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
	printf("*�����ѯ���棺\n����1������ID����\n����2��������������\n\n����0������\n\n*���������ѡ��");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("����ID��");
		scanf("%d", &id);
		for (p = L; p != NULL; p = p->next) {
			if ((p->data.source) == 1)
				continue;
			if (p->data.id == id) {
				printf("�ҵ���\n");
				flag = true;
				output_one_item(p);
				printf("\n");
				continue;
			}
		}
		if (flag == false)
			printf("Ī����Ҫ�ҵ�\n");
		else
			printf("ȫ����ӡ�ˣ�\n");
		break;
	case 2:
		printf("��������(xxxx/xx/xx)��");
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
			printf("Ī����Ҫ�ҵ�\n");
		else
			printf("�ҵ��˶�����ȫ����ӡ�ˣ�>_<\n");
		break;
	case 0:
		system("cls");
		break;
	default:
		printf("����������ֲ���1��2���������ز˵�");
		printf("\n");
		break;
	}
}
void income_delete(List* L)
{
	List* prev = NULL, * cur = L;
	int id;
	bool flag = false;

	printf("����ID��");
	scanf("%d", &id);
	for (cur = L; cur != NULL; cur = cur->next) {
		if (cur->data.id == id) {
			printf("�ҵ���!���½���ɾ��\n");
			output_one_item(cur);
			flag = true;
			break;
		}
		prev = cur;
	}
	if (flag == false)
		printf("Ī����Ҫ�ҵ�\n");
	else {
		prev->next = cur->next;
		free(cur);
		printf("ɾ���ɹ���\n");
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

	printf("����ID��");
	scanf("%d", &id);
	for (p = L; p != NULL; p = p->next) {
		if (p->data.id == id) {
			flag = true;
			break;
		}
	}
	if (flag == false)
		printf("Ī����Ҫ�ҵ�\n");
	else {
		printf("�ҵ���,��ѡ����Ҫ�޸ĵ����ݣ�\n1.����\n2.��Ŀ\n3.ԭ��\n���ѡ���ǣ�");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("�����µ����ڣ�");
			read_line(date, 11);
			strcpy(p->data.date, date);
			break;
		case 2:
			printf("�����µ�֧����ȣ�");
			scanf("%f", &amount);
			p->data.amount = amount;
			break;
		case 3:
			printf("�����µ�ԭ��");
			read_line(reason, Length);
			strcpy(p->data.reason, reason);
			break;
		default:
			printf("����������ֲ���1~3֮�䣡\n");
			break;
		}
	}
	if (flag == true)
		printf("�޸ĳɹ���");
	else
		printf("�޸�ʧ�ܣ�");
}
int income_print_txt(List* L)
{
	if (L == NULL)
		return 0;
	List* p = L->next;
	FILE* fp;
	if ((fp = fopen("intcome.txt", "w")) == NULL)
	{
		printf("��ʾ�����ܴ���Ʒ�ļ�\n");
		return 0;
	}
	int save_count = 0, id = 1;
	fprintf(fp, "�����Ǵ�ӡ���������������������:\n");
	fprintf(fp, "���\t����\t\t���\tԭ��\n");
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

//֧�������еĺ�����
bool outcome_add(List *L,Info dataInfo)
{
	dataInfo.source = 1;
	List* temp;
	List* pre = L, * p = L->next;

	if (CurrentCnt >= 100) {
		printf("��Ϣ��������Ҫ��������ɾ��һ��������Ʒ����!\n");
		return false;
	}
	//β������
	while (p != NULL)
	{
		pre = p;
		p = p->next;
	}
	temp = (List*)malloc(sizeof(List));
	temp->data = dataInfo;
	pre->next = temp;
	temp->next = NULL;
	printf("Tips:���IDΪ%d����Ŀ�ɹ�\n",dataInfo.id);
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
	printf("*֧����ѯ���棺\n����1������ID����\n����2��������������\n\n����0������\n\n*���������ѡ��");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("����ID��");
		scanf("%d", &id);
		for (p = L; p != NULL; p = p->next){
			if ((p->data.source) == 0)
				continue;
			if (p->data.id == id) {
				printf("�ҵ���\n");
				flag = true;
				output_one_item(p);
				printf("\n");
				continue;
			}
		}
		if (flag == false)
			printf("Ī����Ҫ�ҵ�\n");
		else 
			printf("ȫ����ӡ�ˣ�\n");
		break;
	case 2:
		printf("��������(xxxx/xx/xx)��");
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
			printf("Ī����Ҫ�ҵ�\n");
		else
			printf("�ҵ��˶�����ȫ����ӡ�ˣ�>_<\n");
		break;
	case 0:
		system("cls");
		break;
	default:
		printf("����������ֲ���1��2���������ز˵�");
		printf("\n");
		break;
	}
}

void outcome_delete(List *L)
{
	List* prev = NULL, * cur = L;
	int id;
	bool flag = false;

	printf("����ID��");
	scanf("%d", &id);
	for (cur = L; cur != NULL; cur = cur->next) {
		if (cur->data.id == id) {
			printf("�ҵ���!���½���ɾ��\n");
			output_one_item(cur);
			flag = true;
			break;
		}
		prev = cur;
	}
	if (flag == false)
		printf("Ī����Ҫ�ҵ�\n");
	else {
		prev->next = cur->next;
		free(cur);
		printf("ɾ���ɹ���\n");
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

	printf("����ID��");
	scanf("%d", &id);
	for (p = L; p != NULL; p = p->next) {
		if (p->data.id == id) {
			flag = true;
			break;
		}
	}
	if (flag == false)
		printf("Ī����Ҫ�ҵ�\n");
	else {
		printf("�ҵ���,��ѡ����Ҫ�޸ĵ����ݣ�\n1.����\n2.��Ŀ\n3.ԭ��\n���ѡ���ǣ�");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("�����µ����ڣ�");
			read_line(date,11);
			strcpy(p->data.date,date);
			break;
		case 2:
			printf("�����µ�֧����ȣ�");
			scanf("%f", &amount);
			p->data.amount = amount;
			break;
		case 3:
			printf("�����µ�ԭ��");
			read_line(reason, Length);
			strcpy(p->data.reason, reason);
			break;
		default:
			printf("����������ֲ���1~3֮�䣡\n");
			break;
		}
	}
	if (flag == true)
		printf("�޸ĳɹ���");
	else
		printf("�޸�ʧ�ܣ�");
}

int outcome_print_txt(List *L)
{
	if (L == NULL)
		return 0;
	List* p = L->next;
	FILE* fp;
	if ((fp = fopen("outcome.txt", "w")) == NULL)
	{
		printf("��ʾ�����ܴ���Ʒ�ļ�\n");
		return 0;
	}
	int save_count = 0, id = 1;
	fprintf(fp, "�����Ǵ�ӡ֧�������������������:\n");
	fprintf(fp, "���\t����\t\t���\tԭ��\n");
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

//�����ѯ�еĺ�����
void print_all_of_datas(List *L)
{
	List* p = L->next;
	printf("��ǰ��%d����Ŀ\n", CurrentCnt);
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
	printf("*�����������ͨ��ID�ҵ�ĳһ����������ҵ�����\n����1������ID����\n����2��������������\n\n����0������\n\n*���������ѡ��");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
		printf("����ID��");
		scanf("%d", &id);
		for (p = L; p != NULL; p = p->next) {
			if (p->data.id == id) {
				printf("�ҵ���\n");
				flag = true;
				output_one_item(p);
				printf("\n");
				continue;
			}
		}
		if (flag == false)
			printf("Ī����Ҫ�ҵ�\n");
		else
			printf("�Ѿ���ӡ�ˣ�\n");
		system("pause");
		break;
	case 2:
		printf("��������(xxxx/xx/xx)��");
		scanf("%s", date);
		for (p = L; p != NULL; p = p->next) {
			if (!strcmp(p->data.date, date)) {
				flag = true;
				output_one_item(p);
				continue;
			}
		}
		if (flag == false)
			printf("Ī����Ҫ�ҵ�\n");
		else
			printf("�ҵ��˶�����ȫ����ӡ�ˣ�>_<\n");
		system("pause");
		break;
	case 0:
		system("cls");
		break;
	default:
		printf("����������ֲ���1��2���������ز˵�");
		printf("\n");
		system("pause");
		break;
	}
	system("cls");
}

void qs(void)                                 //by �¿���
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
	printf("��������ɹ������Ǵ�С����ģ�\n");

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
	//ð������
	if (L == NULL || L->next == NULL)
		printf("��ǰ������û����Ʒ\n");
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

//ͨ�ú���
/*��ȡһ��*/
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

/*��ȡ�ṹ��*/
Info read_item_info() {
	Info item_info;

	item_info.id = CurrentCnt;
	printf("�������ڣ�xxxx/xx/xx)��");
	read_line(item_info.date, 11);
	item_info.source = -1;
	printf("��Ŀ��");
	scanf("%f", &item_info.amount);
	printf("���ͣ�");
	read_line(item_info.reason, Length);
	
		return item_info;
}

/*��ӡ�����ڵ���Ϣ*/
void output_one_item(List* p) {
	if (p == NULL)
		return;
	printf("ID:%d\t|����:%s||",p->data.id, p->data.date);
	if ((p->data.source) == 1)
		printf("֧����%8.2f", -p->data.amount);
	else
		printf("���룺%8.2f", +p->data.amount);
	printf("|ԭ��:%s\n",p->data.reason);
}
/*�����ˮ�˵�txt*/
int save_to_file(List* L) {
	if (L == NULL)
		return 0;
	List* p = L->next;
	FILE* fp;
	if ((fp = fopen("datainfo.txt", "w")) == NULL)
	{
		printf("��ʾ�����ܴ���Ʒ�ļ�\n");
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
/*ɾ�������ļ�*///��ʱ��������,���ļ�ûɾ������
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

/*ͳ�����*/
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

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#pragma warning(disable : 4996)


struct Line_
{
	char id[20];
	char fname[20];
	char lname[20];
	char fon_number[11];

	struct tm date;
	int debt;
	struct Line_* next;

	char err_msg[100];
};
typedef struct Line_ Line;

 Line* createNewLine()
{
	printf("%d\n", sizeof( Line));
	void* v = malloc(sizeof( Line));
	if (!v) {
		printf(" %s\n", "is null");
		return NULL;
	}
	printf(" %s\n", "execute");

	return  (struct Line*)v;
}

void init_fname_lname( Line* l, char* temp, int num_field) {
	char err_msg[30];
	char* field;
	switch (num_field)
	{
	case 1:
		strcpy(err_msg, "error in fname field, ");
		field = l->fname;
		break;
	case 2:
		strcpy(err_msg, "error in lname field, ");
		field = l->lname;
		break;
	default:
		field = NULL;
		break;
	}
	for (int i = 0;i < strlen(temp);i++) {
		if (temp[i] < 'A' || temp[i]>'z') {
			strcat(l->err_msg, err_msg);
			break;
		}
	}
	if (!strlen(temp))
		strcat(l->err_msg, err_msg);
	strcpy(field, temp);
}

void init_id_fon( Line* l, char* temp, int num_field) {
	char err_msg[30];
	char* field;
	switch (num_field)
	{
	case 1:
		strcpy(err_msg, "error in id field, ");
		field = l->id;
		break;
	case 2:
		strcpy(err_msg, "error in fon_number field, ");
		field = l->fon_number;
		break;
	default:
		field = NULL;
		break;
	}
	for (int i = 0;i < strlen(temp);i++) {
		if (temp[i] < '0' || temp[i]>'9')
			strcat(l->err_msg, err_msg);
	}
	if (!strlen(temp))
		strcat(l->err_msg, err_msg);
	strcpy(field, temp);

}

void init_date( Line* l, char* temp) {

	int dd_mm_yy[] = { 0,0,0 };
	int j = 0;

	for (int i = 0; i < strlen(temp); i++)
	{
		if (temp[i] == '.' || temp[i] == '\\' || temp[i] == '/') {
			j++;
			temp[i] = ' ';
		}
		else if (temp[i] < '0' || temp[i]>'9') {
			strcat(l->err_msg, "error in date field, ");
			return;
		}
	}
	if (j != 2) {
		strcat(l->err_msg, "error in date field, ");
		return;
	}
	sscanf(temp, "%d %d %d", dd_mm_yy, dd_mm_yy + 1, dd_mm_yy + 2);

	struct tm date = { 0, 0, 0, dd_mm_yy[0], dd_mm_yy[1],dd_mm_yy[2] - 1900, 0, 0, 0 };
	l->date = date;
}

void init_debt( Line* l, char* temp) {
	int debt;
	sscanf(temp, "%d", &debt);
	if (!(debt <= 0)) {
		strcat(l->err_msg, "error in debt field, ");
	}
	l->debt = debt;
}

void freeList( Line* l) {
	if (!l)
		return;
	freeList(l->next);
	free(l);
}

int main()
{
	char temp[20];
	int x, i;
	Line head = { 0,0,0,0,0,0,0,0 };
	Line *temp_line, * new_line;

	FILE* file = fopen("taskRavTech.csv", "a+t");

	if (!file)
		return -1;

	//ignore from high line
	while ((x = fgetc(file)) != EOF) {
		printf("%c", x);
	}
	rewind(file);

	while ((x = fgetc(file)) != '\n');
	while (x != EOF)
	{
		//create new item in list
		new_line = ( Line*)malloc(sizeof( Line));
		if (!new_line)
			return -1;
		//insert values to a new item
		for (int j = 0; j < 6; j++)
		{
			for (i = 0; i < 19 && (x = fgetc(file)) != ',' && x != '\n' && x != EOF; i++)
			{
				temp[i] = x;
			}

			if (x != ',' && x != '\n')
				while ((x = fgetc(file)) != ',');
			temp[i] = 0;

			switch (j)
			{
			case 0:
				init_id_fon(new_line, temp, 1);
				break;
			case 1:
				init_fname_lname(new_line, temp, 1);
				break;
			case 2:
				init_fname_lname(new_line, temp, 2);
				break;
			case 3:
				init_id_fon(new_line, temp, 2);
				break;
			case 4:
				init_date(new_line, temp);
				break;
			case 5:
				init_debt(new_line, temp);
				break;

			default:
				break;
			}
		}

			temp_line = &head;
			while (temp_line ->next && (temp_line->next->debt > new_line->debt))
			{
				temp_line = temp_line->next;
			}
			new_line->next = temp_line->next;
			temp_line->next= new_line;
		

	}
	fclose(file);

	//print all list
	temp_line = head.next;
	while (temp_line)
	{
		strftime(temp, 20, "%x", &temp_line->date);
		printf("%s, %s, %s, %s, %s, %d\n",
			temp_line->id, temp_line->fname, temp_line->lname, temp_line->fon_number, temp, temp_line->debt);
		temp_line = temp_line->next;
	}

	freeList(head.next);
	return 0;
}












// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

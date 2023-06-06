# **Лабораторна робота 16**

# **Панкеєв Владислав Олексійович 922-Б**

# **Умова**

```
На базі попередньо розробленого функціоналу по роботі з прикладною областю сформувати динамічний масив елементів розробленої структури. Реалізувати наступні функції роботи зі списком:

-вивід вмісту списку на екран;
-реалізувати функцію №1 з категорії "Методи для роботи з колекцією" (див. завдання з РЗ);
-додавання об’єкта у кінець списку;
-видалення об’єкта зі списку за індексом.
-сортування вмісту списку за одним з критеріїв
```
## **Основна частина:**

### **Вміст файлу main.c**

```c
#include "lib.h"

int main(int argc, char *argv[])
{
	unsigned int result_check[2] = { 0, 0 };
	unsigned int *res_check = result_check;
	struct student_arr *arr_found_students;
	struct student_arr *arr_students;
	int finish_program = 1;

	if ((*res_check = check_arguments(argc, argv)) != 0) {
		write_info_error(res_check);
		return 1;
	}

	check_input_data(res_check, *(argv + 1));
	if (*res_check != 0) {
		write_info_error(res_check);
		return 1;
	}

	arr_students = read_from_file(*(argv + 1));
	if (arr_students == NULL) {
		*res_check = 12;
		write_info_error(res_check);
		return 1;
	}
	write_on_screen(arr_students);

	while (finish_program != 0) {
		int n_operation = 0;
		printf("\nЩо ви хочете зробити\n1.Вивести студентів певного року вступу\n2.Сортувати студентів\n3.Сгенерувати рандомних студентів.\n4.Взяти список студентів з файлу\n5.Видалити студента\n6.Додати студента\n7.Вийти\nВведіть номер операції:");
		scanf("%d", &n_operation);
		while (getchar() != '\n')
			;

		if (n_operation == SEARCH_STUDENTS) {
			unsigned int enroll_year;
			printf("\nВведіть рік, вступники якого вас цікавлять:");
			scanf("%d", &enroll_year);
			printf("\nСтуденти які вступили у %d році\n", enroll_year);
			while (getchar() != '\n')
				;

			arr_found_students = search_student(arr_students, enroll_year, *(argv + 2));
			if (arr_found_students == NULL) {
				free_memory(arr_students);
				*res_check = 12;
				write_info_error(res_check);
				return 1;
			}

			printf("\n\n");
			write_on_screen(arr_students);

			free(arr_found_students->students);
			free(arr_found_students);

		} else if (n_operation == SORT_STUDENTS) {
			printf("\n\nВідсортовані студенти за роком вступу від старошого до молодшого\n");
			sort_student_by_year_decreas(arr_students, *(argv + 2));

			printf("\nВідсортовані студенти за роком вступу від молодшого до старошого\n");
			sort_student_by_year_increas(arr_students, *(argv + 2));

			printf("\nВідсортовані студенти за алфавітом\n");
			sort_student_by_alphabet(arr_students, *(argv + 2));

		} else if (n_operation == GENERATION_STUDENTS) {
			char yes_no = 'n';

			if (arr_students != NULL) {
				printf("\n!!!УВАГА!!!");
				printf("\nСписок студентів який є зараз зітреться!\nВи впевнені що хочите це зробити?\n[Y/n] ");
				scanf("%c", &yes_no);
				while (getchar() != '\n')
					;
			}

			if (yes_no == 'y' || yes_no == 'Y') {
				unsigned int n_students = 0;
				printf("\nВведіть кількість студентів яку ви хочите сгенерувати:");
				scanf("%d", &n_students);
				while (getchar() != '\n')
					;

				if (arr_students != NULL)
					free_memory(arr_students);

				arr_students = auto_generate_arr_student(n_students);
				if (arr_students == NULL) {
					*res_check = 12;
					write_info_error(res_check);
					return 1;
				}
			}

			write_on_screen(arr_students);

		} else if (n_operation == WRITE_OUT_FILE) {
			if (arr_students != NULL)
				free_memory(arr_students);

			arr_students = read_from_file(*(argv + 1));
			if (arr_students == NULL) {
				*res_check = 12;
				write_info_error(res_check);
				return 1;
			}

			write_on_screen(arr_students);

		} else if (n_operation == DELETE_STUDENT) {
			unsigned int n_student = 0;
			printf("\nВедіть номер студента якого ви хочете видалити:");
			scanf("%d", &n_student);
			while (getchar() != '\n')
				;

			arr_students = delete_student(arr_students, n_student);
			if (arr_students == NULL) {
				*res_check = 12;
				write_info_error(res_check);
				return 1;
			}

			write_on_screen(arr_students);
			rewrite_input_file(*(argv + 1), arr_students);

		} else if (n_operation == ADD_STUDENT) {
			arr_students = add_student(arr_students);
			if (arr_students == NULL)
				return 1;

			rewrite_input_file(*(argv + 1), arr_students);
			write_on_screen(arr_students);

		} else if (n_operation == CLOSE_PROGRAM)
			finish_program = 0;
		else {
			*res_check = 1;
			write_info_error(res_check);
		}
	}

	free_memory(arr_students);

	return 0;
}
```

### **Вміст файлу lib.c**

```c
#include "lib.h"

unsigned int check_arguments(int number_arguments, char *arr_arguments[])
{
	FILE *output_file;
	FILE *input_file;

	if (number_arguments > 1) {
		if (number_arguments != 3)
			return 2;
		else {
			output_file = fopen(*(arr_arguments + 2), "w");
			input_file = fopen(*(arr_arguments + 1), "r");

			if (output_file == NULL || input_file == NULL)
				return 3;
			else {
				fclose(input_file);
				fclose(output_file);
				return 0;
			}
		}
	}

	return 1;
}


void check_input_data(unsigned int *res_check, char *path_input_file)
{
	FILE *input_file = fopen(path_input_file, "r");
	unsigned int n_str;
	char budget_edu[8];
	char name_student[55];
	char name_kurator[55];
	char name_faculti[15];
	char group_n[21];
	char name_building_cafedra[8];

	if (fscanf(input_file, "Кількість студентів:%d", &n_str) != 1) {
		*(res_check) = 4;
		fclose(input_file);
		return;
	} else if (n_str == 0) {
		*(res_check) = 5;
		fclose(input_file);
		return;
	}

	for (unsigned int i = 1; i <= n_str; i++) {
		fscanf(input_file, "\n%7[^;\n];", budget_edu);
		if (fgetc(input_file) != ' ' && strcmp(budget_edu, "так") != 0 && strcmp(budget_edu, "ні") != 0) {
			*(res_check) = 6;
			*(res_check + 1) = i;
			fclose(input_file);
			return;
		}

		fscanf(input_file, "%54[^;\n];", name_student);
		if (fgetc(input_file) != ' ') {
			*(res_check) = 7;
			*(res_check + 1) = i;
			fclose(input_file);
			return;
		}

		fscanf(input_file, "%54[^;\n];", name_kurator);
		if (fgetc(input_file) != ' ') {
			*(res_check) = 8;
			*(res_check + 1) = i;
			fclose(input_file);
			return;
		}

		fscanf(input_file, "%14[^;\n];", name_faculti);
		if (fgetc(input_file) != ' ') {
			*(res_check) = 9;
			*(res_check + 1) = i;
			fclose(input_file);
			return;
		}

		fscanf(input_file, "%20[^;\n];", group_n);
		if (fgetc(input_file) != ' ') {
			*(res_check) = 10;
			*(res_check + 1) = i;
			fclose(input_file);
			return;
		}

		fscanf(input_file, "%7[^;\n];", name_building_cafedra);
		if (fgetc(input_file) != '\n' || (strcmp(name_building_cafedra, "ВК") != 0 && strcmp(name_building_cafedra, "ГАК") != 0 &&
						  strcmp(name_building_cafedra, "У1") != 0 && strcmp(name_building_cafedra, "У2") != 0)) {
			*(res_check) = 11;
			*(res_check + 1) = i;
			fclose(input_file);
			return;
		}
	}
	fclose(input_file);

	*(res_check + 1) = n_str;
	return;
}

void write_info_error(unsigned int *res_check)
{
	switch (*res_check) {
	case 1:
		printf("Ви ввели не коректні дані\n");
		break;

	case 2:
		printf("Ви ввели некоректну кількість даних\n");
		break;

	case 3:
		printf("Не можливо отримати доступ до файлу або дерикторії\n");
		break;

	case 4:
		printf("Ви не ввели кількість студентів\n\n");
		break;

	case 5:
		printf("Кількість студентів для сортування дорівню 0\n\n");
		break;

	case 6:
		printf("У вас помилка на строці:%d, у полі форма навчання\n\n", *(res_check + 1));
		break;

	case 7:
		printf("У вас помилка на строці:%d, у полі ім'я студента\n\n", *(res_check + 1));
		break;

	case 8:
		printf("У вас помилка на строці:%d, у полі ім'я куратора\n\n", *(res_check + 1));
		break;

	case 9:
		printf("У вас помилка на строці:%d, у полі назва факультету\n\n", *(res_check + 1));
		break;

	case 10:
		printf("У вас помилка на строці:%d, у полі код групи\n\n", *(res_check + 1));
		break;

	case 11:
		printf("У вас помилка на строці:%d, у полі назва корпусу кафедри\n\n", *(res_check + 1));
		break;

	case 12:
		printf("Для роботи програми не вистачає ресурсів комп'ютера\n\n");
		break;
	}
}

void free_memory(struct student_arr *arr_students)
{
	for (unsigned int i = 0; i < arr_students->n_students; i++) {
		if ((*(arr_students->students + i))->name_student != NULL) {
			free((*(arr_students->students + i))->name_student);
			free((*(arr_students->students + i))->name_kurator);
			free((*(arr_students->students + i))->faculti_and_group->name_faculti);
			free((*(arr_students->students + i))->faculti_and_group->group_n);
		}
		free((*(arr_students->students + i))->faculti_and_group);
		free(*(arr_students->students + i));
	}

	free(arr_students->students);
	free(arr_students);
}

struct student_arr *create_arr_students(unsigned int num_students)
{
	struct student_arr *array = malloc(sizeof(struct student_arr));
	if (array == NULL)
		return NULL;

	array->students = malloc(num_students * sizeof(struct student_data *));
	if (array->students == NULL) {
		free(array);
		return NULL;
	}

	array->n_students = num_students;

	return array;
}

struct student_data *create_students()
{
	struct student_data *student = malloc(sizeof(struct student_data));
	if (student == NULL)
		return NULL;

	student->faculti_and_group = malloc(sizeof(struct group));
	if (student->faculti_and_group == NULL) {
		free(student);
		return 0;
	}

	return student;
}

void generate_year(struct student_arr *arr_students)
{
	srand((unsigned int)time(NULL));
	for (unsigned int i = 0; i < arr_students->n_students; i++) {
		if ((*(arr_students->students + i))->enroll_year == 0) {
			(*(arr_students->students + i))->enroll_year = (unsigned int)rand() % 5 + 2017;

			for (unsigned int j = i; j < arr_students->n_students; j++)
				if (strcmp((*(arr_students->students + j))->faculti_and_group->group_n,
					   (*(arr_students->students + i))->faculti_and_group->group_n) == 0)
					(*(arr_students->students + j))->enroll_year = (*(arr_students->students + i))->enroll_year;
		}
	}
}

struct student_data *recording_info_student(char *budget_edu, char *name_student, char *name_kurator, char *name_faculti, char *group_n,
					    char *name_building_cafedra)
{
	struct student_data *student = create_students();
	if (student == NULL)
		return NULL;

	student->enroll_year = 0;

	student->name_student = strdup(name_student);
	if (student->name_student == NULL) {
		free(student->faculti_and_group);
		free(student);
		return NULL;
	}

	student->name_kurator = strdup(name_kurator);
	if (student->name_kurator == NULL) {
		free(student->name_student);
		free(student->faculti_and_group);
		free(student);
		return NULL;
	}

	student->faculti_and_group->name_faculti = strdup(name_faculti);
	if (student->faculti_and_group->name_faculti == NULL) {
		free(student->name_student);
		free(student->name_kurator);
		free(student->faculti_and_group);
		free(student);
		return NULL;
	}

	student->faculti_and_group->group_n = strdup(group_n);
	if (student->faculti_and_group->group_n == NULL) {
		free(student->name_student);
		free(student->name_kurator);
		free(student->faculti_and_group->name_faculti);
		free(student->faculti_and_group);
		free(student);
		return NULL;
	}

	if (strcmp(budget_edu, "так") == 0)
		student->budget_edu = BUDGET_ON;
	else if (strcmp(budget_edu, "ні") == 0)
		student->budget_edu = BUDGET_OFF;
	else
		student->budget_edu = NO_INDICATE;

	if (strcmp(name_building_cafedra, "ВК") == 0)
		student->name_building_cafedra = BK;
	else if (strcmp(name_building_cafedra, "ГАК") == 0)
		student->name_building_cafedra = GAK;
	else if (strcmp(name_building_cafedra, "У1") == 0)
		student->name_building_cafedra = Y1;
	else if (strcmp(name_building_cafedra, "У2") == 0)
		student->name_building_cafedra = Y2;
	else
		student->name_building_cafedra = NO_INDICA;

	return student;
}

struct student_arr *read_from_file(char *path_input_file)
{
	FILE *input_file = fopen(path_input_file, "r");
	unsigned int n_str;
	char budget_edu[8];
	char name_student[55];
	char name_kurator[55];
	char name_faculti[15];
	char group_n[21];
	char name_building_cafedra[8];

	fscanf(input_file, "Кількість студентів:%d", &n_str);

	struct student_arr *arr_students = create_arr_students(n_str);
	if (arr_students == NULL)
		return NULL;

	for (unsigned int i = 0; i < n_str; i++) {
		arr_students->n_students = i;

		fscanf(input_file, "\n%7[^;]; %54[^;]; %54[^;]; %14[^;]; %20[^;]; %7[^;];\n", budget_edu, name_student, name_kurator, name_faculti,
		       group_n, name_building_cafedra);

		(*(arr_students->students + i)) =
			recording_info_student(budget_edu, name_student, name_kurator, name_faculti, group_n, name_building_cafedra);
		if ((*(arr_students->students + i)) == NULL) {
			free_memory(arr_students);
			return NULL;
		}
	}
	fclose(input_file);
	arr_students->n_students++;

	generate_year(arr_students);

	return arr_students;
}

struct student_arr *search_student(struct student_arr *arr_students, unsigned int enroll_year, char *path_to_file)
{
	unsigned int n_students = 0;
	struct student_arr *arr_found_students;

	for (unsigned int i = 0; i < (arr_students->n_students); i++) {
		if ((*(arr_students->students + i))->enroll_year == enroll_year) {
			n_students++;
		}
	}

	arr_found_students = create_arr_students(n_students);
	if (arr_found_students == NULL)
		return NULL;

	for (unsigned int i = 0, j = 0; i < (arr_students->n_students); i++) {
		if ((*(arr_students->students + i))->enroll_year == enroll_year) {
			*(arr_found_students->students + j) = *(arr_students->students + i);
			j++;
		}
	}

	FILE *file = fopen(path_to_file, "w");
	fprintf(file, "Студенти які вступили у %d році\n", enroll_year);
	fclose(file);

	write_out_file(path_to_file, arr_found_students);
	write_on_screen(arr_found_students);

	return arr_found_students;
}

void sort_student_by_year_decreas(struct student_arr *sort_arr_student, char *path_to_file)
{
	int stop_sort;

	if ((sort_arr_student->n_students) > 0) {
		for (unsigned int i = 0; i < (sort_arr_student->n_students) - 1; i++) {
			stop_sort = 0;

			for (unsigned int j = 0; j < (sort_arr_student->n_students) - i - 1; j++) {
				if ((*(sort_arr_student->students + j))->enroll_year < (*(sort_arr_student->students + j + 1))->enroll_year) {
					struct student_data *student = *(sort_arr_student->students + j);
					*(sort_arr_student->students + j) = *(sort_arr_student->students + j + 1);
					*(sort_arr_student->students + j + 1) = (student);
					stop_sort = 1;
				}
			}

			if (stop_sort == 0)
				break;
		}
	}

	FILE *file = fopen(path_to_file, "w");
	fprintf(file, "Відсортовані студенти за роком вступу від старошого до молодшого\n");
	fclose(file);

	write_out_file(path_to_file, sort_arr_student);
	write_on_screen(sort_arr_student);
}

void sort_student_by_year_increas(struct student_arr *sort_arr_student, char *path_to_file)
{
	int stop_sort;

	if ((sort_arr_student->n_students) > 0) {
		for (unsigned int i = 0; i < (sort_arr_student->n_students) - 1; i++) {
			stop_sort = 0;

			for (unsigned int j = 0; j < (sort_arr_student->n_students) - i - 1; j++) {
				if ((*(sort_arr_student->students + j))->enroll_year > (*(sort_arr_student->students + j + 1))->enroll_year) {
					struct student_data *student = *(sort_arr_student->students + j);
					*(sort_arr_student->students + j) = *(sort_arr_student->students + j + 1);
					*(sort_arr_student->students + j + 1) = (student);
					stop_sort = 1;
				}
			}

			if (stop_sort == 0)
				break;
		}
	}

	FILE *file = fopen(path_to_file, "a");
	fprintf(file, "Відсортовані студенти за роком вступу від молодшого до старошого\n");
	fclose(file);

	write_out_file(path_to_file, sort_arr_student);
	write_on_screen(sort_arr_student);
}

void sort_student_by_alphabet(struct student_arr *sort_arr_student, char *path_to_file)
{
	int stop_sort;

	if ((sort_arr_student->n_students) > 0) {
		for (unsigned int i = 0; i < (sort_arr_student->n_students) - 1; i++) {
			stop_sort = 0;

			for (unsigned int j = 0; j < (sort_arr_student->n_students) - i - 1; j++) {
				if (strcmp((*(sort_arr_student->students + j))->name_student, (*(sort_arr_student->students + j + 1))->name_student) >
				    0) {
					struct student_data *student = *(sort_arr_student->students + j);
					*(sort_arr_student->students + j) = *(sort_arr_student->students + j + 1);
					*(sort_arr_student->students + j + 1) = (student);
					stop_sort = 1;
				}
			}

			if (stop_sort == 0)
				break;
		}
	}

	FILE *file = fopen(path_to_file, "a");
	fprintf(file, "Відсортовані студенти за алфавітом\n");
	fclose(file);

	write_out_file(path_to_file, sort_arr_student);
	write_on_screen(sort_arr_student);
}

struct student_arr *auto_generate_arr_student(unsigned int n_students)
{
	char budget_edu[8] = { 0 };
	char name_student[55] = { 0 };
	char name_kurator[55] = { 0 };
	char name_faculti[15] = { 0 };
	char group_n[21] = { 0 };
	char name_building_cafedra[8] = { 0 };
	char letter_arr[] = "АаБбВвГгҐґДдЕеЄєЖжЗзИиІіЇїЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщьЮюЯя";

	struct student_arr *arr_students = create_arr_students(n_students);
	if (arr_students == NULL)
		return NULL;

	srand((unsigned int)time(NULL));

	for (unsigned int t = 0; t < n_students; t++) {
		arr_students->n_students = t;

		for (int i = 0; i < 10; i++) {
			int j = (int)(rand() % (int)strlen(letter_arr)) - 1;
			if (j % 2 != 0)
				j++;
			strncat(name_student, &letter_arr[j], 2);
		}

		for (int i = 0; i < 10; i++) {
			int j = (int)(rand() % (int)strlen(letter_arr));
			if (j % 2 != 0)
				j++;
			strncat(name_kurator, &letter_arr[j], 2);
		}

		for (int i = 0; i < 3; i++) {
			int j = (int)(rand() % (int)strlen(letter_arr));
			if (j % 2 != 0)
				j++;
			strncat(name_faculti, &letter_arr[j], 2);
		}

		for (int i = 0; i < 4; i++) {
			int j = (int)(rand() % (int)strlen(letter_arr));
			if (j % 2 != 0)
				j++;
			strncat(group_n, &letter_arr[j], 2);
		}

		int budget = rand() % 2;
		int cafedra = rand() % 4;

		if (budget == BUDGET_ON)
			strcpy(budget_edu, "так");
		else
			strcpy(budget_edu, "ні");

		if (cafedra == 0)
			strcpy(name_building_cafedra, "ВК");
		else if (cafedra == 1)
			strcpy(name_building_cafedra, "ГАК");
		else if (cafedra == 2)
			strcpy(name_building_cafedra, "У1");
		else
			strcpy(name_building_cafedra, "У2");

		(*(arr_students->students + t)) =
			recording_info_student(budget_edu, name_student, name_kurator, name_faculti, group_n, name_building_cafedra);
		if ((*(arr_students->students + t)) == NULL) {
			free_memory(arr_students);
			return NULL;
		}

		memset(name_building_cafedra, 0, 8);
		memset(budget_edu, 0, 8);
		memset(name_student, 0, 55);
		memset(name_kurator, 0, 55);
		memset(name_faculti, 0, 15);
		memset(group_n, 0, 21);
	}
	arr_students->n_students = n_students;

	generate_year(arr_students);

	return arr_students;
}

struct student_arr *delete_student(struct student_arr *arr_students, unsigned int n_student)
{
	if (n_student <= 0 || n_student > arr_students->n_students)
		return arr_students;

	struct student_arr *arr_new_students = create_arr_students((arr_students->n_students) - 1);
	if (arr_new_students == NULL) {
		free_memory(arr_students);
		return NULL;
	}

	for (unsigned int i = 0, j = 0; i < (arr_students->n_students); i++) {
		if (i != n_student - 1) {
			*(arr_new_students->students + j) = *(arr_students->students + i);
			j++;
		}
	}

	free((*(arr_students->students + n_student - 1))->name_student);
	free((*(arr_students->students + n_student - 1))->name_kurator);
	free((*(arr_students->students + n_student - 1))->faculti_and_group->name_faculti);
	free((*(arr_students->students + n_student - 1))->faculti_and_group->group_n);
	free((*(arr_students->students + n_student - 1))->faculti_and_group);
	free(*(arr_students->students + n_student - 1));
	free(arr_students->students);
	free(arr_students);

	return arr_new_students;
}

struct student_arr *add_student(struct student_arr *arr_students)
{
	char budget_edu[8];
	char name_student[55];
	char name_kurator[55];
	char name_faculti[15];
	char group_n[21];
	char name_building_cafedra[8];

	struct student_arr *arr_new_students = create_arr_students(arr_students->n_students + 1);
	if (arr_new_students == NULL) {
		free_memory(arr_students);
		return NULL;
	}

	for (unsigned int i = 0; i < arr_students->n_students; i++)
		*(arr_new_students->students + i) = *(arr_students->students + i);

	free(arr_students->students);
	free(arr_students);

	scanf("\n%7[^;]; %54[^;]; %54[^;]; %14[^;]; %20[^;]; %7[^;];", budget_edu, name_student, name_kurator, name_faculti, group_n,
	      name_building_cafedra);

	(*(arr_new_students->students + arr_new_students->n_students - 1)) =
		recording_info_student(budget_edu, name_student, name_kurator, name_faculti, group_n, name_building_cafedra);
	if ((*(arr_new_students->students + arr_new_students->n_students - 1)) == NULL) {
		arr_new_students->n_students = arr_students->n_students - 1;
		free_memory(arr_new_students);
		return NULL;
	}

	while (getchar() != '\n')
		;

	generate_year(arr_new_students);

	return arr_new_students;
}

void rewrite_input_file(char *path_output_file, struct student_arr *arr_sort_students)
{
	FILE *output_file = fopen(path_output_file, "w");

	fprintf(output_file, "Кількість студентів:%d\n", arr_sort_students->n_students);

	for (unsigned int i = 0; i < (arr_sort_students->n_students); i++) {
		if ((*(arr_sort_students->students + i))->budget_edu == BUDGET_ON)
			fprintf(output_file, "\nтак; ");
		else if ((*(arr_sort_students->students + i))->budget_edu == BUDGET_OFF)
			fprintf(output_file, "\nні; ");
		else
			fprintf(output_file, "\nне вказано; ");

		fprintf(output_file, "%s; %s; %s; %s;", (*(arr_sort_students->students + i))->name_student,
			(*(arr_sort_students->students + i))->name_kurator, (*(arr_sort_students->students + i))->faculti_and_group->name_faculti,
			(*(arr_sort_students->students + i))->faculti_and_group->group_n);

		switch ((*(arr_sort_students->students + i))->name_building_cafedra) {
		case BK:
			fprintf(output_file, " ВК\n");
			break;
		case GAK:
			fprintf(output_file, " ГАК\n");
			break;
		case Y1:
			fprintf(output_file, " У1\n");
			break;
		case Y2:
			fprintf(output_file, " У2\n");
			break;
		case NO_INDICA:
			fprintf(output_file, " не вказано\n");
			break;
		}
	}
	fclose(output_file);
}

void write_out_file(char *path_output_file, struct student_arr *arr_sort_students)
{
	FILE *output_file = fopen(path_output_file, "a");

	fprintf(output_file, "Кількість студентів дорівнює %d\n", arr_sort_students->n_students);

	for (unsigned int i = 0; i < (arr_sort_students->n_students); i++) {
		if ((*(arr_sort_students->students + i))->budget_edu == BUDGET_ON)
			fprintf(output_file, "\nтак; ");
		else if ((*(arr_sort_students->students + i))->budget_edu == BUDGET_OFF)
			fprintf(output_file, "\nні; ");
		else
			fprintf(output_file, "\nне вказано; ");

		fprintf(output_file, "%s; %s; %s; %s; %d;", (*(arr_sort_students->students + i))->name_student,
			(*(arr_sort_students->students + i))->name_kurator, (*(arr_sort_students->students + i))->faculti_and_group->name_faculti,
			(*(arr_sort_students->students + i))->faculti_and_group->group_n, (*(arr_sort_students->students + i))->enroll_year);

		switch ((*(arr_sort_students->students + i))->name_building_cafedra) {
		case BK:
			fprintf(output_file, " ВК\n\n");
			break;
		case GAK:
			fprintf(output_file, " ГАК\n\n");
			break;
		case Y1:
			fprintf(output_file, " У1\n\n");
			break;
		case Y2:
			fprintf(output_file, " У2\n\n");
			break;
		case NO_INDICA:
			fprintf(output_file, " не вказано\n");
			break;
		}
	}

	fclose(output_file);
}

void write_on_screen(struct student_arr *arr_sort_students)
{
	printf("Кількість студентів дорівнює %d\n", arr_sort_students->n_students);

	for (unsigned int i = 0, j = 1; i < (arr_sort_students->n_students); i++, j++) {
		if ((*(arr_sort_students->students + i))->budget_edu == BUDGET_ON)
			printf("\n%d.так; ", j);
		else if ((*(arr_sort_students->students + i))->budget_edu == BUDGET_OFF)
			printf("\n%d.ні; ", j);
		else
			printf("\n%d.не вказано; ", j);

		printf("%s; %s; %s; %s; %d;", (*(arr_sort_students->students + i))->name_student, (*(arr_sort_students->students + i))->name_kurator,
		       (*(arr_sort_students->students + i))->faculti_and_group->name_faculti,
		       (*(arr_sort_students->students + i))->faculti_and_group->group_n, (*(arr_sort_students->students + i))->enroll_year);

		switch ((*(arr_sort_students->students + i))->name_building_cafedra) {
		case BK:
			printf(" ВК\n\n");
			break;
		case GAK:
			printf(" ГАК\n\n");
			break;
		case Y1:
			printf(" У1\n\n");
			break;
		case Y2:
			printf(" У2\n\n");
			break;
		case NO_INDICA:
			printf(" не вказано\n\n");
			break;
		}
	}
}
```

### **Вміст файлу lib.h**
```c
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SEARCH_STUDENTS 1
#define SORT_STUDENTS 2
#define GENERATION_STUDENTS 3
#define WRITE_OUT_FILE 4
#define DELETE_STUDENT 5
#define ADD_STUDENT 6
#define CLOSE_PROGRAM 7

#define BUDGET_ON 1
#define BUDGET_OFF 0

#define NO_INDICATE 3

enum cafedra { BK, GAK, Y1, Y2 ,NO_INDICA};

struct group {
	char *name_faculti;
	char *group_n;
};

struct student_data {
	unsigned int budget_edu;
	char *name_student;
	char *name_kurator;
	unsigned int enroll_year;
	struct group *faculti_and_group;
	enum cafedra name_building_cafedra;
};

struct student_arr {
	unsigned int n_students;
	struct student_data **students;
};

unsigned int check_arguments(int number_arguments, char *arr_arguments[]);

void check_input_data(unsigned int *res_check, char *path_input_file);

void write_info_error(unsigned int *res_check);

struct student_arr *create_arr_students(unsigned int num_students);

struct student_data *create_students();

void generate_year(struct student_arr *arr_students);

void free_memory(struct student_arr *arr_students);

struct student_arr *read_from_file(char *path_input_file);

struct student_arr *search_student(struct student_arr *student, unsigned int enroll_year, char *path_to_file);

void sort_student_by_year_increas(struct student_arr *sort_arr_student, char *path_to_file);

void sort_student_by_year_decreas(struct student_arr *sort_arr_student, char *path_to_file);

void sort_student_by_alphabet(struct student_arr *sort_arr_student, char *path_to_file);

struct student_data *recording_info_student(char *budget_edu, char *name_student, char *name_kurator, char *name_faculti, char *group_n, char *name_building_cafedra);

struct student_arr *auto_generate_arr_student(unsigned int n_students);

struct student_arr *delete_student(struct student_arr *arr_students, unsigned int n_student);

struct student_arr *add_student(struct student_arr *arr_students);

void rewrite_input_file(char *path_output_file, struct student_arr *arr_students);

void write_out_file(char *path_output_file, struct student_arr *student);

void write_on_screen(struct student_arr *student);
```

### **Вміст файлу test.c**
```c
#include "../src/lib.h"
#include <check.h>

START_TEST(test_no_write_arguments)
{
	int number_arguments = 0;
	char *arr_arguments[1];
	unsigned int expected_code_error = 1;
	unsigned int actual_code_error;

	actual_code_error = check_arguments(number_arguments, arr_arguments);

	ck_assert_int_eq(expected_code_error, actual_code_error);
}
END_TEST

START_TEST(test_wrong_qu_arguments)
{
	int number_arguments = 2;
	char *arr_arguments[1];
	unsigned int expected_code_error = 2;
	unsigned int actual_code_error;

	actual_code_error = check_arguments(number_arguments, arr_arguments);

	ck_assert_int_eq(expected_code_error, actual_code_error);

	number_arguments = 4;

	actual_code_error = check_arguments(number_arguments, arr_arguments);

	ck_assert_int_eq(expected_code_error, actual_code_error);
}
END_TEST

START_TEST(test_true_arguments)
{
	int number_arguments = 3;
	char *arr_arguments[3];

	*(arr_arguments) = "0";
	*(arr_arguments + 1) = "test/test_input.txt";
	*(arr_arguments + 2) = "dist/true_arguments.txt";

	unsigned int expected_code_error = 0;
	unsigned int actual_code_error;

	actual_code_error = check_arguments(number_arguments, arr_arguments);

	ck_assert_int_eq(expected_code_error, actual_code_error);
}
END_TEST

START_TEST(test_check_n_students)
{
	char input_file[] = "test/check_n_students/check_n_students"; 
	char input_file2[] = "test/check_n_students/check_n_students2"; 
	unsigned int result_function[2] = { 0, 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 4, 0 };

	check_input_data(res_func, input_file);

	ck_assert_int_eq(*(expected_code_error), *(res_func));

	*(expected_code_error) = 5;

	check_input_data(res_func, input_file2);

	ck_assert_int_eq(*(expected_code_error), *res_func);
}
END_TEST

START_TEST(test_check_form_education)
{
	char input_file[] = "test/check_form_education/check_form_education";
	unsigned int result_function[2] = { 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 6, 1 };

	check_input_data(res_func, input_file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_code_error + i), *(res_func + i));
}
END_TEST

START_TEST(test_check_name_students)
{
	char input_file[] = "test/check_name_students/check_name_students";
	unsigned int result_function[2] = { 0, 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 7, 1 };

	check_input_data(res_func, input_file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_code_error + i), *(res_func + i));
}
END_TEST

START_TEST(test_check_name_curator)
{
	char input_file[] = "test/check_name_curator/check_name_curator";
	unsigned int result_function[2] = { 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 8, 1 };

	check_input_data(res_func, input_file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_code_error + i), *(res_func + i));
}
END_TEST

START_TEST(test_check_name_faculti)
{
	char input_file[] = "test/test_check_name_faculti/test_check_name_faculti";
	unsigned int result_function[2] = { 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 9, 1 };

	check_input_data(res_func, input_file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_code_error + i), *(res_func + i));
}
END_TEST

START_TEST(test_check_group_number)
{
	char input_file[] = "test/check_group_number/check_group_number";
	unsigned int result_function[2] = { 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 10, 1 };

	check_input_data(res_func, input_file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_code_error + i), *(res_func + i));
}
END_TEST

START_TEST(test_check_name_cafedra)
{
	char input_file[] = "test/check_name_cafedra/check_name_cafedra";
	unsigned int result_function[2] = { 0 };
	unsigned int *res_func = result_function;
	int expected_code_error[2] = { 11, 1 };

	check_input_data(res_func, input_file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_code_error + i), *(res_func + i));
}
END_TEST

START_TEST(test_true_write_data)
{
	char file[] = "test/test_input.txt";
	unsigned int actual_result_function[2] = { 0 };
	unsigned int *actual_res_func = actual_result_function;
	int expected_result_function[2] = { 0, 4 };

	check_input_data(actual_res_func, file);

	for (int i = 0; i < 2; i++)
		ck_assert_int_eq(*(expected_result_function + i), *(actual_res_func + i));
}
END_TEST

START_TEST(test_geration_enroll_year)
{
	char file[] = "test/test_input.txt";
	struct student_arr *actual_arr_students;
	struct student_arr *expected_arr_sort_students = create_arr_students(0);

	actual_arr_students = read_from_file(file);

	for (int i = 0; i < 4; i++) {
		ck_assert_int_le((*(actual_arr_students->students + i))->enroll_year, 2022);
		ck_assert_int_ge((*(actual_arr_students->students + i))->enroll_year, 2017);
	}

	for (unsigned int i = 0; i < actual_arr_students->n_students; i++)
		free(*(actual_arr_students->students + i));
	free(actual_arr_students->students);
	free(actual_arr_students);
	free(expected_arr_sort_students->students);
	free(expected_arr_sort_students);
}
END_TEST

//Тести для функції "search_student"

START_TEST(test_expected_2_students_found)
{
	struct student_arr *arr_students = auto_generate_arr_student(2);
	(*(arr_students->students))->enroll_year = 2018;
	(*(arr_students->students + 1))->enroll_year = 2018;
	
	int expcted_n_students = 2;
	
	struct student_arr *actual_arr_search_students = search_student(arr_students, 2018, "dist/output.txt");

	ck_assert_int_eq(expcted_n_students, actual_arr_search_students->n_students);
	
	free(actual_arr_search_students->students);
	free(actual_arr_search_students);
	free_memory(arr_students);
	
}
END_TEST

START_TEST(test_expected_0_students_found)
{
	struct student_arr *arr_students = auto_generate_arr_student(2);
	(*(arr_students->students))->enroll_year = 2017;
	(*(arr_students->students + 1))->enroll_year = 2017;

	int expcted_n_students = 0;

	struct student_arr *actual_arr_sort_students = search_student(arr_students, 2018, "dist/output.txt");

	ck_assert_int_eq(expcted_n_students, actual_arr_sort_students->n_students);
	
	free_memory(arr_students);
	free(actual_arr_sort_students->students);
	free(actual_arr_sort_students);
}
END_TEST

//Тести для функції "sort_student_by_year_decreas"

START_TEST(test_sorts_4_students_year_decreas)
{
	struct student_arr *arr_students = auto_generate_arr_student(4);
	(*(arr_students->students))->enroll_year = 2017;
	(*(arr_students->students + 1))->enroll_year = 2018;
	(*(arr_students->students + 2))->enroll_year = 2019;
	(*(arr_students->students + 3))->enroll_year = 2020;

	int expcted_sequence[4] = {2020,2019,2018,2017};

	sort_student_by_year_decreas(arr_students, "dist/output.txt");
	
	for(unsigned int i = 0; i < 4; i++)
		ck_assert_int_eq(*(expcted_sequence + i), (*(arr_students->students + i))->enroll_year);
	
	free_memory(arr_students);

}
END_TEST

START_TEST(test_sorts_1_students_year_decreas)
{
	struct student_arr *arr_students = auto_generate_arr_student(1);
	(*(arr_students->students))->enroll_year = 2017;
	

	int expcted_sequence[1] = {2017};

	sort_student_by_year_decreas(arr_students, "dist/output.txt");
	
	for(unsigned int i = 0; i < 1; i++)
		ck_assert_int_eq(*(expcted_sequence + i), (*(arr_students->students + i))->enroll_year);
	
	free_memory(arr_students);

}
END_TEST

START_TEST(test_sorts_0_students_year_decreas)
{
	struct student_arr *arr_students = auto_generate_arr_student(0);

	int expcted_sequence = 0;

	sort_student_by_year_decreas(arr_students, "dist/output.txt");
	
	ck_assert_int_eq(expcted_sequence, arr_students->n_students);
	
	free(arr_students->students);
	free(arr_students);

}
END_TEST

START_TEST(test_sorts_4_students_year_increas)
{
	struct student_arr *arr_students = auto_generate_arr_student(4);
	(*(arr_students->students))->enroll_year = 2020;
	(*(arr_students->students + 1))->enroll_year = 2019;
	(*(arr_students->students + 2))->enroll_year = 2018;
	(*(arr_students->students + 3))->enroll_year = 2017;

	int expcted_sequence[4] = {2017,2018,2019,2020};

	sort_student_by_year_increas(arr_students, "dist/output.txt");
	
	for(unsigned int i = 0; i < 4; i++)
		ck_assert_int_eq(*(expcted_sequence + i), (*(arr_students->students + i))->enroll_year);
	
	free_memory(arr_students);

}
END_TEST

START_TEST(test_sorts_1_students_year_increas)
{
	struct student_arr *arr_students = auto_generate_arr_student(1);
	(*(arr_students->students))->enroll_year = 2017;
	
	int expcted_sequence[1] = {2017};

	sort_student_by_year_increas(arr_students, "dist/output.txt");
	
	for(unsigned int i = 0; i < 1; i++)
		ck_assert_int_eq(*(expcted_sequence + i), (*(arr_students->students + i))->enroll_year);
	
	free_memory(arr_students);

}
END_TEST

START_TEST(test_sorts_0_students_year_increas)
{
	struct student_arr *arr_students = auto_generate_arr_student(0);

	int expcted_sequence = 0;

	sort_student_by_year_increas(arr_students, "dist/output.txt");
	
	ck_assert_int_eq(expcted_sequence, arr_students->n_students);
	
	free(arr_students->students);
	free(arr_students);

}
END_TEST

//Тести для функції "sort_student_by_alphabet"

START_TEST(test_sorts_4_students_alphabet)
{
	struct student_arr *arr_students = read_from_file("assets/input.txt");
	
	write_on_screen(arr_students);

	char expcted_sequence[][55] = {"Борисенко Г.В.","Гвардійцев О.О.","Кузьменко А.А.","Назаренко Б.Є."};
	
	printf("Студенти за алфавітом\n");
	
	sort_student_by_alphabet(arr_students, "dist/output.txt");
	
	for(unsigned int i = 0; i < 4; i++)
		ck_assert_str_eq((*(expcted_sequence + i)), (*(arr_students->students + i))->name_student);
	
	free_memory(arr_students);

}
END_TEST

START_TEST(test_sorts_1_students_alphabet)
{
	struct student_arr *arr_students = auto_generate_arr_student(1);
	strcpy((*(arr_students->students))->name_student,"a");
	
	char expcted_sequence[] = "a";

	sort_student_by_alphabet(arr_students, "dist/output.txt");
	
	ck_assert_str_eq(expcted_sequence, (*(arr_students->students))->name_student);
	
	free_memory(arr_students);

}
END_TEST

START_TEST(test_sorts_0_students_alphabet)
{
	struct student_arr *arr_students = auto_generate_arr_student(0);

	int expcted_sequence = 0;

	sort_student_by_year_increas(arr_students, "dist/output.txt");
	
	ck_assert_int_eq(expcted_sequence, arr_students->n_students);
	
	free(arr_students->students);
	free(arr_students);

}
END_TEST


START_TEST(test_delete_one_student) {

  struct student_arr *arr_students = auto_generate_arr_student(5);

  int expected_n_students = 4;

  struct student_arr *actual_arr_students = delete_student(arr_students, 5);

  ck_assert_int_eq(expected_n_students, actual_arr_students->n_students);

  free_memory(actual_arr_students);
}
END_TEST

START_TEST(test_delete_student_n_zero) {
  struct student_arr *arr_students = auto_generate_arr_student(5);

  int expected_n_students = 5;

  struct student_arr *actual_arr_students = delete_student(arr_students, 0);

  ck_assert_int_eq(expected_n_students, actual_arr_students->n_students);

  free_memory(actual_arr_students);
}
END_TEST

START_TEST(test_delete_student_non_existent) {
  struct student_arr *arr_students = auto_generate_arr_student(5);

  int expected_n_students = 5;

  struct student_arr *actual_arr_students = delete_student(arr_students, 6);

  ck_assert_int_eq(expected_n_students, actual_arr_students->n_students);

  free_memory(actual_arr_students);
}
END_TEST


START_TEST(test_add_student) {
  struct student_arr *arr_students = auto_generate_arr_student(5);

  int expected_n_students = 6;

  FILE *file = freopen("test/test_add_student/test_add_student", "r", stdin);
  struct student_arr *actual_arr_students = add_student(arr_students);
  fclose(file);

  ck_assert_int_eq(expected_n_students, actual_arr_students->n_students);

  free_memory(actual_arr_students);
}
END_TEST

Suite *lab_test_suite(void)
{
	Suite *s;
	TCase *check_path_to_files;
	TCase *check_input_data;
	TCase *read_from_file;
	TCase *student_search;
	TCase *student_sortby_year_decreas;
	TCase *student_sortby_year_increas;
	TCase *student_sortby_alphabet;
	TCase *delete_student;
	TCase *add_student;
	

	s = suite_create("lab15");

	check_path_to_files = tcase_create("check_path_to_files");
	check_input_data = tcase_create("check_input_data");
	read_from_file = tcase_create("read_from_file");
	student_search = tcase_create("student_search");
	student_sortby_year_decreas = tcase_create("student_sortby_year_decreas");
	student_sortby_year_increas = tcase_create("student_sortby_year_increas");
	student_sortby_alphabet = tcase_create("student_sortby_alphabet");
	delete_student = tcase_create("delete_student");
	add_student = tcase_create("add_student");

	tcase_add_test(check_path_to_files, test_no_write_arguments);
	tcase_add_test(check_path_to_files, test_wrong_qu_arguments);
	tcase_add_test(check_path_to_files, test_true_arguments);

	suite_add_tcase(s, check_path_to_files);

	tcase_add_test(check_input_data, test_check_n_students);
	tcase_add_test(check_input_data, test_check_form_education);
	tcase_add_test(check_input_data, test_check_name_students);
	tcase_add_test(check_input_data, test_check_name_curator);
	tcase_add_test(check_input_data, test_check_name_faculti);
	tcase_add_test(check_input_data, test_check_group_number);
	tcase_add_test(check_input_data, test_check_name_cafedra);
	tcase_add_test(check_input_data, test_true_write_data);

	suite_add_tcase(s, check_input_data);

	tcase_add_test(read_from_file, test_geration_enroll_year);

	suite_add_tcase(s, read_from_file);

	tcase_add_test(student_search, test_expected_2_students_found);
	tcase_add_test(student_search, test_expected_0_students_found);

	suite_add_tcase(s, student_search);
	
	tcase_add_test(student_sortby_year_decreas, test_sorts_4_students_year_decreas);
	tcase_add_test(student_sortby_year_decreas, test_sorts_1_students_year_decreas);
	tcase_add_test(student_sortby_year_decreas, test_sorts_0_students_year_decreas);

	suite_add_tcase(s, student_sortby_year_decreas);
	
	tcase_add_test(student_sortby_year_increas, test_sorts_4_students_year_increas);
	tcase_add_test(student_sortby_year_increas, test_sorts_1_students_year_increas);
	tcase_add_test(student_sortby_year_increas, test_sorts_0_students_year_increas);

	suite_add_tcase(s, student_sortby_year_increas);
	
	tcase_add_test(student_sortby_alphabet, test_sorts_4_students_alphabet);
	tcase_add_test(student_sortby_alphabet, test_sorts_1_students_alphabet);
	tcase_add_test(student_sortby_alphabet, test_sorts_0_students_alphabet);

	suite_add_tcase(s, student_sortby_alphabet);
	
	tcase_add_test(delete_student, test_delete_one_student);
	tcase_add_test(delete_student, test_delete_student_n_zero);
	tcase_add_test(delete_student, test_delete_student_non_existent);

	suite_add_tcase(s, delete_student);
	
	tcase_add_test(add_student, test_add_student);

	suite_add_tcase(s, add_student);

	return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = lab_test_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

### **Структура проекту лабораторної роботи:**
```   
lab15
├──Doxyfile
├──Makefile
├──README.md
├──test
│   └──test.c
│   └──test_input.txt
├──assets
│   └──input.txt
└──src
    ├──lib.c
    ├──lib.h
    └──main.c
```
### **Скріншоти**

![Program](https://github.com/Azivus/lab16/blob/main/Снимок%20экрана%20(4013).png)
![Program](https://github.com/Azivus/lab16/blob/main/Снимок%20экрана%20(4014).png)
![Program](https://github.com/Azivus/lab16/blob/main/Снимок%20экрана%20(4015).png)
![Program](https://github.com/Azivus/lab16/blob/main/Снимок%20экрана%20(4017).png)
![Program](https://github.com/Azivus/lab16/blob/main/Снимок%20экрана%20(4018).png)


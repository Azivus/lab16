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

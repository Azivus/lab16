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

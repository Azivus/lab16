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

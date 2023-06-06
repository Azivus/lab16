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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "list.h"

/* mpasswdsort.c
 *  Max Malmer, c17mmr@cs.umu.se
 *  Laboration 1 5DV088 HT19
 *  Version 1.0, 2019-09-12
 *  An implementation of a input and sorting program for the UNIX passwd file.
 *  If the input is correct the output is a sorted list of users and UID.
 *  If the input is incorrect a instructional output is given to stderr.
 */

// ===========INTERNAL DATA TYPES============

#define ERROR -1
#define STDIN 1
#define EFILE 2

typedef struct user_info user;

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * new_user() - Creates a new empty user.
 * @new_uid: The UID of the new user.
 * @name: The username of the new user.
 *
 * Returns: A pointer to the new user.
 */
user *new_user(unsigned int new_uid, char* name);

/**
 * set_user_uid() - Sets the UID of a user.
 * @u: The specified user.
 * @new_uid: The new UID of the new user.
 *
 * Returns: The updated user.
 */
user set_user_uid(user *u, unsigned int uid);

/**
 * set_username() - Sets the username of a user.
 * @u: The specified user.
 * @new_uid: The new username of the new user.
 *
 * Returns: The updated user.
 */
user set_username(user *u, char* name);

/**
 * get_user_uid() - Gets the UID of the user specified.
 * @u: The specified user.
 *
 * Returns: The UID of the user specified.
 */
unsigned int get_user_uid(user *u);

/**
 * get_user_uid() - Gets the username of the user specified.
 * @u: The specified user.
 *
 * Returns: The username of the user specified.
 */
char* get_username(user *u);

/**
 * user_free() - Clears the memory of a list filled of users.
 * @l: The list to be murdered.
 *
 * Returns: Nothing.
 */
void user_free(list *l);

/**
 * user_print() - Prints a list of all users in order.
 * @l: The list to be printed.
 *
 * Returns: Nothing.
 */
void users_print(list *l);

/**
 * bubble_sort() - Sorts a list of user by a basic implementation of
 * bubble sort.
 * @l: The list to be sorted.
 *
 * Returns: 1 if the list was sorted. ERROR if the list can't be sorted.
 */
int bubble_sort(list *l);

/**
 * build_user_list() - Builds a list of valid users from STIDIN or a file.
 * @l: The list to be filled.
 * @argv: The input to the program.
 * @file_type: The type of input file stream.
 * Returns: 0 if the users list is built successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int build_user_list(list *l, const char *argv[], int file_type);

/**
 * user_control() - Controls a given string line and creates the user.
 * @l: The list to be filled.
 * @n: The current position in the user list.
 * @user_string: The current read string from STDIN or document.
 * @user_number: The line number of the current user.
 * Returns: 0 if the user is built successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int user_control(list *l, pos_node n, char* user_string, int user_number);

/**
 * file_control() - Controls that a given input file can be opened.
 * @argv: The filename.
 * @argc: The type of input file.
 * Returns: 0 if the file is opened successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int file_control(const char *argv[], int argc);

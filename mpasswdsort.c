#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "mpasswdsort.h"

/* mpasswdsort.c
 *  Max Malmer, c17mmr@cs.umu.se
 *  Laboration 1 5DV088 HT19
 *  Version 1.0, 2019-09-12
 *  An implementation of a input and sorting program for the UNIX passwd file.
 *  If the input is correct the output is a sorted list of users and UID.
 *  If the input is incorrect a instructional output is given to stderr.
 */

// ===========INTERNAL DATA TYPES============

struct user_info {
    unsigned int uid;
    char* uname;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * new_user() - Creates a new empty user.
 * @new_uid: The UID of the new user.
 * @name: The username of the new user.
 *
 * Returns: A pointer to the new user.
 */
user *new_user(unsigned int new_uid, char* name) {
    user *u = malloc(sizeof(struct user_info));
    u->uname = malloc(sizeof(char) * 1023);
    memory_control(u);
    memory_control(u->uname);
    strcpy(u->uname, name);
    u->uid = new_uid;
    return u;
}

/**
 * set_user_uid() - Sets the UID of a user.
 * @u: The specified user.
 * @new_uid: The new UID of the new user.
 *
 * Returns: The updated user.
 */
user set_user_uid(user *u, unsigned int uid) {
    u->uid = uid;
    return *u;
}

/**
 * set_username() - Sets the username of a user.
 * @u: The specified user.
 * @new_uid: The new username of the new user.
 *
 * Returns: The updated user.
 */
user set_username(user *u, char* name) {
    strcpy(u->uname, name);
    return *u;
}

/**
 * get_user_uid() - Gets the UID of the user specified.
 * @u: The specified user.
 *
 * Returns: The UID of the user specified.
 */
unsigned int get_user_uid(user *u) {
    return u->uid;
}

/**
 * get_user_uid() - Gets the username of the user specified.
 * @u: The specified user.
 *
 * Returns: The username of the user specified.
 */
char* get_username(user *u) {
    return u->uname;
}

/**
 * user_free() - Clears the memory of a list filled of users.
 * @l: The list to be murdered.
 *
 * Returns: Nothing.
 */
void user_free(list *l) {
    pos_node current_node = list_first(l);

    while (current_node != list_last(l)) {
        user *current_user = list_inspect(l, current_node);
        free(current_user->uname);
        free(current_user);
        current_node = list_remove_node(l, current_node);
    }
}

/**
 * user_print() - Prints a list of all users in order.
 * @l: The list to be printed.
 *
 * Returns: Nothing.
 */
void users_print(list *l) {
    pos_node current_node = list_first(l);

    while (current_node != list_last(l)) {
        user *current_user = list_inspect(l, current_node);
        printf("%d:%s\n", get_user_uid(current_user),
        get_username(current_user));
        current_node = list_next(l, current_node);
    }
}

/**
 * bubble_sort() - Sorts a list of user by a basic implementation of
 * bubble sort.
 * @l: The list to be sorted.
 *
 * Returns: 1 if the list was sorted. ERROR if the list can't be sorted.
 */
int bubble_sort(list *l) {
    pos_node current_node = list_first(l);

    if (list_is_empty(l)) {
        fprintf(stderr, "We're trying to sort an empty list.\n");
        return ERROR;
    }
    bool swapped= true;

    while (swapped) {
        swapped = false;
        current_node = list_first(l);

        while (list_next(l, current_node) != list_last(l)) {
            user *user_left = list_inspect(l, current_node);
            user *user_right = list_inspect(l, list_next(l, current_node));

            if (get_user_uid(user_left) > get_user_uid(user_right)) {
                list_replace(current_node, user_right);
                list_replace(list_next(l, current_node), user_left);
                swapped = true;
            }
            current_node = list_next(l, current_node);
        }
    }
    return 1;
}

/**
 * build_user_list() - Builds a list of valid users from STIDIN or a file.
 * @l: The list to be filled.
 * @argv: The input to the program.
 * @file_type: The type of input file stream.
 * Returns: 0 if the users list is built successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int build_user_list(list *l, const char *argv[], int file_type) {
    FILE *f;

    if (file_type == STDIN) {
		f = stdin;
	} else {
		f = fopen(argv[1], "r");
	}
    char* user_string = malloc(sizeof(char) * 2048);
    memory_control(user_string);
    int user_number = 1;
    pos_node current_node = list_first(l);
    int is_complete = 0;

    while (fgets(user_string, 2048, f) != NULL) {

        if (strlen(user_string) > 1032) {
            is_complete = ERROR;
            fprintf(stderr, "Line %d: Line in file is over 1032 characters.\n",
            user_number);
            break;
        }

        if (strcmp(user_string, "\n") == 0) {
            is_complete = ERROR;
            fprintf(stderr, "Line %d: Encountered a <BLANKLINE>\n",
             user_number);
            continue;
        }
        is_complete = user_control(l, current_node,
                                    user_string, user_number);
        user_number++;
    }

    if (f != NULL && file_type != STDIN) {
		fclose(f);
	}
    free(user_string);
	return is_complete;
}

/**
 * user_control() - Controls a given string line and creates the user.
 * @l: The list to be filled.
 * @n: The current position in the user list.
 * @user_string: The current read string from STDIN or document.
 * @user_number: The line number of the current user.
 * Returns: 0 if the user is built successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int user_control(list *l, pos_node n, char* user_string, int user_number) {
    char* username = strsep(&user_string, ":");
    char* password = strsep(&user_string, ":");
    char* uid_string = strsep(&user_string, ":");
    char* gid_string = strsep(&user_string, ":");
    strsep(&user_string, ":");
    char* home_url = strsep(&user_string, ":");

    if (user_string == NULL) {
        fprintf(stderr, "Line %d: Invalid format: \"%s\"",
        user_number, username);
        return ERROR;
    }

    if (username[0] == '\0') {
        fprintf(stderr, "The field username has to filled.\n");
        return ERROR;
    }

    if (strlen(username) > 32) {
        fprintf(stderr,
        "The field username can't be longer then 32 characters.\n");
        return ERROR;
    }

    if (password[0] == '\0') {
        fprintf(stderr, "The field password has to filled.\n");
        return ERROR;
    }

    if (uid_string[0] == '\0') {
        fprintf(stderr, "The field UID has to filled.\n");
        return ERROR;
    }

    if (gid_string[0] == '\0') {
        fprintf(stderr, "The field GID has to filled.\n");
        return ERROR;
    }

    if (gid_string[0] == '\0') {
        fprintf(stderr, "The field GID has to filled.\n");
        return ERROR;
    }

    if (home_url[0] == '\0') {
        fprintf(stderr, "The field directory has to filled.\n");
        return ERROR;
    }
    int uid = (int) strtol(uid_string, (char **)NULL, 10);

    if (errno == EINVAL) {
        fprintf(stderr, "UID has to be a number. Got \"%s\"\n", uid_string);
        return ERROR;
    }

    if ((floor(log10(abs(uid))) + 1) < strlen(uid_string)) {
        fprintf(stderr, "There can't be text in UID. Got \"%s\"\n", uid_string);
        return ERROR;
    }

    if (uid < 0) {
        fprintf(stderr, "UID has to be a positive number. Got \"%d\"\n", uid);
        return ERROR;
    }
    int gid = (int) strtol(gid_string, (char **)NULL, 10);

    if (errno == EINVAL) {
        fprintf(stderr, "GID has to be a number. Got \"%s\"\n", gid_string);
        return ERROR;
    }

    if ((floor(log10(abs(gid))) + 1) < strlen(gid_string)) {
        fprintf(stderr, "There can't be text in GID. Got \"%s\"\n", gid_string);
        return ERROR;
    }

    if (gid < 0) {
        fprintf(stderr, "GID has to be a positive number. Got \"%d\"\n", gid);
        return ERROR;
    }

    user *u = new_user(uid, username);
    n = list_insert_node(l, n, u);
    return 0;
}

/**
 * file_control() - Controls that a given input file can be opened.
 * @argv: The filename.
 * @argc: The type of input file.
 * Returns: 0 if the file is opened successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int file_control(const char *argv[], int argc) {
    int file_type;
    FILE *f;

    if (argc == 1) {
        file_type = STDIN;
    } else if (argc == 2) {
        file_type = EFILE;
    } else {
        fprintf(stderr, "Input arguments are incorrect.\n");
        return ERROR;
    }

    if (file_type == EFILE) {
        f = fopen(argv[1], "r");

        if (f == NULL) {
            fprintf(stderr, "Specified file cannot be read.\n");
            return ERROR;
        }
        fclose(f);
    }
    return file_type;
}

/**
 * main() - Runs all the functions in correct order.
 * @argv: The filename.
 * Returns: 0 if the program is run successfully. ERROR if something
 *          is out of the specification of passwd file.
 */
int main(int argc, char const *argv[]) {
    int file_status = file_control(argv, argc);

    if (file_status) {
        list *l = list_empty(1);
        file_status = build_user_list(l, argv, file_status);
        bubble_sort(l);
        users_print(l);
        user_free(l);
        list_free(l);
    } else if (file_status == ERROR) {
        fprintf(stderr, "Value of errno: %d\n", errno);
        return errno;
    }

    if (file_status == ERROR) {
        return EXIT_FAILURE;
    }
    return 0;
}

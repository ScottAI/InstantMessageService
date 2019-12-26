//
// Created by 刘金亮 on 2019-12-26.
//

#ifndef INSTANTMESSAGESERVICE_FRIENDS_H
#define INSTANTMESSAGESERVICE_FRIENDS_H
#include "list.h"


typedef struct friend_info friend_info;
struct friend_info{
    char *name;
    char *information;
};

typedef struct friend_list_info friend_list_info;
struct friend_list_info {
    int timestamp;
};

typedef list friends;
typedef list_iterator fri_iterator;

/* =========================================================================
 *  Structs access
 * =========================================================================*/

#define fri_get_name(friend_info) \
		(friend_info->name)

#define fri_get_information(friend_info) \
		(friend_info->information)

#define fri_get_num_friends(friends) \
		list_num_elems(friends)

#define fri_get_timestamp(friends, friends_timestamp) \
	do{ \
		friend_list_info *aux; \
		aux = list_info(friends); \
		friends_timestamp = aux->timestamp; \
	}while(0)

#define fri_set_timestamp(friends, friends_timestamp) \
	do{ \
		friend_list_info *aux; \
		aux = list_info(friends); \
		aux->timestamp = friends_timestamp; \
	}while(0)


/* =========================================================================
 *  Friends iterator
 * =========================================================================*/
#define fri_get_friends_iterator(list) \
		(fri_iterator*)list_iterator(list)

#define fri_iterator_next(list, iterator) \
		(fri_iterator*)list_iterator_next(list, iterator)

#define fri_get_info(iterator) \
		(friend_info*)list_iterator_info(iterator)


/* =========================================================================
 *  Friend struct API
 * =========================================================================*/

/*
 * Allocates a new friend list
 * Returns a pointer to the list or NULL if fails
 */
friends *fri_new(int max);

/*
 * Frees the friend list
 */
void fri_free(friends *friends);

/*
 * Prints all friends line by line
 */
void fri_print_friend_list(friends *friends);

/*
 * Creates a new friend_node in the list with the provided info
 * Returns 0 or -1 if fails
 */
int fri_add_friend(friends *friends, const char *name, const char *information);

/*
 * Removes and frees the first node that matches the provided "name"
 * Returns 0 or -1 if "name" does not exist in the list
 */
int fri_del_friend(friends *friends, const char *name);

/*
 * Finds the chat whos id is chat_id
 * Returns a pointer to the chat_info of NULL if fails
 */
friend_info *fri_find_friend(friends *friends, const char *name);

#endif //INSTANTMESSAGESERVICE_FRIENDS_H

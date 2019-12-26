//
// Created by 刘金亮 on 2019-12-26.
//

#ifndef INSTANTMESSAGESERVICE_CHAT_MEMBERS_H
#define INSTANTMESSAGESERVICE_CHAT_MEMBERS_H
#include "list.h"
#include "friends.h"


typedef struct member_info member_info;
struct member_info{
    char *name;
    friend_info *info;
};

typedef struct member_list_info member_list_info;
struct member_list_info {
    int timestamp;
};

typedef list chat_members;
typedef list_iterator member_iterator;

/* =========================================================================
 *  Structs access
 * =========================================================================*/

#define member_name(member_info) \
		(member_info->name)

#define member_is_friend(member_info) \
		( (member_info->info != NULL)? (TRUE) : (FALSE) )

#define member_friend_info(member_info) \
		(member_info->info)


#define member_num_members(members) \
		list_num_elems(members)

#define member_get_timestamp(members, member_timestamp) \
	do{ \
		member_list_info *aux; \
		aux = list_info(members); \
		member_timestamp = aux->timestamp; \
	}while(0)

#define member_set_timestamp(members, member_timestamp) \
	do{ \
		member_list_info *aux; \
		aux = list_info(members); \
		aux->timestamp = member_timestamp; \
	}while(0)


/* =========================================================================
 *  Messages iterator
 * =========================================================================*/
#define member_get_members_iterator(list) \
		(member_iterator*)list_iterator(list)

#define member_iterator_next(list, iterator) \
		(member_iterator*)list_iterator_next(list, iterator)

#define member_get_info(iterator) \
		(member_info*)list_iterator_info(iterator)


/* =========================================================================
 *  Friend struct API
 * =========================================================================*/

/*
 * Allocates a new member list
 * Returns a pointer to the list or NULL if fails
 */
friends *members_new(int max);

/*
 * Frees the member list
 */
void members_free(chat_members *members);

/*
 * Prints all members line by line
 */
void members_print_friend_list(chat_members *members);

/*
 * Creates a new chat member in the list with the provided info
 * Returns 0 or -1 if fails
 */
int members_add_member(chat_members *members, const char *name, friend_info *info);

/*
 * Removes and frees the first node that matches the provided "name"
 * Returns 0 or -1 if "name" does not exist in the list
 */
int members_del_members(chat_members *members, const char *name);

/*
 * Finds the chat whos id is chat_id
 * Returns a pointer to the chat_info of NULL if fails
 */
member_info *member_find_member(chat_members *members, const char *name);

#endif //INSTANTMESSAGESERVICE_CHAT_MEMBERS_H

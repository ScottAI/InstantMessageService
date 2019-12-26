//
// Created by 刘金亮 on 2019-12-26.
//

#include "friend_requests.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "debug_def.h"

#ifdef DEBUG
#include "leak_detector_c.h"
#endif


#define sizeofstring(string) \
	(strlen(string) + sizeof(char))

void request_list_info_free(void *info) {
    free(info);
}

void request_free(void *request) {
    free(((request_info*)request)->name);
    free(request);
}

int request_name_comp(const void *request, const void *name) {
    return strcmp(((request_info*)request)->name, (char*)name);
}

int request_comp(const void *request1, const void *request2) {
    return strcmp(((request_info*)request1)->name, ((request_info*)request2)->name);
}


/* =========================================================================
 *  Friend requests struct API
 * =========================================================================*/

/*
 * Allocates a new friend request list
 * Returns a pointer to the list or NULL if fails
 */
friend_requests *req_new(int max) {
    DEBUG_TRACE_PRINT();
    friend_requests *requests_new;
    request_list_info *list_info;

    if ( (list_info = malloc(sizeof(request_list_info))) == NULL ) {
        return NULL;
    }

    list_info->timestamp = 0;

    if ( (requests_new = list_new(list_info, max, request_list_info_free, request_free)) == NULL ) {
        return NULL; // could not allocate list
    }
    requests_new->item_value_comp = request_name_comp;
    requests_new->item_comp = request_comp;

    return requests_new;
}


/*
 * Frees the request list
 */
void req_free(friend_requests *requests) {
    DEBUG_TRACE_PRINT();

    list_free(requests);
}


/*
 * Prints all requests line by line
 */
void req_print_friend_list(friend_requests *requests) {
    DEBUG_TRACE_PRINT();
}


/*
 * Creates a new request in the list with the provided info
 * Returns 0 or -1 if fails
 */
int req_add_request(friend_requests *requests, const char *name, int timestamp) {
    DEBUG_TRACE_PRINT();
    request_info *info;

    if ( (info = malloc(sizeof(request_info))) == NULL ) {
        return -1;
    }

    if ( (info->name = malloc(sizeofstring(name)) ) == NULL ) {
        free(info);
        return -1;
    }

    strcpy(info->name, name);
    info->timestamp = timestamp;

    if ( list_add_item(requests, info) == -1 ) {
        DEBUG_FAILURE_PRINTF("Could not add request to list");
        request_free(info);
        return -1;
    }

    return 0;
}


/*
 * Removes the request from the list
 * Returns 0 or -1 if it does not exist
 */
int req_del_request(friend_requests *requests, const char *name) {
    DEBUG_TRACE_PRINT();
    list_node *node;
    node = list_find_node(requests, name);
    if (node == NULL) {
        DEBUG_FAILURE_PRINTF("Request does not exists in list");
        return -1;
    }

    list_delete_node(requests, node);
    return 0;
}


/*
 * Finds the request especified by "name"
 * Returns a pointer to the request_info or NULL if fails
 */
request_info *req_find_request(friend_requests *requests, const char *name) {
    DEBUG_TRACE_PRINT();

    return (request_info*)list_find_item(requests, name);
}
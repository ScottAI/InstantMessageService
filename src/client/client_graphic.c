//
// Created by 刘金亮 on 2019-12-26.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "ims_client.h"
#include "bool.h"

#include "debug_def.h"

#define MAX_USER_NAME_CHARS 20
#define MAX_USER_PASS_CHARS 20
#define MAX_USER_INFO_CHARS 100
#define MAX_DESCRIPTION_CHARS 100
#define MAX_MESSAGE_CHARS 300

#define SCAN_INPUT_STRING(string, index, max_chars) \
	index = 0; \
	while( (index < max_chars) && ((string[index++] = getchar()) != '\n') ); \
	string[index-1] = '\0'


#define FLUSH_INPUT(ch) \
	while ( ((ch = getchar()) != '\n') && (ch != EOF) )

typedef enum {DEFAULT, EXIT, LOGIN, USER_MAIN, USER_LIST, USER_SEND, USER_RECEIVE} menu_type;


boolean continue_fetching;
pthread_mutex_t continue_fetching_mutex;
boolean continue_graphic;
pthread_mutex_t continue_graphic_mutex;

pthread_t notifications_tid = -1;

void retrieve_user_data(psd_ims_client *client);
void stop_client(int sig);
void *notifications_fetch(void *arg);

int run_notifications_thread(psd_ims_client *client);
int configure_signal_handling();


void menu_header_show(const char *string) {
    write(1,"\E[H\E[2J",7);
    printf("=============================================\n");
    printf("              %s\n", string);
    printf("=============================================\n");
}

void menu_footer_show() {
    printf("\n");
    printf(" Opcion -> ");
}

int get_user_input() {
    char input;
    char aux_char;
    input = getchar();
    FLUSH_INPUT(aux_char);

    return ( (input >= '0') && (input <= '9'))?((int)input - '0'):(-1);
}

void wait_user() {
    char aux_char;
    printf("\n Press ENTER to continue...");
    FLUSH_INPUT(aux_char);
}

void save_state(psd_ims_client *client) {
    printf(" = Saving state =\n");
    printf(" (NOT implemented)\n");
}


/* =========================================================================
 *  User receive Menu
 * =========================================================================*/


int recv_notifications(psd_ims_client *client) {
    printf("\n\n Retrieving notifications...\n");
    if (psd_recv_notifications(client) != 0 ) {
        printf(" Failed to retrieve the notifications\n");
        wait_user();
        return -1;
    }

    printf(" Notifications obtained from server\n");
    wait_user();
    return 0;
}

int recv_pending_messages(psd_ims_client *client) {
    int n_messages;
    char aux_char;
    int chat_id;
    int i;

    printf("\n\n = Chats =\n");
    psd_print_chats(client);

    printf("\n Chat id: ");
    scanf("%d", &chat_id);
    FLUSH_INPUT(aux_char);


    printf("\n\n Retrieving new messages of chat '%d'\n", chat_id);
    if ( (n_messages = psd_recv_messages(client, chat_id)) < 0 ) {
        printf(" Failed to retrieve new messages");
        wait_user();
        return -1;
    }

    if (n_messages == 0 ) {
        printf(" No new messages to add\n");
    }
    else {
        printf(" Added %d new messages\n", n_messages);
    }
    wait_user();
    return 0;
}

int recv_new_chats(psd_ims_client *client) {
    int n_chats;

    printf("\n\n Retrieving new chats...\n");
    if ( (n_chats = psd_recv_chats(client)) < 0 ) {
        printf(" Failed to retrieve new chats");
        wait_user();
        return -1;
    }

    if (n_chats == 0 ) {
        printf(" No new chats to add\n");
    }
    else {
        printf(" Added %d new chats\n", n_chats);
    }
    wait_user();
    return 0;
}

int recv_new_friends(psd_ims_client *client) {
    int n_friends;

    printf("\n\n Retrieving new friends...\n");
    if ( (n_friends = psd_recv_friends(client)) < 0 ) {
        printf(" Failed to retrieve new friends");
        wait_user();
        return -1;
    }

    if (n_friends == 0 ) {
        printf(" No new friends to add\n");
    }
    else {
        printf(" Added %d new friends\n", n_friends);
    }
    wait_user();
    return 0;
}


void screen_menu_recv_show() {
    menu_header_show("PSD IMS - Receive menu");
    printf(" 1. Recibir notificaciones pendientes\n");
    printf(" 2. Recibir mensajes pendientes\n");
    printf(" 3. Recibir nuevos chats\n");
    printf(" 4. Recibir nuevos amigos\n");
    printf("\n 0. Salir\n");
    menu_footer_show();
}

int menu_recv(psd_ims_client *client, menu_type *next_menu_ret) {
    int option = -1;
    menu_type default_next_menu = USER_MAIN;
    menu_type next_menu = default_next_menu;

    do {
        screen_menu_recv_show();
        option = get_user_input() ;
        switch(option) {
            case 0: break;   // salir
            case 1:	// go to listing menu
                recv_notifications(client);
                break;
            case 2: // go to send menu
                recv_pending_messages(client);
                break;
            case 3: // go to receive menu
                recv_new_chats(client);
                break;
            case 4: // go to receive friends
                recv_new_friends(client);
        }
    } while( option > 0 );

    next_menu_ret = DEFAULT;

    return next_menu;
}


/* =========================================================================
 *  User send Menu
 * =========================================================================*/

int create_chat(psd_ims_client *client) {
    char description[MAX_DESCRIPTION_CHARS];
    char member[MAX_USER_NAME_CHARS];
    int i;
    int chat_id;

    printf("\n description: ");
    SCAN_INPUT_STRING(description, i, MAX_DESCRIPTION_CHARS);
    printf("\n member: ");
    SCAN_INPUT_STRING(member, i, MAX_USER_NAME_CHARS);

    if( (chat_id = psd_create_chat(client, description, member)) < 0 ) {
        printf(" Failed to create the chat\n");
        wait_user();
        return -1;
    }

    if( psd_add_member_to_chat(client, member, chat_id) != 0 ) {
        printf(" Failed to create the chat\n");
        wait_user();
        return -1;
    }

    printf(" Chat with %s created\n", member);
    return 0;
}

int add_friend_to_chat(psd_ims_client *client) {
    char member[MAX_USER_NAME_CHARS];
    char aux_char;
    int chat_id;
    int i;

    printf("\n\n = Chats =\n");
    psd_print_chats(client);
    printf("\n chat id: ");
    scanf("%d", &chat_id);
    FLUSH_INPUT(aux_char);

    printf("\n\n = Friends =\n");
    psd_print_friends(client);
    printf("\n member: ");
    SCAN_INPUT_STRING(member, i, MAX_USER_NAME_CHARS);


    if( psd_add_member_to_chat(client, member, chat_id) != 0 ) {
        printf(" Failed to create the chat\n");
        wait_user();
        return -1;
    }
    return 0;
}

int send_message(psd_ims_client *client) {
    char text[MAX_MESSAGE_CHARS];
    char aux_char;
    int chat_id;
    int i;

    printf("\n\n = Chats =\n");
    psd_print_chats(client);

    printf("\n Chat id: ");
    scanf("%d", &chat_id);
    FLUSH_INPUT(aux_char);
    printf("\n Text: ");
    SCAN_INPUT_STRING(text, i, MAX_MESSAGE_CHARS);

    printf("Send message %s\n", text);

    if( psd_send_message(client, chat_id, text, NULL, NULL, NULL) != 0 ) {
        printf(" Failed to send the message\n");
        wait_user();
        return -1;
    }

    printf(" Message sended\n");
    wait_user();

    return 0;
}

int send_friend_request(psd_ims_client *client) {
    char name[MAX_USER_NAME_CHARS];
    char aux_char;

    printf("\n\n User name: ");
    scanf("%s", name);
    FLUSH_INPUT(aux_char);

    if( psd_send_friend_request(client, name) != 0 ) {
        printf(" Failed the request friendship to '%s'\n", name);
        wait_user();
        return -1;
    }

    printf(" Sended a friend request to '%s'\n", name);
    wait_user();

    return 0;
}

int send_request_accept(psd_ims_client *client) {
    char name[MAX_USER_NAME_CHARS];
    char aux_char;

    printf("\n\n = Friend requests =\n");
    psd_print_friend_requests(client);
    printf("\n User name: ");
    scanf("%s", name);
    FLUSH_INPUT(aux_char);

    if( psd_send_request_accept(client, name) != 0 ) {
        printf(" Could not accept the friend request\n");
        wait_user();
        return -1;
    }

    printf(" Friend '%s' accepted\n", name);
    wait_user();
    return 0;
}

int send_request_decline(psd_ims_client *client) {
    char name[MAX_USER_NAME_CHARS];
    char aux_char;

    printf("\n\n = Friend requests =\n");
    psd_print_friend_requests(client);
    printf("\n User name: ");
    scanf("%s", name);
    FLUSH_INPUT(aux_char);

    if( psd_send_request_decline(client, name) != 0 ) {
        printf(" Could not accept the friend request\n");
        wait_user();
        return -1;
    }

    printf(" User '%s' rejected as friend\n", name);
    wait_user();

    return 0;
}


void screen_menu_send_show() {
    menu_header_show("PSD IMS - Send menu");
    printf(" 1. Create chat\n");
    printf(" 2. Add friend to chat\n");
    printf(" 3. Enviar mensaje\n");
    printf(" 4. Enviar peticion de amistad\n");
    printf(" 5. Aceptar peticion de amistad\n");
    printf(" 6. Rechazar peticion de amistad\n");
    printf("\n 0. Salir\n");
    menu_footer_show();
}

int menu_send(psd_ims_client *client, menu_type *next_menu_ret) {
    int option = -1;
    menu_type default_next_menu = USER_MAIN;
    menu_type next_menu = default_next_menu;

    do {
        screen_menu_send_show();
        option = get_user_input() ;
        switch(option) {
            case 0: break;   // salir
            case 1:
                create_chat(client);
                break;
            case 2:
                add_friend_to_chat(client);
                break;
            case 3:	// go to listing menu
                send_message(client);
                break;
            case 4: // go to send menu
                send_friend_request(client);
                break;
            case 5: // go to receive menu
                send_request_accept(client);
                break;
            case 6:
                send_request_decline(client);
                break;
        }
    } while( option > 0 );

    next_menu_ret = DEFAULT;

    return next_menu;
}


/* =========================================================================
 *  User list Menu
 * =========================================================================*/

void list_friends(psd_ims_client *client) {
    printf("\n\n = Friends =\n");
    psd_print_friends(client);
    wait_user();
}

void list_chats(psd_ims_client *client) {
    printf("\n\n = Chats =\n");
    psd_print_chats(client);
    wait_user();
}

void list_chat_members(psd_ims_client *client) {
    int chat_id;
    char aux_char;

    printf("\n\n = Chats =\n");
    psd_print_chats(client);
    printf("\n Select a chat id: ");
    scanf("%d", &chat_id);
    FLUSH_INPUT(aux_char);
    psd_print_chat_members(client, chat_id);
    wait_user();
}

void list_chat_messages(psd_ims_client *client) {
    int chat_id;
    char aux_char;

    printf("\n\n = Chats =\n");
    psd_print_chats(client);
    printf("\n Select a chat id: ");
    scanf("%d", &chat_id);
    FLUSH_INPUT(aux_char);
    psd_print_chat_messages(client, chat_id);
    wait_user();
}

void list_pending_notification(psd_ims_client *client) {
    printf("\n\n = Pending notifications =\n");
    printf(" No notifications...(NOT implemented)\n");
    wait_user();
}

void list_friend_requests(psd_ims_client *client) {
    printf("\n\n = Friend requests =\n");
    psd_print_friend_requests(client);
    wait_user();
}

void screen_menu_list_show() {
    menu_header_show("PSD IMS - List menu");
    printf(" 1. Listar amigos\n");
    printf(" 2. Listar chats\n");
    printf(" 3. Listar miembros de un chat\n");
    printf(" 4. Listar mensajes de un chat\n");
    printf(" 5. Listar notificaciones pendientes\n");
    printf(" 6. Listar peticiones de amistad\n");
    printf("\n 0. Salir\n");
    menu_footer_show();
}

int menu_list(psd_ims_client *client, menu_type *next_menu_ret) {
    int option = -1;
    menu_type default_next_menu = USER_MAIN;
    menu_type next_menu = default_next_menu;

    do {
        screen_menu_list_show();
        option = get_user_input() ;
        switch(option) {
            case 0: break;   // salir
            case 1:
                list_friends(client);
                break;
            case 2:
                list_chats(client);
                break;
            case 3:
                list_chat_members(client);
                break;
            case 4:
                list_chat_messages(client);
                break;
            case 5:
                list_pending_notification(client);
                break;
            case 6:
                list_friend_requests(client);
                break;
        }
    } while( option > 0 );

    next_menu_ret = DEFAULT;

    return next_menu;
}


/* =========================================================================
 *  User Menu
 * =========================================================================*/


void screen_menu_main_show() {
    menu_header_show("PSD IMS - Main user menu");
    printf(" 1. Listar\n");
    printf(" 2. Enviar\n");
    printf(" 3. Recibir\n");
    printf(" 4. logout\n");
    printf("\n 0. Salir de la aplicacion\n");
    menu_footer_show();
}

int menu_user(psd_ims_client *client, menu_type *next_menu_ret) {
    int option = -1;
    menu_type default_next_menu = EXIT;
    menu_type next_menu = default_next_menu;

    do {
        screen_menu_main_show();
        option = get_user_input() ;
        switch(option) {
            case 0: break;   // salir
            case 1:	// go to listing menu
                next_menu = USER_LIST;
                option = 0;
                break;
            case 2: // go to send menu
                next_menu = USER_SEND;
                option = 0;
                break;
            case 3: // go to receive menu
                next_menu = USER_RECEIVE;
                option = 0;
                break;
            case 4: // go to login menu
                save_state(client);
                psd_logout(client);
                next_menu = LOGIN;
                option = 0;
                break;
        }
    } while( option > 0 );

    next_menu_ret = DEFAULT;

    return next_menu;
}


/* =========================================================================
 *  Login Menu
 * =========================================================================*/

int user_register(psd_ims_client *client) {
    char name[MAX_USER_NAME_CHARS];
    char pass[MAX_USER_PASS_CHARS];
    char info[MAX_USER_INFO_CHARS];
    int i = 0;
    char aux_char;

    printf("\n\n User name: ");
    scanf("%s", name);
    FLUSH_INPUT(aux_char);
    printf(" User password: ");
    scanf("%s", pass);
    FLUSH_INPUT(aux_char);
    printf(" Describe yourself: ");
    SCAN_INPUT_STRING(info, i, MAX_USER_INFO_CHARS);

    if ( psd_user_register(client, name, pass, info) != 0 ) {
        printf(" Register failed, maybe the name is already registered or the conection is failing\n");
        wait_user();
        return -1;
    }

    printf(" User '%s' correctly registered\n", name);
    wait_user();

    return 0;
}

int login(psd_ims_client *client) {
    char name[MAX_USER_NAME_CHARS];
    char pass[MAX_USER_PASS_CHARS];
    char aux_char;

    printf("\n\n User name: ");
    scanf("%s", name);
    FLUSH_INPUT(aux_char);
    printf(" User password: ");
    scanf("%s", pass);
    FLUSH_INPUT(aux_char);

    if ( psd_login(client, name, pass) != 0 ) {
        printf(" Login failed, maybe the credentials are incorrect or the conection is failing\n");
        wait_user();
        return -1;
    }

    printf(" Logged in as '%s'\n", name);
    printf(" Retrieving user data...");
    retrieve_user_data(client);

    printf(" Done\n");
    wait_user();

    return 0;
}


void screen_login_show() {
    menu_header_show("PSD IMS - Login");
    printf(" 1. Alta\n");
    printf(" 2. Login\n");
    printf("\n 0. Salir\n");
    menu_footer_show();
}

int menu_login(psd_ims_client *client, menu_type *next_menu_ret) {
    int option = -1;
    menu_type default_next_menu = EXIT;
    menu_type next_menu = default_next_menu;

    do {
        screen_login_show();
        option = get_user_input() ;
        switch(option) {
            case 0: break;   // salir
            case 1:
                user_register(client);
                break;
            case 2:
                if( login(client) == 0) {
                    next_menu = USER_MAIN;
                    option = 0;
                    configure_signal_handling();
                    //run_notifications_thread(client);
                }
                break;
        }
    } while( option > 0 );

    next_menu_ret = DEFAULT;
    return next_menu;
}


/* =========================================================================
 *  Main function
 * =========================================================================*/


int graphic_client_run(psd_ims_client *client) {
    menu_type ret_menu = DEFAULT;
    menu_type next_menu = DEFAULT;
    boolean exit_graphic;

    //next_menu = (client != NULL)? USER_MAIN: LOGIN;
    next_menu = LOGIN;

    pthread_mutex_init(&continue_graphic_mutex, NULL);
    continue_graphic = TRUE;

    do {
        switch(next_menu) {
            case EXIT: break;
            case LOGIN:        next_menu = menu_login(client, &ret_menu); break;
            case USER_MAIN:    next_menu = menu_user(client, &ret_menu); break;
            case USER_LIST:    next_menu = menu_list(client, &ret_menu); break;
            case USER_SEND:    next_menu = menu_send(client, &ret_menu); break;
            case USER_RECEIVE: next_menu = menu_recv(client, &ret_menu); break;
        }
        pthread_mutex_lock(&continue_graphic_mutex);
        exit_graphic = (!continue_graphic) || (next_menu == EXIT);
        pthread_mutex_unlock(&continue_graphic_mutex);

    } while(!exit_graphic);
}


void retrieve_user_data(psd_ims_client *client) {
    if ( psd_recv_friends(client) < 0 ) {
        printf(" Failed to retrieve new friends");
        wait_user();
        return;
    }

    if ( psd_recv_chats(client) < 0 ) {
        printf(" Failed to retrieve new chats");
        wait_user();
        return;
    }

    if ( psd_recv_all_messages(client) < 0 ) {
        printf(" Failed to retrieve the chats messages");
        wait_user();
        return;
    }
}


int configure_signal_handling() {
    if (signal(SIGINT, stop_client) == SIG_ERR) {
        DEBUG_FAILURE_PRINTF("Could not attach SIGINT handler");
        return -1;
    }
    if (signal(SIGTERM, stop_client) == SIG_ERR) {
        DEBUG_FAILURE_PRINTF("Could not attach SIGTERM handler");
        return -1;
    }
    if (signal(SIGABRT, stop_client) == SIG_ERR) {
        DEBUG_FAILURE_PRINTF("Could not attach SIGABRT handler");
        return -1;
    }
}

int run_notifications_thread(psd_ims_client *client) {
    // start the thread to get notifications
    if (pthread_create(&notifications_tid, NULL, &notifications_fetch, client) != 0 ) {
        printf("Could not create the notifications thread\n");
        return 0;
    }
}

void *notifications_fetch(void *arg) {
    psd_ims_client *client;
    sigset_t sig_blocked_mask;
    sigset_t old_sig_mask;

    client = (psd_ims_client *)arg;

    continue_fetching = TRUE;
    pthread_mutex_init(&continue_fetching_mutex, NULL);

    while(1) {
        sleep(1);
        psd_recv_notifications(client);

        pthread_mutex_lock(&continue_fetching_mutex);
        if( !continue_fetching ) {
            pthread_mutex_unlock(&continue_fetching_mutex);
            DEBUG_INFO_PRINTF("Ending notifications service...");
            break;
        }
        pthread_mutex_unlock(&continue_fetching_mutex);
    }
}


void stop_client(int sig) {
    DEBUG_INFO_PRINTF("Freeing client resources");

    pthread_mutex_lock(&continue_fetching_mutex);
    continue_fetching = FALSE;
    pthread_mutex_unlock(&continue_fetching_mutex);

    // wait for the thread to end
    if( notifications_tid != -1) {
        pthread_join(notifications_tid, NULL);
    }

    // Now the notification service is not running

    // Save the state
    // To save the state, I should be sure that the system is stable
    // but the process could be accesing a structure when the signal was thrown...
    pthread_mutex_lock(&continue_graphic_mutex);
    continue_graphic = FALSE;
    pthread_mutex_unlock(&continue_graphic_mutex);
    fclose(stdin);

}

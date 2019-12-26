//
// Created by 刘金亮 on 2019-12-26.
//

#ifndef INSTANTMESSAGESERVICE_IMS_H
#define INSTANTMESSAGESERVICE_IMS_H
#import "soap12.h"
#import "xop.h"
#import "xmime5.h"
typedef struct ims__file{
    unsigned char* __ptr;
    int __size;
}ims__file;

typedef struct ims__string{
    char* string;
}ims__string;

typedef struct ims__notif_friend_info{
    ims__string name;
    int send_date;
}ims__notif_friend_info;

typedef struct ims__notif_friend_list{
    int __sizenelems;
    ims__notif_friend_info* user;
}ims_notif_friend_list;

typedef struct ims__notif_chat_info{
    int chat_id;
    int timestamp;
}ims_notif_chat_info;

typedef struct ims__notif_chat_list{
    int __sizenelems;
    ims__notif_chat_info* chat;
}ims__notif_chat_list;

typedef struct ims__notif_member_info{
    ims__string name;
    int chat_id;
    int timestamp;
}ims__notif_member_info;

typedef struct ims__notif_chat_member_list{
    int __sizenelems;
    ims__notif_member_info* member;
}ims__notif_chat_member_list;

typedef struct ims__sync{
    ims__notif_chat_list chat_read_timestamp;
}ims__sync;

typedef struct ims__new_chat{
    char* description;
    char* member;
}ims__new_chat;

typedef struct ims__login_info{
    char* name;
    char* password;
}ims__login_info;

typedef struct ims__register_info{
    char* name;
    char* password;
    char* information;
}ims__register_info;

//Users
typedef struct ims__user_info{
    char* name;
    char* information;
}ims__user_info;

typedef struct ims__user_list{
    int __sizenelems;
    ims__user_info* user;
    int last_timestamp;
}ims__user_list;

//Messages
typedef struct ims__message_info{
    char* user;
    char* text;
    char* file_name;
    int send_date;
}ims__message_info;

typedef struct ims__message_list{
    int __sizenelems;
    ims__message_info* messages;
    int read_timestamp;
    int last_timestamp;
}ims__message_list;

//chats and chats members
typedef struct ims__member_list{
    int __sizenelems;
    ims__string* name;
    int last_timestamp;
}ims__member_list;

typedef struct ims__chat_info{
    int chat_id;
    char* description;
    char* admin;
    int read_timestamp;
    int all_read_timestamp;
    ims__member_list members;
}ims__chat_info;

typedef struct ims__chat_list{
    int __sizenelems;
    ims__chat_info* chat_info;
    int last_timestamp;
}ims__chat_list;

typedef struct ims__notifications{
    ims__notif_friend_list friend_request;
    ims__user_list new_friends;
    ims__notif_chat_list chats_with_messages;
    ims__notif_chat_list chats_read_times;
    ims__notif_chat_member_list chat_members;
    ims__notif_chat_member_list rem_chat_members;
    ims__notif_chat_member_list chat_admins;
    int last_timestamp;
}ims__notifications;

typedef struct ims__client_data{
    ims__chat_list chats;
    ims__user_list friends;
    ims__notif_friend_list friend_requests;
    int timestamp;
}ims__client_data;

//*******************
//  方法
//*******************

//register user
int ims__user_register(ims__register_info* user_info, int* ERRCODE);

//用户注销
int ims__user_unregister(ims__login_info* login,int* ERRCODE);

//获取用户信息
int ims__get_user(ims__login_info* login,ims__user_info* user);

//获取好友列表
int ims__get_friends(ims__login_info* login,int timestamp,ims__user_list* friends);

//获取好友信息
int ims__get_friend_info(ims__login_info* login,char* name,ims__user_info* friend_info);

//获取聊天列表
int ims__get_chats(ims__login_info* login,int timestamp,ims__chat_list* chats);

//获取聊天信息
int ims__get_chat_info(ims__login_info* login,int chat_id,ims__chat_info* chat);

//获取聊天的具体信息
int ims__get_chat_messages(ims__login_info* login,int chat_id,int timestamp,ims__message_list* messages);

//获取信息的附件
int ims__get_attachment(ims__login_info* login,int chat_id,int msg_timestamp,ims__file* file);

//获取暂定信息
int ims__get_pending_notifications(ims__login_info* login,int timestamp,ims__sync* sync,ims__notifications* notifications);

//获取所有数据
int ims__get_all_data(ims__login_info* login,ims__client_data* client_data);

//创建新聊天
int ims__create_chat(ims__login_info* login,ims__new_chat* new_chat,int* chat_id);

//在聊天中添加新人员
int ims__add_member(ims__login_info* login,char* name,int chat_id,int* ERRCODE);

//从聊天中删除人员
int ims__remove_member(ims__login_info* login,char* name,int chat_id,int* ERRCODE);

//从聊天中离开
int ims__quit_from_chat(ims__login_info* login,int chat_id,int* ERRCODE);

//发送信息
int ims__send_message(ims__login_info* login,int chat_id,ims__message_info* message,int* timestamp);

//发送文件
int ims__send_attachment(ims__login_info* login,int chat_id,int msg_timestamp,ims__file* file,int* ERRCODE);

//请求权限
int ims__send_friend_request(ims__login_info* login,char* request_name,int* timestamp);

//接收请求
int ims__accept_request(ims__login_info* login,char* request_name,int* timestamp);

//
int ims__decline_request(ims__login_info* login,char* request_name,int* timestamp);

#endif //INSTANTMESSAGESERVICE_IMS_H

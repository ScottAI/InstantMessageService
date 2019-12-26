//
// Created by 刘金亮 on 2019-12-26.
//

#ifndef INSTANTMESSAGESERVICE_IMS_SERVER_H
#define INSTANTMESSAGESERVICE_IMS_SERVER_H

#include "soapH.h"
#include "persistence.h"
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_CHARS (10485760)


int init_server(int bind_port, char persistence_user[], char persistence_pass[]);

void free_server();

int listen_connection();

#endif //INSTANTMESSAGESERVICE_IMS_SERVER_H

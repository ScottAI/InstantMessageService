//
// Created by 刘金亮 on 2019-12-26.
//

#define N_ERRORS (3)
#define MAX_ERROR_MSG_CHARS (50)

enum {
    ERR_SRV_LOGIN = 0,
    ERR_SRV_EMPTY_FILEDS,
    ERR_SRV_BAD_FIELDS
}error_type;

char error_msgs[N_ERRORS][MAX_ERROR_MSG_CHARS] = {
        "Login incorrect",
        "There are empty mandatory fields",
        "Some files have incorrect values"
};

#define printerror(err_code)\
    printf(" %s",error_msgs[err_code])

#ifndef INSTANTMESSAGESERVICE_ERRORS_H
#define INSTANTMESSAGESERVICE_ERRORS_H

#endif //INSTANTMESSAGESERVICE_ERRORS_H

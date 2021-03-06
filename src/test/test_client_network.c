//
// Created by 刘金亮 on 2019-12-26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "ims_client.h"

#include "leak_detector_c.h"


int _login(network *network) {

    char name[50];
    char pass[50];
    char info[50];

    printf(" User NAME => ");
    scanf("%s", name);
    printf(" User PASS => ");
    scanf("%s", pass);

    return (net_login(network, name, pass) != NULL)? 1 : 0;
}


int _user_register(network *network) {
    char name[50];
    char pass[50];
    char info[50];

    printf(" User NAME => ");
    scanf("%s", name);
    printf(" User PASS => ");
    scanf("%s", pass);
    printf(" User INFO => ");
    scanf("%s", info);

    return net_user_register(network, name, pass, info);
}



int main( int argc, char **argv ) {

    int operation = 0;
    int return_value;
    network *network;


    atexit(report_mem_leak);




    if (argc < 3) {
        printf("Usage: %s http://server:port <operation>\n", argv[0]);
        exit(-1);
    }
    operation = atoi(argv[2]);

    if( (network = net_new(argv[1])) == NULL) {
        printf("Coult not create network estructure\n");
        return 0;
    }

    switch (operation) {
        case 0:
            if( _login(network) != 0 ) {
                printf("Error at login\n");
            }
            break;
        case 1:
            if( _user_register(network) != 0 ){
                printf("Error at register\n");
            }
            break;
            /*
                case 2:
                soap_call_psdims__friend_request(&soap, serverURL, "", name, passwd, dummyname, &result);
                break;
            case 3:
                //soap_call_psdims__get_request(&soap, serverURL, "", name, passwd, dummyname, NULL);
                break;
            case 4:
                soap_call_psdims__accept_request(&soap, serverURL, "", name, passwd, dummyname, &result);
                break;
            case 5:
                soap_call_psdims__decline_request(&soap, serverURL, "", name, passwd, dummyname, &result);
                break;
            */
    }

    //printf("Result is = %d\n", result);

    net_free(network);

    return 0;
}


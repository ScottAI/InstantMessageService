//
// Created by 刘金亮 on 2019-12-26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ims_client.h"
#include "client_graphic_v2.h"
#include "bool.h"

#ifdef DEBUG
#include "leak_detector_c.h"
#endif


int main( int argc, char **argv ) {

    if( argc < 2 ) {
        printf("Usage: %s <url>:<port>\n", argv[0]);
        return 0;
    }

    // Initialize client
    psd_ims_client *client = psd_new_client();
    psd_bind_network(client, argv[1]);

    // TODO If posible, load info from local files


    // Run graphic client
    graphic_client_run(client);


    // free client
    psd_free_client(client);

    return 0;
}

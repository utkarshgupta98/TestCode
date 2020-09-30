#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function to check and call the transformation required */
int check_transform(char type[], int route_id, char * transport_key, char * transport_value, char * SENDER) {
    if ((!strcmp(type, "Json_transform")) && (!strcmp(transport_value, "HTTP"))) {
        printf("\nApplying HTTP_Json transformation\n");
        HTTP_Json_transform(route_id, transport_key, SENDER);
        return 1;
    } else if ((!strcmp(type, "Json_transform")) && (!strcmp(transport_value, "email"))) {
        printf("\nApplying Email_Json transformation\n");
        get_emailID(route_id, transport_key);
        return 1;
    } else if ((!strcmp(type, "Json_transform")) && (!strcmp(transport_value, "SFTP"))) {
        printf("\nApplying SFTP_Json transformation\n");
        get_sftpserver(route_id, transport_key);
        return 1;
    } else {
        printf("\nNo transformation needed\n");
        return 0;
    }

}

/* Transformation for having transport request as HTTP
 * And File format to be json
  */
void HTTP_Json_transform(int route_id, char * transport_key, char * SENDER) {
    char temp[100];
    char * filename;
    strcpy(filename, SENDER);
    strcat(filename, "Payload.json");
    FILE * fp = fopen(filename, "r");
    fp = fopen(filename, "r");

    if (fp == NULL)
        return;

    int i = 0;
    char c = fgetc(fp);
    while (c != EOF) {
        temp[i++] = c;
        c = fgetc(fp);
    }
    temp[i] = '\0';

    char payload[strlen(temp)];
    int k = 0;
    for (int i = 0; i < strlen(temp); i++) {
        if (temp[i] == ':') {
            for (int j = i + 2; j < strlen(temp); j++) {
                if (temp[j] == '"')
                    break;
                payload[k++] = temp[j];
            }
            break;
        }
    }

    printf("\nPayload:\t%s\n", payload);

    add_payload(payload, route_id, transport_key);

}

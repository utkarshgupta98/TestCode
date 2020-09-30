#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Apply transport service */
int Apply_transport_service(char URL[], char transport_service[], char SENDER[]) {
    int status = 1;
    if (!strcmp(transport_service, "HTTP")) {
        printf("\nHTTP service applied\n");
        send_http_request(URL);
        return status;
    } else if (!strcmp(transport_service, "email")) {
        printf("\nemail service applied\n");
        char filename[200];
    	strcpy(filename, SENDER);
    	strcat(filename, "Payload.json");
       
        int mail_status = send_mail(URL, filename);
        if (mail_status == 0) {
            printf("Mail sent\n");
            return status;
        } else {
            printf("Failed to send the Mail\n");
            status = 0;
        }
    } else if (!strcmp(transport_service, "SFTP")) {
        printf("\nSFTP service applied\n");
        char filename[200];
    	strcpy(filename, SENDER);
    	strcat(filename, "Payload.json");
        
        status = sftp_upload(URL, filename);
        return status;
    } else {
        status = 0;
        printf("\nNo service found\n");
    }
    return status;

}

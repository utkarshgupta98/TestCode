#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../bmd/bmd_parser.h"
#include "database.h"

int insert_in_esb_request(BMD * bmd) {
    int success = 1; //1=>success , -ve for errors

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;

    int8_t received_temp[100]; // eg: "2020-08-12 05:18:00+0000";
    int8_t received_on[100];
    strcpy(received_temp, bmd -> bmd_envelope -> CreationDateTime);

    /*Changing DateTime format*/
    int n = strlen(received_temp);
    int j = 0;
    for (int i = 0; i <= n; i++) {
        received_on[j] = received_temp[i];
        if (received_on[i] == 'T') //test for character
        {
            received_on[i] = ' '; // change T to space
        }
        if (i == n - 3) {
            char ch = ':';
            strncat(received_on, & ch, 1);
            j++;
        }
        j++;
    }

    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
        success = -1;
        return success;
    }

    /*sql query to insert in table*/
    char * status = "available";
    char query1[5000];
    char query2[5000];
    int check = 1; //checking for duplicacy

    sprintf(query1, INSERT_IN_ESB_REQUEST,
        bmd -> bmd_envelope -> Sender,
        bmd -> bmd_envelope -> Destination,
        bmd -> bmd_envelope -> MessageType,
        bmd -> bmd_envelope -> ReferenceID,
        bmd -> bmd_envelope -> MessageID,
        received_on, status);

    printf("\n\n%s\n\n", query1);

    if (mysql_query(conn, query1)) {
        printf("Failed to execute query.Error: %s\n", mysql_error(conn));
        success = -1;
        return success;
    }

    res = mysql_use_result(conn);

    /*free the result*/

    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
    return success;
}

/* Returns route_id if an active route exists for a given sender , destination and message type */
int select_active_route(const unsigned char * Sender,
    const unsigned char * Destination,
        const unsigned char * MessageType) {
    int route;

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /* Execute SQL query to fetch all table names.*/
    sprintf(query, SELECT_ACTIVE_ROUTE, Sender, Destination, MessageType);

    if (mysql_query(conn, query)) {
        printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    route = atoi(row[0]);

    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
    return route;
}

int select_transport_config(int route_id) {
    int success = 0;

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /* Execute SQL query to fetch all table names.*/
    sprintf(query, SELECT_TRANSPORT_CONFIG, route_id);
    printf("%s", query);
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    int retval = mysql_num_rows(res);
    if (retval > 0) {
        success = 1;
    } else {
        success = -1;
    }

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
    return success;
}

int select_transform_config(int route_id) {
    int success = 0;

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server,
            user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    sprintf(query, SELECT_TRANSFORM_CONFIG, route_id);
    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    int retval = mysql_num_rows(res);
    if (retval > 0) {
        success = 1;
    } else {
        success = -1;
    }

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
    return success;
}

int check_new_request(int id) {
    int success = 0;

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    sprintf(query, CHECK_NEW_REQUEST, id);
    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    int retval = mysql_num_rows(res);
    if (retval > 0) {
        success = 1;
    } else {
        success = -1;
    }

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
    return success;
}

int get_route_id(char SENDER[], char DEST[], char MTYPE[]) {

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    sprintf(query, GET_routeid, SENDER, DEST, MTYPE);
    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    int route_id = atoi(row[0]);

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
    return route_id;
}

void get_transform_key(int route_id, char * key) {

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /*Get transform config_value*/
    sprintf(query, GET_transformkey, route_id);

    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    strcpy(key, strdup(row[0]));

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
    
}

void add_payload(char Payload_value[], int route_id, char * transport_key) {
    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /* Get transform config_key */
    sprintf(query, GET_transportkey, route_id);

    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    strcpy(transport_key, row[0]);

    for (int i = 0; i < strlen(Payload_value); i++) {
        char ch = Payload_value[i];
        strncat(transport_key, & ch, 1);
    }

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);

}

void get_emailID(int route_id, char * transport_key) {
    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /* Get transform config_key */
    sprintf(query, GET_transportkey, route_id);

    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    strcpy(transport_key, row[0]);

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);

}

void get_sftpserver(int route_id, char * transport_key) {
    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /* Get transform config_key */
    sprintf(query, GET_transportkey, route_id);

    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    strcpy(transport_key, row[0]);

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);

}

void get_transport_value(int route_id, char * transport_value) {

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    /*Get transform config_value*/
    sprintf(query, GET_transportvalue, route_id);

    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    strcpy(transport_value, strdup(row[0]));

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);

}

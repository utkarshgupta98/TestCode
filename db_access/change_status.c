#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"

void change_available_to_taken(int id) {

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    sprintf(query, AVAILABLE_TO_TAKEN, id);

    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
}

void change_taken_to_done(int id) {

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    sprintf(query, TAKEN_TO_DONE, id);
    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
}

void change_status_to_error(int id) {

    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char query[5000];
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    sprintf(query, STATUS_TO_ERROR, id);
    /* Execute SQL query.*/
    if (mysql_query(conn, query)) {
        printf("Failed to execute query. Error: %s\n", mysql_error(conn));
    }

    res = mysql_store_result(conn);

    /* free results */
    mysql_free_result(res);
    
    /*Close the connection */
    mysql_close(conn);
}

void get_status(char *message_id, char* status)
{
    	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[5000];
	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		return NULL;
	}

	/*Get transform config_value*/
	sprintf(query,GET_STATUS,message_id);
	/* Execute SQL query.*/
	if (mysql_query(conn, query))
	{
		return NULL;
	}

	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	
	strcpy(status,row[0]);

	/* free results */
	mysql_free_result(res);
	
	/*Close the connection */
    	mysql_close(conn);

}

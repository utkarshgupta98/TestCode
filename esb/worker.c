#include <stdio.h>
#include <unistd.h>
#include "esb.h"
#include "../bmd/bmd_parser.h"
#include <mysql.h>
#include "../db_access/database.h"
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

/* Create a lock */
pthread_mutex_t lock;

/* Check for new requests with status available */
int fetch_new_request_from_db(int id) {
    printf("Checking for new requests in esb_requests table.\n");
    if (check_new_request(id) >= 1) {
        printf("\nFound a new request\n");
        return 1; // 1 => OK, -1 => Errors
    } else {
        printf("\nno new requests found\n");
        return 0;
    }
}

/* Poll the database */
void * poll_database_for_new_requets(void * vargp) {
    // Step 1: Open a DB connection
    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);

    /* Connect to database */
    if (!mysql_real_connect(conn, server,
            user, password, database, 0, NULL, 0)) {
        printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
    }

    pthread_mutex_lock( & lock); //lock this section of code to avoid race conditon

    /* Query to see rows with status = available */
    while (!mysql_query(conn, "SELECT id, sender_id, dest_id, message_type FROM esb_request WHERE status = 'available' ORDER BY id LIMIT 1")) {

        /**
         * Step 2: Query the esb_requests table to see if there
         * are any newly received BMD requets.
         */

        int ID;
        char SENDER[50];
        char DEST[50];
        char MTYPE[50];

        res = mysql_store_result(conn);
        int columns = mysql_num_fields(res);

        printf("Entries in the table esb_request with status = available :\n");

        /* Store the values of esb_request
         * for {SENDER, DESTINATION, MESSAGE_TYPE}
         * and look for records in routes
         */
        while ((row = mysql_fetch_row(res))) {
            for (int i = 0; i < 4; i++) {
                printf("%s ", row[i] ? row[i] : "NULL");
                switch (i) {
                case 0:
                    ID = atoi(row[i]);
                    break;

                case 1:
                    strcpy(SENDER, row[i]);
                    break;

                case 2:
                    strcpy(DEST, row[i]);
                    break;

                case 3:
                    strcpy(MTYPE, row[i]);
                    break;
                }
            }
            printf("\n");
        }

        mysql_free_result(res);

        BMD * req;
        /**
         * Step 3: Fetch the new requests from esb_request table
         */
        if (fetch_new_request_from_db(ID)) {

            printf("\nApplying transformation and transporting steps.\n");
            /*Step 1: Change status from available to taken*/

            change_available_to_taken(ID);

            /*Step 2: Transformation steps: */

            /*Find the route_id for which transformation is to applied*/
            int route_id = get_route_id(SENDER, DEST, MTYPE);
            if (route_id == NULL) {
                change_status_to_error(ID);
                return;
            }

            /*Get the config_key to check for transformation*/
            char transform_key[50];
            get_transform_key(route_id, transform_key);

            if (transform_key == NULL) {
                change_status_to_error(ID);
                return;
            }

            printf("\ntransform key: %s\n", transform_key);

            /* Check if transformation is required */

            /* Get for transport service value */
            char transport_value[50];
            get_transport_value(route_id, transport_value);

            if (transport_value == NULL) {
                change_status_to_error(ID);
                return;
            }

            printf("\ntransport value: %s\n", transport_value);

            /* Get for transport service key */
            char transport_key[50];
            int transform_status = check_transform(transform_key, route_id, transport_key, transport_value, SENDER);

            if (transport_key == NULL || transform_status == 0) {
                change_status_to_error(ID);
                return;
            }

            printf("\ntransport key: %s\n", transport_key);

            /* Step 3: Transportation steps: */

            /* Check & Apply the transport service */
            int transport_status = Apply_transport_service(transport_key, transport_value,SENDER);
            if (transport_status == 0) {
                change_status_to_error(ID);
                return;
            }

            /* Step 4: Check and update the status */

            /*Change status taken to done*/

            change_taken_to_done(ID);

        } else {
            pthread_mutex_unlock( & lock);
            break;
        }
        /* Sleep for polling interval duration, say, 5 second.*/

        printf("Sleeping for 5 seconds.\n");
        sleep(5);
    }
    pthread_mutex_unlock( & lock); //unlock so that other threads can now access this section of code
}

/* Create threads for polling database */
pthread_t t1, t2, t3;

void call_threads() {

    // Start threads for task polling
    pthread_create( & t1, NULL, poll_database_for_new_requets, NULL);
    pthread_create( & t2, NULL, poll_database_for_new_requets, NULL);
    pthread_create( & t3, NULL, poll_database_for_new_requets, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_mutex_destroy( & lock);

}

void cancel_threads() {
    pthread_cancel(t1);
    pthread_cancel(t2);
    pthread_cancel(t3);
}

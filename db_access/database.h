#ifndef DATABASE 
#define DATABASE 

/* Inserting bmd values into database */
#define INSERT_IN_ESB_REQUEST  "INSERT INTO esb_request(sender_id,dest_id,message_type,reference_id,message_id,received_on,status) VALUES ('%s','%s','%s','%s','%s','%s','%s')"

/* Validation Queries: */
#define SELECT_ACTIVE_ROUTE "SELECT route_id FROM routes WHERE sender = '%s' AND destination = '%s' AND message_type= '%s'  AND is_active=b'1'"
#define SELECT_TRANSPORT_CONFIG "SELECT * FROM transport_config WHERE route_id='%d'"
#define SELECT_TRANSFORM_CONFIG "SELECT * FROM transform_config WHERE route_id='%d'"

/* Check new request for worker threads */
#define CHECK_NEW_REQUEST "SELECT * FROM esb_request WHERE status = 'available' AND id='%d' LIMIT 1" 

/* Status change Queries */
#define AVAILABLE_TO_TAKEN "UPDATE esb_request SET status = 'taken'  WHERE id='%d'"  
#define TAKEN_TO_DONE "UPDATE esb_request SET status = 'done'  WHERE id='%d'"
#define STATUS_TO_ERROR "UPDATE esb_request SET status = 'error'  WHERE id='%d'"

/* Queries for worker thread transformation and transportation */
#define GET_routeid "SELECT route_id FROM routes WHERE sender = '%s' AND destination = '%s' AND message_type= '%s'"
#define GET_transformkey "SELECT config_key FROM transform_config WHERE route_id='%d'"
#define GET_transportkey "SELECT config_key FROM transport_config WHERE route_id='%d'"
#define GET_transportvalue "SELECT config_value FROM transport_config WHERE route_id='%d'"

/* REST API Query */
#define GET_STATUS "SELECT status from esb_request WHERE message_id='%s'"

#define server "localhost"
#define user "splunk_user"
#define password "password"
#define database "esb_db"
#define port 3306


//int insert_in_esb_request(BMD * bmd);
int select_active_route(const unsigned char * Sender, const unsigned char * Destination, const unsigned char * MessageType);
int select_transport_config(int route_id);
int select_transform_config(int route_id);

#endif

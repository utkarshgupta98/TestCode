#include <kore/kore.h>
#include <kore/http.h>
#include "db_access/database.h"

typedef struct
{
    int status;
    char *message_id;
} rest_result;

int esb_rest(struct http_request *);
rest_result get_messageID(struct http_request *);

//Receives a request
int esb_rest(struct http_request *req)
{
    printf("Received the REST request.\n");
    rest_result get_messageid = get_messageID(req);

    if (get_messageid.status > 0)
    {
        return KORE_RESULT_OK;
    }

    else
    {
        return KORE_RESULT_ERROR;
    }

    return KORE_RESULT_OK;
}
rest_result get_messageID(struct http_request *req)
{
    rest_result rs_res;
    rs_res.status = 1; //1-->OK , -ve for errors
    http_populate_get(req);
    if (req->method != HTTP_METHOD_GET)
    {
        rs_res.status = -1;
        kore_log(LOG_ERR, "Rejecting non GET request.");
        http_response(req, 405, "Wrong Method\n", 13);
        return rs_res;
    }

    /* HTTP_response to get status for message_id */
    char *out;
    if (http_argument_get_string(req, "MessageID", &out))
    {
        rs_res.message_id = malloc(strlen(out) * sizeof(char) + 1);
        strcpy(rs_res.message_id, out);
        char status[20];
        get_status(out, status);
        http_response(req, 200, status, strlen(status));
    }
    else
    {
        rs_res.status = -1;
    }

    return rs_res;
}

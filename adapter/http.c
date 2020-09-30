#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "http.h"

void init_string(struct string * s) {
    s -> len = 0;
    s -> ptr = malloc(s -> len + 1);
    if (s -> ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s -> ptr[0] = '\0';
}

size_t writefunc(void * ptr, size_t size, size_t nmemb, struct string * s) {
    size_t new_len = s -> len + size * nmemb;
    s -> ptr = realloc(s -> ptr, new_len + 1);
    if (s -> ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s -> ptr + s -> len, ptr, size * nmemb);
    s -> ptr[new_len] = '\0';
    s -> len = new_len;

    return size * nmemb;
}

int send_http_request(char * URL) // eg. URL="https://ifsc.razorpay.com/HDFC0CAGSBK"
{
    CURL * curl;
    CURLcode res;
    int status = 0;
    curl = curl_easy_init();
    if (curl) {
        struct string s;
        init_string( & s);

        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, & s);
        res = curl_easy_perform(curl);

        printf("%s\n", s.ptr);
        free(s.ptr);

        /* always cleanup */
        curl_easy_cleanup(curl);
        status = 1;
    }
    return status;
}

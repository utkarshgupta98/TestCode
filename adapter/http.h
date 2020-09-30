
// @authors Kucharla Rajashekhar and Vinay Prabhakar

#ifndef _HTTP_H
#define _HTTP_H

#include<stdio.h>
#include<stdlib.h>


struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);
int send_http_request(char *URL) ;

#endif

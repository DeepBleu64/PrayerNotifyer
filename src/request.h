#ifndef REQUEST_H

#define REQUEST_H
#include <stdlib.h>

//extern char response[2048];

typedef struct memory{
  char *response;
  size_t size;
}memory;

struct memory make_curl_request(const char* url);

#endif

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "request.h"
#include "path.h"
#include "request.h"


//const char *url = "https://ipinfo.io";
//char response[2048] = {0};



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {

    size_t totalSize = size * nmemb;
    memory *mem = (struct memory *) userp;
    /* strncat((char *)userp, (char *)contents, totalSize); */
    /* return totalSize; */
    char *response = realloc(mem->response, mem->size + totalSize + 1);

    if(!response) return 0;

    mem->response = response;
    memcpy(&(mem->response[mem->size]), contents, totalSize);
    mem->size += totalSize;
    mem->response[mem->size] = 0;
    return totalSize;
}

struct memory make_curl_request(const char *url) {

  CURL *curl;
  CURLcode res;
  struct memory mem = {0};
  curl = curl_easy_init();

  assert(curl != NULL);

  //  memset(response, '\0', sizeof(response));

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&mem);
  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {

    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));

    curl_easy_cleanup(curl);
    exit(1);

  }

  return mem;
}

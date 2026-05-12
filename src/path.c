#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include "request.h"
#include "path.h"




#define ASSERT(cond, msg) ((cond) ? 1 :  (fprintf(stderr, msg), exit(1)))

char *get_json_value(const char *filename, const char *key, const char *subkey);

char fullpath_to_loc[MAX_PATH] = {0}; // path to a JSON that has longitude, latitude values
char fullpath_to_elevation_val[MAX_PATH] = {0}; // path to a JSON that has elevation above sea level value

void mkdir_recursive(char *fullpath, mode_t mode) {

  char path[MAX_PATH] = {0};
  short unsigned int i = 0;

  assert(fullpath != NULL && strlen(fullpath) < MAX_PATH &&
	 *fullpath == '/');

  
  while(*fullpath) {

    
    path[i++] = *fullpath;

    if(*fullpath == '/' && i > 0) {


	//	printf("dir doesn't exist for path: %s\n", path);
      	// printf("Creating cache dir in path: %s\n", path);
	int result = mkdir(path, mode);
	
	if(result < 0 && errno != EEXIST) fprintf(stderr, "Error creating dir %s: %s\n", path,  strerror(errno));
      }

    fullpath++;
  }



    //	printf("dir dosen't exist for path: %s\n", path);
  //    printf("Creating dir in path: %s\n", path);
    int result = mkdir(path, mode);
    if(result < 0 && errno != EEXIST)
      fprintf(stderr, "Error creating dir %s: %s\n", path, strerror(errno));



#ifdef DEBUG	

  printf("Full path parsed: %s\n", path);

#endif
}



char *strcat_multi(size_t buffer_size, char *src, ...) {

  
    va_list args;
    va_start(args, src);
    char *head = va_arg(args, char*);
    size_t total_size = 0;

    while(head) {
      
    total_size += strlen(head);
    ASSERT(buffer_size > total_size, "Lenght of strings has excceded buffer size\n");
    strcat(src, head);
    head = va_arg(args, char*);

  }

  va_end(args);
  
  return src;
}


void create_default_paths() {

  // Constructs paths for the cache dir, those files being:
  // $HOME/.cache/Prayer/coordinates.json
  // $HOME/.cache/Prayer/elevation_above_sea_lvl.json

  char *home = getenv("HOME");
  char *filename1 = "/coordinates.json";
  char *filename2 = "/elevation_above_sea_lvl.json";

  assert(home != NULL);

  strcat_multi(sizeof(fullpath_to_loc), fullpath_to_loc, home, "/.cache/Prayer", NULL);

  mkdir_recursive(fullpath_to_loc, 0700);

  strcat(fullpath_to_loc, filename1);

  strcat_multi(sizeof(fullpath_to_elevation_val), fullpath_to_elevation_val, home, "/.cache/Prayer", filename2, NULL);


}

bool does_cache_exist() {

  // if paths are not constructed do so.

  if(!*fullpath_to_loc && !*fullpath_to_elevation_val) create_default_paths();

  int fd1, fd2;

  if((fd1 = open(fullpath_to_loc, O_RDONLY)) < 0 ||
     (fd2 = open(fullpath_to_elevation_val, O_RDONLY)) < 0) return false;



    close(fd1);
    close(fd2);

    return true;
}


void create_cache() {

           FILE *file1;
	   FILE *file2;
	   struct memory request = {0};
	   const char *urls[] = {"https://ipinfo.io/", "https://api.open-elevation.com/api/v1/lookup?locations="};

  
    	   if((file1 = fopen(fullpath_to_loc, "w")) == NULL ||
	      (file2 = fopen(fullpath_to_elevation_val, "w")) == NULL) {

	     fprintf(stderr,"Error Creating a json file: %s\n", strerror(errno));

	    exit(1);

	   }

	   request = make_curl_request(urls[0]);

           fprintf(file1,"%s\n", request.response);

	   free(request.response);
	   fclose(file1);

           // extract longitude, latitude to values from newly written file to-
           // make a request to https://api.open-elevation.com with the appropiate
           // URL query string (being longitude, and latitude)
	   char *long_lat = get_json_value(fullpath_to_loc, "loc", NULL); 

	   size_t url_query_size = strlen(urls[1]) + strlen(long_lat) + 1;
	   char full_url_query_string[url_query_size];

	   memset(full_url_query_string, '\0', sizeof(url_query_size));
	   
	   strcat_multi(url_query_size, full_url_query_string, urls[1], long_lat, NULL);


	   request = make_curl_request(full_url_query_string);

           fprintf(file2,"%s\n", request.response);

	   free(request.response);
	   fclose(file2);

	   free(long_lat);
}

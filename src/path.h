#ifndef PATH_H

#define PATH_H


#include <stdlib.h>
#include <stdbool.h>

#define MAX_PATH 305  // MAX_PATH to cache file $HOME/.cache/Prayer/filename
                      // 305 because: max home dir name length = NAME_MAX = 255, longest hard-coded filename length = 29
                      // length: "/home/$NAME_MAX/.cache/Prayer/$LONGEST_FILENAME" = 304 + 1

bool does_cache_exist();
void create_cache();
void mkdir_recursive(char *fullpath, mode_t mode);
char *strcat_multi(size_t buffer_size, char *src, ...); // concatenates multiple string to src buffer, last argument must be NULL


extern char fullpath_to_loc[MAX_PATH];
extern char fullpath_to_elevation_val[MAX_PATH];


#endif

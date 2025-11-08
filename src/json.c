#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *get_json_value(const char *filename, const char *key, const char *subkey) {


  // returns a value given a key value, or the first element of an array if subkey is supplied,
  // crashes otherwise if the key or subkey isn't found, or if subkey is supplied but the value isn't an array.
  json_object *root,*loc, *item;

  char *output = NULL;
  root = json_object_from_file(filename);


  if(!root) {

    fprintf(stderr,"Couldn't parse the json file exiting... :(\n");

    exit(1);

  }

  //  loc = json_object_object_get(root, key);

  // https://api.open-elevation.com/api/ returns an array as value for the key "results"
  // we want elevation so we check if the given value is array

  if(json_object_object_get_ex(root, key, &loc) && subkey) {

    item = json_object_array_get_idx(loc, 0); // Get first element
    json_object_object_get_ex(item, subkey, &loc);

  }

  if(!loc) {

    fprintf(stderr,"Couldn't find given key \"%s\" in the json file exiting... :(\n", key);

    exit(1);


  }

  const char* str = json_object_get_string(loc);

  output = malloc(strlen(str) + 1);

  strcpy(output, str);

  json_object_put(root);


  return output;


}

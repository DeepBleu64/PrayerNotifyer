#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "path.h"
#include "calc.h"
#include "config.h"
#include <assert.h>



char *get_json_value(const char *response, const char *key, const char *subkey) ;

int main(int argc, char **argv) {



  // By default stdout if fully buffered for redirected output
  // so we enable line buffering for stdout, to see
  // live writes for redirected output to a file.
  setvbuf(stdout, NULL, _IOLBF, 0);

 #ifndef MANUAL_MODE

  if(!does_cache_exist())create_cache();


  #endif

  // force cache creation/update if requsted
  if(argc >= 2 && strcmp("-f", argv[1]) == 0) create_cache();

  Prayers *prayers = calc_prayers_times(CURRENT_CAL_METHOD);

  while(1) {

  times *time = init_time();

  double current_decimal_time = convert_time_to_decimal(time);

  Prayers next_prayer = get_next_prayer(current_decimal_time, prayers);

  unsigned short int *times = next_prayer_time_remaining(next_prayer.prayer_time, current_decimal_time);


 #if DEBUG

  double JD = georgian_to_julian();

  #ifndef MANUAL_MODE

  char *output = get_json_value(fullpath_to_loc, "loc", NULL);
  double *loc = to_double(output);

  printf("JD: %lf\n%s\n",JD,output);

  free(output);
  #endif

  printf("lat: %f\nlong: %f\n",loc[0], loc[1]);
  printf("Sun declination: %f\n", get_sun_declination());
  printf("Equation of time: %f\n", get_equation_of_time());
  printf("Next prayer: %lf\n", next_prayer.prayer_time);
  printf("Time remaining til next prayer %s: [%d:%d]\n", next_prayer.prayer_name,  times[0], times[1]);
  printf("Current calc method: %s %d\n", methods_strings[CURRENT_CAL_METHOD], CURRENT_CAL_METHOD);
  for(int i = 0 ; i < 6; ++i) printf("%lf ", prayers[i].prayer_time);

  printf("\n");

  print_prayers_times(prayers);

  #ifndef MANUAL_MODE

  free(loc);

  #endif

  #endif


  count_down_next_prayer(times[0], times[1], &next_prayer);

  // To avoid recalculation of prayers each time do it
  // only after Isha prayer.
  if(strcmp(next_prayer.prayer_name, "Isha") == 0) {

    free(prayers);
    prayers = calc_prayers_times(CURRENT_CAL_METHOD);

  }

  free(time);
  free(times);

  }

  return 0;
}

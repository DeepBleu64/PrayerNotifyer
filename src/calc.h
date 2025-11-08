#ifndef CALC_H

#define CALC_H

#include <stdbool.h>

typedef struct times {

  int year  ;
  int month ;
  int day   ;
  int hour  ;
  int min   ;
  int sec   ;
  int Z     ;


}times;


typedef struct Prayers {

  const char *prayer_name;
  double prayer_time;

} Prayers;

typedef enum methods {

  ISNA = 0,
  MWL,
  UMM_QURRA,
  GULF,
  ALGERIAN,
  KARACHI,
  DIYANET,
  EGYPT,
  EGYPTBIS,
  KEMENAG,
  MUIS,
  JAKIM,
  UOIF,
  FRANCE15,
  FRANCE18,
  TUNISIA,
  TEHRAN,
  JAFARI,


}methods;




times *init_time();
double georgian_to_julian();
double *to_double(const char *s);
double get_sun_declination();
double get_equation_of_time();
double convert_time_to_decimal(struct times *time);
Prayers get_next_prayer(double time_now_in_decimal, Prayers *prayers);
unsigned short int *next_prayer_time_remaining(double next_prayer, double current_decimal_time);
Prayers *calc_prayers_times(enum methods method);

void count_down_next_prayer(unsigned short int hour, unsigned short int min, Prayers *prayer);

void print_prayers_times(Prayers *prayers);

#endif

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


// Store remaining times in decimal
typedef struct __Prayers_remaining_times {
  unsigned short int hour;
  unsigned short int min;
}Prayer_remaining_times;

typedef struct Prayers {
  const char *prayer_name;
  double prayer_time;
  struct __Prayers_remaining_times prayers_remaining_times;
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
int get_next_prayer(double time_now_in_decimal, Prayers *prayers);
void next_prayer_time_remaining(Prayers *prayers, double next_prayer_time, double current_decimal_time);
Prayers *calc_prayers_times(enum methods method);

void count_down_next_prayer(Prayers *prayer);

void print_prayers_times(Prayers *prayers);

#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "path.h"
#include "calc.h"
#include "config.h"

#define DEGREE_TO_RADIAN(degree) ((degree) * (M_PI / 180.0))
#define RADIAN_TO_DEGREE(radian) ((radian) * (180.0 / M_PI))

#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')

#define TEN_MINS 600

float FAJR_ANGLE;
float ISHA_ANGLE;
short int MAGRIB_DURATION;
short int DESCEND_CORRECTION = 2;





typedef enum coordinates {

  LATITUDE  = 0,
  LONGITUDE ,

}cordinates;


char *get_json_value(const char *filename, const char *key, const char *subkey);

times *init_time() {

  times *timep = malloc(sizeof(times));

  time_t secs  = time(NULL);
  struct tm *p = localtime(&secs);
  timep->year  = p->tm_year + 1900;
  timep->month = p->tm_mon  + 1;
  timep->day   = p->tm_mday;
  timep->hour  = p->tm_hour;
  timep->min   = p->tm_min;
  timep->sec   = p->tm_sec;
  timep->Z     = p->tm_gmtoff / 3600;

#ifdef DEBUG
#if 0
  printf("Year: %d, Month: %d, Day: %d, Hour: %d, Min: %d, Sec: %d, Z: %d\n",
       timep->year, timep->month, timep->day, timep->hour, timep->min, timep->sec, timep->Z);

  printf("Time in decimal: %lf\n", (timep-> hour) + (timep->min / 60.0) + (timep->sec / (60.0 * 60.0)));
  #endif
  #endif

  return timep;

}


double georgian_to_julian() {


  times *time = init_time();

  if(time->month <= 2) {

    time->year -= 1;
    time->month += 12;
  }

  int A = time->year / 100;
  int B = 2 - A + (A / 4);


  double JD = 1720994.5 + (365.25 * time->year) + (30.6001 * (time->month + 1)) \
    + B + time->day + ((time->hour * 3600 + time->min * 60 + time->sec) / (double)86400) - (time->Z / 24.0);


  free(time);

  return JD ;
}


double get_sun_declination() {


  double T = (georgian_to_julian() - 2451545) * 360 / 365.25; // computes T in degrees 


  double DELTA = 0.37877 +
                 23.264  * sin(DEGREE_TO_RADIAN(T - 79.547)) +          \
	         0.3812  * sin(DEGREE_TO_RADIAN(2 * T - 82.682)) +	\
	         0.17132 * sin(DEGREE_TO_RADIAN(3 * T - 59.722));



  return DELTA;
}


double get_equation_of_time() {

  double U = (georgian_to_julian() - 2451545) / (double)36525;

  double L0 = 280.46607 + 36000.7698 * U ;


  double ET1000 = -(1789 + 237 * U) * sin(DEGREE_TO_RADIAN(L0)) -        \
                  (7146 - 62 * U) * cos(DEGREE_TO_RADIAN(L0)) +		 \
                  (9934 - 14 * U) * sin(DEGREE_TO_RADIAN(2 * L0)) -      \
                  (29 + 5 * U) * cos(DEGREE_TO_RADIAN(2 * L0)) +	 \
                  (74 + 10 * U) * sin(DEGREE_TO_RADIAN(3 * L0)) +	 \
                  (320 - 4 * U) * cos(DEGREE_TO_RADIAN(3 * L0)) -	 \
                  212 * sin(DEGREE_TO_RADIAN(4 * L0)) ;

  double ET = ET1000 / (double)1000 ;


  return ET;
}


double *to_double(const char *s) {

  // Converts latitude and longitude pair given as char* and saperated by ,
  // e.g: "24.687700,46.721900"

  double *loc = malloc(2 * sizeof(double));

  int i = 0;

  char buff[100] = {0};

  assert(loc != NULL);

  assert(strlen(s) < sizeof(buff));

  while(*s != '\0') {

    if(*s == ',') {

      loc[0] = atof(buff);
      memset(buff, '\0', sizeof(buff));
      i = 0;

    } else if(ISDIGIT(*s) || *s == '.' || *s == '-') buff[i++] = *s;

    s++;
  }

  loc[1] = atof(buff);

  return loc;
}

double calc_transit_time() {

  #ifndef MANUAL_MODE

  char *output = get_json_value(fullpath_to_loc, "loc", NULL);
  double *loc = to_double(output);

  #endif

  double ET = get_equation_of_time();
  times *time = init_time();
  double TT = 12 + time->Z - (loc[LONGITUDE] / (double)15) - (ET / (double)60);

  free(time);


  #ifndef MANUAL_MODE

  free(output);
  free(loc);

  #endif

  return TT ;

}

double acot(double angle_degree) {

  return M_PI_2 - atan(angle_degree);

}

void set_method_vars(enum methods method) {



  switch(method) {

  case   ISNA: FAJR_ANGLE = ISHA_ANGLE = 15; break;

  case   MWL: FAJR_ANGLE  = 18, ISHA_ANGLE = 17; break;

  case   UMM_QURRA: FAJR_ANGLE = 18.5, MAGRIB_DURATION = 90; break;

  case   GULF: FAJR_ANGLE = 19.5, MAGRIB_DURATION = 90; break;

  case   ALGERIAN:  FAJR_ANGLE = 18 , ISHA_ANGLE = 17; break;

  case   KARACHI: FAJR_ANGLE = ISHA_ANGLE = 18; break;

  case   DIYANET: FAJR_ANGLE = 18, ISHA_ANGLE = 17; break;

  case   EGYPT: FAJR_ANGLE = 19.5, ISHA_ANGLE = 17.5; break;

  case   EGYPTBIS: FAJR_ANGLE = 20, ISHA_ANGLE = 18; break;

  case   KEMENAG: FAJR_ANGLE = 20, ISHA_ANGLE = 18; break;

  case   MUIS: FAJR_ANGLE = 20, ISHA_ANGLE = 18; break;

  case   JAKIM: FAJR_ANGLE = 20, ISHA_ANGLE = 18; break;

  case   UOIF: FAJR_ANGLE = 12, ISHA_ANGLE = 12; break;

  case   FRANCE15: FAJR_ANGLE = 15, ISHA_ANGLE = 15; break;

  case   FRANCE18: FAJR_ANGLE = 18, ISHA_ANGLE = 18; break;

  case   TUNISIA: FAJR_ANGLE = 18, ISHA_ANGLE = 18; break;

  case   TEHRAN: FAJR_ANGLE = 17.7, ISHA_ANGLE = 14; break;

  case   JAFARI: FAJR_ANGLE = 16, ISHA_ANGLE = 14; break;

  default: FAJR_ANGLE = ISHA_ANGLE = 15; break; // Defaults to ISNA method

  }
}


Prayers *calc_prayers_times(enum methods method) {

  set_method_vars(method);

  double delta = DEGREE_TO_RADIAN(get_sun_declination());

 #ifndef MANUAL_MODE

  char *output = get_json_value(fullpath_to_loc, "loc", NULL);
  char *elevation_lvl_above_sea = get_json_value(fullpath_to_elevation_val, "results", "elevation");
  double H = atof(elevation_lvl_above_sea);
  double *loc = to_double(output);

  #endif

  double lat_in_radian = DEGREE_TO_RADIAN(loc[LATITUDE]);


  double SA_FAJR    = DEGREE_TO_RADIAN(-FAJR_ANGLE);
  double SA_SUNRISE = DEGREE_TO_RADIAN(-0.8333 - (0.0347 * sqrt(H))) ;
  double SA_ASR     = acot(SHADOW_FACTOR + tan(fabs(delta - lat_in_radian)));
  //  double SA_MAGHRIB = SA_SUNRISE;
  double SA_ISHA    = -(DEGREE_TO_RADIAN(ISHA_ANGLE));

  double COS_HA_FAJR    = (sin(SA_FAJR) - sin(lat_in_radian) * sin(delta)) / (cos(lat_in_radian) * cos(delta));
  double COS_HA_SUNRISE = (sin(SA_SUNRISE) - sin(lat_in_radian) * sin(delta)) / (cos(lat_in_radian) * cos(delta));
  double COS_HA_ASR     = (sin(SA_ASR) - sin(lat_in_radian) * sin(delta)) / (cos(lat_in_radian) * cos(delta));
  double COS_HA_MAGHRIB = COS_HA_SUNRISE;
  double COS_HA_ISHA    = (sin(SA_ISHA) - sin(lat_in_radian) * sin(delta)) / (cos(lat_in_radian) * cos(delta));


  double HA_FAJR    = acos(COS_HA_FAJR);
  double HA_SUNRISE = acos(COS_HA_SUNRISE);
  double HA_ASR     = acos(COS_HA_ASR);
  double HA_MAGHRIB = HA_SUNRISE;
  double HA_ISHA    = acos(COS_HA_ISHA);

double transit_time = calc_transit_time();

#ifdef DEBUG
#if 0
  printf("HA_FAJIR: %lf\nHA_SUNRISE: %lf\nHA_ASR: %lf\nHA_MAGHRIB: %lf\nHA_ISHA: %lf\nTransit Time: %lf\n", HA_FAJR,
	 HA_SUNRISE,
	 HA_ASR,
	 HA_MAGHRIB,
	 HA_ISHA,
	 transit_time);
#endif
  #endif




  Prayers* prayers = malloc(6 * sizeof(Prayers));

  assert(prayers != NULL);

  prayers[0].prayer_name = "Fajir";
  prayers[0].prayer_time = transit_time - (RADIAN_TO_DEGREE(HA_FAJR) / 15);


  prayers[1].prayer_name = "Shurq";
  prayers[1].prayer_time = transit_time - (RADIAN_TO_DEGREE(HA_SUNRISE) / 15);


  prayers[2].prayer_name = "Duhur";
  prayers[2].prayer_time = transit_time + (DESCEND_CORRECTION / 60.0);


  prayers[3].prayer_name = "Asir";
  prayers[3].prayer_time = transit_time + (RADIAN_TO_DEGREE(HA_ASR) / 15);


  prayers[4].prayer_name = "Magrib";
  prayers[4].prayer_time = transit_time + (RADIAN_TO_DEGREE(HA_MAGHRIB) / 15);


  prayers[5].prayer_name = "Isha";

  if(method == UMM_QURRA || method == GULF) prayers[5].prayer_time = prayers[4].prayer_time + MAGRIB_DURATION / 60.0;

  else prayers[5].prayer_time = transit_time + (RADIAN_TO_DEGREE(HA_ISHA) / 15);





  #ifndef MANUAL_MODE

  free(loc);
  free(output);
  free(elevation_lvl_above_sea);

  #endif

  return prayers;


}


double convert_time_to_decimal(times *time) {

  double decimal_time = time->hour + (time->min / 60.0) + (time->sec / (60.0 * 60.0));

  return decimal_time;
}

Prayers get_next_prayer(double time_now_in_decimal, Prayers *prayers) {

  // next prayer is always going to have a greater time stamp than the current time.
  // so the next prayer is the prayer whose time is greater than the current time.

  Prayers prayer;

  for(int i = 0; i < 6; ++i) {

     if(time_now_in_decimal < prayers[i].prayer_time) {

      prayer.prayer_time = prayers[i].prayer_time;
      prayer.prayer_name = prayers[i].prayer_name;

      return prayer;

     }

  }

  // if all prayers' times are smaller than current time then next prayer must be
  // fajir.

  prayer.prayer_name = prayers[0].prayer_name;
  prayer.prayer_time = prayers[0].prayer_time;

  return prayer;

}


unsigned short int *next_prayer_time_remaining(double next_prayer, double current_decimal_time) {

  unsigned short int *time_tuple = malloc(sizeof(unsigned short int) * 2);

  assert(time_tuple != NULL);

  // exeptional case for Fajir prayer, since fajir has the
  // smallest prayer time value of all the prayers, we increment it
  // by 24 to avoid interger underflow when subtracting 
  // next_prayer - current_decimal_time.

  if(current_decimal_time > next_prayer) next_prayer += 24;

  //  printf("next: %lf curr: %lf\n", next_prayer, current_decimal_time);

  unsigned short int hour = next_prayer - current_decimal_time;
  double tmp  = next_prayer - current_decimal_time;
  unsigned short int min  = (tmp - (unsigned int)tmp) * 60;


  while(60 <= min) {

    hour++;
    min -= 60;

  }

  time_tuple[0] = hour;
  time_tuple[1] = min;

  return time_tuple;
}

void count_down_next_prayer(unsigned short int hour, unsigned short int min, Prayers *prayer) {

  unsigned short int sec = 60;



  while(hour || min || sec) {


    printf("Time untill %s prayer: %d:%d:%d\n",prayer->prayer_name,  hour, min, sec);

    sleep(1);

    if(0 < sec) {

      sec--;

    }else if(0 < min) {

      min--;
      sec = 60;

    }else if(0 < hour) {

      hour--;
      min = 60;

    }


  }

  printf("It's %s prayer time now\n", prayer->prayer_name);
  sleep(TEN_MINS);
}

void print_prayers_times(Prayers *prayers) {

  // prints all prayers's times in human readable format.


  for(int i = 0; i < 6; ++i) {

    unsigned short int hour = prayers[i].prayer_time;
    double tmp = prayers[i].prayer_time;
    unsigned short int min = (tmp - hour) * 60;

    printf("%-15s: [%d:%d]\n", prayers[i].prayer_name, (hour  > 12 ? hour - 12 : hour), min);

  }


}

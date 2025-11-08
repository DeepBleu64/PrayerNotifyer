#ifndef CONFIG_H

#define CONFIG_H


#define CURRENT_CAL_METHOD UMM_QURRA /* Available calculation methods:

					  ISNA
					  MWL
					  UMM_QURRA
					  GULF
					  ALGERIAN
					  KARACHI
					  DIYANET
					  EGYPT
					  EGYPTBIS
					  KEMENAG
					  MUIS
					  JAKIM
					  UOIF
					  FRANCE15
					  FRANCE18
					  TUNISIA
					  TEHRAN
					  JAFARI
				     */

#define SHADOW_FACTOR 1             // Used for asir prayer calculation.
                                    // SHADOW_FACTOR = 1 => Shafii school of thought.
                                    // SHADOW_FACTOR = 2 => Hanafi school of thought.

// The program uses external services to get important values for calculations
// those being latitude, longitude and elevation above sea level.
// uncomment the below macro and adjust the values manualy if wish
// to do so without reliance on the services.


//#define MANUAL_MODE

#ifdef MANUAL_MODE

static double loc[] = {24.6877, 46.7219}; // {latitude, longitude}
static double H = 612.0; // elevation

#endif

#ifdef DEBUG

static const char *methods_strings[] = {"ISNA",
			   	        "MWL",
			   	        "UMM_QURRA",
			   	        "GULF",
			   	        "ALGERIAN",
			   	        "KARACHI",
			   	        "DIYANET",
			   	        "EGYPT",
			   	        "EGYPTBIS",
			   	        "KEMENAG",
			   	        "MUIS",
			   	        "JAKIM",
			   	        "UOIF",
			   	        "FRANCE15",
			   	        "FRANCE18",
			   	        "TUNISIA",
			   	        "TEHRAN",
			   	        "JAFARI",};

#endif

#endif

/*
 * Algorithm.c
 *
 *  Created on: May 8, 2020
 *      Author: AviMaitra
 */
#include <math.h>
#include "Algorithm.h"
#include <complex.h>
#include "RTC.h"

#define LATITUDE 35.3
#define GCR (2005.0 / 3350.0)

double sind(double degrees)
{
    double radians = degrees * (M_PI / 180.0);
    return sin(radians);
}

double asind(double input)
{
    double degrees;
    double radians = asin(input);
    degrees = radians * (180.0 / M_PI);
    return degrees;
}

double cosd(double degrees)
{
    double radians = degrees * (M_PI / 180.0);
    return cos(radians);
}

double acosd(double input)
{
    double degrees;
    double radians = acos(input);
    degrees = radians * (180.0 / M_PI);
    return degrees;
}

double tand(double degrees)
{
    double radians = degrees * (M_PI / 180.0);
    return tan(radians);
}

double atand(double input)
{
    double degrees;
    double radians = atan(input);
    degrees = radians * (180.0 / M_PI);
    return degrees;
}

double algo_getDeclination(void)
{
    // Based off Prof. Dolan Algorithm
    double myDay = (double) RTC_GetCurrentDay();
    return (23.45 * sind((360 / 365) * (myDay - 80.0)));
}

double algo_getHourAngle(void)
{
    // Based off Prof. Dolan Algorithm
    return 15.0 * (12.0 - RTC_GetCurrentTime());
}

double algo_getAltitude(void)
{
    // Based off Prof. Dolan Algorithm
    double alt;
    double declination = algo_getDeclination();
    double hourAngle = algo_getHourAngle();

    double m = (sind(declination) * sind(LATITUDE)) +
            (cosd(declination) * cosd(LATITUDE) * cosd(hourAngle));

    alt = asind(m);
    if (alt <= 0)
        alt = .0001;

    return alt;
}

// TODO Azimuth function
double algo_getAzimuth(void)
{
    double azim;
    double declination = algo_getDeclination();
    double alt = algo_getAltitude();
    double n = ((sind(alt)*sind(LATITUDE)) - sind(declination)) / (cosd(alt)*cosd(LATITUDE));
    //confused about how to write the part where azm = -acosd(n)
    azim = acosd(n);
    return 0.0;
}

double algo_getBeta(void)
{
    // Based off Prof. Dolan Algorithm
    double p;
    double b = (sind(algo_getAzimuth()) / tand(algo_getAltitude()));
    double complex inverseGCR = (double complex)(1 / GCR);

    // double complex exponent = csqrt()
    // double complex p_img = cpow(myGCR,)

    return 0.0;
}

double Algorithm_GetAngle(void)
{
    // TODO get angle
    return 0.0;
}




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
#define NOON 12

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

double algo_getAzimuth(void)
{
    double azim;
    double declination = algo_getDeclination();
    double alt = algo_getAltitude();
    double n = (sind(alt) * sind(LATITUDE) - sind(declination)) / (cosd(alt) * cosd(LATITUDE));
    azim = (RTC_GetHour() < NOON) ? (-1 * acosd(n)) : (acosd(n));
    return azim;
}

double algo_getBeta(void)
{
    // Returns results in radians
    // Based off Prof. Dolan Algorithm
    double realBeta;
    double complex beta;
    double alt = algo_getAltitude();
    double azim = algo_getAzimuth();
    double complex b = sind(azim) / tand(alt);
    double x = cosd(alt) * sind(azim);
    double y = sind(alt);
    double complex inverseGCR = (double complex)(1 / GCR);
    double complex rootArg = cpow(b, 4) - ((cpow(GCR, -2) - 1) * cpow(b, 2));
    double complex rootRes = csqrt(rootArg);
    double complex p = (inverseGCR + rootRes) / (1 + cpow(b,2));
    beta = cacos(p);
    realBeta = creal(beta);
    return realBeta;
}



double Algorithm_GetAngle(void)
{
    double myRad;

    myRad = (RTC_GetHour() < 12) ? (-1 * algo_getBeta()) : (algo_getBeta());
    return myRad * (180.0 / M_PI);
}




/* Satellite Orbit Tracking Pointer
Grady Hillhouse 2015
Modified 18 Nov 2017 by Giselle Koo
*/

#include "mbed.h"
#include <string>
#include <cstring>
#include "SGP4_vallado_gradyh/sgp4ext.h"
#include "SGP4_vallado_gradyh/sgp4unit.h"
#include "SGP4_vallado_gradyh/sgp4io.h"
#include "SGP4_vallado_gradyh/sgp4coord.h"
#include "stepper.h"

Serial pc(USBTX, USBRX);

// pin assignments

#define ALT1 p9
#define ALT2 p10
#define ALT3 p11
#define ALT4 p12

#define ALTHOME p13

#define AZ1 p5
#define AZ2 p6
#define AZ3 p7
#define AZ4 p8

#define AZHOME p14

// Sketchy global vars go here or whatever.
// TLE lines 1 and 2
char longstr1[200];
char longstr2[200];
// Site location data
double siteLat, siteLon, siteAlt, siteLatRad, siteLonRad;

//Function prototypes
void setTime(int);
void setTLE(char[], char[]);
void setLocData(double, double, double);

void setTime(int time) {
  set_time(time); // time in epoch
}

void setTLE(std::string TLE1, std::string TLE2) {
  std::memcpy(longstr1, &TLE1, 200);
  std::memcpy(longstr2, &TLE2, 200);
}

void setLocData(double lat, double lon, double alt) {
  siteLat = lat; //+North
  siteLon = lon; //+East
  siteAlt = alt; //km
  siteLatRad = siteLat * pi / 180.0;
  siteLonRad = siteLon * pi / 180.0;
}


int main()
{
    //INITIALIZE AZIMUTH STEPPER MOTOR
    Stepper *azMotor = new Stepper(AZ1, AZ2, AZ3, AZ4, AZHOME);
    azMotor->findHome(); // home should be pointing North
    pc.printf("AZstep find home\n");

    //INITIALIZE ALTITUDE STEPPER MOTOR
    Stepper *altMotor = new Stepper(ALT1, ALT2, ALT3, ALT4, ALTHOME);
    altMotor->findHome(); // home should be pointing -90 deg (straight down)
    pc.printf("ALTstep find home\n");

    //SET UP SOME VARIABLES
    double ro[3];
    double vo[3];
    double recef[3];
    double vecef[3];
    char typerun, typeinput, opsmode;
    gravconsttype  whichconst;

    double sec, secC, jd, jdC, tsince, startmfe, stopmfe, deltamin;
    double tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2;
    double latlongh[3]; //lat, long in rad, h in km above ellipsoid
    double razel[3];
    double razelrates[3];
    int  year; int mon; int day; int hr; int min;
    int yearC; int monC; int dayC; int hrC; int minC;
    typedef char str3[4];
    str3 monstr[13];
    elsetrec satrec;

    float elevation;
    float azimuth; //-180 to 0 to 180

    //SET REAL TIME CLOCK (Set values manually using custom excel function until I find a way to do it automatically)
    set_time(1511065766779);

    //SET VARIABLES
    opsmode = 'i';
    typerun = 'c';
    typeinput = 'e';
    whichconst = wgs72;
    getgravconst( whichconst, tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2 );
    strcpy(monstr[1], "Jan");
    strcpy(monstr[2], "Feb");
    strcpy(monstr[3], "Mar");
    strcpy(monstr[4], "Apr");
    strcpy(monstr[5], "May");
    strcpy(monstr[6], "Jun");
    strcpy(monstr[7], "Jul");
    strcpy(monstr[8], "Aug");
    strcpy(monstr[9], "Sep");
    strcpy(monstr[10], "Oct");
    strcpy(monstr[11], "Nov");
    strcpy(monstr[12], "Dec");

    //ENTER TWO-LINE ELEMENT HERE
    std::memcpy(longstr1, "1 25544U 98067A   17322.95053927  .00004162  00000-0  69955-4 0  9996", 200);
    std::memcpy(longstr2, "2 25544  51.6415 346.8032 0004306 128.6427 307.0316 15.54181392 85825", 200);
    pc.printf("TLE 1: %s \n", longstr1);
    pc.printf("TLE 2: %s \n", longstr2);

    //ENTER SITE DETAILS HERE
    siteLat = 41.798599; //+North (UHart)
    siteLon = -72.714000; //+East (UHart)
    siteAlt = 0.0143; //km (UHart)
    siteLatRad = siteLat * pi / 180.0;
    siteLonRad = siteLon * pi / 180.0;

    //INITIALIZE SATELLITE TRACKING
    //pc.printf("Initializing satellite orbit...\n");
    twoline2rv(longstr1, longstr2, typerun, typeinput, opsmode, whichconst, startmfe, stopmfe, deltamin, satrec );
    //pc.printf("twoline2rv function complete...\n");
    //Call propogator to get initial state vector value
    sgp4(whichconst, satrec, 0.0, ro, vo);
    //pc.printf("SGP4 at t = 0 to get initial state vector complete...\n");
    jd = satrec.jdsatepoch;

    invjday(jd, year, mon, day, hr, min, sec);
    pc.printf("Scenario Epoch   %3i %3s%5i%3i:%2i:%12.9f \n", day, monstr[mon], year, hr, min, sec);
    jdC = getJulianFromUnix(time(NULL));
    invjday( jdC, yearC, monC, dayC, hrC, minC, secC);
    pc.printf("Current Time    %3i %3s%5i%3i:%2i:%12.9f \n", dayC, monstr[monC], yearC, hrC, minC, secC);
    //pc.printf("            Time            PosX            PosY            PosZ              Vx              Vy              Vz\n");
    //pc.printf("            Time             Lat            Long          Height           Range         Azimuth       Elevation\n");

    //BEGIN SATELLITE TRACKING
    while(1)
    {
        //RUN SGP4 AND COORDINATE TRANSFORMATION COMPUTATIONS
        jdC = getJulianFromUnix(time(NULL));
        tsince = (jdC - jd) * 24.0 * 60.0;
        sgp4(whichconst, satrec, tsince, ro, vo);
        teme2ecef(ro, vo, jdC, recef, vecef);
        ijk2ll(recef, latlongh);
        rv2azel(ro, vo, siteLatRad, siteLonRad, siteAlt, jdC, razel, razelrates);

        //CHECK FOR ERRORS
        if (satrec.error > 0)
        {
            pc.printf("# *** error: t:= %f *** code = %3d\n", satrec.t, satrec.error);
        }
        else
        {
            azimuth = razel[1]*180/pi;
            elevation = razel[2]*180/pi;

            pc.printf("Altitude: %f \n Azimuth: %f \n", elevation, azimuth);

            azMotor->setAngle((double)azimuth);
            altMotor->setAngle((double)elevation);

            azMotor->stepperLoop();
            altMotor->stepperLoop();

            //pc.printf("%16.8f%16.8f%16.8f%16.8f%16.8f%16.8f%16.8f\n", satrec.t, recef[0], recef[1], recef[2], vecef[0], vecef[1], vecef[2]);
            //pc.printf("%16.8f%16.8f%16.8f%16.8f%16.8f%16.8f%16.8f\n", satrec.t, latlongh[0]*180/pi, latlongh[1]*180/pi, latlongh[2], razel[0], razel[1]*180/pi, razel[2]*180/pi);

        }

        wait(1);

    }

}

/*
  *
    To report problems, download updates and examples, suggest enhancements or get technical support:

       patreon:  www.patreon.com/curiosityworkshop
       email:    flightpro42@gmail.com
       YouTube:  https://youtube.com/channel/UCISdHdJIundC-OSVAEPzQIQ

    Created by Michael Gerlicher September 2020.  Initial public release.  You may use this code for any purpose.
  *
  *
  */
  

#ifndef Resolver_h
#define Resolver_h

class Resolver
{
public:
    Resolver(int sensorPinRef, int sensorPinSin, int sensorPinCos, int exciterPin, int sinDivider, int cosDivider, int samples);
    float readResolver();
    float ReturnHighestSin();
    float ReturnHighestCos();
    float ReturnLowestSin();
    float ReturnLowestCos();
    float ReturnAngle();
    float ReturnAverage();

 private:
   
    int gsensorPinRef;
    int gsensorPinSin;
    int gsensorPinCos;
    int gexciterPin;
    int gsinDivider;           
    int gcosDivider;
    int gsamples;
    float highestSin;
    float highestCos;
    float lowestSin;
    float lowestCos;
    int average;
    int angle;
    int numReadings=10;
    int readings[10];
    int readIndex;
    int total;


};

#endif



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

#include <Resolver.h>
//#include "resolver.h"
#include <arduino.h>

const int numSamples = 10;  // Number of samples for moving average
float samples[numSamples];  // Array to store the samples
int currentIndex = 0;       // Index to keep track of the current sample

float applyDigitalFilter(float newValue)
{
  // Add the new value to the samples array
  samples[currentIndex] = newValue;

  // Calculate the moving average of the samples
  float filteredValue = 0.0;
  for (int i = 0; i < numSamples; i++)
  {
    filteredValue += samples[i];
  }
  filteredValue /= numSamples;

  // Increment the current index
  currentIndex = (currentIndex + 1) % numSamples;

  return filteredValue;
}

Resolver::Resolver(int sensorPinRef, int sensorPinSin, int sensorPinCos, int exciterPin, int sinDivider, int cosDivider)
{
	//pinMode(exciterPin, INPUT);

   // Read the excitation signal
  float excitationSignal = analogRead(exciterPin);

  // Apply the digital filter
  float filteredSignal = applyDigitalFilter(excitationSignal);
  pinMode(exciterPin, OUTPUT);
  analogWrite(exciterPin, 128);

    gsensorPinRef = sensorPinRef;
    gsensorPinSin = sensorPinSin;
    gsensorPinCos = sensorPinCos;
    gsinDivider = sinDivider;    highestSin = sinDivider;  lowestSin = sinDivider;
    gcosDivider = cosDivider;    highestCos = cosDivider;  lowestCos = cosDivider;
}

float Resolver::ReturnHighestSin()
{
    return highestSin;

}

float Resolver::ReturnHighestCos()
{
    return highestCos;
}

float Resolver::ReturnLowestSin()
{
    return lowestSin;

}

float Resolver::ReturnLowestCos()
{
    return lowestCos;
}


/*
 * This samples the sin and cos of the resolver and averages multiple samples as set in the for loop then converts to degrees.
 */
float Resolver::readResolver()
{

    long readSinHigh = 0;
    long readCosHigh = 0;
    long readSinLow = 0;
    long readCosLow = 0;

    int samplesSinHigh = 0;
    int samplesCosHigh = 0;
    int samplesSinLow = 0;
    int samplesCosLow = 0;

    noInterrupts();                            // make this atomic so we get as many samples as possible

    for (int i = 0; i < 20; i++)
    {
        while (analogRead(gsensorPinRef) > 100) {}           //  find dropping edge
        while (analogRead(gsensorPinRef) < 100) {}         //  until we find a rising edge

        while (analogRead(gsensorPinRef) > 100)           //  sample until the next dropping edge
        {
            readSinHigh += analogRead(gsensorPinSin);
            samplesSinHigh++;
        }

        while (analogRead(gsensorPinRef) < 100)           //  sample until the next rising edge
        {
            readSinLow += analogRead(gsensorPinSin);
            samplesSinLow++;
        }


        while (analogRead(gsensorPinRef) > 100)           //  sample until the next dropping edge
        {
            readCosHigh += analogRead(gsensorPinCos);
            samplesCosHigh++;
        }
        while (analogRead(gsensorPinRef) < 100)           //  sample until the next rising edge
        {
            readCosLow += analogRead(gsensorPinCos);
            samplesCosLow++;
        }
    }

    interrupts();

    float finalSin = (readSinHigh / samplesSinHigh) - (readSinLow / samplesSinLow);
    float finalCos = (readCosHigh / samplesCosHigh) - (readCosLow / samplesCosLow);

    if (finalSin > highestSin) { highestSin = finalSin;  gsinDivider = finalSin; }
    if (finalCos > highestCos) { highestCos = finalCos;  gcosDivider = finalCos; }
    if (finalSin < lowestSin) { lowestSin = finalSin;  }
    if (finalCos < lowestCos) { lowestCos = finalCos; }
    
    finalSin /= gsinDivider;          
    finalCos /= gcosDivider;

    return (atan2(finalSin, finalCos) + 3.14159) * 57.2958;

}







#include <Resolver.h>
//#include "resolver.h"
#include <arduino.h>



Resolver::Resolver(int sensorPinRef, int sensorPinSin, int sensorPinCos, int exciterPin, int sinDivider, int cosDivider, int samples)
{
	pinMode(exciterPin, OUTPUT);
	analogWrite(exciterPin, 128);

    gsensorPinRef= sensorPinRef;
    gsensorPinSin= sensorPinSin;
    gsensorPinCos= sensorPinCos;
    gexciterPin= exciterPin;
    gsinDivider= sinDivider;
    gcosDivider= cosDivider;
    gsamples = samples;

    sensorPinRef = sensorPinRef;
    sensorPinSin = sensorPinSin;
    sensorPinCos = sensorPinCos;
    highestSin = sinDivider;  lowestSin = sinDivider;
    highestCos = cosDivider;  lowestCos = cosDivider;


  //readings[numReadings];  // the readings from the analog input
  readIndex = 0;          // the index of the current reading
  total = 0;              // the running total
  average = 0;            // the average

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
  readings[thisReading] = 0;
  }

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

float Resolver::ReturnAngle()
{
    return angle;
}

float Resolver::ReturnAverage(){
    
    
    //------Moving Average Calculation--------
      // subtract the last reading:
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = angle;
      // add the reading to the total:
      total = total + readings[readIndex];
      // advance to the next position in the array:
      readIndex = readIndex + 1;

      // if we're at the end of the array...
      if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
      }
      // calculate the average:
      average = total / numReadings;
      // send it to the computer as ASCII digits
      
    
    
    return average;
}


/*
 * This samples the sin and cos of the resolver and averages multiple samples as set in the for loop then converts to degrees.
 */
float Resolver::readResolver()
{


//-------Gathering Resolver Data-----------
    float sinHigh = 0;
    float cosHigh = 0;
    float sinLow = 0;
    float cosLow = 0;
  
 
    noInterrupts();                            // make this atomic so we get as many samples as possible

for (int i = 0; i < gsamples; i++)
    {
        while (analogRead(gsensorPinRef) > 100) {}           //  find dropping edge
        while (analogRead(gsensorPinRef) < 100) {}         //  until we find a rising edge

        /*while (analogRead(sensorPinRef) > 100) {}           //  find dropping edge
        while (analogRead(sensorPinRef) < 100) {}         //  until we find a rising edge
        //delay(.015);
        sinHigh+=analogRead(sensorPinSin);          //this is a method to read sin and cos at the same time
        //delay(.007);                                //this method is a bit tricky to find proper delays
        cosHigh+=analogRead(sensorPinCos);
        while (analogRead(sensorPinRef) > 100){}
        //delay(.015);
        cosLow+=analogRead(sensorPinCos);
        //delay(.007);
        sinLow+=analogRead(sensorPinSin);
        while (analogRead(sensorPinRef) < 100){}*/


        //delay(.026);
        cosHigh+=analogRead(gsensorPinCos);
        while (analogRead(gsensorPinRef) > 100){}
        //delay(.022);
        sinLow+=analogRead(gsensorPinSin);
        while (analogRead(gsensorPinRef) < 100){}
        //delay(.016);
        sinHigh+=analogRead(gsensorPinSin);
        while (analogRead(gsensorPinRef) > 100){};
        //delay(.016);
        cosLow+=analogRead(gsensorPinCos);
        while (analogRead(gsensorPinRef) < 100){} 
    }



/*
for (int i = 0; i < 20; i++){

    // Check for rising edge detection
      if (writeValue > lastWriteValue) {
    //  risingEdgeDetected = true;
      delay(.4);
      readSinHigh = analogRead(gsensorPinSin);
      readCosHigh = analogRead(gsensorPinCos);
      samplesSinHigh++;
      samplesCosHigh++;
      lastWriteValue = writeValue;
    }

    // Check for falling edge detection
    else if (writeValue < lastWriteValue) {
    //  fallingEdgeDetected = true;
      delay(.4);
      readSinLow = analogRead(gsensorPinSin);
      readCosLow = analogRead(gsensorPinCos);
      samplesSinLow++;
      samplesCosLow++;
      lastWriteValue = writeValue;
    }
    
    else{
      i--;
      lastWriteValue=writeValue;
    }
}
*/  
    interrupts();
//------Angle Calculation--------

    float avgSinHigh=sinHigh/gsamples;
    float avgSinLow=sinLow/gsamples;
    float avgCosHigh=cosHigh/gsamples;
    float avgCosLow=cosLow/gsamples;


    float finalSin = avgSinHigh-avgSinLow;
    float finalCos = avgCosHigh-avgCosLow;

    if (finalSin > highestSin) { highestSin = finalSin;  gsinDivider = finalSin; }
    if (finalCos > highestCos) { highestCos = finalCos;  gcosDivider = finalCos; }
    if (finalSin < lowestSin) { lowestSin = finalSin;  }
    if (finalCos < lowestCos) { lowestCos = finalCos; }
    
    //finalSin /= sinDivider;
    //finalCos /= cosDivider;
    if (finalSin>=0){
      finalSin/= highestSin;
    }
    if (finalSin<0){
      finalSin/= -lowestSin;
    }
    if (finalCos>=0){
      finalCos/= highestCos;
    }
    if (finalCos<0){
      finalCos/= -lowestCos;
    }

    angle=(atan2(finalSin, finalCos) + 3.14159) * 57.2958;

    return angle;
}

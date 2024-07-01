int sensorPinRef;  //reference signal sensing pin
int sensorPinSin;  //sine pin connection
int sensorPinCos;  //cosine pin connection
int exciterPin;   //pin that sends excitation signal
float sinDivider;   //used for later calculations
float cosDivider;   //used for later calculations
int refValue;    //used for reading refernce pin voltage
int refValueHigh;   //high reference pin voltage
int refValueLow;    //low reference pin voltage
float angle;
int samples; //number of samples taken for averaging values
//int samplesLow;
//float samplesHigh;
float sinHigh;
float sinLow;
float cosHigh;
float cosLow;
float avgSinHigh;
float avgSinLow;
float avgCosHigh;
float avgCosLow;
float highestSin;
float highestCos;
float lowestSin;
float lowestCos;

const int numReadings = 10;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

void setup() {
  // put your setup code here, to run once:
  exciterPin=4; //exciter pin where square signal is sent
  sensorPinRef=34; //sensor reference pin
  sensorPinSin=36; //sine sensor pin
  sensorPinCos=39; //cosine sensor pin
  samples=40;

	pinMode(exciterPin, OUTPUT);
	analogWrite(exciterPin, 128);
  Serial.begin(115200);

      for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

}

void loop() {

//-------Gathering Resolver Data-----------
  //refValueHigh=0;
  //refValueLow=0;
  sinHigh=0;
  sinLow=0;
  cosHigh=0;
  cosLow=0;;
noInterrupts();
  for (int i = 0; i < samples; i++)
    {
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

        while (analogRead(sensorPinRef) > 100) {}           //  find dropping edge
        while (analogRead(sensorPinRef) < 100) {}         //  until we find a rising edge
        //delay(.026);
        cosHigh+=analogRead(sensorPinCos);
        while (analogRead(sensorPinRef) > 100){}
        //delay(.016);
        cosLow+=analogRead(sensorPinCos);
        while (analogRead(sensorPinRef) < 100){}
        //delay(.016);
        sinHigh+=analogRead(sensorPinSin);
        while (analogRead(sensorPinRef) > 100){};
        //delay(.022);
        sinLow+=analogRead(sensorPinSin);
        while (analogRead(sensorPinRef) < 100){} 
    }
interrupts();

//------Angle Calculation--------

    avgSinHigh=sinHigh/samples;
    avgSinLow=sinLow/samples;
    avgCosHigh=cosHigh/samples;
    avgCosLow=cosLow/samples;


    float finalSin = avgSinHigh-avgSinLow;
    float finalCos = avgCosHigh-avgCosLow;

    if (finalSin > highestSin) { highestSin = finalSin;  sinDivider = finalSin; }
    if (finalCos > highestCos) { highestCos = finalCos;  cosDivider = finalCos; }
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


//-----Printing Everything------
    Serial.println("");
    Serial.print(angle);
    Serial.print(" / ");
    Serial.print(average);
    Serial.print(" // ");
    Serial.print(avgSinHigh);
    Serial.print(" / ");
    Serial.print(avgSinLow);
    Serial.print(" // ");
    Serial.print(avgCosHigh);
    Serial.print(" / ");
    Serial.print(avgCosLow);
    //Serial.print(" // ");
    //Serial.print(samplesHigh);
    //Serial.print(" // ");
    //Serial.print(samplesLow);
    delay(100);
}

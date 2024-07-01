#include <Resolver.h>

/*
 * 
 * ResolverDemo.  This reads and prints the position of a connected resolver. 
 * This is experimental and is designed for use with slow moving resolvers, like those that change with a human moving a knob.
 * Accuracy is not perfect due to the square wave stimulation but it should work OK for many applications that just need a rough 
 * position.
 * 
 * This was developed and tested on an Arduino Nano for general use.
 * Accuracy should be increased with faster boards and those that have 12bit sampling capability (M0 and such)
 * 
 * To increase accuracy on slow boards, no timeout processing is built in so if a pulse is not seen on the reference input pin
 * the code will wait forever for it.
 * 
 
   To report problems, download updates and examples, suggest enhancements or get technical support:
   
      patreon:  www.patreon.com/curiosityworkshop
      email:    flightpro42@gmail.com
      YouTube:  https://youtube.com/channel/UCISdHdJIundC-OSVAEPzQIQ
 * 
 * 
 */

#include <Resolver.h>

#include <arduino.h>

Resolver myResolver(34, 35, 39, 4, 439, 219);   // calibrate to your specific application

                      
// Parameters are:  Sensor Reference Pin, connect the exciter pulse directly to this pin to read
//                  Sin Pin
//                  Cos Pin
//                  Exciter Pin (output to exciter coil)
//                  Highest Cos sampled (for divider).  Run your resolver through all positions and use the highest number found
//                  Highest Sin sampled (for divider).  Run your resolver through all positions and use the highest number found

 
void setup() 
{
   
  Serial.begin(115200);

}
 

void loop() 
{

  myResolver.readResolver();   

   
  Serial.print("Resolver Position: ");
  Serial.print(myResolver.readResolver());
  
  Serial.print(", highestSin: ");
  Serial.print(myResolver.ReturnHighestSin());
  Serial.print(", highestCos: ");
  Serial.println(myResolver.ReturnHighestCos());
 
  delay(1000);
}

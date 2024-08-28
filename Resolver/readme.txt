
 *
 * 
 * Resolver Arduino Library.  This returns the position of a connected resolver. 
 *
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
 *

Resolver Arduino Library

Initial Release 10/29/2020

Installation
-------------
Extract the Resolver folder and place it into the libraries folder of your arduino sketches


Usage
------


// Include the XPLDirect library:
#include <Resolver.h> 

// Create an instance:
//                     
// Parameters are:  Sensor Reference Pin, connect the exciter pulse directly to this pin to read
//                  Sin Pin
//                  Cos Pin
//                  Exciter Pin (output to exciter coil)
//                  Highest Cos sampled (for divider).  Run your resolver through all positions and use the highest number found
//                  Highest Sin sampled (for divider).  Run your resolver through all positions and use the highest number found

Resolver (int RefPin, int SinPin, int CosPin, int ExciterPin, int HighestCos, int HighestSin)
Example:  Resolver myResolver(A1, A2, A3, 11, 247, 219);  

// Functions:

float readResolver();           // reads and returns position as a float value in degrees
float ReturnHighestSin();       // returns the highest sin sample found since activation.  Use this for calibration
float ReturnHighestCos();       // returns the highest cos sample found since activation.  Use this for calibration


// See included sample sketch for simple example.

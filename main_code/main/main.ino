// Imports 

// for the calculation of distance
#include <math.h>

// for the sd card
#include <SPI.h>
#include <SD.h>



// Variables
const int chipSelect = 10;
boolean start = false;
boolean going = true;

void setup(){
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  init_sd();
  Serial.print("Ready to begin recording");
}

void loop(){
  
}



// Ensures the sd card is initialized
void init_sd()
{
  while (!Serial) {
 // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present"); // When the screen is set up, use the screen to display message
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized."); // When the screen is set up, use the screen to display message
}


// Writes speed, rpm, distance and time to sd card 
void write_sd(double spd, double rpm, double t) 
{
  
}

//calculate the various values given the initial time and final time
void values(double t_i, double t_f, double arr[])
{
  double ds, spd, rpm, dt;

  dt = t_f - t_i;
  arr[0] = dt;
  
  ds = 2*M_PI*0.3; //Assuming the radius is 30 cm
  spd = ds/dt;
  arr[1] = ds;
  arr[2] = spd;

  rpm = 3600/dt; // Assuming dt is in seconds
  arr[3] = rpm;
}

//int main() 
//{ 
//  while(!start)
//  {
//  // Initialize SD Card
//  
//  // Turn on each light on dashboard
//  
//  // Make sure all buttons work by pressing them
//  
//  /*
//  * if(button_pressed == true) start = true;
//  */
//  }
//
//
//  while(going)
//  {
//  // if there is an interrupt, calculate speed, rpm, distance and delta t
//
//  // Log data in .csv
//  // Display data on LCD screen
//  /*
//   * if(recording_button == true) 
//   * {
//   * going = false;
//   * break;
//   * }
//  */
//  } 
//  return 0;
//}

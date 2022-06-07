// Imports 

// for the calculation of distance
//#define PI 3.141592653589793

// for the sd card
#include <SPI.h>
#include <SD.h>

//For the LCD screen
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);




// Variables
const int chipSelect = 10;
boolean start = false;
boolean going = true;

void setup(){
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // Testing
  init_sd();
  lcd_init();
  lcd.print("Ready to begin recording");
  lcd.clear();
  lcd.print("Testing logging...");
  for(int i=0; i<10; i++){
    write_sd(i*2.5, i*5, 7.5*i);
  }

  // testing of the display function
  lcd_init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello world!");
  delay(1000);
  lcd_init();
  delay(1000);
  write_lcd(50.0, 34.6);

  //testing of the odo function
  double *p;
  p = odo(8.0);
  lcd_init();
  lcd.setCursor(0,0);
  lcd.print((char) p[0] + ""+(char) p[1]);
  
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
    Serial.print("Card failed, or not present"); // When the screen is set up, use the screen to display message
    // don't do anything more:
    while (1);
  }
  Serial.print("card initialized."); // When the screen is set up, use the screen to display message
}


// Writes speed, rpm, distance and time to sd card 
void write_sd(double spd, double rpm, double t) 
{
  String dataString = (String) spd +","+ (String) rpm +","+ (String) t;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
    
   // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.print(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("error opening datalog.txt");
  }
}
// LCD Initialization
void lcd_init(){
  lcd.init();
  lcd.clear();
}

// LCD Write function
void write_lcd(double rpm, double spd){
  lcd_init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print((String) rpm + " RPM");
  lcd.setCursor(0,1);
  lcd.print((String) spd +" kmh");
}

//calculate the various values given the initial time and final time
double * odo(double dt){

    static double kin[3];
    double ds, spd, rpm;
    double radius = 0.3; // The radius of the wheel in meters

    ds = 2*PI*0.3; //Assuming the radius is 30 cm
    spd = ds/dt;
    rpm = 3600/dt; // Assuming dt is in seconds
    kin[0] = ds;
    kin[1] = spd;
    kin[2] = rpm;

   return kin;
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

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


const byte interruptPin = 2; // Hall sensor
const byte startButton = 8; // Start Button
const byte LED_PIN =9; // To blink LED

volatile unsigned long prevHall = 0;
int timeChange;

boolean intTriggered = false;

boolean startRecording = false;

// Variables
const int chipSelect = 10;
boolean start = false;
boolean going = true;

void setup(){
  // Open serial communic;ations and wait for port to open:
  Serial.begin(9600);

  pinMode(interruptPin, INPUT_PULLUP); // Initialize the interrupt pin
  pinMode(startButton, INPUT); // Initialize pin for on/off operations
  pinMode(LED_PIN, OUTPUT);
  double delta_t = 0.05; // Test value in seconds
  init_sd();
  
  // display setup
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("START RECORDING?");
  
  while(true){
    startRecording = digitalRead(startButton);
    Serial.print(startRecording);
    if(startRecording == true) {
      lcd.clear();
      lcd.print("STARTED");
      break;
    }
    delay(10);
    
  }
}`

void loop(){
  attachInterrupt(digitalPinToInterrupt(interruptPin), trigger, RISING);
  attachInterrupt(digitalPinToInterrupt(startButton), doneRecording, RISING); // Get this to immediately stop the recording of the data
    if(intTriggered == true) {
      // calculate deltaTime
      long dt = deltaTime();
      double arr[2] = {0};
      pop_array(arr, dt);  // calculate kinematics
      write_sd(arr[0], arr[1]); // log to SD card
  
      // if a certain amount of time has passed, log to the screen
      if (timeChange > 200) {
        //update screen 
        write_lcd(arr[0], arr[1]);
        timeChange = 0;
      } else {
        timeChange += dt;
      }
      intTriggered = false;
    }
  }


// SD card is currently causing issues.
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
void write_sd(double spd, double t) 
{
  String dataString = (String) spd +","+ (String) t +"ms";
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
    Serial.print("error"); // opening datalog.txt
  }
}


// LCD Write function
void write_lcd(double spd, double dt){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print((String) spd +" kmh");
}

//calculate the various values given the initial time and final time
void pop_array(double *kin, double dt) {

    double ds, spd, rpm;
    double radius = 0.3; // The radius of the wheel in meters

    ds = 2*PI*radius; //Assuming the radius is 30 cm
    spd = ds*1000/dt;
    rpm = 3600000/dt; // Assuming dt is in seconds
    kin[0] = spd;
    kin[1] = dt;
}

// The magic routine
void trigger(){
  intTriggered = true;
}

void doneRecording(){
  lcd.clear();
  lcd.print("Recording done.");
  while(true){
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
}

//Returns time difference between triggers in ms 
volatile long deltaTime() {
  int elapsed = millis() - prevHall;
  prevHall = millis();
  return elapsed;
}

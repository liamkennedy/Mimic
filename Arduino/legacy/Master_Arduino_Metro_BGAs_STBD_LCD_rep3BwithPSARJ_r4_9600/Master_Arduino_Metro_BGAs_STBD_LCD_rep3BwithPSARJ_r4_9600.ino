
/// MIMIC!!! MIMIC!!!
//
// This version is just for STBD BGAs, board 78
// 1A is Motor 1, Encoder pins 2,12
// 3A is Motor 2, Encoder pins 8,11
// 1B is Motor 3, Encoder pins 14,15
// 3B is Motor 4, Encoder pins 16,17

// for LCD  =============================================

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
static char dtostrfbuffer[21];  // added Oct 20
#include <avr/dtostrf.h> 

#define OLED_RESET 4 // likely unnecessary
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
// end LCD =============================================


#include <Encoder.h>

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <string.h>
//#include <Servo.h>
//Servo servo_PTRRJ;
//Servo servo_STRRJ;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x78);
//Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x68);

// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1,2,4
Adafruit_DCMotor *myMotorB1A = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorB3A = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorB1B = AFMS.getMotor(3);
//Adafruit_DCMotor *myMotorB3B = AFMS.getMotor(4);
Adafruit_DCMotor *myMotorPSARJ = AFMS.getMotor(4);
//Adafruit_DCMotor *myMotorPSARJ = AFMS2.getMotor(1);
//Adafruit_DCMotor *myMotorSSARJ = AFMS2.getMotor(2);




Encoder myEnc1A(2, 12);
Encoder myEnc3A(8, 11); 
// Pins 9, 10 are used by Servos. 
// Odd issues with Pins 2-4. 
// Pin 13 is nominally used by LED and general guidance from encoder library is to avoid it.
Encoder myEnc1B(14, 15);
//Encoder myEnc3B(16, 17);
Encoder myEncPSARJ(17, 16);


// Per here, Analog pins on Metro M0 are also digtial IO. https://learn.adafruit.com/adafruit-metro-m0-express-designed-for-circuitpython/pinouts
// From here, A1 is digital 15, A2 is 16, A3 is 17, A4 is 18, A5 is 19. http://www.pighixxx.net/portfolio_tags/pinout-2/#prettyPhoto[gallery1368]/0/



//Encoder myEncPSARJ(5,6); // 14,15 is A0, A1
//Encoder myEnc1A(0, 1); // 14,15 is A0, A1    ============================================================================================== KILLED 1A encoder here and call to counter
//Encoder myEncSSARJ(16, 17); // 16,17 is A2 ,A3
// Also avoiding A4, A5 as these are coupled to the I2C SLA, SCA pins via the Motor Shield (for backward compatibility with older Ards).

//int D0 =0;
//int D1 =0;
int D2 =0;
//int D3 =0;
int D4 =0;
int D5 =0;
int D6 =0;
int D7 =0;
int D8 =0;
//int D9 =0;
//int D10=0;
int D11=0;
int D12=0;
//int D13=0;
int D14=0;
int D15=0;
int D16=0;
int D17=0;

void setup() {
//pinMode(0, INPUT_PULLUP);
//pinMode(1, INPUT_PULLUP);
//pinMode(2, INPUT_PULLUP);
//pinMode(3, INPUT_PULLUP);


  

//// for LCD  =============================================
//
//
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

// end for LCD  =========================================


  // Set some pins to high, just for convenient connection to power Hall Effect Sensors - can't, per above use of these pins
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);
//pinMode(A1, OUTPUT);
//digitalWrite(A1, HIGH);
//pinMode(A2, OUTPUT);
//digitalWrite(A2, HIGH);

  // Attach a servo to dedicated pins 9,10.  Note that all shields in the stack will have these as common, so can connect to any.
//  servo_PTRRJ.attach(9);
//  servo_STRRJ.attach(10);
  AFMS.begin(200);  // I set this at 200 previously to reduce audible buzz.
//  AFMS2.begin(200);  // I set this at 200 previously to reduce audible buzz.
  
  Serial.begin(9600);
  //Serial3.begin(115200);          //Serial1 is connected to the RasPi
  Serial.setTimeout(50);
 // Serial1.begin(9600);

  //Serial.println("Motor test!");

  // turn on motor   NOTE: May be able to remove all of these setSpeed and run commands here, although they are fine.
  myMotorB1A->setSpeed(150);
  myMotorB1A->run(RELEASE); 

  myMotorB3A->setSpeed(150);
  myMotorB3A->run(RELEASE);

  myMotorB1B->setSpeed(150);
  myMotorB1B->run(RELEASE);

//  myMotorB3B->setSpeed(150);
//  myMotorB3B->run(RELEASE);

    myMotorPSARJ->setSpeed(150);
  myMotorPSARJ->run(RELEASE);

//  myMotorSSARJ->setSpeed(150);
//  myMotorSSARJ->run(RELEASE);

  // For debugging
  //  pinMode(13, OUTPUT); //LED
}

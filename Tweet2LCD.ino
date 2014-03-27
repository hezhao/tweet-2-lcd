/*
 * See http://forum.arduino.cc/index.php?topic=158312.0
 * NewLiquidCrystal library
 * https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
 */

// TODO
// - printString() handle text longer than 80 char

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27  // Define I2C Address for the PCF8574T

//---(Following are the PCF8574 pin assignments to LCD connections )----
// This are different than earlier/different I2C LCD displays
#define BACKLIGHT_PIN  3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define  LED_OFF  1
#define  LED_ON  0


long currentTime = 0;
long lastTime = 0;
char buf[256];

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

void printString(char *str) 
{
  int lineCount = 0;
  int lineNumber = 0;
  int charIndex = 0;
  char mystr[256];
  
  memcpy(mystr, str, strlen(str)+1);
  
  while(str[charIndex]) 
  {
    // move cursor down if over 20 chars
    if (++lineCount > 20) 
    {
      lineNumber += 1;
      lcd.setCursor(0, lineNumber);
      lineCount = 1;
    }
    
    // print current char
    if (lineNumber < 4) {
      lcd.print(mystr[charIndex]);
      delay(30);
    }
    
    else {
      lineNumber = 0;
      lineCount = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
    }
    
    // next char
    charIndex += 1;
  }
}

void setup()
{
  Serial.begin(9600);
  
  lcd.begin (20, 4);  // 20x4 characters
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(LED_ON);
  
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(6, 0);
  printString("@hezhao");
}

void loop()
{
  static int i = 0;
  static bool stringReady = true;
  currentTime = millis();

  if (Serial.available() > 0)
  {
    // save char to buffer
    stringReady = false;
    char c = Serial.read();
    buf[i++] = c;
    lastTime = currentTime;
  }
  
  // print string after 100ms of receiving no serial data
  else if (currentTime - lastTime >= 100 && stringReady == false)
  {
    stringReady = true;
    Serial.print('stringReady');
    buf[i] = '\0';
    i = 0;
    lcd.clear();
    printString(buf);
  }
}


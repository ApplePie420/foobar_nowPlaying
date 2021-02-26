#include <LiquidCrystal_PCF8574.h>
#include <Wire.h> 

LiquidCrystal_PCF8574 lcd(0x27); 

void setup() {
  lcd.begin(16, 2);
  lcd.setBacklight(255);

  lcd.print("Test Line 1");
  lcd.setCursor(0,1);
  lcd.print("Test Line 2");
}

void loop() {
    
}
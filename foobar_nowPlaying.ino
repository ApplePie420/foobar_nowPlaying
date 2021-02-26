#include <LiquidCrystal_PCF8574.h>
#include <Wire.h> 

// create our display instance on default I2C address (0x27)
LiquidCrystal_PCF8574 lcd(0x27); 

void setup() {
  // start serial communication
  Serial.begin(9600);
  // initialize LCD, tell the library we have 16x2 display
  lcd.begin(16, 2);
  // turn on backlight
  lcd.setBacklight(255);

  // write some stuff onto the screen, to test if everything is ok
  lcd.print("Initialized");
  lcd.setCursor(0,1);
  lcd.print("Now Playing...");
  // wait and clear
  delay(1000);
  lcd.clear();
}


void loop() {
  // check if there is some data in serial buffer
  if (Serial.available()) {
    // create these new variables, and initialize them to 0. This essentially acts as lcd.clear() but should be a lot faster
    // title and artist is self explanatory, ptr is pointer for array indexes. To answer why are the arrays long 16 characters, scroll a bit 
    // further down to the wall of text :)
    char title[16] = {0};
    char artist[16] = {0};
    int ptr = 0;
    
    // wait for data to arrive
    delay(100);

    // read serial buffer byte by byte, save to artist array (since artist is the first thing we're sending)
    while (Serial.available() > 0) {
      artist[ptr] = Serial.read();
      ptr++;
    }
    // reset pointer
    ptr = 0;
    
    // wait for buffer to be flushed and to have some time so data will arrive safely
    delay(1000);
    
    // read the buffer in the same manner as before, except in "title" array
    while (Serial.available() > 0) {
      title[ptr] = Serial.read();
      ptr++;
    }
    ptr = 0;

    // set cursor to top left
    lcd.setCursor(0,0);
    /* print out whole "artist" array. Now because artist's names vary in length, we can't just declare randomly sized huge array. Also, String 
       is broken and does not work. Since our display size is 16 characters in width, there is no point storing anything longer. That's why 
       the array is only 16 characters long. All other characters that are NOT anything between ASCII 32 (space) to 126 (~), 
       or simply said, commonly used characters, is considered to be empty character and is replaced with space. This allows us to
       not use clear, and have "varying" artist names. I presume there is a lot more elegant solution to this, but it works fine. Display redraws
       very quickly even like this, and since we're updating it every 4 minutes or so.. it doesn't really matter.
    */
    for(int i = 0; i < 16; i++) {
      if(artist[i] >= 32 && artist[i] <= 126) {
         lcd.print(artist[i]);
      } else {
        lcd.print(' ');
      }
    }
    
    // same thing as above, but with song's title
    lcd.setCursor(0,1);
    for(int i = 0; i < 16; i++) {
      if(title[i] >= 32 && title[i] <= 126) {
         lcd.print(title[i]);
      } else {
        lcd.print(' ');
      }
    }
  }
}

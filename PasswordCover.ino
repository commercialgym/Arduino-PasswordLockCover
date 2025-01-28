#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//number of rows and columns in keypad
const byte ROWS = 4;
const byte COLS = 4;
int cursor = 0;
int cursorRow = 0;
int servoAngle = 0;

//keymap for keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//connect keypad's rows and columns to pins
byte rowPins[ROWS] = {2, 3, 4, 5};  
byte colPins[COLS] = {6, 7, 8, 9};  

//keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//initialize lcd obj
LiquidCrystal_I2C lcd(0x27, 16, 2); 
//initialize servo motor (barrier/cover)
Servo servoBarrier;

void setup() {
  lcd.init(); //initialize lcd
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  servoBarrier.attach(11);
  servoBarrier.write(0);

  Serial.begin(9600);  
  Serial.println("4x4 Keypad Test");
}

void loop() {
  char key = keypad.getKey();  

  if (key) {  //if a key is pressed it won't be null
    Serial.print("Key Pressed: ");
    Serial.println(key);
    // lcd.clear();
    if (cursorRow == 1 && cursor > 16)
    {
      lcd.clear();
      cursorRow = 0;
      cursor = 0;
    }
    if (cursor > 16)
    {
      cursorRow = 1;
      cursor = 0;
    }
    lcd.setCursor(cursor, cursorRow);
    lcd.print(key);
    cursor++;

    if (servoAngle == 0)
    {
      servoAngle = 90;
    }
    else
    {
      servoAngle = 0;
    }

    servoBarrier.write(servoAngle);
  }
}

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//number of rows and columns in keypad
const byte ROWS = 4;
const byte COLS = 4;
int cursor = 0;
int cursorRow = 0;
int servoAngle = 0; 

char key; //key input from user
char password[] = {'1', '2', '3', '4'}; //original password, subject to change
// bool correct = false; //flag for when a user is entering a password to guess


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
  lcd.write("*ENTER PASSWORD*");
  lcd.setCursor(1, 1);
  lcd.write("TO OPEN DOOR");

  servoBarrier.attach(11);
  servoBarrier.write(0);

  Serial.begin(9600);  
  Serial.println("4x4 Keypad Test");
}

void loop() {
  key = keypad.getKey();  //constantly waiting for input

  if (key) {  //if a key is pressed it won't be null

    if(key == '*')
    {
      //call function to allow user to update password
    }
    else
    {
      //get the code being inputted from user
      if (validateInput())
      {
        //call function to open door
      }
    }

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

bool validateInput()
{
  int i = 0;
  int j = 0;
  int correct = 0;

  while (key != '#') //make sure sizeof
  {
    lcd.setCursor(j, 1);
    lcd.print('*'); //or is it double quotes

    if (key == password[i] && i < sizeof(password))
    {
      correct++;
    }
    else if (key != password[i] && i < sizeof(password))
    {
      correct--;
    }

    j++;
    i++;
  }

  if (correct == sizeof(password))
  {
    return true;
  }
}

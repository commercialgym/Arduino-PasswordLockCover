#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//prototypes
void reset();
void openDoor();
void invalidPassword();
bool validateInput();
void changePassword();

//number of rows and columns in keypad
const byte ROWS = 4;
const byte COLS = 4;
int cursor = 0; //del?
int cursorRow = 0; //del?
const int servoAngle = 110; 
int delayDoorOpen = 2000;
int delayIncorrect = 1000;
int delayTime = 2000;

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
  lcd.print("*ENTER PASSWORD*");
  lcd.setCursor(1, 1);
  lcd.print("TO OPEN DOOR");

  servoBarrier.attach(11);
  servoBarrier.write(0);

  Serial.begin(9600);  
  Serial.println("4x4 Keypad Test");
}

void loop() {
  key = keypad.getKey();  //constantly waiting for input

  if (key) 
  {  //if a key is pressed it won't be null

    if(key == '*')
    {
      //call function to allow user to update password
      key = NO_KEY; //do i need this
      changePassword();
      delay(delayTime);
      reset();
    }
    else
    {
      //get the code being inputted from user
      if (validateInput())
      {
        //call function to open door
        openDoor();
        //delay before reset ?
        delay(delayDoorOpen); //maybe put in open door function?
        reset();
      }
      else
      {
        invalidPassword();
        //delay before reset
        delay(delayIncorrect); 
        reset();
      }
    }
  }
}

bool validateInput()
{
  int i = 0;
  int j = 0;
  int correct = 0;
  //clear the bottom of the lcd 
  lcd.setCursor(0, 1);
  lcd.print("                ");

  while (key != '#') //make sure sizeof
  {
    if (key)
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

      key = NO_KEY;
      j++;
      i++;
    }

    key = keypad.getKey();
  }

  key = NO_KEY;

  if (correct == sizeof(password))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void invalidPassword()
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("INVALID CODE");
}

// void validPassword()
// {
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("DOOR OPENING");
// }

void openDoor()
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("DOOR OPENING");
  servoBarrier.write(servoAngle);
}

void reset()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*ENTER PASSWORD*");
  lcd.setCursor(1, 1);
  lcd.print("TO OPEN DOOR");

  key = NO_KEY;

  servoBarrier.write(0);
}

void changePassword()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER CURRENT");
  lcd.setCursor(0, 1);
  lcd.print("PASSWORD "); //debug didn't print

  key = keypad.getKey();
  if (validateInput())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ENTER NEW CODE");

    char newPassword[4] = {'0', '0', '0', '0'};;
    int i = 0;
    int j = 0;
    //int same = 0; //to check if it's the same password


    while (key != '#' && i <= 4) 
    {
      key = keypad.getKey();

      if (key)
      {
        lcd.setCursor(j, 1);
        lcd.print(key); 

        // if (key == password[i] && i < sizeof(password)) 
        // {
        //   //same++;
        // }
        // else if (key != password[i] && i < sizeof(password))
        // {
        //   //same--;
        // }

        if (i <= sizeof(password)) 
        {
          newPassword[i] = key; //later: assign to original password if it's valid
          password[i] = key;
        }

        key = NO_KEY;
        j++;
        i++;
      }
      key = NO_KEY;

    }

    key = NO_KEY;


    // if (same == sizeof(password))
    // {
    //   //the password is the same as before
    // }
    // else
    // {
    //   //it's a new password
        // if (i == sizeof(password)) // && same != sizeof(password)
        // {
        //   //press pound to verify they want to change
        //   //then assign the newPassword to old password
        // }
    // }

  }
  else
  {
    invalidPassword();
    // delay(delayIncorrect); //did in main
    // reset();
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SUCCESS");

}
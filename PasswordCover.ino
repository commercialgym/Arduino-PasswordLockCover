#include <Keypad.h>

//number of rows and columns in keypad
const byte ROWS = 4;
const byte COLS = 4;

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

void setup() {
  Serial.begin(9600);  
  Serial.println("4x4 Keypad Test");
}

void loop() {
  char key = keypad.getKey();  

  if (key) {  //if a key is pressed it won't be null
    Serial.print("Key Pressed: ");
    Serial.println(key);
  }
}

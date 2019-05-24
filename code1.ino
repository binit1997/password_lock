
#include <Password.h> //http://playground.arduino.cc/uploads/Code/Password.zip //password library
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip  //keypad library


Password password = Password( "1234" ); //password to unlock, change accordingly

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { A0, A1, A2, A3 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { A4, A5, 3, 2 };


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  Serial.write(254);
  Serial.write(0x01);
  delay(200);
  pinMode(7, INPUT); //reset pushbutton
  int pin7 = 7;
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);//door latch relay
  pinMode(11, OUTPUT); //buzzer
  delay(10);
  digitalWrite(4, HIGH);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
}

void loop() {
  keypad.getKey();
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
}
//take care of some special events
void keypadEvent(KeypadEvent eKey)
{
  switch (keypad.getState()) {
    case PRESSED:


      Serial.print("Enter:");
      Serial.println(eKey);
      delay(10);

      Serial.write(254);

      switch (eKey) {
        case '*': checkPassword(); delay(1); break;

        case '#': password.reset(); delay(1); break;

        default: password.append(eKey); delay(1);
      }
  }
}
void checkPassword() {
  int pin7 = 7;
  pinMode(7, INPUT);
  pinMode(4, OUTPUT);

  if (password.evaluate()) { //if password is right open

    Serial.println("Accepted");
    Serial.write(254); delay(10);
    analogWrite(11, 250);
    delay(500); //wait 5 seconds
    analogWrite(11, 0);
    digitalWrite(4, 0); //actuate the door latch relay

    while (digitalRead(pin7) == LOW)
    {
      delay(10);
    }
    digitalWrite(4, 1);



    //  while (keypad.getKey() != '1')
    //    {
    //      delay(10);
    //      }
    //
    delay(100);

  }
  else {
    Serial.println("Denied"); //if passwords wrong keep locked
    Serial.write(254); delay(10);
    digitalWrite(4, 1);
    //add code to run if it did not work
    //buzzer connected at pin 11

    int i;
    i = 10;
    while (i > 2)
    { analogWrite(11, 110);
      delay(500);
      analogWrite(11, 0);
      delay(100);
      i = i - 2;
    }

  }
}


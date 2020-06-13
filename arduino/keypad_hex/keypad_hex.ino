#include <Keypad.h>

// Set Keypad
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {6, 7, 8, 9}; 
byte colPins[COLS] = {10, 11, 12, 13}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


// HEX display
#define Bp 2
#define Ap 3
#define Fp 4
#define Gp 5

#define Ep A2
#define Dp A3
#define Cp A4


// Hex fonts
void clear(){
  int pins[7] = {Bp, Ap, Fp, Gp, Ep, Dp, Cp};
  for (int i = 0; i <= 6; i++){
    digitalWrite(pins[i], LOW);
  }
}

void key1(){
  clear();
  digitalWrite(Bp, HIGH);
  digitalWrite(Cp, HIGH);
}

void key2(){
  clear();
  int pins[5] = {Ap, Bp, Gp, Ep, Dp};
  for (int i = 0; i <= 4; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key3(){
  clear();
  int pins[5] = {Ap, Bp, Gp, Cp, Dp};
  for (int i = 0; i <= 4; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key4(){
  clear();
  int pins[4] = {Fp, Gp, Bp, Cp};
  for (int i = 0; i <= 3; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key5(){
  clear();
  int pins[5] = {Ap, Fp, Gp, Cp, Dp};
  for (int i = 0; i <= 4; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key6(){
  clear();
  int pins[6] = {Ap, Fp, Gp, Cp, Dp, Ep};
  for (int i = 0; i <= 5; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key7(){
  clear();
  int pins[3] = {Ap, Bp, Cp};
  for (int i = 0; i <= 2; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key8(){
  clear();
  int pins[7] = {Ap, Bp, Cp, Dp, Ep, Fp, Gp};
  for (int i = 0; i <= 6; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key9(){
  clear();
  int pins[6] = {Ap, Bp, Gp, Fp, Cp, Dp};
  for (int i = 0; i <= 5; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void key0(){
  clear();
  int pins[6] = {Ap, Bp, Cp, Dp, Ep, Fp};
  for (int i = 0; i <= 5; i++){
    digitalWrite(pins[i], HIGH);
  }
}

void setup(){
  //Serial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT); 
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey == '1')
    key1();
  if (customKey == '2')
    key2();
  if (customKey == '3')
    key3();
  if (customKey == '4')
    key4();
  if (customKey == '5')
    key5();
  if (customKey == '6')
    key6();
  if (customKey == '7')
    key7();
  if (customKey == '8')
    key8();
  if (customKey == '9')
    key9(); 
  if (customKey == '0')
    key0(); 
}

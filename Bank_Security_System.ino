#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(11, 12);

LiquidCrystal_I2C lcd(0x27, 16, 2);  
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

String pad;
const byte ROWS = 4;
const byte COLS = 4;
String password = "4234";
String passenc = "7183";
char keypressed;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };
byte servoMotorPin = 10;
byte speakerPin = 13;
byte greenPin = 16;
byte redPin = 15;
byte c = 0;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myservo;

int correctPin = 4234;
int encryption = 7183;

void setup() 
{
  Serial.begin(9600);
  finger.begin(57600);
  myservo.attach(10);
  pinMode(speakerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  myservo.write(0);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME");
  delay(1000);
  lcd.clear();
}
void loop() 
{
  getFingerprint();

  readKeypad();

  char key = ' ';
  String input = "";
  String in = "";
  int Pin = 0;

  if (keypressed == '#') 
  {
    lcd.setCursor(0, 0);
    lcd.print("ENTER PASSWORD");
    while (key != '#') 
    {
      input += key;
      in += key;
      lcd.setCursor(0, 1);
      lcd.print(in);
      Pin = Pin * 10 + input.toInt();
      key = keypad.getKey();
      while (!key) 
      {
        key = keypad.getKey();
      }
      input = "";
    }
    if (Pin == correctPin || pad == password) 
    {
      lcd.setCursor(0, 1);
      lcd.print("ACCESS GRANTED");
      digitalWrite(greenPin, HIGH);
      delay(1000);
      myservo.write(90);
      delay(2000);
      myservo.write(0);
      digitalWrite(greenPin, LOW);
      Pin = 0;
    } 
    else if (Pin == encryption || pad == passenc) 
    {
      lcd.setCursor(0, 1);
      lcd.print("ACCESS GRANTED");
      delay(1500);
      lcd.setCursor(0, 0);
      lcd.print("*VAULT HAS BEEN*");
      lcd.setCursor(0, 1);
      lcd.print("****BREACHED****");
      digitalWrite(greenPin, HIGH);
      myservo.write(90);
      delay(2000);
      myservo.write(0);
      digitalWrite(greenPin, LOW);
      for (int n = 0; n < 3; n++) 
      {
        for (int i = 1000; i < 4000; i += 36) 
        {
          tone(speakerPin, i - 50, 18);
          delay(10);
        }
        for (int i = 4000; i > 1000; i -= 36) 
        {
          tone(speakerPin, i - 50, 18);
          delay(10);
        }
      }
      Pin = 0;
    } 
    else if (Pin != 0) 
    {
      lcd.setCursor(0, 1);
      lcd.print("ACCESS DENIED");
      c++;
      digitalWrite(redPin, HIGH);
      delay(1500);
      digitalWrite(redPin, LOW);
      Pin = 0;
      if (c > 3) 
      {
        c = 0;
        digitalWrite(redPin, HIGH);
        delay(2000);
        digitalWrite(redPin, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("****VAULT IS****");
        lcd.setCursor(0, 1);
        lcd.print("*****LOCKED*****");
        for (int n = 0; n < 3; n++) {
          for (int i = 1000; i < 4000; i += 36) 
          {
            tone(speakerPin, i - 50, 18);
            delay(10);
          }
          for (int i = 4000; i > 1000; i -= 36) 
          {
            tone(speakerPin, i - 50, 18);
            delay(10);
          }
        }
      }
    }
  }
  lcd.setCursor(0, 0);
  lcd.print(pad);
  delay(100);
}
void readKeypad() 
{
  keypressed = keypad.getKey();
  if (keypressed != '#') 
  {
    String konv = String(keypressed);
    pad += konv;
  }
}
void getFingerprint() 
{
  lcd.clear();
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("NO FINGERPRINT");
    lcd.setCursor(0, 1);
    lcd.print("DETECTED");
    delay(2000);
    return;
  }

  p = finger.image2Tz();
  p = finger.fingerFastSearch();

  lcd.clear();
  if (p == FINGERPRINT_OK) 
  {
    lcd.print("ACCESS GRANTED");
    delay(3000);
  } else 
  {
    lcd.print("ACCESS DENIED");
    delay(2000);
  }
  delay(2000);
  lcd.clear();
}
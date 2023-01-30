/*
  |\/|   | _  |
  |  |(|(|(/_ |)\/
                /
                  ___     _   _ _               _
                 / _ \___| |_(_) |_/\   /\_   _| | ___ __ _ _ __
                / /_)/ _ \ __| | __\ \ / / | | | |/ __/ _` | '_ \
               / ___/  __/ |_| | |_ \ V /| |_| | | (_| (_| | | | |
               \/    \___|\__|_|\__| \_/  \__,_|_|\___\__,_|_| |_|

              ITEM : ELIOTT'S STRONGBOX - REV : V1.2 => January 2023
*/

/*
    Libraries Imports
*/
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

/*
    LCD Configuration
*/
// Const for LCD Configuration
const byte LCD_NB_ROWS = 2;
const byte LCD_NB_COLUMNS = 16;
// Instance of LiquidCrystal_I2C lcd( I2CAddress => 0x27 , NumberOfColumns => 16 , NumberOfRows => 2 )
LiquidCrystal_I2C lcd(0x27, LCD_NB_COLUMNS, LCD_NB_ROWS);

/*
    Servo Configuration
*/
Servo myservo;                  // create Servo object to control servo
const int SERVO_PIN_ATTACH = 4; // Define arduino's servo signal pin (here pin 4)

/*
    KeyPad Configuration
*/
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {12, 11, 10, 9}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5};    // connect to the column pinouts of the keypad

// Initialize an instance of class Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/*
    Code Configuration
*/
// Init reference password
const String Password = "<SetYouPassHERE>";

// Init empty string for user input
String userCode = "";

/*
    FUNCTIONS
*/

// Function for Initialize LCD on SETUP
void InitLCD()
{
  // Initialize LCD Screen
  lcd.init();
  // turn ON backlight
  lcd.backlight();
  // Clear LCD Screen
  lcd.clear();
  // Message initialisation print line 1
  // Set Cursor on char 2 line 1
  lcd.setCursor(1, 0);
  lcd.print("Bonjour");
  // Set Cursor on char 9 line 2
  lcd.setCursor(8, 1);
  // Print message on line 2
  lcd.print("Eliott");
  // Wait 1 sec
  delay(1000);
  // Reset screen
  lcd.clear();
}

// Function for Initialize SERVO on SETUP
void InitServo()
{
  // Attache du servo pin
  myservo.attach(SERVO_PIN_ATTACH);
  // Initialisation de la position fermée
  myservo.write(130);
}

// Function when wrong password is set
void accessRefused()
{
  // CLOSE POSITION SERVO
  myservo.write(130);
  lcd.clear();
  // Print message on Line 1
  lcd.print("Acces");
  // set cursor on line 2 char 6
  lcd.setCursor(5, 1);
  // Print message on Line 2
  lcd.print("refuse");
  // Wait 3 sec
  delay(3000);
}

// Function when good password is set
void accessGranted()
{
  // Clear LCD Screen
  lcd.clear();
  // Print message on Line 1
  lcd.print(" Acces");
  // set cursor on line 2 char 6
  lcd.setCursor(5, 1);
  // Print message on Line 2
  lcd.print("Verifie");
  // Wait 0.5 sec
  delay(500);
  // Clear LCD Screen
  lcd.clear();
  // OPEN POSITION SERVO
  myservo.write(40);
  // Print message on Line 1
  lcd.print("Appuyez pour");
  // set cursor on line 2 char 1
  lcd.setCursor(0, 1);
  // Print message on Line 1
  lcd.print("fermer la porte");
  // Wait for keydown
  customKeypad.waitForKey();
  // Clear LCD Screen
  lcd.clear();
  lcd.print("Fermeture...");
  myservo.write(130);
  // Wait 1 sec
  delay(1000);
  // Clear LCD Screen
  lcd.clear();
  // Print message on Line 1
  lcd.print(" Porte");
  // set cursor on char 6 line 2 
  lcd.setCursor(5, 1);
  // Print message on Line 2
  lcd.print("Verrouillee");
  // Wait 1 sec
  delay(1000);
  // Clear LCD Screen
  lcd.clear();
}

// Loop Function
void VerifyUserCode()
{
  // Display Message on Screen
  lcd.print("Veuillez saisir");
  lcd.setCursor(0, 1);
  lcd.print("le password:");
  // Set Cursor on Char 14 Line 2 for user input
  lcd.setCursor(12, 1);
  // Loop for catching user input and print "*"" for each keydown
  for (int i = 0; i < Password.length(); i++)
  {
    char customKey = customKeypad.waitForKey();
    userCode += customKey;
    lcd.print("*");
  }

  // Conditionnal structure return Granted Or Refuse Option from user input
  if (userCode == Password) // If user input equal reference Password
    accessGranted();        // Executing accessGranted() function
  else                      // Or
    accessRefused();        // Executing accessRefused() function

  // Set usercode to Empty string
  userCode = "";
  // reset screen
  lcd.clear();
}

/*
    SETUP
*/
void setup()
{
  // Execute InitLCD Function
  InitLCD();
  // Execute InitServo Function
  InitServo();
  // Display Message
  lcd.print("Initialisation");
  lcd.setCursor(4, 1);
  lcd.print("Terminee");
  delay(1000);
  lcd.clear();
}

/*
    LOOP
*/
void loop()
{
  VerifyUserCode();
}

/* ----------------------------------------------------- *
 *                                                       *
 * Author: Brack Harmon (AKA Electro The Folf)           *
 * Date: 5/9/2021                                        *
 * Title: Automatic Watering System                      *                                                
 *                                                       *
 * ----------------------------------------------------- */


#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <Wire.h> 


const int RECV_PIN = 11; // I/0 pin for infrared sensor
const int TEMP_PIN = 7; //I/0 pin for temperature
const int PUMP_PIN = 2; // I/0 pin for water pump
const int MOISTURE_PIN = A0;//I/0 pin for moisture sensor
const int MOISTURE_THRESHOLD = 500; // highest moisture sensor value before the water pump turns on
const int PUMP_DELAY = 2000; //milliseconds it takes for water to go through tube 

String welcome = "Welcome! ^w^"; // welcome message to display when the LCD screen turns on
bool displayOn = false; // current LCD display state
bool autoMode = false; // current system state

IRrecv irrecv(RECV_PIN); //initializes the Infrared reciever 
decode_results results; // input data from infrared sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); //initialize LCD dispaly to two 16bit lines


void setup()
{

  Serial.begin(9600); //start serial module for debugging
  irrecv.enableIRIn(); // Start the Infrared receiver

  pinMode(MOISTURE_PIN,INPUT); // Start the moisture sensor
  pinMode(PUMP_PIN, OUTPUT);// Start the water pump
  digitalWrite(PUMP_PIN,LOW);// Start the moisture sensor
  
  lcd.begin(); // initialize the LCD
  lcd.noBacklight(); //turn of LCD dispaly

}

void loop()
{
  if (irrecv.decode(&results)) // check for infrared input
    {
     Serial.println(results.value); // prints the infrared value to the serial module for debugging purpouses
     
     
     UpdateLcdState(); // turn the LCD on or off if appropriate
      
     UpdateLcd(); // change the text on the LCD if appropriate
     
     irrecv.resume(); // Receive the next Infrared value
     
    } else if(analogRead(MOISTURE_PIN)  > MOISTURE_THRESHOLD){ // check if the soil is dry enough to water

        TurnOnPump();
      
    } 
    
}


// turns the LCD on or off depending on the state it is currently in 
void UpdateLcdState(){
  if(results.value == 16753245){//16753245 = on Button
        if(displayOn){
          displayOn = false; 
        } else {
          displayOn = true;
          lcd.clear(); // clears the LCD text 
          lcd.backlight(); // turns on the backlighing of the LCD
          lcd.print(welcome); //display the welcome message
        }
     }
}


// changes the text on the LCD depending on which button on the reomte is pressed
void UpdateLcd(){
  if(displayOn){ // check if the display should be on 
      
        if(results.value == 16724175){ //16724175 == "1" button
          lcd.clear();
          lcd.print("Moisture: ");
          lcd.print(analogRead(MOISTURE_PIN)); // prints the moisture value
        } else if(results.value == 16718055){//16718055 == "2" button
          lcd.clear();
          lcd.print("TEMP: ");
          lcd.print(analogRead(TEMP_PIN)); // prints the temperature value
        } 
      
     }
}


//turns on the water pump for a brief peiod of time
void TurnOnPump(){
      digitalWrite(PUMP_PIN,HIGH);
      delay(PUMP_DELAY); // delay length depends on length of water tube
      digitalWrite(PUMP_PIN,LOW);
      delay(1000);
}

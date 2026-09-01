#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#define slave_ard 0x08

//constant values
const int ir_pin = 7;
const int temp_emergency = 45;

//Defining LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//System state
enum State {
  Standby = 0,
  Active = 1,
  Gas_Alert = 2,
  Blackout = 3,
  Temp_Emergency = 4,
  Multi_Fault = 5
};

State currentState = Standby;
State lastDisplayedState = (State)-1;
bool lastShowLight = true;

//Sensor values
int light = 0;
int gas = 0;
int temp = 0;

//Display
bool showLight = true;

//IR buttons
unsigned long BUTTON_POWER = 0xFF00BF00; //Sets state from Standby to Active
unsigned long BUTTON_0 = 0xF30CBF00; //Resets temp emergency
unsigned long BUTTON_1 = 0xEF10BF00; //toggles light/gas display

void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(ir_pin, ENABLE_LED_FEEDBACK);
  pinMode(ir_pin, INPUT);
  currentState = Standby;
  Wire.begin(slave_ard);
  Wire.onReceive(receiveData);
  lcd.begin(16, 2);
  lcd.print("Start");
  showState();
}

void loop()
{
  checkIR();
  showState();
}

//Receiving values from master Arduino
void receiveData(int numBytes)
{
  if (numBytes < 6) return;

  int gasHigh = Wire.read();
  int gasLow  = Wire.read();
  byte receivedState = Wire.read();
  int newLight = Wire.read();
  int tempHigh = Wire.read();
  int tempLow  = Wire.read();

  gas   = (gasHigh << 8) | gasLow;
  light = newLight;
  temp  = (int16_t)((tempHigh << 8) | tempLow);

  if (currentState == Standby) return;
  if (currentState == Temp_Emergency) return;
  if (receivedState <= Multi_Fault)
  {
    currentState = (State)receivedState;
  }
}

void checkIR() {
  if (IrReceiver.decode()) {
    unsigned long command = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("IR Command: 0x");
    Serial.println(command, HEX);

    if (command == BUTTON_POWER && currentState == Standby) {
      currentState = Active;
    }
    else if (command == BUTTON_0 && temp < temp_emergency && currentState == Temp_Emergency) {
      currentState = Standby;
    }
    else if (command == BUTTON_1) {
      showLight = !showLight;
    }

    IrReceiver.resume();
  }
}

//Display
void showState() {
  if ((currentState == lastDisplayedState) && (lastShowLight == showLight)) return; // avoiding flicker
  lastDisplayedState = currentState;
  lastShowLight = showLight;

  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (currentState == Standby) {
    lcd.print("AWAITING RITUAL");
  }
  else if (currentState == Temp_Emergency) {
    lcd.print("COOKED");
  }
  else if (currentState == Gas_Alert) {
    lcd.print("TOXIC PURGE");
  }
  else if (currentState == Blackout) {
    lcd.print("NOCTIS PROTOCOL");
  }
  else if (currentState == Multi_Fault) {
    lcd.print("MULTIPLE PROBLEM"); 
    lcd.setCursor(0, 1);
    lcd.print("DETECTED");
  }
  else if (currentState == Active) {
    if (showLight == true) {
      lcd.print("Light:    ");
      lcd.setCursor(0, 1);
      lcd.print(light);
    }
    else {
      lcd.print("Gas:    ");
      lcd.setCursor(0, 1);
      lcd.print(gas);
    }
  }
}

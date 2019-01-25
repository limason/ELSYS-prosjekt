//Inkluderer biblioteker
  #include <TheThingsNetwork.h>
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #include "LowPower.h"
  

//Innganger
  #define turbPin A0
  #define oneWireBus 2
  #define powerPin 1

//TTU variabler
  //Definerer appEui og appKey fra TTN
  const char *appEui = "70B3D57ED0016CB0";
  const char *appKey = "A7565B717147C1FE3A11D453722510F2";

  //Frekvensplanet til LoRaWAN i EU
  #define freqPlan TTN_FP_EU868

  //Forenkler med å skille mellom loraSerial og debugSerial
  #define loraSerial Serial1
  #define debugSerial Serial
  
//Objekter
  TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
  OneWire oneWire(oneWireBus);
  DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(powerPin, OUTPUT);
  Serial.begin(9600);
  
  //Setter bauderate
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  //Venter litt
  while(!debugSerial && millis() < 10000);

  //Skriver status serielt
  debugSerial.println("STATUS");
  ttn.showStatus();

  //Kobler til TTN
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  
  //Temperatursensor
  sensors.begin();
}

void loop() {
  byte payload[3];

  digitalWrite(powerPin, HIGH);
  int temp = MeasureTemp();
  int turb = MeasureTurb();
  digitalWrite(powerPin, LOW);

  toByteBuffer(temp, turb, payload);
  
  ttn.sendBytes(payload, sizeof(payload));

  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

int MeasureTemp() {
  sensors.requestTemperatures();
  int temp = (sensors.getTempCByIndex(0) + 10)*100;
  constrain(temp,0,65535);
  return temp;
}

int MeasureTurb() {
  int turb = analogRead(turbPin);
  turb = map(turb, 0, 1023, 0, 255);
  return turb;
}

void toByteBuffer(int temp, int turb, byte *payload) {
  payload[0] = highByte(temp);
  payload[1] = lowByte(temp);
  payload[2] = (byte) turb;
}

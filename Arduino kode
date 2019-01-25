//Inkluderer biblioteker
#include <TheThingsNetwork.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Definerer appEui og appKey fra TTN
const char *appEui = "70B3D57ED0016CB0";
const char *appKey = "A7565B717147C1FE3A11D453722510F2";

//Frekvensplanet til LoRaWAN i EU
#define freqPlan TTN_FP_EU868

//Forenkler med å skille mellom loraSerial og debugSerial
#define loraSerial Serial1
#define debugSerial Serial

//Innganger
int turbPin = A0;
#define oneWireBus 2

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup()
{
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

void loop()
{
  // ---- Sensorer ----
  /*
    Leser av sensorene. 
    Turbiditet:
    AnalogRead gir en verdi mellom 0 og 1023, derfor må man bruke map
    for å sende det som en byte. Gjør det om til et prosenttall på TTN

    Temperatur:
    Antar en temperatur mellom -10 og 40 grader.
   */
  //Leser av temperatur
  sensors.requestTemperatures();
  float floatTemp = sensors.getTempCByIndex(0);
  debugSerial.println(floatTemp);
  int temp = (floatTemp+10)*100;
  debugSerial.println(temp);
  //Sikrer at ikke temp blir større en et ord
  constrain(temp,0,65535);

  //Leser av turbiditet
  int turb = analogRead(turbPin);
  //Så turb er mellom 0 og 255
  turb = map(turb, 0,1023, 0, 255);
  

  // ---- Bytes ----
  /* 
    Sensordata må sendes som en liste med bytes. Variablene må derfor være mellom 0 og 255. 
    Evt bruke et word med 2^16 verdier. Kan bruke High- og LowByte()-funksjonene for å dele i to bytes.     
  */
  //Definerer et array, som inneholder bytes som skal sendes
  byte payload[3];
  //Definerer de ulike bytene:
  payload[0] = highByte(temp);
  payload[1] = lowByte(temp);
  payload[2] = (byte) turb;

  // ---- Sende til TTN ----
  /*
    Sender bufferen laget tidligere.
   */
  ttn.sendBytes(payload, sizeof(payload));

  // ---- Søvn ----
  /*
    TTU bør skrus av til neste måling?
   */
  delay(20000); //For ikke å sende for ofte
}

/* Generelle retningslinjer for programmering
 *  
 * Kode skrives på engelsk men kommentarer er på dansk
 * camelBack bruges som defination af variabler og funktioner osv.
 * eksempel: hejJegHedderMikkel
 * husk man kan ikke sammenligne to floats med ==
 * brug i stedet granuleringsmetoden forklaret af jens
 * Operatorer til variabler: * + - / < > <= >= != = 
 * Operatorer til f.eks. if statements: && || ==
 * Brug altid korrekte indryk for at gøre koden læsbart/overskuelig
 * Kommenter funktioer og udregninger så dem der læser ved hvad de gør
 * Funktionsnavne skal kunne fortælle hvad funktionen gør
 */

//Inkluder de forskellige libraries ligesom nedenstående
#include<dht.h>
#include<SoftwareSerial.h>
#include<DS3231.h>
#include<Wire.h>


// definationer 
#define dataSize 300// 300 til endelige prototype 4 til at teste tallet skal være en del af 4 tabellen
#define MEASUREMENTDELAY 5 // betyder endten hvert 6. sek, hvert 6. minut eller hver 6. time
#define MAXINSMS 4
#define DHT11_PIN 5
// definering af variabler

byte dowSet, hourSet, minuteSet, secondSet, yearSet, monthSet, daySet;

int index = 0;
int pakkeNummer = 1;
int numberOfPackages;

bool firstZero = true;// test værdi
dht DHT;
DS3231 rtcModule;
SoftwareSerial sim900(10,11);
struct sensorType{
  int co2Data;
  int soundData;
  int tempData;
  int humidData;
  char timeData[13];
};

struct sensorType sensorData[dataSize];

byte timeNow;
void setup() {
  Wire.begin();
  Serial.begin(19200);
  while (!Serial);
  sim900.begin(19200);
  serialFlush();
  rtcSetup(); // Hvis tiden skal sættes kræver det at der bruges "no line ending" i serial terminalen
  displayTime();
  GSM_setup();  
  numberOfPackages = dataSize / 4;
  timeNow = 30;
  
}


void loop() {
  //if(rtcModule.getSecond()-timeNow == MEASUREMENTDELAY || rtcModule.getSecond() == 0) // hvis der skal måles med sekunder som interval
  if(rtcModule.getSecond()-timeNow == MEASUREMENTDELAY || (rtcModule.getSecond() == 0 && firstZero == true)) // Hvis der skal måles med minutter som interval
  {
    
    if(rtcModule.getSecond() == 0)
    {
      timeNow = rtcModule.getSecond();
    }
    firstZero = false;
    if(timeNow != rtcModule.getSecond())
    {
      firstZero = true;
    }
    
    timeNow = rtcModule.getSecond();
    
    
    
    getAndSaveTime(&(sensorData[index]));
    dhtOpsamleDataRef(&(sensorData[index]));
    readCO2_volts(&(sensorData[index]));
    Serial.print(String(index+1) + " Sample collected"); 
    delay(10);
    Serial.println("Time "); Serial.println(sensorData[index].timeData);
    index++;
    
    if(index == dataSize)
    {
      GSM_sendDataPackage(numberOfPackages);
      index = 0;
    }
    
  }

}

#define         CO2_PIN              (A0)     //define which analog input channel you are going to use

#define         CO2_INTERVAL         (1000)    //define the time interval(in milisecond) between each samples in 
#define         CO2_SAMPLES          (5)     //define how many samples you are going to take in normal operation

unsigned long previousMillis = 0;    // Til timeren. En timer-funktion er oprettet, hvor millis hentes fra
int CO2_counter = 0;                 // Bruges i funktionen readCO2_volts, hvor der skal foretages antallet CO2_SAMPLES målinger
float CO2_volts = 0;                 // Spændingen fra sensoren i volt

void readCO2_volts(struct sensorType* ptr)
{
  bool isDataCollected = false;
  while(!isDataCollected)
  {
    if (timer() - previousMillis >= CO2_INTERVAL / CO2_SAMPLES) // Timer der gør, at hele funktionen er 1 sekund om at blive helt gennemført
    {
      CO2_volts = CO2_volts + analogRead(CO2_PIN);      // Der måles en værdi på analogpinnen
      previousMillis = timer();                         // rpeviousMillis værdien opdateres til den nuværende millis værdi
      CO2_counter++;                                    // En måling er foretaget, derfor tælles der op.
  
      if (CO2_counter == CO2_SAMPLES)                   // Hvis counteren er lig med antallet af målinger der skal foretages, køres følgende if-statement
      {
        CO2_volts = (CO2_volts * 5.0 / 1024);             // Omregning fra analogsignal til volt
        (*ptr).co2Data = round(CO2_voltsToPpm(CO2_volts));                      // Funktionen der skal omregne fro volt til ppm køres, med målte volt som parameter
        CO2_counter = 0; // Counteren nulstilles
        CO2_volts = 0;
        isDataCollected = true;
      }                       
    }
  }
}
float CO2_voltsToPpm(float volts)                 // Funktionen køres, med parametren fra readCO2_volts
{
  float a, b, ppm;

  a = 0.9996; 
  b = 4.0658;

  float loga = -0.000194; //Log10(a), tallet er så lavt at arduino opfatter det som -0.0

  ppm = (log10(volts / CO2_SAMPLES) - log10(b)) / loga; // For at finde ppm værdien i den eksponentielle funktion f(x) = b*a^x
  /*Serial.print(ppm); Serial.println(" ppm"); // Værdierne printes
  Serial.println(volts / CO2_SAMPLES);*/

  return ppm;
}

unsigned long timer() // FUnktion til at opdatere currentMillis
{
  unsigned long currentMillis = millis();
  return currentMillis;
}


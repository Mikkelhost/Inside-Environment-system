void GSM_setup()
{
  int startTime = millis();
  bool turnedOn = false;
  serialFlush();
  sim900.print("AT\r\n");
  while(!turnedOn) 
  {
    if(millis() - startTime > 3000) 
    {
      Serial.println("Didn't find OK turning on module");
      break;
    }
    if(sim900.available())
    {
      if(sim900.find("OK"))
      {
        Serial.println("Found OK not turning on module");
        turnedOn = true;
      }
    }
  }
  serialFlush();
  if(!turnedOn)
  {
    pinMode(9, OUTPUT);
    digitalWrite(9, LOW);
    delay(1000);
    digitalWrite(9, HIGH);
    delay(2000);
    digitalWrite(9, LOW);
    delay(3000);
  }
  sim900.print("AT+CMGF=1\r\n");
  delay(1000);
}
void restartGSM()
{
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(6000);
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(3000);
}
void serialFlush(){
  while(sim900.available() > 0) {
    sim900.read();
  }
} 

void checkConnectionStatus()
{
  bool connection = false;
  String connectionVal = "Ingenting";
  while(!connection)
  {
    sim900.print("AT+CREG?\r\n");
    delay(500);
    if(sim900.available())
    {
      if(sim900.find("+CREG"))
      {
      Serial.println("CREG FOUND"); 
      connectionVal = sim900.readStringUntil("OK");
      int indexOfSignal = connectionVal.indexOf(": ");
      connectionVal = connectionVal.substring(indexOfSignal+4,indexOfSignal+5);
      Serial.print("Connection status is: "); Serial.println(connectionVal.toInt());
      serialFlush();
      }
    }

    if(connectionVal.toInt() == 1)
    {
      Serial.println("Connected to network"); Serial.println();
      connection = true;
    }
    else if(connectionVal.toInt() == 0)
    {
      Serial.println("Not registered to network, not trying to. Restarting module"); Serial.println();
      restartGSM();        
    }
    else if(connectionVal.toInt() == 2)
    {
      Serial.println("Not registered to network, trying to connect..."); Serial.println();
    }
    delay(1000);
  }
  connection = false;
}

void GSM_sendDataPackage(int antalPakker) 
{ 
  int dataNummer = 0;
  bool found = false;
  
  
  for(int i = 0; i < antalPakker; i++)
  {
    Serial.println("Checking if module is ready"); Serial.println(); 
    
    while(i != 0 && !found)
    {
      if(sim900.available())
      {
        if(sim900.find("OK")) found = true; 
      }
      
    }
    delay(1000);
    serialFlush();
    checkConnectionStatus(); 
    serialFlush();
    found = false;
    Serial.println("Setting up sms for 51184194");
    sim900.print("AT+CMGS=\"51184194\"\r\n"); // lav alle Serial om til sim900
    delay(1000);// Sim skal bruge det

    Serial.println("Sending sms with data packages");
    
    sim900.print(String(pakkeNummer)+'|');    
    for (int p = 0; p < 4; p++) {
      sim900.print(sensorData[dataNummer].timeData);
      delay(50);
      sim900.print('\r');
      delay(100);
      sim900.print('|' + String(sensorData[dataNummer].soundData) + '|' + String(sensorData[dataNummer].co2Data) + '|' + 
                   String(sensorData[dataNummer].tempData) + '|' + String(sensorData[dataNummer].humidData) + '\r' + '\n');   //sortKasse.soundData[dataNummer] i stedet for millis
      dataNummer++;
      delay(100);
    }
    
    sim900.write((char)26);
    Serial.println("Sms Sent");
    pakkeNummer++;    
  }
  pakkeNummer = 1; 
}

String dowStr;
bool Century = false;
bool h12 = false;
bool PM = false;

byte dow, hour, minute, second, year, month, date;
void displayTime()
{
  getAndSaveTime(&(sensorData[index]));
  Serial.println(sensorData[index].timeData);
}
void byte2Ascii(char charArray[], int i, byte data)
{
  if(data < 10)
  {
    charArray[i] = 48;
    charArray[i+1] = data + 48;
    charArray[12] = NULL;
  }
  else
  {
    charArray[i] = data/10+48;
    charArray[i+1] = (data - (data/10)*10)+48;
    charArray[12] = NULL;
  }
}
void getAndSaveTime(struct sensorType* ptr)
{
  dow = rtcModule.getDoW();
  hour = rtcModule.getHour(h12, PM);
  minute = rtcModule.getMinute();
  second = rtcModule.getSecond();
  year = rtcModule.getYear();
  month = rtcModule.getMonth(Century);
  date = rtcModule.getDate();
  byte2Ascii((*ptr).timeData, 0, date);
  byte2Ascii((*ptr).timeData, 2, month);
  byte2Ascii((*ptr).timeData, 4, year);
  byte2Ascii((*ptr).timeData, 6, hour);
  byte2Ascii((*ptr).timeData, 8, minute);
  byte2Ascii((*ptr).timeData, 10, second);
  
  /*switch(dow)
  {
    case 1:
      dowStr = "Mandag";
      break;
    case 2:
      dowStr = "Tirsdag";
      break;
    case 3: 
      dowStr = "Onsdag";
      break;
    case 4:
      dowStr = "Torsdag";
      break;
    case 5:
      dowStr = "Fredag";
      break;
    case 6:
      dowStr = "Loerdag";
      break;
    case 7:
      dowStr = "Soendag";
      break;
  }
  Serial.println(dowStr + " " + String(date) + "-" + String(month) + "-" + String(year) + " " + String(hour) + ":" + String(minute) + ":" + String(second));
  if(wait)
  {
    delay(1000);
  }*/
}

void setTime(byte& yearSet, byte& monthSet, byte& daySet, byte& dowSet, 
    byte& hourSet, byte& minuteSet, byte& secondSet) {
  // Call this if you notice something coming in on 
  // the serial port. The stuff coming in should be in 
  // the order YYMMDDwHHMMSS, with an 'x' at the end.
  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[20];
  Serial.println("the time should be in this order YYMMDDwHHMMSS, with an 'x' at the end.");
  byte j=0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
    }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] -48;
  Temp2 = (byte)InString[1] -48;
  yearSet = Temp1*10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] -48;
  Temp2 = (byte)InString[3] -48;
  monthSet = Temp1*10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] -48;
  Temp2 = (byte)InString[5] -48;
  daySet = Temp1*10 + Temp2;
  // now Day of Week
  dowSet = (byte)InString[6] - 48;   
  // now Hour
  Temp1 = (byte)InString[7] -48;
  Temp2 = (byte)InString[8] -48;
  hourSet = Temp1*10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] -48;
  Temp2 = (byte)InString[10] -48;
  minuteSet = Temp1*10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] -48;
  Temp2 = (byte)InString[12] -48;
  secondSet = Temp1*10 + Temp2;

  rtcModule.setYear(yearSet);
  rtcModule.setMonth(monthSet);
  rtcModule.setDate(daySet);
  rtcModule.setDoW(dowSet);
  rtcModule.setHour(hourSet);
  rtcModule.setMinute(minuteSet);
  rtcModule.setSecond(secondSet);
}

void rtcSetup()
{
  rtcModule.setClockMode(h12);
  int timeOut = millis();
  char userInput;
  while(true)
  {
    Serial.println("Do you want to set the time?");
    Serial.println("press y and enter for yes, wait 5 seconds to skip");
    while(!Serial.available() && millis() - timeOut < 5000)// ! = not. Vent på input
    {
      
    }
    if(Serial.available())
    {
      userInput = Serial.read();
    }
    else
    {
      userInput = '0';
    }
  
    if(userInput == 'y')// eventuelt lav til switch case
    {
      setTime(yearSet, monthSet, daySet, dowSet, hourSet, minuteSet, secondSet);
      break;
    }
    else if(userInput == 'n')
    {
      Serial.println("You chose not to set the time");
      break;
    }
    else if(userInput == '0')
    {
      Serial.println ( "Serial communication timed out");
      Serial.println ("Using last known time");
      break;
    }
    else
    {
      Serial.println ("You did not use the right command please try again");
    }
  }
}



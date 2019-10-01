void dhtOpsamleDataRef(struct sensorType* ptr) {

  int chk = DHT.read11(DHT11_PIN);
  (*ptr).tempData = round(DHT.temperature);
  (*ptr).humidData = round(DHT.humidity);
  delay(1000);
}

int interval = 5; // Minuten tussen elke check van de wind
int intervalMillis = interval * 60000; //reken de minuten om naar miliseconden
long previousMillis = 99999999;

void checkMetItervalString() {
  unsigned long currentMillis = millis();
  int8_t uur = (hour());

  //  check om de zoveel minuten of er een update beschikbaar is
  if (currentMillis - previousMillis >= intervalMillis)   {
    windString = getWindString(server);
    Serial.print("De windstring van de server om: ");
    Serial.print (hour());
    Serial.print (":");
    Serial.println (minute());
    MQTT_debug.publish("wind ophalen");
    MQTT_debug.publish("NTP Tijdserver geeft dit uur: ");
    MQTT_debug.publish(uur);


    previousMillis = millis();
    if  (oudeWindString != windString ) {
      Serial.print ("Nieuwe winddata in de string gevonden om: ");
      MQTT_debug.publish("--------- Nieuwe winddata !! ");
      verseWind.publish(1); //stuur een waarde naar de MQTT van adafruit om bij te houden wanneer de updates zijn

      stringNaarArrays(windString);
      oudeWindString = windString;

      // updateGalore();


    } else {
      Serial.print ("...  nog geen verse wind ");
    //  MQTT_debug.publish("...  nog geen verse wind om ");// stuur dit naar de MQTT debug
      verseWind.publish(0); // stuur dat er geen verse update is naar Adafruit IO MQTT
    }
  }
}

// ----------------- Functie voor het binnenhalen van de wind string
String getWindString(String server) {
  String url = server;
  Serial.println("Loading: " + url);
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String windString = http.getString();
      delay(200);
      return windString;
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: % s\n", http.errorToString(httpCode).c_str());
    http.end();
    delay(500);
    return "Niks";
  }
}



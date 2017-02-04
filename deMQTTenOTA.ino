

/// **********---- OTA SEtUP SERVER DINGEN ------////
void OTAserver() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


/// ****** MQTT_connect switch zaken *********
void OTAenMQTT() {
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(500))) {// getal is aantal millis tussen de lees en schrijf acties
    if (subscription == &onoffbutton) {
      uint16_t num = atoi((char *)onoffbutton.lastread);
      updateOTA = num;
    }
  }  
  if (updateOTA == 1) {
    // OTA mode nu kan je updaten over Air
    ArduinoOTA.handle();
    colorWipe(strip.Color(200, 0, 0), 0);
    delay(20);
   colorWipe(strip.Color(0, 0, 200), 0);
    delay(20);
    Serial.println("OTA mode");
  } else   {
    ///// //////////////////////////////////////////////////////
    //////   ---------- DE LOOP Van WINDCHECK ---------  ///////
    /// Als hij niet in OTA modus zit voert hij deze functies al loopend uit

    checkMetItervalString(); // -- af en toe de string van de server halen

    // -- ledhelderheid instellen
    unsigned int helderheid = map(analogRead(A0), 1, 700, 3, 100);
    strip.setBrightness(helderheid);
    strip.show();
    
    verstrekenLedsUit(hour()); // -- Leds uitzetten die al voorbij zijn
  }
 // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--; // hij probeert het 3x, als het dan niet lukt dan niet
    if (retries == 0) {
    //Als er geen verbinding komt met de MQTT laat dan maar dit zien
   Serial.println("MQTT connection failed");
    }
  }
  Serial.println("MQTT Connected!");
}

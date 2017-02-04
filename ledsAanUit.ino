int ledPositie;
int windArrayOffset;

void colorWipe(uint32_t kleur, uint8_t pixel) {
  strip.setPixelColor(pixel, kleur);
  strip.show();
}

// ---------------- de juiste led de juiste kleur
void geefLicht() {
  int temp = 0;
  int ledPositie;
  //de hele ledstrip leegmaken
  for (int x = 0; x < itemsInArray; x++ ) {
    colorWipe(strip.Color(0, 0, 0), x);
  }

  // Het aantal items in de Array delen door 2 omdat wind en tijd ze tegelijk worden
  // -1 omdat het laaste item in de Array de updatetijd bevat
  for (int x = 0; x < (itemsInArray / 2) - 1; x++ ) {
    // data naar de terminal
    Serial.print("Op dit uur");
    Serial.print(windArray[x]);
    Serial.print(" zijn er deze knopen:");

    // variabele met de positie van de winddata in de data array
    int windArrayPos = x + startWindData;


    // ********* verschuif het beginpunt van de led's afhankelijk van de eerste waarde of van de tijd
    // voor 20:00 uur alleen de startled verschuiven (waar eerste waarde van array in komt)
    if (hour() > 8 && hour() < 19) { //
      windArrayOffset = (windArray[0] - 10); //10 uur is de starttijd is, dus bij 10:00 is led 0 de startled

      // tussen 20:00 en 8 uur: een hele dag verschuiven (dus Ledpositie 0 is voorspelling van 10:00 de volgende dag
    } else {
      windArrayOffset = (windArray[0] - 23); //als 10uur eerste tijd in array is dan is de startled 13 (dus volgende dag)
    }
    ledPositie = x + windArrayOffset;

    Serial.print(windArray[windArrayPos]);
    Serial.print("    Ledpositie: ");
    Serial.println(ledPositie);
    // ****** Stel de ledkleuren in ************
    if (windArray[windArrayPos] <= 14) {
      colorWipe(strip.Color(0, 0, 255), ledPositie);// 13 knopen is kansloos, blauw
    } else if (windArray[windArrayPos] <= 17) {
      colorWipe(strip.Color(0, 255, 0), ledPositie);// 14kn t/m 17kn, groen = Laf Kiten
    } else if (windArray[windArrayPos] <= 20) {
      colorWipe(strip.Color(255, 255, 0), ledPositie);// 18kn t/m 20kn, Oranje = Vol kiten!
    } else if (windArray[windArrayPos] <= 29) {
      colorWipe(strip.Color(250, 0, 0), ledPositie);// 20kn t/m 29kn, rood = Windsurfen!
    } else {
      colorWipe(strip.Color(250, 0, 225), ledPositie); //vanaf 30kn, paars = Beuken!!!!
    }
  }
}

// ************  zet de LED's van de verstreken tijd uit ***********
void verstrekenLedsUit(int uur) {
  for (int x = 0; x < (23 - windArray[0]); x++ ) { // alleen de eerste 13leds uitzetten als nodig
    if (uur >= windArray[x]) {
      colorWipe(strip.Color(0, 0, 0), x + windArrayOffset);
      /*
            Serial.print("Tijd in de array: ");
            Serial.print(windArray[x] );
            Serial.print(" uur: ");
            Serial.print(uur);
            Serial.print(" Ledpositie: ");
            Serial.println(x + windArrayOffset);
      */

    }

  }

}

//  ************  er is een update dus de galore lampjes aanzetten
void updateGalore() {
  strip.setBrightness(200);
  for (int x = 0; x < 40; x++ ) {
    colorWipe(strip.Color(255, 0, 0), x);
    delay (20);
  }
  for (int x = 0; x < 40; x++ ) {
    colorWipe(strip.Color(0, 255, 0), x);
    delay (20);
  }
  for (int x = 0; x < 40; x++ ) {
    colorWipe(strip.Color(0, 0, 255), x);
    delay (20);
  }
}



// ---------  Stop de data uit de string in een Array en kijk waar de tijden zitten en waar de wind zit in deze array
void stringNaarArrays(String windString) {
  String temp = "";
  String thischar = "";
  int item = 0;
  //bekijk hoe lang de string is en loop er zo lang door heen
  int lengteString = windString.length();
  for (int i = 0; i < lengteString; i++ ) {
    thischar = windString.substring(i, i + 1);
    //Aan het einde van de string zit een Z (in PHP toegevoegd) zodat het helder is hoeveel entrys in de Array zitten
    if  (thischar == "Z") {
      itemsInArray = item;
    } else if  (thischar == ",") {
      item = item + 1;
      temp = "";
      // _ ___ overgang van TIJD naar WIND
    } else if (thischar == "_") {
      item = item + 1;
      temp = "";
      startWindData = item + 1;     // dit nummer opslaan zodat ik weet waar de WindData begint in de  Array met wind
      // alles in de array beuken
    } else {
      temp += thischar;
      windArray[item] = temp.toInt();
    }
  }
  geefLicht() ;
}

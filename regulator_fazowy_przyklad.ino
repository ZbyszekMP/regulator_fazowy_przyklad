/*
 Opis dzialania:
 na pocztku czekamy na przerwanie od zerePin czyli przejscia przez zero wywolywana jest w tedy funkcja zeroCross
 w ktorej ustawimy wartosc poczotkowa timertowi i wlaczmy przerwanie ( wydaje mi sie ze wlaczanie i wylaczanie przerwania mozna usunąc)
 Co do timera to dzila to mniej wicej tak ze do myslnie liczy on do 2^16 = 65536 przy czestotliwosci 16MHz liczy 16 milionow na sekunde wychodzi z tego ze jeden cykl timera to jakies 4ms.
 Ustawimy prescaler na 8 czyli dzielimy czestotliwowsc 16Mhz / 8  = 2Mhz i stawimy wywolanie przerwania przy 20000. Wynika to ztego ze zegar osiagnie 20000 po 10ms a akurat tyle poniewaz jest to czas miedzy zerami 
 w sinusie przy 50Hz (tak jest w gniazdku)
 2 000 000 = 1000ms | /100
 20 000 = 10ms
Gdy timer osiagnie 20000 wywoluje funkcje ISR ktora wlacza zarowke.

Teraz jak sterowac jasnoscia wystarczy zmiana wartosci zmiennej 'w' tylko trzaba zakomentowac linie 43 czyli przykladowe dzialanie
 jesli ustawimy timerowi wartosc pocztkowa 0 (czyli ustawimy zmienna w na 0 ) oznaczo to ze tiemer liczy od 0 do 20000 i zajmie mu to 10 ms czyli w kolejnym zerze wiec zarowka sie nie zaswieci
 kiedy ustawimy 20000 to zarowka zostanie wlaczona odrazu czyli będzie swiecic pelna moca
 a gdy utawimy np. 10000 to zarowka zostanie wlaczona po 5ms wiec powinna swiecic polowa mocy

 
 
Jak cos to w interneci znajdziesz pod nazwa 'regulator fazowy arduino' lub ' sciemniacz fazowy arduino' cos takiego pewnie
 
 */

int zeroPin=2;//Pin do wykrywania przejscia przez zero(ZeroCross) koniecznie 2 lub 3 tylko te pincy obsluguja przerwania
int sPin = 3; // Pin d owlaczania triaka tu w zasadzie dowolny pin
int w = 1000; // to moment wlaczenia triaka wartosci od 0 do 20000 gdzie 20000 to pelna jasnosc a 0 to wylaczona zarowka
bool p = true; // to tylko pomocniczna zmienna jej stana odpowiada temu czy zarowka sie zorjasnia czy zciemnia. Jesto tylko potrzebne do dzialania funkcji example 

//Przykladowe dzialanie
void example(){
  if (p){
    w = w+10;
  }
  else{
    w = w-10;
  }
  if (w<=1000 || w >= 19000){ 
    p = !p;
  }
}
//Funkacja wywolywana przerwaniem z pinu zeroPin czyli  w momencie przejscia sinusa przez zero
void zeroCross() {
 digitalWrite(sPin,LOW);//Wylaczenie triaka czyli z zasadzie zarowki
 example()// wywolanie funkju przykladowej  zmieniajaca jasnos prze zmiane wartosci zmienne w 
 TCNT1 = w;// przypisanie wartosci startowej zegarowi 
 TIMSK1 |= (1 << OCIE1A);//wlaczenie przerwania od zegara w tym przypadku zawsze przy 20000 
 
}
void setup() {
  pinMode(zeroPin,INPUT);
  pinMode(sPin, OUTPUT);
  noInterrupts();           // Wyłączamy przerwania, by bezpiecznie zmienić ustawienia
  TCCR1A = 0;               // Ustawiamy rejestr kontrolny A
  TCCR1B = 0;               // Ustawiamy rejestr kontrolny B
  TCNT1 = 0;                // Zerujemy licznik
  // Ustawiamy preskaler (dzielnik) - wybor dla Timer1
  // Tu ustawiamy preskaler na 8
  TCCR1B |= B00000010;    
  // Ustawiamy wartość porównania - przerwanie co 10ms bo tyle wynosi pol okresu przy 50Hz
  OCR1A = 20000;               // 16MHz / 8 / 100 = 20000
  interrupts();// Ponowne wlaczenie przerwan 
  attachInterrupt(digitalPinToInterrupt(zeroPin), zeroCross, FALLING); /*Przerwanie od pinu wykrywającego przejscie przez zero w arduino uno i nano przewania obsluguja tylko piny D2,D3 !!! 
  Wywoluje funkcje zeroCross
  FALLING oznacza ze zmiana jest ze stanu wysokiego na niski 
  */
  
}
//Funkcja wywolywana przerwaniem od zegara
ISR(TIMER1_COMPA_vect) {  
        
   TIMSK1 &= ~(1 << OCIE1A);//Wylacznie zegara
   digitalWrite(sPin,HIGH);//Wlaczenie triaka czy w zasadzie zarowki
 
}
void loop() {
   
}

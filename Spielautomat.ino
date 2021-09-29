
#include <stdint.h>

#define DELAY 250
#define BATT_DELAY 60000
#define INIT_DELAY 60000

#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define UP 5
#define CODE 6
#define BATT 7
#define ENTER 8

#define POWER 12

char pin_string[8];

void pressBtn(uint8_t btn) {
  digitalWrite(btn, LOW);
  delay(DELAY);
  digitalWrite(btn, HIGH);
  delay(DELAY);
}

void pressBtnN(uint8_t btn, uint8_t n) {
  for(uint8_t i = 0; i < n; i++) {
    pressBtn(btn);
  }
}

void dialCode(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  // Code anzeigen
  digitalWrite(UP, LOW);
  
  pressBtnN(DOWN, a);
  pressBtn(LEFT);

  pressBtnN(DOWN, b);
  pressBtn(LEFT);

  pressBtnN(DOWN, c);
  pressBtn(LEFT);

  pressBtnN(DOWN, d);

  delay(500);
  digitalWrite(UP, HIGH);
  delay(500);
  pressBtn(ENTER);
}

void setup() {
  // Erste Relais Active Low
  for(uint8_t i = 2; i <= 8; i++) {
    digitalWrite(i, HIGH);
    pinMode(i, OUTPUT);
  }
  // Enable Relais
  pinMode(POWER, HIGH);

  Serial.begin(9600);

  Serial.setTimeout(1000 * 60 * 60);
}

// the loop function runs over and over again forever
void loop() {
  Serial.flush();
  Serial.println("PIN?");

  if(Serial.readBytesUntil(';', pin_string, sizeof(pin_string)) == 0) return;

  int pin = atoi(pin_string);
  // Pin zerlegen
  int a = (pin / 1000) % 10;
  int b = (pin / 100) % 10;
  int c = (pin / 10) % 10;
  int d = (pin / 1) % 10;

  Serial.print("DIAL ");

  Serial.print(a);
  Serial.print(b);
  Serial.print(c);
  Serial.print(d);
  Serial.println();
  
  // Initialisierung
  // Batterie Reset
  digitalWrite(BATT, LOW);
  delay(BATT_DELAY);
  digitalWrite(BATT, HIGH);
  delay(1000);

  // Kiste an
  digitalWrite(POWER, HIGH);
  delay(INIT_DELAY);

  // Sequenz
  pressBtn(CODE);

  // Code Eingabe
  dialCode(a, b, c, d);

  delay(1000);

  Serial.println("OK");
  delay(5000);

  // Kiste aus
  digitalWrite(POWER, LOW);
  
 
}

#define PIN_LED 7
int cnt=0;
void setup() {
  pinMode(PIN_LED, OUTPUT);
  while (!Serial){
    ;
  }
}

void loop() {
  if (cnt==0){
    digitalWrite(PIN_LED, 0);
    delay(1000);
    cnt++;
  }
  if (cnt<=5){
    digitalWrite(PIN_LED, 1);
    delay(100);
    digitalWrite(PIN_LED, 0);
    delay(100);
    cnt++;
  }
  if (cnt>5){
    digitalWrite(PIN_LED, 1);
    while (1){;}
  }
}

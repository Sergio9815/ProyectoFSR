/*CONTROL DE LAS LUCES*/

//PINES DE ENTRADA
int pinV = 6;
int pinR = 7;

//VARIABLES DE LECTURA
int valorV;
int valorR;

//PINES DE LEDS
int ledR = 9;
int ledV = 10;

void setup() {
  pinMode(pinV, INPUT);
  pinMode(pinR, INPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
}

void loop() {
  valorR = digitalRead(pinR);
  valorV = digitalRead(pinV);

  if(valorR == HIGH){//ENCENDER LED ROJA
    digitalWrite(ledR, HIGH);
    digitalWrite(ledV, LOW);
  }
  if(valorV == HIGH){//ENCENDER LED VERDE
    digitalWrite(ledR, LOW);
    digitalWrite(ledV, HIGH);
  }
  
}

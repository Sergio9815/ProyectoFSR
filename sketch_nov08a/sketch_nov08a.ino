#include <Servo.h>

//PROYECTO FINAL SISTEMAS ROBÓTICOS

//Movimientos
#define ADELANTE  {digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}   
#define ATRAS     {digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}                         
#define GIROI     {digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}   
#define GIROD     {digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}  
#define IZQUIERDA {digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,HIGH);}        
#define DERECHA   {digitalWrite(IN1,HIGH);digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}       
#define ALTO      {digitalWrite(IN1,LOW);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,LOW);} 
   
#define TRIGGER_PIN  A4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A5  // Arduino pin tied to echo pin on the ultrasonic sensor.

//MOTORES
int VelocidadMotor1 = 5; //ENA
int VelocidadMotor2 = 6; //ENB
int IN1 = 7;                
int IN2 = 8;                 
int IN3 = 12;                
int IN4 = 13;   

//SENSOR ULTRASÓNICO
int Echo = A5;                   
int Trig = A4;                  
int dis = 0;

//VARIABLES DE BÚSQUEDA CON SERVO
boolean go = false;
boolean ok;
boolean ok2;
boolean ok3;

//VARIBALES PARA LAS FASES
boolean fase1 = true;
boolean fase2 = false;


//SERVO MOTORES
Servo myservo, servoA;  // crea el objeto servo

//LEDS
int pinVerde = 11;
int pinRojo = 2;

void setup()
{  
    pinMode(pinVerde, OUTPUT);
    pinMode(pinRojo, OUTPUT);
    myservo.attach(9);  // vincula el servo al pin digital 9
    servoA.attach(10); //PINZA DERECHA
    myservo.write(35); //POSICION INICIAL DEL SERVO PARA EL SENSOR
    servoA.write(0); // POSICION INICIAL DEL SERVO PARA LA PINZA
    Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
    pinMode(ECHO_PIN , INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(VelocidadMotor1, OUTPUT);
    pinMode(VelocidadMotor2, OUTPUT);
    pinMode(IN1,OUTPUT);            
    pinMode(IN2,OUTPUT);          
    pinMode(IN3,OUTPUT);          
    pinMode(IN4,OUTPUT);         
    analogWrite(VelocidadMotor1, 100); //motor derecho
    analogWrite(VelocidadMotor2, 100);  //motor izquierdo
}

void loop()
{  
    dis = getDistance();

    if(fase2 == false){
            Serial.println("FASE 1");  

            if((dis < 10 || go == false))
            {
                digitalWrite(pinVerde, LOW);
                digitalWrite(pinRojo, HIGH);//MANTIENE EL LED ROJO ENCENDIDO HASTA QUE ENCUENTRE UN OBJETO
                Serial.print("Fase DETENER: ");
                Serial.print(dis); // Send ping, get distance in cm and print result (0 = outside set distance range)
                Serial.println(" cm");  
                ALTO;  
                  
                if(fase1 == true){
                  
                      ok = search1();
                      ok2 = search2();
                      ok3 = search3();
              
                      if(ok == true){
                        Serial.print("ENCONTRADO OK1");
                        //digitalWrite(pinVerde, LOW);
                        myservo.write(35); 
                        ALTO;  
                        delay(1000);
                        GIROD;
                        delay(250);
                        go = true;
                        fase1 = false;
                      }
                      else if(ok2 == true){
                        Serial.print("ENCONTRADO OK2");
                        myservo.write(35);  
                        ALTO;  
                        delay(2000);
                        go = true;
                        fase1 = false;
                      }
                      else if(ok3 == true){       
                        Serial.print("ENCONTRADO OK3");
                        myservo.write(35);
                        ALTO;  
                        delay(1000);     
                        GIROI;    
                        delay(250);
                        go = true;
                        fase1 = false;
                      }
                      else{
                        go = false;
                      }
               
                      if(go == false)
                      {
                        myservo.write(35);
                        ALTO;  
                        delay(2000);
                        IZQUIERDA;
                        delay(2000);
                      }
                }
                else{
                  Serial.println("PINZA OK");  
                  delay(2000);
                  servoA.write(30); // Mueve la pinza izquierda
                  delay(2000);
                  fase2 = true;
                }
                
             }
             else if(go == true && dis > 10 && dis < 100)
            {
                digitalWrite(pinVerde, HIGH);
                digitalWrite(pinRojo, LOW);
                Serial.print("Fase ObjetoEncontrado: ");
                Serial.print(dis); // Send ping, get distance in cm and print result (0 = outside set distance range)
                Serial.println(" cm");    
                ADELANTE;             
            }
    }
    
    
    if(fase2 == true) {
      Serial.println("FASE 2");  
        digitalWrite(pinVerde, HIGH);
        digitalWrite(pinRojo, LOW);
        GIROD;
        delay(2000);
        digitalWrite(pinVerde, LOW);
        digitalWrite(pinRojo, HIGH);
        GIROI;
        delay(1000);
        digitalWrite(pinVerde, HIGH);
        digitalWrite(pinRojo, LOW);
        GIROD;
        delay(1000);
        digitalWrite(pinVerde, LOW);
        digitalWrite(pinRojo, HIGH);
        GIROD;
        delay(1000);
        digitalWrite(pinVerde, HIGH);
        digitalWrite(pinRojo, LOW);
        GIROI;
        delay(1000);
        digitalWrite(pinVerde, LOW);
        digitalWrite(pinRojo, HIGH);
        ADELANTE;            
        delay(2000);
      }

 }

boolean search1(){
  myservo.write(-20);  ;          
  delay(3000);
  dis = getDistance();
 /* dis = getDistance();
  dis = getDistance();
  dis = getDistance();
  dis = getDistance();
  dis = getDistance();*/

  if(dis > 20 && dis < 100){
    Serial.print("ENCONTRADO SEARCH1");
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinRojo, LOW);
    return true;
  }
  else{
    return false;
  }
}

boolean search2(){
  myservo.write(35);  ;          
  delay(3000);
  dis = getDistance();
 /* dis = getDistance();
  dis = getDistance();
  dis = getDistance();
  dis = getDistance();
  dis = getDistance();*/

  if(dis > 20 && dis < 100){
    Serial.print("ENCONTRADO SEARCH2");
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinRojo, LOW);
    return true;
  }
  else{
    return false;
  }
}


boolean search3(){
  myservo.write(80);  ;          
  delay(3000);
  dis = getDistance();
 /* dis = getDistance();
  dis = getDistance();
  dis = getDistance();
  dis = getDistance();
  dis = getDistance();*/

  if(dis > 20 && dis < 100){
    Serial.print("ENCONTRADO SEARCH3");
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinRojo, LOW);
    return true;
  }
  else{
    return false;
  }
}

//OBTENER DISTANCIA
int getDistance(){
    // defines variables
    long duration;
    int distance;
    // Clears the trigPin
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    return distance;
}

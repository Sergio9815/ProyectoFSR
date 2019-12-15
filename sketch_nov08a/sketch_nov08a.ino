#include <Servo.h>

//PROYECTO FINAL SISTEMAS ROBÓTICOS

//Movimientos
#define ADELANTE  {digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}   
#define ATRAS     {digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}                         
#define GIROI     {digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,LOW);digitalWrite(IN4,HIGH);}   
#define GIROD     {digitalWrite(IN1,HIGH);digitalWrite(IN2,LOW);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}  
#define IZQUIERDA {digitalWrite(IN1,LOW);digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,HIGH);}        
#define DERECHA   {digitalWrite(IN1,HIGH);digitalWrite(IN2,HIGH);digitalWrite(IN3,HIGH);digitalWrite(IN4,LOW);}       
#define ALTO      {digitalWrite(IN1,LOW);digitalWrite(IN2,LOW);digitalWrite(IN3,LOW);digitalWrite(IN4,LOW);} 
 
//MOTORES
int VelocidadMotor1 = 5; //ENA
int VelocidadMotor2 = 6; //ENB
int IN1 = 7;                
int IN2 = 8;                 
int IN3 = 12;                
int IN4 = 13;   

//SENSOR ULTRASÓNICO
#define TRIGGER_PIN  A4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A5  // Arduino pin tied to echo pin on the ultrasonic sensor.                 
int dis = 0;

//VARIABLES DE BÚSQUEDA CON SERVO
boolean go = false;
boolean ok = false;
boolean ok2 = false;
boolean ok3 = false;

//VARIBALES PARA LAS FASES
boolean fase1 = true;
boolean fase2 = false;
boolean fase3 = false;

//SERVO MOTORES
Servo myservo, servoA;  // crea el objeto servo

//LEDS
int pinVerde = A1;
int pinRojo = A2;

//INFRARROJO
int infraIZQ = 2;    // izquierdo - pin del infrarrojos utilizado como entrada digital
int infraDER = 11;  //  derecho
int valorInfraI = 0;  // Valor inicial de la lectura digital del infrarrojos izquierdo 
int valorInfraD = 0;  // derecho
boolean gD = false; 
boolean gI = false;
boolean detener = false;
boolean avanzar = false;
int contador = 0 ;

//INFRARROJO DETECTOR
int infraDET = A3;
int valorInfraDET = 0;

void setup()
{  
    pinMode(infraIZQ, INPUT);    
    pinMode(infraDER, INPUT); 
    pinMode(infraDET, INPUT); 
    pinMode(pinVerde, OUTPUT);
    pinMode(pinRojo, OUTPUT);
    myservo.attach(9);  // vincula el servo al pin digital 9
    servoA.attach(10); //PINZA DERECHA
    myservo.write(35); //POSICION INICIAL DEL SERVO PARA EL SENSOR
    servoA.write(45); // POSICION INICIAL DEL SERVO PARA LA PINZA
    Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
    pinMode(ECHO_PIN , INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(VelocidadMotor1, OUTPUT);
    pinMode(VelocidadMotor2, OUTPUT);
    pinMode(IN1,OUTPUT);            
    pinMode(IN2,OUTPUT);          
    pinMode(IN3,OUTPUT);          
    pinMode(IN4,OUTPUT);         
}

void loop()
{  
    dis = getDistance();
    onInfraR();
   
    if(fase2 == false && fase3 == false){
            analogWrite(VelocidadMotor1, 100); //motor derecho
            analogWrite(VelocidadMotor2, 100);  //motor izquierdo
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

                      ok2 = search2(); //GIRA EL SENSOR ULTRA HACIA EL FRENTE
                      if(ok2 == false){
                        ok = search1();  //GIRA EL SENSOR ULTRA HACIA LA DERECHA
                      }

                      if(ok == false){
                        ok3 = search3(); //GIRA EL SENSOR ULTRA HACIA LA IZQUIERDA
                      }
              
                      if(ok == true){
                        Serial.print("ENCONTRADO OK1");
                        //digitalWrite(pinVerde, LOW);
                        myservo.write(35); 
                        ALTO;  
                        delay(1000);
                        servoA.write(0);
                        delay(500);
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
                        servoA.write(0);
                        delay(500);
                        go = true;
                        fase1 = false;
                      }
                      else if(ok3 == true){       
                        Serial.print("ENCONTRADO OK3");
                        myservo.write(35);
                        ALTO;  
                        delay(1000); 
                        servoA.write(0);
                        delay(500);    
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
                  //onInfraR();
                  
                  //if(valorInfraDET == 1){
                  
                    delay(1000);
                    digitalWrite(pinVerde, HIGH);
                    digitalWrite(pinRojo, LOW);
                    Serial.println(valorInfraDET); 
                    servoA.write(70); // Mueve la pinza derecha
                    delay(2000);
                    fase2 = true;
                 /* }
                  else{
                    digitalWrite(pinVerde, LOW);
                    digitalWrite(pinRojo, HIGH);
                    ALTO;
                    delay(1000);
                    ATRAS;
                    delay(500);
                    GIROI;
                    delay(300);
                    fase1 = true;
                    go = false;
                  }*/
                }
                
             }
             else if(go == true && dis > 10 && dis < 50)
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
        analogWrite(VelocidadMotor1, 50); //motor derecho
        analogWrite(VelocidadMotor2, 50);  //motor izquierdo 
        digitalWrite(pinVerde, LOW);
        digitalWrite(pinRojo, HIGH);
        onInfraR();
        
            if(valorInfraI == 1 && valorInfraD == 0){
               analogWrite(VelocidadMotor1, 70); //motor derecho
               analogWrite(VelocidadMotor2, 70);  //motor izquierdo 
               GIROI;
               delay(1000);
               ALTO;
               delay(500);
               fase2 = false;
               fase3 = true;
            }
            
            if(valorInfraI == 0 && valorInfraD == 1){
               analogWrite(VelocidadMotor1, 70); //motor derecho
               analogWrite(VelocidadMotor2, 70);  //motor izquierdo 
               GIROD;
               delay(1000);
               ALTO;
               delay(500);
               fase2 = false;
               fase3 = true;
            }
            
            if(valorInfraI == 0 && valorInfraD == 0){ // Hacia delante
               ADELANTE;   
            }
      }
      
      if(fase3 == true){
            analogWrite(VelocidadMotor1, 70); //motor derecho
            analogWrite(VelocidadMotor2, 70);  //motor izquierdo 
            
            onInfraR();

            digitalWrite(pinVerde, HIGH);
            digitalWrite(pinRojo, HIGH);
            
             // 0 = blanco / 1 = negro
            if(valorInfraI == 0 && valorInfraD == 0){ // Hacia delante
              ADELANTE; 
              if(avanzar == true){
                contador++;
              }

              if(contador >= 5){
                detener = true;  
              }
            }
            
            if(valorInfraI == 0 && valorInfraD == 1){  // Giro derecha
              gD = true;
              gI = false;
              GIROD;
              avanzar = true;
            }
            
            if(valorInfraI == 1 && valorInfraD == 0){ // Giro izquierda
              gD = false;
              gI = true;
              GIROI;
              avanzar = true;
            }

            if(detener == true){
                if(valorInfraI == 1 && valorInfraD == 1){  // STOP
                  ALTO;   
                  delay(1000);
                  servoA.write(45); // POSICION INICIAL DEL SERVO PARA LA PINZA
                  delay(1000);
                  ATRAS;
                  delay(1000);
                  
                  if(gD == true){
                    GIROD;
                    delay(1000);
                  }
                  else if(gI == true){
                    GIROI;
                    delay(1000);
                  }
    
                  ADELANTE;
                  delay(1000);
                  
                  fase2 = false;
                  fase1 = true;
                  go = false;
                  ok = false;
                  ok2 = false;
                  ok3 = false;
                  fase3 = false;
                  detener = false;
                  avanzar = false;
                  contador = 0 ;
                }
            }
            //delay(50);
        }


 }


void onInfraR(){
   valorInfraI = digitalRead(infraIZQ);    // valor de la entrada que lee el infrarrojo izquierdo
   valorInfraD = digitalRead(infraDER);  // derecho
   valorInfraDET = digitalRead(infraDET); 
}

boolean search1(){
  myservo.write(-20);  ;          
  delay(3000);
  dis = getDistance();

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

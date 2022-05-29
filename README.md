#include <AFMotor.h> //https://www.arduino.cc/reference/en/libraries/adafruit-motor-shield-library/
#include <SoftwareSerial.h>

#define pinoLed 23
#define pinoBuzzer 46


SoftwareSerial BTSerial (2,3);
const byte MAX_STRING_LEN = 9;
char inputString[MAX_STRING_LEN];
byte strLen = 0;

AF_DCMotor motor(3);

int valueX;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Enter AT Commands");
  BTSerial.begin(9600);
  Serial1.begin(9600);
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
}

int valoresX(char string[]){
  int valor = 0, x=100, valorInt;
  char auxiliar;
  for (int i=0;i<3;i++){
    auxiliar = string[i];
    valorInt = auxiliar - '0';
    valor += valorInt*x;
    x/=10;
  }
  return valor;
  
}

void valoresY(char string[]){
  int valor = 0, x=100, valorInt;
  char auxiliar;
  for (int i=3;i<6;i++){
    auxiliar = string[i];
    valorInt = auxiliar - '0';
    valor += valorInt*x;
    x/=10;
  }
  valor=map(valor,0,200,-255,255);
  if (valor>0){
    motor.run(FORWARD);
    motor.setSpeed(valor);
  }
  else if (valor<0){
    motor.run(BACKWARD);
    motor.setSpeed(abs(valor));
  }
  else{
    motor.run(RELEASE);
  }
}

void valoresBotao(char string[]){
  int valor = 0, valorInt, cont=0;
  char auxiliar;
  for (int i=6;i<8;i++){
    auxiliar = string[i];
    valorInt = auxiliar - '0';
    if ((valorInt==1)&&(cont==0)){
      Serial.println("liga");
      digitalWrite(pinoLed, HIGH);
    }
    else if(cont == 0) {
      Serial.println("desligado");
      digitalWrite(pinoLed, LOW);
    }
    else if((cont==1)&&(valorInt==1)){
      tone (pinoBuzzer, 4000);
      Serial.println("liga tone");
    }
    
    else{
      noTone (pinoBuzzer);
      Serial.println("Desliga tone");
    }
    cont++;
  }

}

 
void loop() {
  // put your main code here, to run repeatedly:
  if (processSerial()) {
    // Received a complete string. For now, just echo it back
    Serial.println(inputString);
    Serial.print(valoresX(inputString));
    Serial.print(" - ");
    valoresY(inputString);
    valoresBotao(inputString);
    inputString[0] = '\0';         // Make sure array is empty
    strLen         = 0;            // reset length to zero
  }   
  
  if(Serial.available()){
    Serial1.write(Serial.read());
  }
}
 
boolean processSerial() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read(); 
 
    // We are done receiving the string if we received a return (or line feed)
    if ((inChar == '\n') || (inChar == '\r'))
      return true;
 
    // add it to the inputString if we have room
    if (strLen < (MAX_STRING_LEN - 1)) {
      inputString[strLen++] = inChar;
      inputString[strLen]   = '\0';
    }
  }
 
  return false;
}

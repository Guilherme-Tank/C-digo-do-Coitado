//Bibliotecas utilizadas
#include <AFMotor.h> //https://www.arduino.cc/reference/en/libraries/adafruit-motor-shield-library/
#include <Servo.h>

//Definicação dos pinos
#define pinoLed 45
#define pinoBuzzer 44
#define pinoPot A15

Servo servo; //Definicação do servo
AF_DCMotor motor(3); // Definicação do motor

const byte MAX_STRING_LEN = 9;
char inputString[MAX_STRING_LEN];
byte strLen = 0;
int pot, value, lastValor;
//Variaveis globais utilizadas

void setup() {
  //Comunicação Serial
  Serial.begin(9600);
  Serial1.begin(9600);
  //Definição dos modos dos pinos
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoPot, INPUT);
  servo.attach(9); //Definicação do pino do PWM do Servo Motor
}

//Função para utilização dos valores X
int valoresX(char string[]){
  int valor = 0, x=100, valorInt;
  char auxiliar;
  for (int i=0;i<3;i++){
    auxiliar = string[i];
    valorInt = auxiliar - '0';
    valor += valorInt*x;
    x/=10;
  }
  valor=map(valor, 0, 200, 50, 180);
  if(valor != lastValor)
    servo.write(valor);
  lastValor = valor;
  return valor;
  
}

//Função para utilização de valores Y
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

//Função para utilização dos valores dos botões X e Quadrado
void valoresBotao(char string[]){
  int valor = 0, valorInt, cont=0;
  char auxiliar;
  for (int i=6;i<8;i++){
    auxiliar = string[i];
    valorInt = auxiliar - '0';
    if ((valorInt==1)&&(cont==0)){
      digitalWrite(pinoLed, HIGH);
    }
    else if(cont == 0) {
      digitalWrite(pinoLed, LOW);
    }
    else if((cont==1)&&(valorInt==1)){
      tone (pinoBuzzer, 400);
    }
    
    else{
      noTone (pinoBuzzer);
    }
    cont++;
  }

}

void loop() {
  if (processSerial()) {
    //Apos receber uma string completa
    valoresY(inputString); //Função
    valoresBotao(inputString); //Função
    inputString[0] = '\0';         // Esvazia o array
    strLen         = 0;            // Reseta o tamanho da string para zero
  }   

  //Escrevendo para Serial1 (teste)
  if(Serial.available()){
    Serial1.write(Serial.read());
  }
}

//Função para ler uma string completa com um número de caracteres especifico
boolean processSerial() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read(); 
 
    // Caso receba um return ou line feed, acabou a string
    if ((inChar == '\n') || (inChar == '\r'))
      return true;
 
    // Coloque na string caso ainda tenha espaço
    if (strLen < (MAX_STRING_LEN - 1)) {
      inputString[strLen++] = inChar;
      inputString[strLen]   = '\0';
    }
  }

  //Diz que o processo ainda não acabou, porque a string inteira ainda não foi recebida
  return false;
}

/****************************************************************
GestureTest.ino
APDS-9960 RGB and Gesture Sensor
Shawn Hymel @ SparkFun Electronics
May 30, 2014
https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor

Conexões

Importante: O módulo APDS-9960 funciona somente com 3.3V!!!
 
Arduino Pin  APDS-9960 Board  Function
 
 3.3V            VCC            Power
 GND             GND            Ground
 A4              SDA            I2C Data
 A5              SCL            I2C Clock
 D2              INT            Interrupt
 
****************************************************************/

//Inclusão de bibliotecas
#include <Wire.h>
#include <SparkFun_APDS9960.h>

//Pinos
#define APDS9960_INT    2 // Needs to be an interrupt pin

//Variáveis globais
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup() {

  //Seta o pino de interrupção como entrada
  pinMode(APDS9960_INT, INPUT);

  //Inicializa a comunicação serial
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - Teste de Gestos"));
  Serial.println(F("--------------------------------"));
  
  //Iniciando rotina de interrupção
  attachInterrupt(0, interruptRoutine, FALLING);

  //Iniciando o módulo APDS-9960
  if ( apds.init() ) {
    Serial.println(F("APDS-9960: Inicialização completa!"));
  } else {
    Serial.println(F("APDS-9960: Algo deu errado durante a inicialização!"));
  }
  
  //Iniciando o sensor de gestos
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("O sensor de gestos está funcionando!"));
  } else {
    Serial.println(F("Algo deu errado durante a unidade do sensor de gestos!"));
  }
}

void loop() {
  
  //Lógica de interrupção
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

//Função responsável pela interrupção
void interruptRoutine() {
  isr_flag = 1;
}

//função responsável por identificar e retornar os resultados da leitura
void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("CIMA");
        break;
      case DIR_DOWN:
        Serial.println("BAIXO");
        break;
      case DIR_LEFT:
        Serial.println("ESQUERDA");
        break;
      case DIR_RIGHT:
        Serial.println("DIREITA");
        break;
      case DIR_NEAR:
        Serial.println("APROXIMANDO");
        break;
      case DIR_FAR:
        Serial.println("DISTANCIANDO");
        break;
      default:
        Serial.println("GESTO NÃO RECONHECIDO");
    }
  }
}
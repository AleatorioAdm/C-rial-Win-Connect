/*******************************************************
 * Nome do Programa: C-rial Win Connect Arduino Program
 * Descrição: Código para que o Arduino seja detectado no C-rial Win Connect
 * Data de Criação: 09/11/2023
 * Data de Lançamento: 17/11/2023
 * Autor: AleatorioAdm (Ademilson Filho)
 * Emails: aleatorioadm.dev@gmail.com /ademilsonfilho.dev@gmail.com
 * Versão: 0.6.1
 *******************************************************/
#include<String.h>

char receivedString[64];
bool detected = 0;
int counter1 = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    // Verifica a resposta do computador
  
  if (detected == 0){
    Serial.write("Identity:VIRTUAL_HEART_CONTROLLER_DEVICE",40);
    Serial.flush();
    delay(1000);
    Serial.readBytes(receivedString,19);
    
    if (strstr(receivedString, "VHC_DEVICE_DETECTED")){

      digitalWrite(LED_BUILTIN, HIGH);
      detected++;
    }
  } else {
  //programa aqui
    delay(1000);
    Serial.readBytes(receivedString,30);
    if (strstr(receivedString, "LED_TEST_PROTOCOL")){
      delay(100);
      Serial.write("Led Testing Protocol Started!",29);
     
      while(counter1 < 6){
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
        delay(1000);                      // wait for a second
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
        delay(1000);
        counter1++;
      }
      counter1 = 0;
      delay(100);
      Serial.write("Led Testing Protocol Done!",26);
    }

    if (strstr(receivedString, "VHC_DEVICE_NOT_DETECTED")){

      detected = 0;
    }

    sprintf(receivedString, "");
      
  }
  
}

#include <RF24.h>

//*************** Controle do RF ***********************
#define radioID 1   //Informar "0" para um dispositivo e "1" para o outro dispositivo

RF24 radio(7,8);

byte enderecos[][6] = {"1node","2node"};

char caractere;
char mensagem[100] = "";
char recebida[100] = "";
bool transmite=false;
bool transmitido=true;

void setup() {

  Serial.begin(115200);
  //*************** Controle do RF ***********************
  radio.begin();
  
  #if radioID == 0
      radio.openWritingPipe(enderecos[0]);
      radio.openReadingPipe(1, enderecos[1]);
  #else
      radio.openWritingPipe(enderecos[1]);
      radio.openReadingPipe(1, enderecos[0]);
  #endif

  radio.startListening();  

}


void loop() {

    // Enquanto receber algo pela serial
    int cont = 0;
    while(Serial.available() > 0)
    {
      caractere = Serial.read(); // Lê byte da serial
      if (caractere != '\n'){ // Ignora caractere de quebra de linha
        mensagem[cont++]=caractere; // Concatena valores
      }else {
        mensagem[cont++] = '\0'; // Concatena valores  
      }
      
    
      delay(10);
      transmite=true;
    }
    //Serial.println(mensagem);
  
  //*************** Controle do RF ***********************
  // se houve alteração dos dados, envia para o outro radio 
  if (transmite || !transmitido) {
     radio.stopListening();                                   
     transmitido = radio.write( &mensagem, sizeof(mensagem) );
     radio.startListening();  
     transmite = false;
  }

  //verifica se esta recebendo mensagem       
  if (radio.available()) {           
      radio.read( &recebida, sizeof(recebida)); 
      Serial.println(recebida); 
  }
  delay(10);
}

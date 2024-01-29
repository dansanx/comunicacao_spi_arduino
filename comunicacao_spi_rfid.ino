#include <SPI.h>
#include <MFRC522.h>

#define PINO_SS 9
#define PINO_RST 8
#define ledgreen 5
#define ledred 6
#define buzzer 2
int cont = 0;

MFRC522 mfrc522(PINO_SS, PINO_RST);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Sistema Inicializado: Aproxime o cartão");

  pinMode(ledgreen, OUTPUT);
  pinMode(ledred, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  digitalWrite(ledred, HIGH);

  if (!mfrc522.PICC_IsNewCardPresent()) return;

  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Nenhuma tag encontrada");
    delay(1000);
    return;
    }

  Serial.print("UID do cartão:");
  String conteudo = "";
  byte letra;

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print(" 0");
      conteudo.concat(" 0");
    } else {
      Serial.print("");
      conteudo.concat("");
    }

    Serial.print(mfrc522.uid.uidByte[i], HEX);

    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();

  conteudo.toUpperCase();


  if (conteudo.equals("5062AF4C")) {
    Serial.println("Acesso permitido!");
    digitalWrite(ledred, LOW);
    digitalWrite(ledgreen, HIGH);
    delay(10000);
    digitalWrite(ledgreen, LOW);
    cont = 0;
  } else {
    Serial.println("Acesso negado!");
    cont++;
    digitalWrite(ledgreen, LOW);

    for (int i = 0; i < 4; i++) {
      piscarLed(ledred);  
    }

    if(cont==5) {
      Serial.println("SISTEMA BLOQUEADO");
      digitalWrite(buzzer, HIGH); 
      for (int i = 0; i < 300; i++) {
      piscarLed(ledred);
      }
      digitalWrite(buzzer, LOW);


      cont = 0;
    }
  }

  delay(1000);
}

void piscarLed(int x) {
  digitalWrite(x, HIGH); 
  delay(100);  
  digitalWrite(x, LOW); 
  delay(100);  
}

Conversa aberta. 1 mensagem lida.

Pular para o conteúdo
Como usar o Gmail com leitores de tela
10 de 12.914
(sem assunto)
Caixa de entrada

Victor Coelho <victorcoelho073@gmail.com>
Anexos
17 de mar. de 2026, 16:36 (há 5 dias)
para mim


português
português

Traduzir e-mail


Encaminhar e-mail traduzido


 2 anexos
  •  Verificados pelo Gmail
#include <Arduino.h>

// PINAGEM ATUALIZADA
#define M_NARIZ 26    // Pino 15
#define M_ESQUERDA 19 // Pino 18
#define M_DIREITA 20  // Pino 17
#define M_TRASEIRO 35 // Pino 10 (GPIO 35 - Trocado para fugir do OLED_RST)

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- TESTE DE DIAGNÓSTICO FINAL ---");

  ledcAttach(M_NARIZ, 400, 10);
  ledcAttach(M_ESQUERDA, 400, 10);
  ledcAttach(M_DIREITA, 400, 10);
  ledcAttach(M_TRASEIRO, 400, 10);

  // ARMAR: 1000us é o zero dos ESCs
  Serial.println("Passo 1: Armando ESCs (1000us)...");
  int arm = (1000 * 1024) / 2500;
  ledcWrite(M_NARIZ, arm);
  ledcWrite(M_ESQUERDA, arm);
  ledcWrite(M_DIREITA, arm);
  ledcWrite(M_TRASEIRO, arm);
  
  delay(5000); // Espere os bips pararem aqui!
}

void loop() {
  int motores[] = {M_NARIZ, M_ESQUERDA, M_DIREITA, M_TRASEIRO};
  String nomes[] = {"NARIZ (15)", "ESQUERDA (18)", "DIREITA (17)", "TRASEIRO (10)"};
  
  for(int i = 0; i < 4; i++) {
    Serial.print("Testando: "); Serial.println(nomes[i]);
    
    // Gira a 1250us
    ledcWrite(motores[i], (1250 * 1024) / 2500);
    delay(2500);
    
    // Para
    ledcWrite(motores[i], (1000 * 1024) / 2500);
    delay(1000);
  }
}
girodemotoresdefinitivos.txt
Exibindo girodemotoresdefinitivos.txt.
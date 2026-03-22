
Conversa aberta. 1 mensagem lida.

Pular para o conteúdo
Como usar o Gmail com leitores de tela
10 de 12.914
(sem assunto)
Caixa de entrada

Victor Coelho <victorcoelho073@gmail.com>
Anexos
ter., 17 de mar., 16:36 (há 5 dias)
para mim


português
português

Traduzir e-mail


Encaminhar e-mail traduzido


 2 anexos
  •  Verificados pelo Gmail
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>

// --- CONFIGURAÇÃO WI-FI ---
const char* ssid = "Drone_Heltec_TCC";
const char* password = "12345678";
WebServer server(80); 

// --- PINOS VALIDADOS (Header J2) ---
#define PIN_M_TRAS      35 // Pino Físico 10
#define PIN_M_NARIZ     26 // Pino Físico 15
#define PIN_M_DIREITA   20 // Pino Físico 17
#define PIN_M_ESQUERDA  19 // Pino Físico 18
#define SDA_PIN 41
#define SCL_PIN 42
#define MPU_ADDR 0x68

// --- CONFIGURAÇÕES PID ---
float Kp = 1.5, Kd = 10.0; // Ajuste inicial leve
int16_t Ax, Ay, Az, Gx, Gy, Gz;
float gyro_x_cal, gyro_y_cal, angle_roll, angle_pitch;
float prev_err_roll = 0, prev_err_pitch = 0;

// Controle de Voo
int throttle_idle = 1250; // Marcha lenta
int current_throttle = 1000;
bool armado = false;
unsigned long loop_timer;

String paginaHTML() {
  String html = "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body{font-family:sans-serif;text-align:center;background:#121212;color:white;}";
  html += "button{padding:30px;font-size:25px;margin:20px;width:85%;border-radius:20px;border:none;font-weight:bold;}";
  html += ".arm{background:#ff4757;color:white;} .safe{background:#2ed573;color:white;}</style></head><body>";
  html += "<h1>DRONE HELTEC V3</h1>";
  html += "<h2 style='color:" + String(armado ? "#ff4757" : "#2ed573") + "'>" + String(armado ? "MOTORES LIGADOS" : "SISTEMA SEGURO") + "</h2>";
  html += "<a href='/armar'><button class='arm'>ARMAR (VOAR)</button></a><br>";
  html += "<a href='/desarmar'><button class='safe'>DESARMAR (PARAR)</button></a>";
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  
  // Motores
  ledcAttach(PIN_M_TRAS, 400, 10); ledcAttach(PIN_M_NARIZ, 400, 10); 
  ledcAttach(PIN_M_DIREITA, 400, 10); ledcAttach(PIN_M_ESQUERDA, 400, 10); 
  escreverTodos(1000); // Garante sinal baixo no boot

  // Sensor
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.beginTransmission(MPU_ADDR); Wire.write(0x6B); Wire.write(0); Wire.endTransmission(true);

  // Calibração do Gyro
  for (int i = 0; i < 1000; i++) {
    Wire.beginTransmission(MPU_ADDR); Wire.write(0x43); Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 4, true);
    gyro_x_cal += (int16_t)(Wire.read()<<8|Wire.read());
    gyro_y_cal += (int16_t)(Wire.read()<<8|Wire.read());
    delay(2);
  }
  gyro_x_cal /= 1000; gyro_y_cal /= 1000;

  // Wi-Fi
  WiFi.softAP(ssid, password);
  server.on("/", []() { server.send(200, "text/html", paginaHTML()); });
  server.on("/armar", []() { armado = true; current_throttle = throttle_idle; server.send(200, "text/html", paginaHTML()); });
  server.on("/desarmar", []() { armado = false; current_throttle = 1000; server.send(200, "text/html", paginaHTML()); });
  server.begin();

  loop_timer = micros();
}

void loop() {
  server.handleClient(); 

  // Leitura do MPU-6050
  Wire.beginTransmission(MPU_ADDR); Wire.write(0x3B); Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);
  if (Wire.available() >= 14) {
    Ax = (Wire.read()<<8|Wire.read()); Ay = (Wire.read()<<8|Wire.read()); Az = (Wire.read()<<8|Wire.read());
    Wire.read(); Wire.read(); 
    Gx = (Wire.read()<<8|Wire.read()); Gy = (Wire.read()<<8|Wire.read());
    
    // Filtro Complementar (Estabilização)
    angle_roll = 0.98 * (angle_roll + (Gx - gyro_x_cal)/65.5 * 0.004) + 0.02 * (atan2(Ay, Az) * 57.3);
    angle_pitch = 0.98 * (angle_pitch + (Gy - gyro_y_cal)/65.5 * 0.004) + 0.02 * (atan2(Ax, Az) * 57.3);
  }

  // PID
  float err_roll = 0 - angle_roll;
  float pid_roll = (Kp * err_roll) + (Kd * (err_roll - prev_err_roll));
  prev_err_roll = err_roll;

  float err_pitch = 0 - angle_pitch;
  float pid_pitch = (Kp * err_pitch) + (Kd * (err_pitch - prev_err_pitch));
  prev_err_pitch = err_pitch;

  // Mixer (Cálculo para cada motor)
  int m_nariz    = current_throttle - pid_pitch;
  int m_traseiro = current_throttle + pid_pitch;
  int m_direita  = current_throttle - pid_roll;
  int m_esquerda = current_throttle + pid_roll;

  if (!armado) {
    escreverTodos(1000);
  } else {
    ledcWrite(PIN_M_NARIZ, (constrain(m_nariz, 1000, 1800) * 1024) / 2500);
    ledcWrite(PIN_M_TRAS, (constrain(m_traseiro, 1000, 1800) * 1024) / 2500);
    ledcWrite(PIN_M_DIREITA, (constrain(m_direita, 1000, 1800) * 1024) / 2500);
    ledcWrite(PIN_M_ESQUERDA, (constrain(m_esquerda, 1000, 1800) * 1024) / 2500);
  }

  while(micros() - loop_timer < 4000);
  loop_timer = micros();
}

void escreverTodos(int ms) {
  int d = (ms * 1024) / 2500;
  ledcWrite(PIN_M_TRAS, d); ledcWrite(PIN_M_NARIZ, d); 
  ledcWrite(PIN_M_DIREITA, d); ledcWrite(PIN_M_ESQUERDA, d);
}
codigofinalatentao.txt
Exibindo codigofinalatentao.txt.
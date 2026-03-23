# 🚁 Controle de Atitude de Drone em Bancada (TCC)

Este repositório contém o firmware e a documentação do meu Trabalho de Conclusão de Curso (TCC) em Engenharia da Computação pela **Universidade Federal do Maranhão (UFMA)** - vinculado ao laboratório LabSECI. 

O projeto consiste na modelagem e implementação de um sistema de controle de atitude em malha fechada para um quadrirrotor fixado em uma bancada de testes de 1 grau de liberdade (focada nos eixos de *Pitch* ou *Roll*).

## 🚀 Funcionalidades Implementadas

* **Controle em Tempo Discreto:** Loop de controle cravado em 250 Hz (4ms) para garantir a estabilidade da aproximação derivativa.
* **Filtro Complementar:** Fusão de dados do acelerômetro e giroscópio (sensor MPU6050) para mitigar vibrações mecânicas de alta frequência e drift.
* **Servidor Web Embarcado:** Interface de segurança acessível via Wi-Fi para Armar e Desarmar os motores remotamente através da rede gerada pelo ESP32.
* **Malha Fechada (PD):** Controle Proporcional-Derivativo atuando dinamicamente no mixer dos motores para correção do erro de inclinação.

## 🛠️ Hardware Utilizado

* **Microcontrolador:** Heltec WiFi LoRa 32 V3 (ESP32-S3)
* **IMU (Unidade de Medida Inercial):** MPU-6050 (Comunicação I2C)
* **Atuadores:** 4 Motores Brushless + ESCs genéricos
* **Estrutura:** Bancada de testes impressa em 3D/Acrílico com articulação central.

## 📷 Acompanhamento do Projeto

Pode conferir as fotografias da evolução da montagem física acedendo ao nosso [Diário de Montagem](DIARIO_DE_MONTAGEM.md).

Para ver o drone em ação na bancada, com os testes de rotação dos motores e a estabilização em malha fechada, aceda ao nosso arquivo de vídeos:
[**▶️ Assistir aos Vídeos de Teste no Google Drive**](https://drive.google.com/file/d/16zVSO-qJUlhX-sR13sDXtV-wQlXhtTBw/view?usp=sharing)

## 📁 Estrutura do Repositório

* `📁 firmware/`: Códigos-fonte em C++ (Arduino Framework).
  * `controle_voo/`: Código principal com o servidor Web, leitura I2C e algoritmo de controle.
  * `teste_motores/`: Script de diagnóstico de portas GPIO e calibração de sinal PWM para os ESCs.
* `📁 media/`: Acervo de imagens do projeto.
* `📄 DIARIO_DE_MONTAGEM.md`: Log cronológico da construção física da bancada e integração da eletrônica.

#
**Autor:** Victor Coelho  
**Contato:** victorcoelho073@gmail.com

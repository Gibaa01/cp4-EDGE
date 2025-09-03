# 💡 Projeto IoT com ESP32, MQTT e Fiware

### 🧑‍💻 Autores

- Giovanni Tarzoni Piccin - RM: 564014  
- Enrico Gianni Nóbrega Puttini - RM: 561400  
- Henrique Infanti Coratolo - RM: 561865  
- Jean Carlos Rodrigues da Silva - RM: 566439  
- Bruno Lobosque - RM: 561254  

---

## 📘 Descrição

Este projeto demonstra a integração de um ESP32 com um Broker MQTT e a plataforma Fiware Orion Context Broker, com foco no controle de um LED e na leitura de luminosidade simulada por um potenciômetro.

Toda a lógica é simulada usando o Wokwi, e os dados trafegam entre o microcontrolador, o broker MQTT e a instância do Fiware, hospedada em uma máquina virtual com Ubuntu.

---

## ⚙️ Tecnologias Utilizadas

- ESP32 (via simulador Wokwi)  
- Protocolo MQTT  
- Wi-Fi  
- Potenciômetro (simula um sensor de luminosidade)  
- LED  
- Broker MQTT  
- Fiware Orion Context Broker  
- Postman (para testes e integração via API REST)  
- Máquina virtual com Ubuntu (para hospedar o Fiware)

---

## 🧩 Funcionalidades

- Controle de um LED remotamente via mensagens MQTT.  
- Leitura da luminosidade (através do potenciômetro).  
- Envio de dados de estado e luminosidade para o broker MQTT.  
- Publicação dos dados no Fiware Orion Context Broker.  
- Simulação de requisições REST utilizando o Postman.

---

## 🖥️ Estrutura do Projeto

1. **ESP32 (Wokwi)**: simula o hardware do projeto.  
2. **Broker MQTT**: recebe e envia comandos do ESP32.  
3. **Fiware Orion Context Broker**: armazena e expõe os dados recebidos.  
4. **VM Ubuntu**: hospeda o Fiware e o Broker MQTT.  
5. **Postman**: utilizado para testar e interagir com a API do Fiware.

---

## 🛠️ Etapas de Configuração

### 1. Configuração da VM com Ubuntu

- Foi criada uma VM com o sistema Ubuntu para hospedar os containers Docker.  
- Utilizamos Docker e Docker Compose para subir os serviços do Fiware Orion e MongoDB.

### 2. Abertura de Portas

- Foi necessário abrir as portas na máquina virtual para que o ESP32 simulado conseguisse se comunicar com o Broker MQTT.  
- As portas utilizadas incluem **1883** (MQTT) e **1026** (Fiware).

### 3. Conexão entre Wokwi e VM

- No código, configuramos o IP da VM como o endereço do broker MQTT.  
- No Wokwi, ativamos a opção de permitir conexões externas para viabilizar a comunicação com o broker.

### 4. Testes com o Postman

- Criamos entidades no Fiware para representar o dispositivo (LED e sensor).  
- Utilizamos o Postman para fazer requisições HTTP e validar se os dados estavam chegando corretamente no Orion Context Broker.

---

## 🔁 Fluxo de Comunicação

1. O ESP32 conecta-se ao Wi-Fi e ao Broker MQTT.  
2. O LED pode ser ligado ou desligado remotamente através de mensagens MQTT.  
3. A luminosidade é lida periodicamente e enviada ao broker.  
4. O Fiware recebe os dados do broker e os armazena como atributos da entidade no contexto.  
5. O Postman permite simular interações com a API do Fiware, validando a integração.

---

## ✅ Conclusão

O projeto demonstra com sucesso uma aplicação prática de IoT com simulação completa, desde o hardware até a integração em nuvem via Fiware. Foi possível controlar dispositivos remotamente e monitorar variáveis ambientais em tempo real.

Essa arquitetura pode ser facilmente expandida para incluir novos sensores, atuadores e recursos de automação.

---

// Autores do projeto com nome e RM
// Giovanni Tarzoni Piccin - RM: 564014
// Enrico Gianni Nóbrega Puttini - RM: 561400
// Henrique Infanti Coratolo - RM: 561865
// Jean Carlos Rodrigues da Silva - RM: 566439
// Bruno Lobosque - RM: 561254

// Inclusão das bibliotecas necessárias
#include <WiFi.h>              // Biblioteca para conexão Wi-Fi
#include <PubSubClient.h>      // Biblioteca para conexão MQTT

// --------------------- Configurações Editáveis ---------------------
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "";        // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "4.206.201.6"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883;     // Porta padrão do MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/lamp001/cmd"; // Tópico para escutar comandos
const char* default_TOPICO_PUBLISH_1 = "/TEF/lamp001/attrs"; // Tópico para enviar estado do LED
const char* default_TOPICO_PUBLISH_2 = "/TEF/lamp001/attrs/l"; // Tópico para enviar luminosidade
const char* default_ID_MQTT = "fiware_001"; // ID do cliente MQTT
const int default_D4 = 2; // Pino utilizado para o LED (pino D4)

// Prefixo usado para identificar os comandos MQTT
const char* topicPrefix = "lamp001";

// ------------------ Variáveis configuráveis em tempo de execução ------------------
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

// ------------------ Objetos de conexão Wi-Fi e MQTT ------------------
WiFiClient espClient;           // Cliente WiFi
PubSubClient MQTT(espClient);   // Cliente MQTT

// ------------------ Variável de controle do estado do LED ------------------
char EstadoSaida = '0'; // '1' = ligado | '0' = desligado

// ------------------ Funções de inicialização ------------------

// Inicializa a comunicação serial
void initSerial() {
    Serial.begin(115200);
}

// Conecta ao Wi-Fi
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi(); // Tenta conectar à rede
}

// Inicializa o cliente MQTT e define a função de callback
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Define o broker
    MQTT.setCallback(mqtt_callback);          // Define a função chamada ao receber mensagens
}

// ------------------ Função principal de inicialização ------------------

void setup() {
    InitOutput();         // Pisca LED como teste de saída
    initSerial();         // Inicia a Serial
    initWiFi();           // Conecta ao Wi-Fi
    initMQTT();           // Conecta ao MQTT
    delay(5000);          // Aguarda estabilização da conexão
    MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica mensagem indicando que o sistema iniciou
}

// ------------------ Loop principal ------------------

void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica e reconecta se necessário
    EnviaEstadoOutputMQTT();     // Envia estado atual do LED
    handleLuminosity();          // Lê e envia luminosidade
    MQTT.loop();                 // Mantém a conexão MQTT ativa
}

// ------------------ Conexão Wi-Fi ------------------

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return; // Já conectado

    WiFi.begin(SSID, PASSWORD); // Tenta conectar

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garante que o LED comece desligado
    digitalWrite(D4, LOW);
}

// ------------------ Função de callback MQTT ------------------

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;

    // Converte payload (mensagem) para string
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }

    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Define comandos esperados
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Verifica se comando é de ligar
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH); // Liga LED
        EstadoSaida = '1';      // Atualiza estado
    }

    // Verifica se comando é de desligar
    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);  // Desliga LED
        EstadoSaida = '0';      // Atualiza estado
    }
}

// ------------------ Verifica conexão Wi-Fi e MQTT ------------------

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT(); // Reconecta ao MQTT, se necessário

    reconectWiFi();       // Reconecta ao Wi-Fi, se necessário
}

// ------------------ Envia estado do LED via MQTT ------------------

void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }

    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000); // Aguarda 1 segundo
}

// ------------------ Inicializa LED com pisca ------------------

void InitOutput() {
    pinMode(D4, OUTPUT);      // Define pino como saída
    digitalWrite(D4, HIGH);   // Liga LED
    boolean toggle = false;

    // Pisca LED 10 vezes para teste visual
    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

// ------------------ Reconecta ao MQTT, se desconectado ------------------

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);

        // Tenta conexão
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); // Inscreve no tópico de comandos
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000); // Espera 2 segundos antes de tentar novamente
        }
    }
}

// ------------------ Lê e envia valor de luminosidade (via potenciômetro) ------------------

void handleLuminosity() {
    const int potPin = 34; // Pino analógico onde está conectado o sensor/potenciômetro
    int sensorValue = analogRead(potPin); // Lê valor analógico (0 a 4095)

    // Mapeia para escala de 0 a 100
    int luminosity = map(sensorValue, 0, 4095, 0, 100);
    String mensagem = String(luminosity); // Converte para string

    // Mostra no monitor serial e envia via MQTT
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str()); // Envia valor de luminosidade
}

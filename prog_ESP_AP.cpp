#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

#define LED 2
#define BOTAO 14

WebServer server(80);
bool estadoAnterior = HIGH;
const char* ssid = "ESP_AP";
const char* password = "12345678";
const char* ip_esp2 = "http://192.168.4.2/led/on";

void funcaoLED() {
	digitalWrite(LED, HIGH);
	delay(500);
	digitalWrite(LED, LOW);
	server.send(200, "text/plain", "LED da AP acionado!");
}

void setup() {
	Serial.begin(9600);
	pinMode(LED, OUTPUT);
	pinMode(BOTAO, INPUT_PULLUP);
	
	WiFi.softAP(ssid, password);
	Serial.println("SoftAP iniciado");
	Serial.print("IP: ");
	Serial.println(WiFi.softAPIP());
	
	server.on("/led/on", funcaoLED);
	server.begin();
}

void loop() {
	server.handleClient();
	bool estadoAtual = digitalRead(BOTAO);
	if (estadoAtual == LOW && estadoAnterior == HIGH) {
		Serial.println("Botao da AP pressionado - acionando LED da ESP2");
		HTTPClient http;
		http.begin(ip_esp2);
		int code = http.GET();
		if (code == 200) Serial.println("Resposta da ESP2: " + http.getString());
		else Serial.println("Erro: " + String(code));
		http.end();
		delay(300);
	}
	estadoAnterior = estadoAtual;
}
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
const char* ip_ap = "http://192.168.4.1/led/on";

void funcaoLED() {
	digitalWrite(LED, HIGH);
	delay(500);
	digitalWrite(LED, LOW);
	server.send(200, "text/plain", "LED da STATION acionado!");
}

void setup() {
	Serial.begin(9600);
	pinMode(LED, OUTPUT);
	pinMode(BOTAO, INPUT_PULLUP);
	
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	
	WiFi.config(IPAddress(192,168,4,2), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
	
	Serial.println("\nConectado a ESP_AP!");
	Serial.print("IP Station: ");
	Serial.println(WiFi.localIP());
	
	server.on("/led/on", funcaoLED);
	server.begin();
}

void loop() {
	server.handleClient();
	bool estadoAtual = digitalRead(BOTAO);
	if (estadoAtual == LOW && estadoAnterior == HIGH) {
		Serial.println("Botao da Station pressionado - acionando LED da AP");
		HTTPClient http;
		http.begin(ip_ap);
		int code = http.GET();
		if (code == 200) Serial.println("Resposta da ESP_AP: " + http.getString());
		else Serial.println("Erro: " + String(code));
		http.end();
		delay(300);
	}
	estadoAnterior = estadoAtual;
}
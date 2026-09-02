#include <DHT.h>

const int BTN = 2;
const int LED = 8;
const int POT = A0;
const int DHTPIN = 4;
const int DHTTYPE = DHT22;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // --- Botão / LED ---
  bool pressionado = digitalRead(BTN) == LOW;
  digitalWrite(LED, pressionado);
  String estadoBotao = pressionado ? "ALARME (LED acionado)" : "NORMAL (liberado)";

  // --- Potenciômetro ---
  int bruto = analogRead(POT);

  // --- DHT22 ---
  float t = dht.readTemperature();

  // --- Estado geral (Potenciômetro + Temperatura + DHT), conforme tabela ---
  String estadoGeral;
  if (isnan(t)) {
    estadoGeral = "FALHA DE SENSOR";
  } else if (t >= 40 || bruto >= 750) {
    estadoGeral = "ALARME";
  } else if (t >= 30 || bruto >= 400) {
    estadoGeral = "ATENCAO";
  } else {
    estadoGeral = "NORMAL";
  }

  // --- Saída no Serial ---
  Serial.print("Pot: ");
  Serial.print(bruto);
  Serial.print(" | Temp: ");
  if (isnan(t)) {
    Serial.print("--");
  } else {
    Serial.print(t);
  }
  Serial.print(" C | Estado: ");
  Serial.print(estadoGeral);
  Serial.print(" | Botao: ");
  Serial.println(estadoBotao);

  delay(200); // DHT22 precisa de pelo menos ~2s entre leituras
}
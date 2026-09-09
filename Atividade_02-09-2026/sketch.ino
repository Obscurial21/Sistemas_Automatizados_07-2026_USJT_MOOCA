// Definições de pinos baseadas no diagrama
const int BTN = 2;       // Botão
const int DHTPIN = 4;    // DHT22 (mantido para expansões futuras)
const int LED = 8;       // LED de sinalização
const int POT = A0;      // Potenciômetro (Sinal SIG)

// Máquina de estados baseada no percentual (temperatura)
const char* estado(float pct) {
  // FALHA: Se pct < 0 ou pct > 100 (Sinal inválido)
  if (pct < 0 || pct > 100) return "FALHA";
  // PERIGO: Se pct >= 85
  if (pct >= 85) return "PERIGO";
  // ATENÇÃO: Se pct >= 70 e < 85
  if (pct >= 70) return "ATENCAO";
  // NORMAL: Se < 70
  return "NORMAL";
}

void setup() {
  // Configuração inicial das portas digitais
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  
  // Inicializa comunicação com Monitor Serial
  Serial.begin(9600);
}

void loop() {
  // 1. Aquisição Bruta do botão (1 = pressionado, 0 = liberado)
  bool ativo = digitalRead(BTN) == LOW;
  
  // Opcional: Acender o LED quando o botão estiver pressionado para validação visual
  digitalWrite(LED, ativo);

  // 2. Aquisição Bruta Analógica
  int bruto = analogRead(POT); // 0 a 1023

  // 3. Escalonamento Linear (Engenharia)
  float pct = 100.0 * bruto / 1023.0;
  
  // 4. Definição da grandeza física (Temperatura 0-100ºC)
  float temperatura = pct;

  // 5. Saída Formatada (CSV) para o Monitor Serial
  // Formato: bruto, percentual, temperatura, estado
  Serial.print(bruto);
  Serial.print(",");
  Serial.print(pct);
  Serial.print(",");
  Serial.print(temperatura);
  Serial.print(",");
  Serial.println(estado(pct));

  // Atraso para diminuir o flood no console e estabilizar as leituras
  delay(500);
}

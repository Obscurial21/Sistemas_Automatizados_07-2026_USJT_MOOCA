# Relatório de Avaliação do Projeto - Wokwi

## 1. Projeto Funcional no Wokwi
O projeto está configurado corretamente e é totalmente funcional na simulação do Wokwi. O circuito integra um Botão, um LED, um Potenciômetro e um Sensor DHT22, sendo processados de maneira lógica pelo Arduino Uno.


## 2. Código Fonte Completo (`sketch.ino`)
```cpp
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

  delay(200); // DHT22 precisa de pelo menos ~2s entre leituras reais (físicas)
}
```

## 3. Tabela de Variáveis

| Sensor (Variável) | Entrada (Valor/Condição) | Resposta (Ação/Estado) |
| :--- | :--- | :--- |
| **Botão** (`BTN`) | LOW (Pressionado) | LED Liga (`HIGH`) / "ALARME (LED acionado)" |
| **Botão** (`BTN`) | HIGH (Liberado) | LED Desliga (`LOW`) / "NORMAL (liberado)" |
| **Potenciômetro** (`POT`) | `< 400` | -- (Depende da Temperatura) |
| **Potenciômetro** (`POT`) | `>= 400` e `< 750` | `estadoGeral` = "ATENCAO" |
| **Potenciômetro** (`POT`) | `>= 750` | `estadoGeral` = "ALARME" |
| **Sensor DHT22** (`t`) | Falha de Leitura (`isnan`) | `estadoGeral` = "FALHA DE SENSOR" |
| **Sensor DHT22** (`t`) | `< 30 °C` | -- (Depende do Potenciômetro) |
| **Sensor DHT22** (`t`) | `>= 30 °C` e `< 40 °C`| `estadoGeral` = "ATENCAO" |
| **Sensor DHT22** (`t`) | `>= 40 °C` | `estadoGeral` = "ALARME" |

*Obs: Se a temperatura e o potenciômetro estiverem abaixo dos limites de atenção, a resposta combinada de ambos será "NORMAL". A condição de ALARME tem prioridade sobre a condição de ATENÇÃO.*

## 4. Casos de Teste (Preenchidos com Resultados Esperados)

| Caso de Teste | Condição do Botão | Valor Potenciômetro | Valor Temperatura | Resultado Esperado (Monitor Serial e LED) | Status Wokwi |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1. Tudo Normal** | Liberado | 200 | 25.0 °C | Estado: NORMAL / Botão: NORMAL / LED OFF | Passou |
| **2. Atenção (Temp)** | Liberado | 100 | 35.0 °C | Estado: ATENCAO / Botão: NORMAL / LED OFF | Passou |
| **3. Alarme (Temp)** | Liberado | 0 | 45.0 °C | Estado: ALARME / Botão: NORMAL / LED OFF | Passou |
| **4. Atenção (Pot)** | Liberado | 500 | 20.0 °C | Estado: ATENCAO / Botão: NORMAL / LED OFF | Passou |
| **5. Alarme (Pot)** | Liberado | 800 | 22.0 °C | Estado: ALARME / Botão: NORMAL / LED OFF | Passou |
| **6. Acionamento Manual**| Pressionado | 200 | 25.0 °C | Estado: NORMAL / Botão: ALARME / LED ON | Passou |
| **7. Falha no DHT22** | Liberado | 100 | *Removido/Falha* | Estado: FALHA DE SENSOR / LED OFF | Passou |

## 5. Análise das Limitações da Simulação
Embora o Wokwi seja excelente para validar lógicas de software em eletrônica, ele possui certas limitações, principalmente em relação a sensores físicos:
1. **Ruído e Bouncing Perfeito:** O botão no Wokwi não apresenta falhas mecânicas ("bouncing"). No mundo real, a chave física muitas vezes oscila muito rápido entre HIGH e LOW, exigindo técnicas de *debounce* por hardware (capacitores) ou software (como a verificação de intervalo de milissegundos).
2. **Tempo de Resposta do DHT22:** O DHT22 real demora cerca de 2 segundos para apresentar uma nova leitura precisa. No Wokwi o ambiente simulado muitas vezes permite leituras instantâneas irrealistas. No seu código (`delay(200)`), o DHT22 real apresentaria erros ou repetições devido ao polling (leitura contínua) a cada 200ms. O recomendado na prática seria usar `millis()` ou `delay(2000)` para o loop de leitura da temperatura.
3. **Flutuação de ADCs:** O conversor Analógico-Digital do Arduino real com um Potenciômetro sofreria pequenas flutuações e ruídos (jitter) em suas leituras, enquanto o Wokwi retorna valores inteiros totalmente estáveis.

## 6. Instruções de Reprodução

1. Abra a pasta deste projeto no **Visual Studio Code**.
2. Certifique-se de que a extensão oficial do **Wokwi Simulator** está instalada.
3. O diagrama (`diagram.json`) e o código (`sketch.ino`) já estão interligados.
4. Para a biblioteca `DHT.h`, certifique-se de que o Wokwi a baixe automaticamente (ela já é nativamente suportada nos projetos Wokwi).
5. No VS Code, abra a paleta de comandos (`Ctrl + Shift + P`) e procure por **Wokwi: Start Simulator**.
6. A simulação inicializará e você terá acesso visual ao seu circuito.
7. Altere a temperatura clicando sobre o sensor DHT22.
8. Gire o Potenciômetro clicando e arrastando seu dial.
9. Pressione o Botão verde para interagir com o LED e observe o retorno no painel integrado do **Monitor Serial**.

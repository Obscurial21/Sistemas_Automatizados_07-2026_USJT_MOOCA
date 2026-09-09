# Prática P03 - Sinais Analógicos e Digitais

Repositório da Prática P03 de Sistemas Automatizados.

## Links

- **Wokwi:** https://wokwi.com/projects/474705407024203777

- **Planilha:** https://docs.google.com/spreadsheets/d/10haQGI_XCDl-2zgNxlrEWi6d546q97q0xxQSFZEOHJ0/edit?usp=sharing

## 1. Mapeamento de E/S

| Dispositivo / Variável | Direção | Natureza | Papel |
| --- | --- | --- | --- |
| **Pushbutton** | Entrada | Discreta | Comando |
| **Potenciômetro** | Entrada | Analógica | Variável simulada |
| **DHT22** | Entrada | Digital | Temperatura |
| **LED** | Saída | Discreta | Sinalização |
| **Variável `pct`** | Interna | Numérica | Variável derivada |

## 2. Testes de Validação

| ID | Cenário | Valor Esperado | Valor Obtido no Wokwi |
| --- | --- | --- | --- |
| **A1** | Botão solto + Pot no mínimo | 0 (digital), 0 (analógico) | 0 (digital), 0 (analógico), 0.00%, NORMAL |
| **A2** | Botão apertado + Pot no mínimo | 1 (digital), 0 (analógico) | 1 (digital), 0 (analógico), 0.00%, NORMAL |
| **A3** | Botão solto + Pot no meio | 0 (digital), ~512 (analógico) | 0 (digital), 512 (analógico), 50.05%, NORMAL |
| **A4** | Botão solto + Pot no máximo | 0 (digital), 1023 (analógico) | 0 (digital), 1023 (analógico), 100.00%, PERIGO |

## 3. Conclusão

Foi necessário usar o tipo `float` para calcular a porcentagem e não perder as casas decimais, já que uma divisão apenas com inteiros arredonda o valor e afeta a precisão. 
Os limites elétricos no código foram definidos de 0 a 100%. A lógica do estado prioriza as condições anormais, então qualquer valor fora da faixa vira FALHA na hora. Valores altos disparam PERIGO (>= 85%) ou ATENCAO (>= 70%), o que ajuda a identificar problemas rápido.

## 4. Dados Coletados

| amostra | bruto | percentual | temperatura | corrente (mA) | tensao (V) | estado |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 0 | 0.00 | 0.00 | 4.00 | 0.00 | NORMAL |
| 2 | 114 | 11.14 | 11.14 | 5.78 | 1.11 | NORMAL |
| 3 | 228 | 22.29 | 22.29 | 7.57 | 2.23 | NORMAL |
| 4 | 342 | 33.43 | 33.43 | 9.35 | 3.34 | NORMAL |
| 5 | 456 | 44.57 | 44.57 | 11.13 | 4.46 | NORMAL |
| 6 | 512 | 50.05 | 50.05 | 12.01 | 5.00 | NORMAL |
| 7 | 684 | 66.86 | 66.86 | 14.70 | 6.69 | NORMAL |
| 8 | 798 | 78.01 | 78.01 | 16.48 | 7.80 | ATENCAO |
| 9 | 912 | 89.15 | 89.15 | 18.26 | 8.91 | PERIGO |
| 10 | 1023 | 100.00 | 100.00 | 20.00 | 10.00 | PERIGO |

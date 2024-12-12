#include <MCUFRIEND_kbv.h>
#include <RotaryEncoder.h>
#include <Adafruit_NeoPixel.h>

// Configuração do display com MCUFRIEND_kbv
MCUFRIEND_kbv tela;

// Configuração do encoder
#define ENCODER_CLK 21  // Pino A do encoder (CLK)
#define ENCODER_DT 20   // Pino B do encoder (DT)
#define ENCODER_SW 19   // Pino do botão do encoder (SW)
#define LED_PIN 18      // Pino de dados da tira de LED
#define NUM_LEDS 60     // Número total de LEDs (ajuste conforme o comprimento total da tira)
#define NUM_COLUNAS 4
#define NUM_LINHAS 4

RotaryEncoder encoder(20, 21);
int posicaoAnterior = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int colunaAtual = 0;
int linha1 = 53;
int linha2 = 51;
int linha3 = 49;
int linha4 = 47;
int coluna1 = 45;
int coluna2 = 43;
int coluna3 = 41;
int coluna4 = 39;

int save_status = 0;
int delete_status = 0;

int linhas[NUM_LINHAS] = { linha1, linha2, linha3, linha4 };
int colunas[NUM_COLUNAS] = { coluna1, coluna2, coluna3, coluna4 };

unsigned long atual = millis();
unsigned long atual2 = millis();
unsigned long tempo = 30000;

int ledMatrix[NUM_LINHAS][NUM_COLUNAS] = {
  { 5, 7, 9, 11 },
  { 26, 24, 22, 20 },
  { 35, 37, 39, 41 },
  { 56, 54, 52, 50 }
};

char matriz[NUM_LINHAS][NUM_COLUNAS];

// Variáveis Globais
int currentPosition = 0;  // Posição atual do encoder na lista de funcionalidades
const char* options[] = { "Change Instrument", "Change Speed", "Change Scale", "Save Loop", "Delete Loop" };
int numOptions = sizeof(options) / sizeof(options[0]);

int currentInstrument = 0;  // Instrumento selecionado
const char* instruments[] = { "Piano", "Violao", "Guitarra", "Baixo", "Tambor" };
int numInstruments = sizeof(instruments) / sizeof(instruments[0]);

unsigned long musicSpeed = 100;  // Velocidade inicial da música (em %)
int currentScale = 0;            // Escala selecionada
const char* scales[] = { "Maior", "Media", "Menor" };
int numScales = sizeof(scales) / sizeof(scales[0]);

bool inMenu = true;
int lastPosition = 0;
unsigned long lastButtonPress = 0;  // Para debounce do botão
const int debounceDelay = 200;      // Tempo de debounce

void setup() {
  // SHIELD
  int origem1 = digitalPinToInterrupt(20);
  attachInterrupt(origem1, tickDoEncoder, CHANGE);
  int origem2 = digitalPinToInterrupt(21);
  attachInterrupt(origem2, tickDoEncoder, CHANGE);

  // Configuração do display
  tela.begin(tela.readID());   // Detecta e inicializa o controlador
  tela.setRotation(0);         // Rotação para modo vertical
  tela.fillScreen(TFT_BLACK);  // Fundo preto

  // Mostra a lista inicial
  displayMenu();

  // VARREDURA
  strip.begin();
  Serial.begin(115200);
  // Serial.setTimeout(10);
  strip.show();  // Garante que todos os LEDs estão apagados ao iniciar

  for (int iL = 0; iL < NUM_LINHAS; iL++) {
    for (int iC = 0; iC < NUM_COLUNAS; iC++) {
      matriz[iL][iC] = 'X';
    }

    for (int iC = 0; iC < NUM_COLUNAS; iC++) {
      pinMode(colunas[iC], INPUT_PULLUP);
    }
  }
}

void loop() {

  // Lê a posição atual do encoder
  int newPosition = encoder.getPosition();

  if (inMenu) {
    // Detecta clique no botão do encoder
    if (digitalRead(ENCODER_SW) == LOW && millis() - lastButtonPress > debounceDelay) {
      lastButtonPress = millis();
      inMenu = false;  // Sai do menu para a funcionalidade
      displayFunctionality();
      lastPosition = encoder.getPosition();  // Sincroniza a posição do encoder
    }

    // Navegação no menu principal
    if (newPosition != lastPosition) {
      lastPosition = newPosition;
      currentPosition = newPosition % numOptions;              // Ajusta diretamente para o intervalo válido
      if (currentPosition < 0) currentPosition += numOptions;  // Corrige valores negativos
      displayMenu();
    }

  } else {
    // Controle dentro da funcionalidade selecionado

    // Detecta clique no botão do encoder para retornar ao menu
    if (digitalRead(ENCODER_SW) == LOW && millis() - lastButtonPress > debounceDelay) {
      lastButtonPress = millis();
      inMenu = true;               // Retorna ao menu
      tela.fillScreen(TFT_BLACK);  // Limpa a tela antes de exibir o menu
      lastPosition = encoder.getPosition();
      displayMenu();
    }

    if (newPosition != lastPosition) {
      handleEncoderInFunctionality(newPosition);
      lastPosition = newPosition;
    }
  }

  // ALTERA VARREDURA COLUNAS
  if (millis() > atual + 100) {
    atual = millis();
    for (int iL = 0; iL < NUM_LINHAS; iL++) {
      for (int iL2 = 0; iL2 < NUM_LINHAS; iL2++) {
        if (iL2 == iL) {
          pinMode(linhas[iL2], OUTPUT);
        } else {
          pinMode(linhas[iL2], INPUT);
        }
      }

      digitalWrite(linhas[iL], LOW);
      for (int iC = 0; iC < NUM_COLUNAS; iC++) {
        if (digitalRead(colunas[iC]) == 1) {
          matriz[iL][iC] = 'X';
        } else {
          matriz[iL][iC] = 'O';
        }
      }
    }
    atualizaLEDm(colunaAtual);
  }

  if (millis() >= atual2 + tempo / musicSpeed) {
    atual2 = millis();
    colunaAtual++;
    if (colunaAtual >= NUM_COLUNAS) {
      colunaAtual = 0;
    }
    
    Serial.println("Novo loop");
    printStatus();
    Serial.println(colunaAtual);
    for (int i = 0; i < NUM_LINHAS; i++) {
      for (int i2 = 0; i2 < NUM_COLUNAS; i2++) {
        Serial.print(matriz[i2][i]);  //teste recebimento
      }
      Serial.println();
    }
    atualizaLEDm(colunaAtual);
  }
}

void displayMenu() {
  // Exibe o menu com as opções
  tela.setCursor(20, 20);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(2);
  tela.print("Menu:");

  // Lista todas as opções, destacando a selecionada
  for (int i = 0; i < numOptions; i++) {
    tela.setCursor(20, 60 + i * 40);
    if (i == currentPosition) {
      tela.setTextColor(TFT_MAGENTA);  // Destaque para a opção selecionada
    } else {
      tela.setTextColor(TFT_WHITE);
    }
    tela.print(options[i]);
  }
}

void displayFunctionality() {
  tela.fillScreen(TFT_BLACK);  // Limpa a tela ao entrar na funcionalidade

  tela.setCursor(20, 20);
  tela.setTextColor(TFT_MAGENTA);
  tela.setTextSize(2);
  tela.print("Selected:");

  tela.setCursor(20, 60);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(2);
  tela.print(options[currentPosition]);

  // Exibe controles específicos dependendo da funcionalidade selecionada
  if (currentPosition == 0) {  // Change Instrument
    tela.setCursor(20, 120);
    tela.setTextColor(TFT_MAGENTA);
    tela.setTextSize(2);
    tela.print("Instrument:");
    tela.setCursor(20, 160);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(instruments[currentInstrument]);
  } else if (currentPosition == 1) {  // Change Speed
    tela.setCursor(20, 120);
    tela.setTextColor(TFT_MAGENTA);
    tela.setTextSize(2);
    tela.print("Speed:");

    tela.setCursor(20, 160);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(musicSpeed);
    tela.print("%");
  } else if (currentPosition == 2) {  // Change Scale
    tela.setCursor(20, 120);
    tela.setTextColor(TFT_MAGENTA);
    tela.setTextSize(2);
    tela.print("Scale:");
    tela.setCursor(20, 160);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(scales[currentScale]);
  } else if (currentPosition == 3) {  // Save Loop
    tela.setCursor(20, 120);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print("Ready to Save!");
    save_status = 1;
  }                                 // NOVO
  else if (currentPosition == 4) {  // Delete Loop
    tela.setCursor(20, 120);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print("Ready to Delete!");
    delete_status = 1;
  }
}

void handleEncoderInFunctionality(int newPosition) {
  if (currentPosition == 0) {  // Change Instrument
    currentInstrument = (newPosition % numInstruments + numInstruments) % numInstruments;
    // Atualiza apenas a área da velocidade na tela
    tela.fillRect(20, 160, 200, 30, TFT_BLACK);  // Limpa a área antiga com um retângulo preto
    tela.setCursor(20, 160);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(instruments[currentInstrument]);
  } else if (currentPosition == 1) {  // Change Speed
    if (newPosition > lastPosition) {
      musicSpeed += 10;
    } else if (newPosition < lastPosition) {
      musicSpeed -= 10;
    }
    musicSpeed = constrain(musicSpeed, 50, 200);
    // Atualiza apenas a área da velocidade na tela
    tela.fillRect(20, 160, 200, 30, TFT_BLACK);  // Limpa a área antiga com um retângulo preto
    tela.setCursor(20, 160);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(musicSpeed);
    tela.print("%");
  } else if (currentPosition == 2) {  // Change Scale
    currentScale = (newPosition % numScales + numScales) % numScales;
    // Atualiza apenas a área da velocidade na tela
    tela.fillRect(20, 160, 200, 30, TFT_BLACK);  // Limpa a área antiga com um retângulo preto
    tela.setCursor(20, 160);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(scales[currentScale]);
  }
  // printStatus();
}


// VARREDURA
void setLED(int row, int col, uint32_t color) {
  int ledIndex = ledMatrix[row][col];
  strip.setPixelColor(ledIndex, color);
}

void setRow(int row, uint32_t color) {
  for (int col = 0; col < NUM_LINHAS; col++) {
    int ledIndex = ledMatrix[row][col];
    strip.setPixelColor(ledIndex, color);
  }
  strip.show();
}

void clearLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

uint32_t colorRGB(uint8_t r, uint8_t g, uint8_t b) {
  return strip.Color(r, g, b);
}

void printStatus() {
  Serial.println(currentInstrument);   // Instrumento atual
  Serial.println(currentScale);        // Escala atual
  Serial.println(tempo / musicSpeed);  // Velocidade atual
  Serial.println(save_status);         // Save status
  Serial.println(delete_status);       // Delete status
  save_status = 0;
  delete_status = 0;
}

void tickDoEncoder() {
  encoder.tick();
}

void atualizaLEDm(int col) {
  for (int linha = 0; linha < NUM_LINHAS; linha++) {
    for (int coluna = 0; coluna < NUM_COLUNAS; coluna++) {
      if (matriz[linha][coluna] == 'O') {
        setLED(linha, coluna, colorRGB(255, 0, 0));
      } else {
        setLED(linha, coluna, colorRGB(0, 0, 0));
      }
    }
  }
  for (int row = 0; row < NUM_COLUNAS; row++) {
    int ledIndex = ledMatrix[row][col];
    strip.setPixelColor(ledIndex, colorRGB(0, 0, 255));
    if (matriz[row][col] != 'O') {
      setLED(row, col, colorRGB(0, 0, 255));
    } else {
      setLED(row, col, colorRGB(0, 255, 0));
    }
  }
  strip.show();
  return;
}

#include <FastLED.h>
//#include <stdint.h>

#define PIN_CRONOMETRO 6
#define PIN_PUNTAJE_LOCAL 7
#define PIN_PUNTAJE_VISITANTE 8
#define PIN_FALTAS_LOCAL 9
#define PIN_FALTAS_VISITANTE 10
#define PIN_PERIODO 11

#define NUM_DIGITOS_CRONOMETRO 4  // Número de displays
#define NUM_DIGITOS_PUNTAJE 3
#define NUM_DIGITOS_CONTADORES 1

#define LEDS_POR_DIGITO 56  // LEDs por cada dígito
#define NUM_LEDS_CRONOMETRO (NUM_DIGITOS_CRONOMETRO * LEDS_POR_DIGITO)
#define NUM_LEDS_PUNTAJE (NUM_DIGITOS_PUNTAJE * LEDS_POR_DIGITO)
#define NUM_LEDS_CONTADORES (NUM_DIGITOS_CONTADORES * LEDS_POR_DIGITO)

CRGB puntaje_local_display[NUM_LEDS_PUNTAJE];
CRGB puntaje_visitante_display[NUM_LEDS_PUNTAJE];
CRGB cronometro_display[NUM_LEDS_CRONOMETRO];
CRGB faltas_local_display[NUM_LEDS_CONTADORES];
CRGB faltas_visitante_display[NUM_LEDS_CONTADORES];
CRGB periodo_display[NUM_LEDS_CONTADORES];

void mostrarNumero(uint8_t num, uint8_t offset, CRGB* display);
void mostrarTiempo(uint8_t minutos, uint8_t segundos, CRGB* display);
void cronometro(uint8_t& minutos, uint8_t& segundos, CRGB* display);
void mostrarPuntaje(uint8_t puntaje, CRGB* display);
void procesarComando(String comando);

const byte numeros[10] = {
  B00111111, B00000110, B01011011, B01001111, B01100110,
  B01101101, B01111101, B00000111, B01111111, B01101111
};

const uint8_t segmentos_leds[7][8] = {
  { 0, 1, 2, 3, 4, 5, 6, 7 },
  { 8, 9, 10, 11, 12, 13, 14, 15 },
  { 16, 17, 18, 19, 20, 21, 22, 23 },
  { 24, 25, 26, 27, 28, 29, 30, 31 },
  { 32, 33, 34, 35, 36, 37, 38, 39 },
  { 40, 41, 42, 43, 44, 45, 46, 47 },
  { 48, 49, 50, 51, 52, 53, 54, 55 }
};

static uint8_t segundos = 0, minutos = 0;
static uint8_t minutos_anteriores = 0, segundos_anteriores = 0;
static uint8_t puntaje_local = 0, puntaje_visitante = 0;
static uint8_t faltas_local = 0, faltas_visitante = 0, periodo = 1;
static bool cronometroActivo = false;
long inicioCronometro = 0;
long transcurrido = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, PIN_CRONOMETRO, GRB>(cronometro_display, NUM_LEDS_CRONOMETRO);
  FastLED.addLeds<WS2812, PIN_PUNTAJE_LOCAL, GRB>(puntaje_local_display, NUM_LEDS_PUNTAJE);
  FastLED.addLeds<WS2812, PIN_PUNTAJE_VISITANTE, GRB>(puntaje_visitante_display, NUM_LEDS_PUNTAJE);
  FastLED.addLeds<WS2812, PIN_FALTAS_LOCAL, GRB>(faltas_local_display, NUM_LEDS_CONTADORES);
  FastLED.addLeds<WS2812, PIN_FALTAS_VISITANTE, GRB>(faltas_visitante_display, NUM_LEDS_CONTADORES);
  FastLED.addLeds<WS2812, PIN_PERIODO, GRB>(periodo_display, NUM_LEDS_CONTADORES);
  FastLED.setBrightness(255);
  FastLED.clear();
  FastLED.show();
}


void loop() {
  if (cronometroActivo) {
    cronometro(minutos, segundos, cronometro_display);
  }
  mostrarPuntaje(puntaje_local, puntaje_local_display);
  mostrarPuntaje(puntaje_visitante, puntaje_visitante_display);
  mostrarNumero(faltas_local, 0, faltas_local_display);
  mostrarNumero(faltas_visitante, 0, faltas_visitante_display);
  mostrarNumero(periodo, 0, periodo_display);
  FastLED.show();

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    procesarComando(comando);
  }
}

void cronometro(uint8_t& minutos, uint8_t& segundos, CRGB* display) {
  static unsigned long ultimoTiempo = 0;
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoTiempo >= 1000)
  {
    ultimoTiempo = tiempoActual;

    long tiempoTranscurridoMS = tiempoActual - inicioCronometro + transcurrido;
    long segundosTotales = (tiempoTranscurridoMS / 1000);
    long minutosTotales = segundosTotales / 60;

    segundos = (uint8_t)(segundosTotales % 60);
    minutos =  (uint8_t)(minutosTotales % 60);
    /*
    if (++segundos >= 60) {
      segundos = 0;
      if (++minutos >= 100) minutos = 0;
    }
    */
    mostrarTiempo(minutos, segundos, display);
  }
}

void mostrarTiempo(uint8_t minutos, uint8_t segundos, CRGB* display) {
  Serial.print(minutos, DEC);
  Serial.print(":");
  Serial.println(segundos, DEC);
  if (minutos != minutos_anteriores) {
    mostrarNumero(minutos / 10, 3 * LEDS_POR_DIGITO, display);
    mostrarNumero(minutos % 10, 2 * LEDS_POR_DIGITO, display);
    minutos_anteriores = minutos;
    //Serial.println(minutos);
  }
  if (segundos != segundos_anteriores) {
    mostrarNumero(segundos / 10, 1 * LEDS_POR_DIGITO, display);
    mostrarNumero(segundos % 10, 0 * LEDS_POR_DIGITO, display);
    segundos_anteriores = segundos;
    //Serial.println(segundos);
  }
}

void mostrarNumero(uint8_t num, uint8_t offset, CRGB* display) {
  byte segmentos = numeros[num];
  for (uint8_t seg = 0; seg < 7; seg++) {
    bool encender = bitRead(segmentos, seg);
    for (uint8_t i = 0; i < 8; i++) {
      display[segmentos_leds[seg][i] + offset] = encender ? CRGB(255, 0, 0) : CRGB::Black;
    }
  }
}

void mostrarPuntaje(uint8_t puntaje, CRGB* display) {
  if (puntaje < 10) {
    mostrarNumero(puntaje % 10, 0 * LEDS_POR_DIGITO, display);
  } else if (puntaje < 100) {
    mostrarNumero((puntaje / 10) % 10, 1 * LEDS_POR_DIGITO, display);
    mostrarNumero(puntaje % 10, 0 * LEDS_POR_DIGITO, display);
  } else {
    mostrarNumero((puntaje / 100) % 10, 2 * LEDS_POR_DIGITO, display);
    mostrarNumero((puntaje / 10) % 10, 1 * LEDS_POR_DIGITO, display);
    mostrarNumero(puntaje % 10, 0 * LEDS_POR_DIGITO, display);
  }
}

void procesarComando(String comando) {
  if (comando == "IC") {
    cronometroActivo = true;
    inicioCronometro = millis();
  } else if (comando == "DC") {
    transcurrido = millis() - inicioCronometro + transcurrido;
    cronometroActivo = false;
  } else if (comando == "APL") {
    if (puntaje_local < 999) puntaje_local++;
  } else if (comando == "DPL") {
    if (puntaje_local > 0) puntaje_local--;
  } else if (comando == "APV") {
    if (puntaje_visitante < 999) puntaje_visitante++;
  } else if (comando == "DPV") {
    if (puntaje_visitante > 0) puntaje_visitante--;
  } else if (comando == "AFL") {
    if (faltas_local < 9) faltas_local++;
  } else if (comando == "DFL") {
    if (faltas_local > 0) faltas_local--;
  } else if (comando == "AFV") {
    if (faltas_visitante < 9) faltas_visitante++;
  } else if (comando == "DFV") {
    if (faltas_visitante > 0) faltas_visitante--;
  } else if (comando == "PP") {
    if (periodo < 9) periodo++;
  } else if (comando == "RP") {
    periodo = 1;
  } else {
    Serial.println("Comando no reconocido");
  }
}


#include "Config.h"

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 323 bytes
    {255, 13, 0, 12, 0, 60, 1, 19, 0, 0, 0, 0, 174, 1, 106, 200, 1, 1, 24, 0,
     2, 79, 14, 26, 13, 0, 36, 26, 31, 31, 79, 78, 0, 79, 70, 70, 0, 129, 38, 1,
     26, 10, 64, 16, 84, 105, 109, 101, 114, 0, 129, 14, 37, 25, 10, 64, 16, 76, 111, 99,
     97, 108, 0, 129, 60, 37, 40, 10, 64, 16, 86, 105, 115, 105, 116, 97, 110, 116, 101, 0,
     67, 6, 50, 44, 16, 93, 2, 200, 67, 58, 50, 44, 16, 93, 2, 200, 1, 4, 70, 21,
     16, 1, 134, 31, 43, 0, 1, 3, 155, 21, 16, 1, 134, 31, 43, 0, 1, 59, 155, 21,
     16, 1, 134, 31, 43, 0, 1, 58, 70, 21, 16, 1, 134, 31, 43, 0, 1, 81, 95, 21,
     16, 1, 134, 31, 43, 0, 1, 28, 70, 21, 16, 1, 1, 31, 45, 0, 1, 82, 70, 21,
     16, 1, 1, 31, 45, 0, 1, 27, 155, 21, 16, 1, 1, 31, 45, 0, 1, 82, 155, 21,
     16, 1, 1, 31, 45, 0, 129, 37, 92, 35, 10, 64, 16, 80, 101, 114, 105, 111, 100, 111,
     0, 67, 27, 103, 52, 16, 93, 190, 200, 1, 81, 112, 21, 16, 1, 1, 31, 45, 0, 129,
     40, 126, 28, 10, 64, 16, 70, 97, 108, 116, 97, 115, 0, 67, 5, 137, 44, 16, 93, 2,
     200, 67, 59, 137, 44, 16, 93, 2, 200, 12, 4, 173, 98, 10, 194, 30, 186, 83, 101, 108,
     101, 99, 99, 105, 111, 110, 101, 0, 70, 117, 116, 115, 97, 108, 0, 72, 97, 110, 100, 98,
     97, 108, 108, 0, 86, 111, 108, 108, 101, 121, 0, 1, 4, 185, 99, 13, 1, 27, 31, 82,
     101, 105, 110, 105, 99, 105, 97, 114, 32, 116, 111, 100, 111, 0, 67, 29, 11, 47, 17, 69,
     135, 187, 7};

// this structure defines all the variables and events of your control interface
struct
{

  // input variables
  uint8_t on_off;                // =1 if switch ON and =0 if OFF
  uint8_t incre_punto_local;     // =1 if button pressed, else =0
  uint8_t incre_falta_local;     // =1 if button pressed, else =0
  uint8_t incre_falta_visitante; // =1 if button pressed, else =0
  uint8_t incre_punto_visitante; // =1 if button pressed, else =0
  uint8_t incre_periodo;         // =1 if button pressed, else =0
  uint8_t decre_punto_local;     // =1 if button pressed, else =0
  uint8_t decre_punto_visitante; // =1 if button pressed, else =0
  uint8_t decre_falta_local;     // =1 if button pressed, else =0
  uint8_t decre_falta_visitante; // =1 if button pressed, else =0
  uint8_t decre_periodo;         // =1 if button pressed, else =0
  uint8_t selector;              // from 0 to 4
  uint8_t reiniciar;             // =1 if button pressed, else =0

  // output variables
  int8_t p_Local;     // -128 .. 127
  int8_t p_Visitante; // -128 .. 127
  int8_t periodo;     // -128 .. 127
  int8_t f_Local;     // -128 .. 127
  int8_t f_Visitante; // -128 .. 127
  char cronometro[7]; // string UTF8 end zero

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

// END REMOTEXY

CRGB puntaje_local_display[NUM_LEDS_PUNTAJE];
CRGB puntaje_visitante_display[NUM_LEDS_PUNTAJE];
CRGB cronometro_display[NUM_LEDS_CRONOMETRO];
CRGB faltas_local_display[NUM_LEDS_CONTADORES];
CRGB faltas_visitante_display[NUM_LEDS_CONTADORES];
CRGB periodo_display[NUM_LEDS_CONTADORES];

static uint8_t segundos = 0, minutos = 0;
static uint8_t minutos_anteriores = 0, segundos_anteriores = 0;
static uint8_t puntaje_local = 0, puntaje_visitante = 0;
static uint8_t faltas_local = 0, faltas_visitante = 0, periodo = 1;
unsigned long inicioCronometro = 0;
unsigned long transcurrido = 0;

bool cronometroActivo = false;

byte NUMEROS[10] = {
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111  // 9
};

const uint8_t SEGMENTOS_LEDS[7][5] = {
    {5, 6, 7, 8, 9},      // A
    {0, 1, 2, 3, 4},      // B
    {20, 21, 22, 23, 24}, // C
    {25, 26, 27, 28, 29}, // D
    {30, 31, 32, 33, 34}, // E
    {10, 11, 12, 13, 14}, // F
    {15, 16, 17, 18, 19}  // G
};

void mostrarNumero(uint8_t num, uint8_t offset, CRGB *display);
void mostrarTiempo(uint8_t minutos, uint8_t segundos, CRGB *display);
void cronometro(uint8_t &minutos, uint8_t &segundos, CRGB *display);
void mostrarPuntaje(uint8_t puntaje, CRGB *display);

void setupLeds();
void setupRemoteXY();
void handlePuntajeLocal();
void handlePuntajeVisitante();
void handleFaltasLocal();
void handleFaltasVisitante();
void handlePerido();
void handleCronometro();
void handleReiniciar();
void handleButtons();

void actulizarDisplay();
void actualizarRemoteXY();

void setup()
{
  RemoteXY_Init();
  setupLeds();
  setupRemoteXY();
  Serial.begin(115200);
}

void loop()
{
  RemoteXY_Handler();
  handleButtons();
  if (cronometroActivo)
  {
    cronometro(minutos, segundos, cronometro_display);
  }
}

void setupLeds()
{
  FastLED.addLeds<WS2813, PIN_CRONOMETRO, GRB>(cronometro_display, NUM_LEDS_CRONOMETRO);
  FastLED.addLeds<WS2813, PIN_PUNTAJE_LOCAL, GRB>(puntaje_local_display, NUM_LEDS_PUNTAJE);
  FastLED.addLeds<WS2813, PIN_PUNTAJE_VISITANTE, GRB>(puntaje_visitante_display, NUM_LEDS_PUNTAJE);
  FastLED.addLeds<WS2813, PIN_FALTAS_LOCAL, GRB>(faltas_local_display, NUM_LEDS_CONTADORES);
  FastLED.addLeds<WS2813, PIN_FALTAS_VISITANTE, GRB>(faltas_visitante_display, NUM_LEDS_CONTADORES);
  FastLED.addLeds<WS2813, PIN_PERIODO, GRB>(periodo_display, NUM_LEDS_CONTADORES);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void cronometro(uint8_t &minutos, uint8_t &segundos, CRGB *display)
{
  if (!cronometroActivo)
    return;

  static unsigned long ultimoTiempo = 0;
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoTiempo >= 1000)
  {
    ultimoTiempo = tiempoActual;

    unsigned long tiempoTranscurridoMS = tiempoActual - inicioCronometro + transcurrido;

    long segundosTotales = (tiempoTranscurridoMS / 1000);
    long minutosTotales = segundosTotales / 60;

    segundos = (uint8_t)(segundosTotales % 60);
    minutos = (uint8_t)(minutosTotales % 60);

    // Serial.print(tiempoTranscurridoMS);
    // Serial.print("/////");
    Serial.print(minutos, DEC);
    Serial.print(":");
    Serial.println(segundos, DEC);
    Serial.print("/////puntaje_local: ");
    Serial.print(puntaje_local, DEC);

    // Aca debe ir el codigo para cambiar el valor de RemoteXY.cronometro
    snprintf(RemoteXY.cronometro, sizeof(RemoteXY.cronometro), "%02d:%02d", minutos, segundos);

    mostrarTiempo(minutos, segundos, display);
  }
}

void mostrarTiempo(uint8_t minutos, uint8_t segundos, CRGB *display)
{

  if (minutos != minutos_anteriores)
  {
    mostrarNumero(minutos / 10, 3 * LEDS_POR_DIGITO, display);
    mostrarNumero(minutos % 10, 2 * LEDS_POR_DIGITO, display);
    minutos_anteriores = minutos;
  }
  if (segundos != segundos_anteriores)
  {
    mostrarNumero(segundos / 10, 1 * LEDS_POR_DIGITO, display);
    mostrarNumero(segundos % 10, 0 * LEDS_POR_DIGITO, display);
    segundos_anteriores = segundos;
  }
}

void mostrarNumero(uint8_t num, uint8_t offset, CRGB *display)
{
  byte segmentos = NUMEROS[num];
  for (uint8_t seg = 0; seg < 7; seg++)
  {
    bool encender = bitRead(segmentos, seg);
    for (uint8_t i = 0; i < 5; i++)
    {
      display[SEGMENTOS_LEDS[seg][i] + offset] = encender ? CRGB(255, 0, 0) : CRGB::Black;
    }
  }
  FastLED.show();
}

void mostrarPuntaje(uint8_t puntaje, CRGB *display)
{
  if (puntaje < 10)
  {
    mostrarNumero(puntaje % 10, 0 * LEDS_POR_DIGITO, display);
  }
  else if (puntaje < 100)
  {
    mostrarNumero((puntaje / 10) % 10, 1 * LEDS_POR_DIGITO, display);
    mostrarNumero(puntaje % 10, 0 * LEDS_POR_DIGITO, display);
  }
  else
  {
    mostrarNumero((puntaje / 100) % 10, 2 * LEDS_POR_DIGITO, display);
    mostrarNumero((puntaje / 10) % 10, 1 * LEDS_POR_DIGITO, display);
    mostrarNumero(puntaje % 10, 0 * LEDS_POR_DIGITO, display);
  }
}

void handlePuntajeLocal()
{
  if (RemoteXY.incre_punto_local)
  {
    RemoteXY.incre_punto_local = 0;
    if (puntaje_local < 255)
    {
      puntaje_local++;
      mostrarPuntaje(puntaje_local, puntaje_local_display);
      RemoteXY.p_Local = puntaje_local;
    }
  }

  if (RemoteXY.decre_punto_local)
  {
    RemoteXY.decre_punto_local = 0;
    if (puntaje_local > 0)
    {
      puntaje_local--;
      mostrarPuntaje(puntaje_local, puntaje_local_display);
      RemoteXY.p_Local = puntaje_local;
    }
  }
}

void handlePuntajeVisitante()
{
  if (RemoteXY.incre_punto_visitante)
  {
    RemoteXY.incre_punto_visitante = 0;
    if (puntaje_visitante < 255)
    {
      puntaje_visitante++;
      mostrarPuntaje(puntaje_visitante, puntaje_visitante_display);
      RemoteXY.p_Visitante = puntaje_visitante;
    }
  }

  if (RemoteXY.decre_punto_visitante)
  {
    RemoteXY.decre_punto_visitante = 0;
    if (puntaje_visitante > 0)
    {
      puntaje_visitante--;
      mostrarPuntaje(puntaje_visitante, puntaje_visitante_display);
      RemoteXY.p_Visitante = puntaje_visitante;
    }
  }
}

void handlePerido()
{
  if (RemoteXY.incre_periodo)
  {
    RemoteXY.incre_periodo = 0;
    if (periodo < 4)
    {
      periodo++;
      mostrarNumero(periodo, 0, periodo_display);
      RemoteXY.periodo = periodo;
    }
  }

  if (RemoteXY.decre_periodo)
  {
    RemoteXY.decre_periodo = 0;
    if (periodo > 1)
    {
      periodo--;
      mostrarNumero(periodo, 0, periodo_display);
      RemoteXY.periodo = periodo;
    }
  }
}

void handleCronometro()
{
  if (RemoteXY.on_off == 1 && !cronometroActivo)
  {
    cronometroActivo = true;
    inicioCronometro = millis();
  }
  else if (RemoteXY.on_off == 0 && cronometroActivo)
  {
    cronometroActivo = false;
    transcurrido = millis() - inicioCronometro + transcurrido;
  }
}

void handleReiniciar()
{
  if (RemoteXY.reiniciar == 1)
  {
    RemoteXY.reiniciar = 0;
    // Reiniciar el cronometro y los puntajes
    strcpy(RemoteXY.cronometro, "00:00");
    puntaje_local = 0;
    puntaje_visitante = 0;
    faltas_local = 0;
    faltas_visitante = 0;
    periodo = 1;
    minutos = 0;
    segundos = 0;
    transcurrido = 0;
    cronometroActivo = false;
    inicioCronometro = 0;
    
    RemoteXY.on_off = 0;
    RemoteXY.f_Local = faltas_local;
    RemoteXY.f_Visitante = faltas_visitante;
    RemoteXY.p_Local = puntaje_local;
    RemoteXY.p_Visitante = puntaje_visitante;
    RemoteXY.periodo = periodo;

    actulizarDisplay();
  }
}

void handleFaltasLocal()
{
  if (RemoteXY.incre_falta_local)
  {
    RemoteXY.incre_falta_local = 0;
    if (faltas_local < 255)
    {
      faltas_local++;
      mostrarNumero(faltas_local, 0, faltas_local_display);
      RemoteXY.f_Local = faltas_local;
    }
  }

  if (RemoteXY.decre_falta_local)
  {
    RemoteXY.decre_falta_local = 0;
    if (faltas_local > 0)
    {
      faltas_local--;
      mostrarNumero(faltas_local, 0, faltas_local_display);
      RemoteXY.f_Local = faltas_local;
    }
  }
}
void handleFaltasVisitante()
{
  if (RemoteXY.incre_falta_visitante)
  {
    RemoteXY.incre_falta_visitante = 0;
    if (faltas_visitante < 255)
    {
      faltas_visitante++;
      mostrarNumero(faltas_visitante, 0, faltas_visitante_display);
      RemoteXY.f_Visitante = faltas_visitante;
    }
  }

  if (RemoteXY.decre_falta_visitante)
  {
    RemoteXY.decre_falta_visitante = 0;
    if (faltas_visitante > 0)
    {
      faltas_visitante--;
      mostrarNumero(faltas_visitante, 0, faltas_visitante_display);
      RemoteXY.f_Visitante = faltas_visitante;
    }
  }
}

void actualizarRemoteXY()
{
  RemoteXY.p_Local = puntaje_local;
  RemoteXY.p_Visitante = puntaje_visitante;
  RemoteXY.f_Local = faltas_local;
  RemoteXY.f_Visitante = faltas_visitante;
  RemoteXY.periodo = periodo;
}

void actulizarDisplay()
{
  mostrarPuntaje(puntaje_local, puntaje_local_display);
  mostrarPuntaje(puntaje_visitante, puntaje_visitante_display);
  mostrarNumero(faltas_local, 0, faltas_local_display);
  mostrarNumero(faltas_visitante, 0, faltas_visitante_display);
  mostrarNumero(periodo, 0, periodo_display);
  mostrarTiempo(minutos, segundos, cronometro_display);
}

void handleButtons()
{
  handleCronometro();
  handleFaltasLocal();
  handleFaltasVisitante();
  handlePerido();
  handlePuntajeLocal();
  handlePuntajeVisitante();
  handleReiniciar();
}

void setupRemoteXY()
{
  RemoteXY.on_off = 0;
  RemoteXY.p_Local = puntaje_local;
  RemoteXY.p_Visitante = puntaje_visitante;
  RemoteXY.f_Local = faltas_local;
  RemoteXY.f_Visitante = faltas_visitante;
  RemoteXY.periodo = periodo;
  strcpy(RemoteXY.cronometro, "00:00");
}
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <FastLED.h>
#include <stdint.h>
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>

// RemoteXY connection settings
#define REMOTEXY_BLUETOOTH_NAME "Tablero_Marcador"
#include <RemoteXY.h>

#define PIN_CRONOMETRO 5
#define PIN_PUNTAJE_LOCAL 13
#define PIN_PUNTAJE_VISITANTE 14
#define PIN_FALTAS_LOCAL 18
#define PIN_FALTAS_VISITANTE 19
#define PIN_PERIODO 21

#define NUM_DIGITOS_CRONOMETRO 4 // Número de displays
#define NUM_DIGITOS_PUNTAJE 3
#define NUM_DIGITOS_CONTADORES 1

#define LEDS_POR_DIGITO 35 // LEDs por cada dígito
#define NUM_LEDS_CRONOMETRO (NUM_DIGITOS_CRONOMETRO * LEDS_POR_DIGITO)
#define NUM_LEDS_PUNTAJE (NUM_DIGITOS_PUNTAJE * LEDS_POR_DIGITO)
#define NUM_LEDS_CONTADORES (NUM_DIGITOS_CONTADORES * LEDS_POR_DIGITO)
#define BRIGHTNESS 50

#endif // CONFIG_H
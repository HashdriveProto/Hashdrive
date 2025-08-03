#pragma once
#include <FastLED.h>
#include "panel_struct.h"

// --- Colores principales ---
#define red 253
#define green 45
#define blue 28

#define redPiercing 128
#define greenPiercing 0
#define bluePiercing 255

// --- Configuración de hardware ---
#define NUM_LEDS 495
#define DATA_PIN 5
#define BRILLO_MAX 10 // Brillo máximo de los LEDs
#define BRILLO_MIN 1  // Brillo mínimo de los LEDs
#define BRILLO_DEFAULT 5 // Brillo por defecto de los LEDs
// ----------------------------------------------
// Métodos
// --- Función de testeo de LEDs ---
void testLeds(CRGB* leds, int numLeds);
// --- Función para inicializar la matriz de LEDs ---
void inicializarMatriz(LedPixel panel[15][33]);
// --- Función para construir la cara por defecto ---
void construirCaraDefault(LedPixel panel[15][33]);
// --- Función para visualizar la matriz de LEDs en el monitor serial ---
void visualizarMatriz(LedPixel panel[15][33]);
// --- Función para calcular el parpadeo ---
int TiempoParpadeo();
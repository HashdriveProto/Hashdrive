#pragma once
#include <FastLED.h>
#include "panel_struct.h"

// Función para pintar la matriz en la tira LED
void pintarPanel(LedPixel panel[15][33], CRGB* leds);
// Funcion para ejecutar el parpadeo de los LEDs
void parpadearLeds(LedPixel panel[15][33], CRGB* leds, int listaElementos[], int numElementos);

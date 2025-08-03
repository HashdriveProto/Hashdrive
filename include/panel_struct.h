#pragma once
#include <FastLED.h>

// Enumeración para los elementos de la cara
enum ElementoCara {
    BOCA = 0,
    NARIZ = 1,
    OJO_IZQUIERDO = 2,
    OJO_DERECHO = 3,
    PIERCING = 4,
    OTRO = 5
};

// Estructura para cada LED del panel
struct LedPixel {
    int posicion;         // Número de LED en la tira
    bool estado;          // Encendido/apagado
    CRGB color;           // Color actual
    ElementoCara elemento;// Tipo de elemento facial
};

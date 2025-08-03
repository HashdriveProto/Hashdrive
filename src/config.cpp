#include <FastLED.h>
#include "config.h"
#include "panel_struct.h"
#include "system.h"

// Función para testear la tira LED: recorre todos los LEDs y los enciende en rojo, verde, azul y blanco
void testLeds(CRGB* leds, int numLeds) {
    // Colores de prueba: rojo, verde, azul, blanco
    CRGB colores[4] = {CRGB(255,0,0), CRGB(0,255,0), CRGB(0,0,255), CRGB(255,255,255)};
    // Se recorre la matriz de LEDs y se encienden con cada color
    for (int c = 0; c < 4; c++) {
        // Para iterar por tira en pares
        for (int tira = 0; tira < 17; tira++) {
            // Para iterar por led
            for(int pos = 0; pos <15; pos++) {
                leds[240 + pos + (tira * 15)] = colores[c];
                leds[240 + pos - (tira * 15)] = colores[c];
            }
            FastLED.show();
            delay(3); // Tiempo visible de cada color
        }
    }
    // Apaga todos los LEDs al terminar
    for (int i = 0; i < numLeds; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
// Funcion para inicializar la matriz de LEDs, la matriz ya no importare el orden de recorrido
void inicializarMatriz(LedPixel panel[15][33]) {
    // Se genera una variable que solo existira aqui para guardar la posicion
    int posicion = 0;
    // Se recorre primero la columna, ya que inicia en la esquina superior izquierda y debe recorrerse en direccion hacia abajo
    for (int y = 0; y < 33; y++) {
        for (int x = 0; x < 15; x++) {
            // El panel debe llenarse de arriba a abajo, y de izquierda a derecha
            if (y%2 == 0) {
                panel[x][y].posicion = posicion;
            } else {
                panel[14 - x][y].posicion = posicion;
            }
            // Se asigna todo lo demas apagado
            panel[x][y].estado = false;
            panel[x][y].color = CRGB::Black;
            panel[x][y].elemento = OTRO;
            posicion++; // Incrementa la posicion para el siguiente LED
        }
    }
}

// Función para construir la cara por defecto
void construirCaraDefault(LedPixel panel[15][33]) {
    // Inicializa todos los LEDs como apagados y color negro
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 33; y++) {
            // Ya no se toca la posicion, ya que esa es fija y nada deberia tocarla salvo la inicializacion
            panel[x][y].estado = false;
            panel[x][y].color = CRGB::Black;
            panel[x][y].elemento = OTRO;
        }
    }
    // Se realiza el recorrido para asignar los elementos de la cara
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 33; y++) {
            // Para la generación del ojo derecho
            if (
                ( y == 4 && (x == 2 || x == 3)) || 
                ( y == 3 && (x == 1 || x == 2 || x == 3 || x == 4)) ||
                ((y == 1 || y == 2) && (x == 0 || x == 1 || x == 2 || x == 3 || x == 4)) ||
                ( y == 0 && (x == 1 || x == 2 || x == 3))
                ){
                panel[x][y].estado = true;
                panel[x][y].color = CRGB(red, green, blue);
                panel[x][y].elemento = OJO_DERECHO;
            }
            // Para la generación del ojo derecho
            if (
                ( y == 28 && (x == 2 || x == 3)) || 
                ( y == 29 && (x == 1 || x == 2 || x == 3 || x == 4)) ||
                ((y == 31 || y == 30) && (x == 0 || x == 1 || x == 2 || x == 3 || x == 4)) ||
                ( y == 32 && (x == 1 || x == 2 || x == 3))
                ){
                panel[x][y].estado = true;
                panel[x][y].color = CRGB(red, green, blue);
                panel[x][y].elemento = OJO_IZQUIERDO;
            }
            //Para la sección de la boca
            if (((y == 5 || y == 7 || y == 9 || y == 11 || y == 13 || y == 15 || y == 17 || y == 19 || y == 21 || y == 23 || y == 25 || y == 27) && (x == 13 || x == 12)) || // Generan el bloque medio de la boca
                ((y == 6 || y == 10 || y == 14 || y == 18 || y == 22 || y == 26) && (x == 13 || x == 14)) || // Genera el bloque bajo de la boca
                ((y == 4 || y == 8 || y == 12 || y == 16 || y == 20 || y == 24 || y == 28) && (x == 11 || x == 12)) // Seccion alta de la boca
            ){
                panel[x][y].estado = true;
                panel[x][y].color = CRGB(red, green, blue);
                panel[x][y].elemento = BOCA;
            }
            // Para la generación de la nariz
            if (((y == 14 || y == 18) && (x == 1 || x == 2 )) || 
                ((y == 13 || y == 19) && (x == 0 || x == 1)) || 
                ((y == 12 || y == 20) && x == 0)
            ){
                panel[x][y].estado = true;
                panel[x][y].color = CRGB(red, green, blue);
                panel[x][y].elemento = NARIZ;
            }
            // Piercing
            if ((y == 16  && x == 5) ||
                ((y == 15 || y == 17)  && x == 5) ||
                ((y == 14 || y == 18)  && x == 4) ||
                ((y == 13 || y == 19)  && (x == 2 || x == 3))
            ){
                panel[x][y].estado = true;
                panel[x][y].color = CRGB(redPiercing, greenPiercing, bluePiercing);
                panel[x][y].elemento = PIERCING;
            }
        }
    }
}

// Se calcula el tiempo de parpadeo
int TiempoParpadeo() {
  // Fórmula logarítmica que simula la frecuencia realista de parpadeo humano
  return round((-4 * log(1 - 0.97 * random(0, 99) / 100) + 2) * 100);
}
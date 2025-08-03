#include "system.h"

// Para Visualizar la matriz de LEDs en el monitor serial
void visualizarMatriz(LedPixel panel[15][33]) {
    Serial.println("// ------------------------------------------- //");
    Serial.println("// Iniciando el recorrido de la matriz de LEDs //");
    Serial.println("// ------------------------------------------- //");
    for (int x = 0; x < 15; x++) {
        Serial.print("| ");
        for (int y = 0; y < 33; y++) {
            Serial.print(panel[x][y].posicion);
            Serial.print(" | ");
        }
        Serial.println("");
    }
    Serial.println("Finalizado el recorrido de la matriz"); 
}

// Recorre la matriz y pinta los LEDs físicos según el estado y color de cada LedPixel
void pintarPanel(LedPixel panel[15][33], CRGB* leds) {
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 33; y++) {
            int idx = panel[x][y].posicion;
            if (panel[x][y].estado) {
                leds[idx] = panel[x][y].color;
            }else {
                leds[idx] = CRGB::Black;
            }
        }
    }
    // Envia los datos a la tira LED
    FastLED.show();
}

// Se agrega la funcion para parpadear los LEDs, se agrega una lista de elementos que se parpadearan
void parpadearLeds(LedPixel panel[15][33], CRGB* leds, int listaElementos[], int numElementos) {
    // Se genera una variable pasa saber si hay un cambio, si no omite el refresh de la pantalla
    bool cambio = false;
    // recorre la patriz de leds para cerrar los ojos, de arriba a abajo
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 33; y++) {
            // Verifica si el elemento actual está en la lista de elementos a parpadear
            for (int i = 0; i < numElementos; i++) {
                if (panel[x][y].elemento == listaElementos[i]) {
                    // Si el elemento está en la lista, cambia su estado a apagado
                    panel[x][y].estado = false;
                    cambio = true;
                }
            }
        }
        // Si hubo un cambio en el estado de los LEDs, se pinta el panel
        if (cambio){
            // Se espera un tiempo para simular el parpadeo
            delay(1);
            // Se manda a pintar el panel
            pintarPanel(panel, leds);
        }
    }
    // Se recorre la matriz para prender los leds de nuevo, pero de abajo hacia arriba
    // recorre la patriz de leds para cerrar los ojos, de arriba a abajo
    for (int x = 14; x >= 0; x--) {
        for (int y = 0; y < 33; y++) {
            // Verifica si el elemento actual está en la lista de elementos a parpadear
            for (int i = 0; i < numElementos; i++) {
                if (panel[x][y].elemento == listaElementos[i]) {
                    // Si el elemento está en la lista, cambia su estado a apagado
                    panel[x][y].estado = true;
                    cambio = true;
                }
            }
        }
        // Si hubo un cambio en el estado de los LEDs, se pinta el panel
        if (cambio){
            // Se espera un tiempo para simular el parpadeo
            delay(1);
            // Se manda a pintar el panel
            pintarPanel(panel, leds);
        }
    }
}
// Librerias
#include <FastLED.h>
#include "config.h" // Incluye pines, número de LEDs y colores definidos en un solo lugar
#include "panel_struct.h"
#include "system.h"
// ------------------------------------------------------------------------
// Se definen las variables necesarias
// ------------------------------------------------------------------------
// Arreglo que representa el estado de cada LED físico en la tira
CRGB leds[NUM_LEDS];
// Variables para controlar el tiempo de parpadeo
unsigned long contadorParpadeo = 0;
unsigned long intervaloParpadeo = 0;
int parpadeoActivo = 0; // Variable para controlar el estado del parpadeo
// Definición de la matriz del panel (ajusta el tamaño según tu panel real)
LedPixel panel[15][33];
// ------------------------------------------------------------------------
// Configuración inicial del sistema
void setup() {
  // Inicializa salida serial para debugging si lo necesitas
  Serial.begin(115200);
  // Inicializa la matriz de LEDs
  inicializarMatriz(panel);
  // Construye la cara por defecto
  construirCaraDefault(panel);
  // Configura la tira LED: tipo de LEDs, pin de datos, orden de color
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  // Ajusta el brillo general de la tira para el testeo
  FastLED.setBrightness(BRILLO_MAX);
  // --- Test de la tira LED al arranque ---
  // Enciende todos los LEDs en rojo, verde, azul y blanco secuencialmente
  // y luego los apaga. Solo se ejecuta una vez al inicio.
  testLeds(leds, NUM_LEDS);
  // Ajusta el brillo general de la tira (puedes modificar)
  FastLED.setBrightness(BRILLO_DEFAULT);
  // --- Fin del testeo de LEDs ---
  // Calcula el primer tiempo de parpadeo
  intervaloParpadeo = TiempoParpadeo();
}
// Bucle principal del programa
void loop() {
  // Verifica si es momento de parpadear
  if (contadorParpadeo < intervaloParpadeo) {
    contadorParpadeo++;
  }else{
    int elementos[] = {OJO_IZQUIERDO, OJO_DERECHO}; // Elementos a parpadear
    int numElementos = sizeof(elementos) / sizeof(elementos[0]); // Calcula el número de elementos
    parpadearLeds(panel, leds, elementos, numElementos);
    // Calcula el nuevo tiempo de parpadeo
    intervaloParpadeo = TiempoParpadeo();
    contadorParpadeo = 0; // Reinicia el contador
  }
  // Se pinta el panel usando pintarPanel
  pintarPanel(panel, leds);
  delay(1); // Pequeño retraso para evitar sobrecargar el bucle
}

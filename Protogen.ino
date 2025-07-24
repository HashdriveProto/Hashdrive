// Librerias
#include <Adafruit_NeoPixel.h> // Libreria de control de tira led WS2812B // Hay un problema con la libreria al manejar mas de 70 leds en diferentes tiras, esta libreria lo medio soluciona https://github.com/teknynja/Adafruit_NeoPixel/tree/esp32_rmt_memory_allocation_fix_safe
#include <BluetoothSerial.h>// 1. Para agregar el Bluethoot
// Para el mabejo del bluetooth
// 1. Se agrega la libreria
// 2. Se genera el objeto
// 3. Se inicializa el objeto
// 4. Se genera un while para trabajar el bluetooth, aqui se puede enviar y recibir información a travez del bluetooth
// ----------------------------------------------------------------------
// Constantes
// Se definen los puertos de salida de las tiras LED
#define portPanelC 12
#define portPanel 13
#define portOrejaI 26
#define portOrejaD 27
//#define pinLed 2 // Led azul del ESP32
// Se define el total de leds para cada tira
#define ledsPanelC 15
//#define ledsOreja 40
#define ledsPanel 240 //279 // 16x, 15y
// Se define el RGB del escarlata
#define red 253
#define green 45
#define blue 28
#define redPiercing 128
#define greenPiercing 0
#define bluePiercing 255
// ----------------------------------------------------------------------
// Variable
int brillo = 5; // Brillo de leds, maxio 10
int parpadeo = 0; // esta variable indica cada cuantos delay hay que iniciar el ciclo de parpadeo
int contadorParpadeo = 0; // Esta variable se usa para saver cuantas veces se ha hecho el ciclo, si se iguala a la del parpadeo, se ejecuta el parpadeo
// se crean las matrices de control
// Matrices de control, 0:Numero de led en la tira, 1:Estado, 2:rojo, 3:Verde, 4: Azul, 5:Indicador de elemento de la cara 1:boca 2:nariz, 3: ojos, 4: piercing respaldo de estado(se usa en parpadeo)
int controlPanelCentral[1][15][6] = {};
int controlPanel[21][15][6] = {};
// Para el control del buetooth
String valorBT; // Para recuperar el valor del Bluetooth
BluetoothSerial serialBT; // 2. Se genera el objeto del pluetooth
// Inicialización d elas tiras
Adafruit_NeoPixel panelC = Adafruit_NeoPixel(ledsPanelC, portPanelC, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel panelOrejaI = Adafruit_NeoPixel(ledsOreja, portOrejaI, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel panelOrejaD = Adafruit_NeoPixel(ledsOreja, portOrejaD, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel panel = Adafruit_NeoPixel(ledsPanel , portPanel, NEO_GRB + NEO_KHZ800);
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Para programar el tiempo del parpadeo
void TiempoParpadeo(){
  parpadeo = round((-4 * log( 1 - 0.97 * random(0,99)/100) + 2)*100); // obj = round((-4 * log( 1 -0.9 * random(0,99)/100) + 0.5)*100);
}

// Se crea un método para visualizar el contenido de las matrices
void VisualizarMatrices(){
  Serial.println("Se muestra el panel lateral");
  Serial.println("Continuidad tira");
  // Para imprimir se recorre primero Y para que se visualice por renglon
  for(int posY = 0; posY < 15; posY++){
    for(int posX = 0; posX < 16; posX++){
      Serial.print("\t");
      Serial.print(controlPanel[posX][posY][0]);
      Serial.print(" - ");
    }
    Serial.println("");
  }
  // para visualizar el panel central
  Serial.println("Se muestra el panel Central:");
  for(int posY = 0; posY < 15; posY++){
    Serial.println(controlPanelCentral[0][posY][0]);
  }
  Serial.println("Estado tira");
  // Para imprimir se recorre primero Y para que se visualice por renglon
  for(int posY = 0; posY < 15; posY++){
    for(int posX = 0; posX < 16; posX++){
      Serial.print("\t");
      Serial.print(controlPanel[posX][posY][1]);
      Serial.print(" - ");
    }
    Serial.println("");
  }
  // para visualizar el panel central
  Serial.println("Se muestra el panel Central:");
  for(int posY = 0; posY < 15; posY++){
    Serial.println(controlPanelCentral[0][posY][1]);
  }
  Serial.println("Roj tira");
  // Para imprimir se recorre primero Y para que se visualice por renglon
  for(int posY = 0; posY < 15; posY++){
    for(int posX = 0; posX < 16; posX++){
      Serial.print("\t");
      Serial.print(controlPanel[posX][posY][2]);
      Serial.print(" - ");
    }
    Serial.println("");
  }
  // para visualizar el panel central
  Serial.println("Se muestra el panel Central:");
  for(int posY = 0; posY < 15; posY++){
    Serial.println(controlPanelCentral[0][posY][2]);
  }
  Serial.println("Verde tira");
  // Para imprimir se recorre primero Y para que se visualice por renglon
  for(int posY = 0; posY < 15; posY++){
    for(int posX = 0; posX < 16; posX++){
      Serial.print("\t");
      Serial.print(controlPanel[posX][posY][3]);
      Serial.print(" - ");
    }
    Serial.println("");
  }
  // para visualizar el panel central
  Serial.println("Se muestra el panel Central:");
  for(int posY = 0; posY < 15; posY++){
    Serial.println(controlPanelCentral[0][posY][3]);
  }
  Serial.println("Azul tira");
  // Para imprimir se recorre primero Y para que se visualice por renglon
  for(int posY = 0; posY < 15; posY++){
    for(int posX = 0; posX < 16; posX++){
      Serial.print("\t");
      Serial.print(controlPanel[posX][posY][4]);
      Serial.print(" - ");
    }
    Serial.println("");
  }
  // para visualizar el panel central
  Serial.println("Se muestra el panel Central:");
  for(int posY = 0; posY < 15; posY++){
    Serial.println(controlPanelCentral[0][posY][4]);
  }
  Serial.println("Elemento tira");
  // Para imprimir se recorre primero Y para que se visualice por renglon
  for(int posY = 0; posY < 15; posY++){
    for(int posX = 0; posX < 16; posX++){
      Serial.print("\t");
      Serial.print(controlPanel[posX][posY][5]);
      Serial.print(" - ");
    }
    Serial.println("");
  }
  // para visualizar el panel central
  Serial.println("Se muestra el panel Central:");
  for(int posY = 0; posY < 15; posY++){
    Serial.println(controlPanelCentral[0][posY][5]);
  }
}
// Se inicializan los controles de los paneles
void Inicializar(){
  int posicionLed = 0;
  // Se rellenan todos los valores de posision con -1
  for(int posX = 0; posX < 21; posX++){
    for (int posY = 0; posY < 15; posY++){
      controlPanel[posX][posY][0] = -1;
      controlPanel[posX][posY][1] = 0;
      controlPanel[posX][posY][2] = 0;
      controlPanel[posX][posY][3] = 0;
      controlPanel[posX][posY][4] = 0;
      controlPanel[posX][posY][5] = 0;
    }
  }
  // Recorrido para inicializar los paneles // 2025/May/12 - Se actualizo por que se quito la saliente mas una tira sobrante de 21 a 16
  for(int posX = 0; posX < 16; posX++){
    for (int posY = 0; posY < 15; posY++){
      //controlPanel[posX][posY][0] = -1; // Se le manda a todos los leds la posicion -1 para indicar que no es un led válido
      // Se inicializa el panel lateral, sobreescribiendo el -1
      if (posicionLed<ledsPanel){
        // Se llena la matriz hasta la columna 17 ahi es cuando se hace la saliente del ojo // 2025/Mayo/12 - Se quitaron 5 columnas al final cambiando de 21 a 16
        if (posX<16){
          if (posX == 0 || posX%2 == 0){
            controlPanel[posX][15 - (posY+1)][0] = posicionLed;
          }
          else{
            controlPanel[posX][posY][0] = posicionLed;
          }
        }
      }
      
      // metodo para generar el mapeo en el panel central
      if (posX == 0){
        controlPanelCentral[0][posY][0] = posicionLed;
      }
      // Al final de todo se configura el cambio de la posicion de la tira
      posicionLed++ ;
    }
  }
  // Se muestran las matrices
  //VisualizarMatrices();
}
// Se ejecuta la configuración de las tiras
void MostrarTiras(){
  panelC.show();
  //panelOrejaI.show();
  //panelOrejaD.show();
  panel.show();
  delay(1);
}
// Se crea un método que ajusta el brillo de manera pareja en todas las tiras
void AjustarBrilloGeneral(){
  panelC.setBrightness(brillo);
  //panelOrejaI.setBrightness(brillo);
  //panelOrejaD.setBrightness(brillo);
  panel.setBrightness(brillo);
}
// Se crea un método que ejecuta la matriz de control en las tiras led
void VisualizaTiras(){
  // Se ajusat el brillo de las tiras
  AjustarBrilloGeneral();
  // Se recorre la matriz para visualizar los paneles // 2025/Mayo/12  - Se cambio de 21 columnas a 16 columnas
  for(int posX = 0; posX < 16; posX++){
    for (int posY = 0; posY < 15; posY++){
      // Se muestra solo si el valor en la posicion es diferente a -1 y el estado es activo (1)
      if (controlPanel[posX][posY][0] >= 0 && controlPanel[posX][posY][1] == 1){
        panel.setPixelColor(controlPanel[posX][posY][0],controlPanel[posX][posY][2],controlPanel[posX][posY][3],controlPanel[posX][posY][4]);
      }else{
        panel.setPixelColor(controlPanel[posX][posY][0],0,0,0);
      }
      // Se muestra el panel central
      if (posX == 0){
        if (controlPanelCentral[posX][posY][0] >= 0 && controlPanelCentral[posX][posY][1] == 1){
          panelC.setPixelColor(controlPanelCentral[posX][posY][0],controlPanelCentral[posX][posY][2],controlPanelCentral[posX][posY][3],controlPanelCentral[posX][posY][4]);
        }else{
          panelC.setPixelColor(controlPanelCentral[posX][posY][0],0,0,0);
        }
      }
    }
  }
  // Al finalizar la configuración se ejecuta la configuración de las tiras
  MostrarTiras();
}
// Se crea un método que apaga todo
void ApagarExistencia(){
  for (int pos = 0; pos < ledsPanel; pos ++){
    panelC.setPixelColor(pos,0,0,0);
    //panelOrejaI.setPixelColor(pos,0,0,0);
    //panelOrejaD.setPixelColor(pos,0,0,0);
    panel.setPixelColor(pos,0,0,0);
  }
  MostrarTiras();
}
// Método de validación de las tiras led, recorre todas las tiras por cada uno de los valores RGB, con el fin de detectar mal funcionamiento o cortes en la señal
void Test(){
  int rojo = 0;
  int verde = 0;
  int azul = 0;
  for (int color = 0; color < 4; color++){
    // Se cambian los ifs por un switch, se quita el blanco por que es la combinacion de todos
    switch (color){
      case 0:
        rojo = 255;
        break;
      case 1:
        rojo = 0;
        verde = 255;
        break;
      case 2:
        verde = 0;
        azul = 255;
        break;
      case 3:
        rojo = 255;
        verde = 255;
    }
    for (int pos = 0; pos <= ledsPanel; pos++){
      if (pos<ledsPanelC){
        panelC.setPixelColor(pos, rojo, verde, azul);
      }
      /*
      if (pos<ledsOreja){
        panelOrejaI.setPixelColor(pos, rojo, verde, azul);
        panelOrejaD.setPixelColor(pos, rojo, verde, azul);
      }
      */
      panel.setPixelColor(pos, rojo, verde, azul);
      // Se agrego un if mostrar cada 15 para que sea mas rapido el arranque
      if (pos%5 ==0){
        MostrarTiras(),
        delay(1);
      }
    }
  }
  delay(10);
}
// Se genera un método para generar la configuración default de la cara, la configuración es a travez de la matriz de control
void ConfiguracionCaraDefault(){
  // ----------------------------------------------------------------
  // Se ntenta crear un ciclo for para llenar la matriz // 2025/Mayo/12 - Se cambio de 21 columnas a 16
  for (int posX = 0; posX < 16; posX++){
    for (int posY = 0; posY < 15; posY++){
      // ----------------------------------------------------------------
      // If para llenar la boca
      // Para llenar Solamente el panel central, el caso posX == 0 abarcala primer columan de los paneles laterales
      if (posX==0 && (posY == 11 || posY == 12)){
        controlPanelCentral[posX][posY][1] = 1;
        controlPanelCentral[posX][posY][2] = red;
        controlPanelCentral[posX][posY][3] = green;
        controlPanelCentral[posX][posY][4] = blue;
        controlPanelCentral[posX][posY][5] = 1;
      }
      // ----------------------------------------------------------------
      // Para llenar los paneles laterales
      if (((posX == 0 || posX == 2 || posX == 4 || posX == 6 || posX == 8 || posX == 10) && (posY == 13 || posY == 12)) || ((posX == 1 || posX == 5 || posX == 9) && (posY == 13 || posY == 14)) || ((posX == 3 || posX == 7 || posX == 11) && (posY == 11 || posY == 12))){
        // Para llenar el panel lateral
        controlPanel[posX][posY][1] = 1;
        controlPanel[posX][posY][2] = red;
        controlPanel[posX][posY][3] = green;
        controlPanel[posX][posY][4] = blue;
        controlPanel[posX][posY][5] = 1;
      }
      // ----------------------------------------------------------------
      // Para la nariz
      if ((posX == 1 && (posY == 1 || posY == 2 )) || (posX == 2 && (posY == 0 || posY == 1)) || (posX == 3 && posY == 0)){
        // Se marca el led como activo
        controlPanel[posX][posY][1] = 1;
        // Se setea al rgb default
        controlPanel[posX][posY][2] = red;
        controlPanel[posX][posY][3] = green;
        controlPanel[posX][posY][4] = blue;
        // Se configura como nariz
        controlPanel[posX][posY][5] = 2;
      }
      // ----------------------------------------------------------------
      // Para el ojo
      if (
        ( posX == 11 && (posY == 2 || posY == 3)) || 
        ( posX == 12 && (posY == 1 || posY == 2 || posY == 3 || posY == 4)) ||
        ((posX == 13 || posX == 14) && (posY == 0 || posY == 1 || posY == 2 || posY == 3 || posY == 4)) ||
        ( posX == 15 && (posY == 1 || posY == 2 || posY == 3))
        ){
        controlPanel[posX][posY][1] = 1;
        controlPanel[posX][posY][2] = red;
        controlPanel[posX][posY][3] = green;
        controlPanel[posX][posY][4] = blue;
        controlPanel[posX][posY][5] = 3;
      }
      // ----------------------------------------------------------------
      // Se me ocurrio un piercing de nariz, nada mas para aprobechar el panel frontal
      if (posX == 0 && posY == 5 ){
        controlPanelCentral[posX][posY][1] = 1;
        controlPanelCentral[posX][posY][2] = redPiercing;
        controlPanelCentral[posX][posY][3] = greenPiercing;
        controlPanelCentral[posX][posY][4] = bluePiercing;
        controlPanelCentral[posX][posY][5] = 4;
      }
      if (
        (posX == 0  && posY == 5) ||
        (posX == 1  && posY == 4) ||
        (posX == 2  && (posY == 2 || posY == 3))
      ){
        controlPanel[posX][posY][1] = 1;
        controlPanel[posX][posY][2] = 128;
        controlPanel[posX][posY][3] = 0;
        controlPanel[posX][posY][4] = 255;
        controlPanel[posX][posY][5] = 4;
      }
      // ----------------------------------------------------------------
    }
  }
}
// Se genera un método para parpadear
void Parpadeo(){
  int tiempoParpadeoEjecucion = 50;
  // Se recoren los renglones
  for (int posY = 0; posY < 15 ; posY++){
    // Se detecto que el recorrido de la matriz aun cuando no encuentra ojos, ejecuta el delay, se agrego una variable para que solo ejecute el pintado y el delay si encontro algun ojo
    bool ojoEncontrado = false;
    // En cada renglon, se identifica a los ojos
    for (int posX = 0; posX < 16; posX++){
      // En cada ojo se cambia su estado controlPanel[X][Y][1] = 0 en donde controlPanel[X][Y][5] == 3
      if (controlPanel[posX][posY][5] == 3){
        controlPanel[posX][posY][1] = 0;
        ojoEncontrado = true;
      }
    }
    if (ojoEncontrado){
      // Una vez configurado el renglon, se visualiza la matriz
      VisualizaTiras();
      // Se agrega un nuevo delay por que parpadea muy rapido
      delay(tiempoParpadeoEjecucion);
    }
  }
  // Terminando de cerrar los ojos, hay que abrirlos, la diferencia es que ahora se recorre el panel de abajo hacia arriba
  for (int posY = 14; posY >=0 ; posY--){
    bool ojoEncontrado = false;
    // En cada renglon, se identifica a los ojos
    for (int posX = 0; posX < 16; posX++){
      // En cada ojo se cambia su estado controlPanel[X][Y][1] = 0 en donde controlPanel[X][Y][5] == 3
      if (controlPanel[posX][posY][5] == 3){
        controlPanel[posX][posY][1] = 1;
        ojoEncontrado = true; 
      }
    }
    if (ojoEncontrado){
      // Una vez configurado el renglon, se visualiza la matriz
      VisualizaTiras();
      // Delay
      delay(tiempoParpadeoEjecucion);
    }
  }
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Metodopara cambiar el color de un elemento en especifico
void ActualizaColor(int elemento, int r, int g, int b){
  for (int posX = 0; posX < 16; posX++){
    for (int posY = 0; posY < 16; posY++){
      Serial.print("Valor en tira X ");
      Serial.print(posX);
      Serial.print(", Y ");
      Serial.print(posY);
      Serial.print(" :");
      Serial.println(controlPanelCentral[posX][posY][5]);
      // Se ajusta el panel central
      if (posX == 0 && controlPanelCentral[posX][posY][5] == elemento){
        controlPanelCentral[posX][posY][2] = r;
        controlPanelCentral[posX][posY][3] = g;
        controlPanelCentral[posX][posY][4] = b;
      }
      if (controlPanel[posX][posY][5] == elemento){
        // Se ajusta los paneles laterales
        controlPanel[posX][posY][2] = r;
        controlPanel[posX][posY][3] = g;
        controlPanel[posX][posY][4] = b;
      }
    }
    Serial.print("termina Y, cambia X");
    Serial.println(posX);
  }
}
// Se genera un metodo que ejecuta la instruccion del bluetooth
void LecturaBT(String cadenaBT){
  Serial.println(cadenaBT);
  // La instruccion se divide en colores XXX-XXX-XXX-L (valores RGB de un elemento-P Piercing C-Panel Central), DEFAULT-L y N-L (Numero, elemento, B-Brillo)
  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Se atienden los elementos de configuración default
  if (cadenaBT.indexOf("DEFAULT")>=0){
    Serial.println("Aplica default");
    String elemento = "";
    elemento = cadenaBT.charAt(8);
    if (elemento == "P"){
      // Se recorre la matriz para cambiar el color del piercing al default
      ActualizaColor(4, redPiercing, greenPiercing, bluePiercing);
    }else if(elemento == "C"){
      // Se recorre la matriz para cambiar el color del piercing al default
      for (int elemento = 1; elemento < 4; elemento++){
        ActualizaColor(elemento, red, green, blue);
      }
    }
  }
  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Para ajustar el brillo, el brillo tiene una longitud de 3 o 4 siendo N-B/NN-B donde N es la intencidad del 1 al 10 el -B es para saber si es brillo
  else if (cadenaBT.indexOf("-B")>=0 && cadenaBT.length()>=3 && cadenaBT.length()<=4){
    String valor = "";
    if (cadenaBT.length() == 3){
      valor = cadenaBT.charAt(0);
      brillo = valor.toInt();
    }else{
      valor = cadenaBT.charAt(0);
      valor.concat(cadenaBT.charAt(1));
      brillo = valor.toInt();
    }
  }
  // -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Para cambiar el color, la cadena esperada es NNN-NNN-NNN-L donde las N son los numeros de R, G y B, y la letra es el elemento al que afectan, tiene una longitud de 12
  else if(cadenaBT.length() == 13){
    // Se recupera el valor del elemento al que se quiere editar
    String elemento = "";
    String r = "";
    String g = "";
    String b = "";
    // Se recuperan los valores RGB
    r = cadenaBT.charAt(0);
    r.concat(cadenaBT.charAt(1));
    r.concat(cadenaBT.charAt(2));
    g = cadenaBT.charAt(4);
    g.concat(cadenaBT.charAt(5));
    g.concat(cadenaBT.charAt(6));
    b = cadenaBT.charAt(8);
    b.concat(cadenaBT.charAt(9));
    b.concat(cadenaBT.charAt(10));
    // Se recupera el valor que se desea editar
    elemento = cadenaBT.charAt(12);
    if (elemento == "P"){
      Serial.println("CambiaColor Piercing");
      // Se recorre la matriz para cambiar el color del piercing al default
      ActualizaColor(4, r.toInt(), g.toInt(), b.toInt());
    }else if(elemento == "C"){
      Serial.println("Cambia Color Panel");
      for (int elemento = 1; elemento < 4; elemento++){
        ActualizaColor(elemento, r.toInt(), g.toInt(), b.toInt());
      }
    }
  }
  // No hay else, cualquier cadena que no cumpla con estos requisitos es descartada
  else{
    Serial.println("Opcion no valida");
  }
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Métodos del ESP32
// Codigo que solo corre en el arranque
void setup() {
  Serial.begin(115200);
  serialBT.begin("Hashdrive.Test-01"); // 3. Se inicializa el serial Bluetooth y se le da nombre
  // Hay que iniciar las tiras Independientemente si sera de prueba o de uso, todo trabajo con las tidas debe ser hecho despues de inicializar las tiras, 
  panelC.begin();
  //panelOrejaI.begin();
  //panelOrejaD.begin();
  panel.begin();
  // Se ejecuta el test de las tiras
  brillo = 10;
  AjustarBrilloGeneral();
  Test();
  brillo = 5;
  // Se valida que los leds esten apagados
  ApagarExistencia();
  // Se inicializan las matrices de control de dibujado
  Inicializar();
  // Se inicializa la configuración default de los paneles
  ConfiguracionCaraDefault();
  // Se configura el tiempo del parpadeo
  TiempoParpadeo();
  //VisualizarMatrices();
}

void loop() {
  // Antes de visualizar las tiras, se valida si hay datos del bluetooth que reconfiguren las tiras
  // 4. Se genera un while para trabajar el bluetooth
  
  while (serialBT.available()){
    // Si trae datos entra
    valorBT = serialBT.readString();
    if (valorBT != ""){
      //VisualizarMatrices();
      // Se mandan los datos al monitor serial
      LecturaBT(valorBT);
      // Se limpia el bñuetooth
      valorBT = "";
    }
  }

  VisualizaTiras();
  // Se valida si ejecuta el parpadeo
  if (contadorParpadeo < parpadeo){
    contadorParpadeo++;
  }else{
    // Se detiene la ejecucion del código y se ejecuta el parpadeo
    Parpadeo();
    // Se reinicia el parpadeo a un nuevo numero aleatorio
    TiempoParpadeo();
    // Se reinicia el contador del parpadeo
    contadorParpadeo = 0;
  }
  
}
// Nota: Para invertir el llenado de la matrix en los nones es el total maximo en Y menos la posicion del iterador

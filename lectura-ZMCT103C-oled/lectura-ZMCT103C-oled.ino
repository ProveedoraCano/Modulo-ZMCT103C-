#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración de la pantalla OLED (tamaño 128x32)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Dirección I2C de la pantalla OLED
#define OLED_ADDR 0x3C

// Crear objeto para manejar la pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Configuración del sensor ZMCT103C
const int sensorPin = A0;       // Pin analógico donde conectas la salida del ZMCT103C
const float burdenResistor = 200.0; // Resistor de carga en ohmios
const float turnsRatio = 1000.0;    // Relación de vueltas (1000:1)
const int numSamples = 5000;        // Número de muestras para calcular RMS
const float VREF = 5.0;            // Voltaje de referencia del Arduino (5V)
const float ADCResolution = 1023.0; // Resolución del ADC (10 bits)

void setup() {
  Serial.begin(9600); // Inicia la comunicación serie para depuración

  // Inicializa la pantalla OLED con la dirección I2C 0x3C
  if (!display.begin(0x3C)) {
    Serial.println(F("No se detectó la pantalla OLED"));
    for (;;); // Detiene el programa si falla la inicialización
  }

  // Configura la pantalla OLED
  display.clearDisplay();             // Limpia la pantalla
  display.setTextSize(2);             // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0, 0);           // Posición del texto
  display.print("Iniciando...");      // Escribe en el buffer
  display.display();                  // Muestra el contenido del buffer
  delay(2000);                        // Pausa para mostrar el mensaje inicial
  display.clearDisplay();             // Limpia la pantalla
}

void loop() {
  float sumSquares = 0.0;     // Suma de los cuadrados de las lecturas
  float offset = VREF / 2;    // Desplazamiento de la señal a 2.5V

  // Leer múltiples muestras para calcular el RMS
  for (int i = 0; i < numSamples; i++) {
    int rawValue = analogRead(sensorPin); // Leer valor analógico
    float voltage = rawValue * (VREF / ADCResolution); // Convertir a voltaje
    float adjustedVoltage = voltage - offset; // Restar el desplazamiento (2.5V)
    sumSquares += adjustedVoltage * adjustedVoltage; // Acumular el cuadrado
  }

  // Calcular el valor RMS del voltaje
  float rmsVoltage = sqrt(sumSquares / numSamples);

  // Calcular la corriente RMS usando la fórmula: I = V / R * relación de vueltas
  float rmsCurrent = (rmsVoltage / burdenResistor) * turnsRatio;

  // Mostrar la corriente RMS en el monitor serial
  //Serial.print("Corriente RMS: ");
  Serial.print(rmsCurrent, 0); // Mostrar con 3 decimales
  Serial.println(" A");

  // Mostrar la corriente RMS en la pantalla OLED
  display.clearDisplay();        // Limpia la pantalla
  display.setCursor(0, 0);       // Posiciona el cursor en la primera fila
  //display.print("Corriente RMS:"); // Imprime el encabezado
  display.setCursor(0, 10);      // Posiciona el cursor en la segunda fila
  display.print(rmsCurrent, 3);  // Imprime el valor de la corriente
  display.print(" A");           // Imprime la unidad (amperios)

  display.display();             // Actualiza la pantalla OLED con los datos nuevos
  delay(1000);                   // Esperar 1 segundo antes de la siguiente medición
}

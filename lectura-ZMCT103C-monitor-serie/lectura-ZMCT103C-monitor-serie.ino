//ProveedoraCano.com/blog
//Sketch para leer la corriente en modulo ZMCT103C
const int sensorPin = A0;
const float burdenResistor = 200.0;
const float turnsRatio = 1000.0;
const int numSamples = 500;
const float VREF = 5.0;
const float ADCResolution = 1023.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  float sumSquares = 0.0;
  float offset = VREF / 2;

  for (int i = 0; i < numSamples; i++) {
    int rawValue = analogRead(sensorPin);
    float voltage = rawValue * (VREF / ADCResolution);
    float adjustedVoltage = voltage - offset;
    sumSquares += adjustedVoltage * adjustedVoltage;
  }

  float rmsVoltage = sqrt(sumSquares / numSamples);
  float rmsCurrent = (rmsVoltage / burdenResistor) * turnsRatio;

  Serial.print("Corriente RMS: ");
  Serial.print(rmsCurrent, 3);
  Serial.println(" A");

  delay(1000);
}

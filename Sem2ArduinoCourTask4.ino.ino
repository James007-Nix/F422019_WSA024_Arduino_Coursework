#define MAX_SAMPLES 180       // 1 sample per second for 3 minutes
#define MOVING_AVG_SIZE 5     //average of last 5 cycles
#define FS 1                  // sampling frequency in hertz

const int B = 4275000;
const int R0 = 100000;
const int pinTempSensor = A0;

float temperatureData[MAX_SAMPLES];
float variationHistory[MOVING_AVG_SIZE];

float currentSamplingRate = FS;
int stableCycles = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
                                          // Collection of temperature data
  for (int i = 0; i < MAX_SAMPLES; i++) {
    int a = analogRead(pinTempSensor);

    float R = 1023.0 / a - 1.0;
    R = R0 * R;

    float temperature = 1.0 / (log(R / R0) / B + 1.0 / 298.15) - 273.15;

    temperatureData[i] = temperature;

    delay(1000 / currentSamplingRate);  // Delay based on current rate
  }

                                      // function to calculate the total variation between samples
  float totalVariation = 0;
  for (int i = 1; i < MAX_SAMPLES; i++) {
    totalVariation += abs(temperatureData[i] - temperatureData[i - 1]);
  }

                              // update of variation
  static int varIndex = 0;
  variationHistory[varIndex] = totalVariation;
  varIndex = (varIndex + 1) % MOVING_AVG_SIZE;

  float variationSum = 0;
  for (int i = 0; i < MOVING_AVG_SIZE; i++) {
    variationSum += variationHistory[i];
  }
  float avgVariation = variationSum / MOVING_AVG_SIZE;

                                    // using DFT
  float real[MAX_SAMPLES];
  float imag[MAX_SAMPLES];
  float magnitude[MAX_SAMPLES];

  for (int k = 0; k < MAX_SAMPLES; k++) {
    real[k] = 0;
    imag[k] = 0;
    for (int n = 0; n < MAX_SAMPLES; n++) {
      float angle = 2 * PI * k * n / MAX_SAMPLES;
      real[k] += temperatureData[n] * cos(angle);
      imag[k] -= temperatureData[n] * sin(angle);
    }
    magnitude[k] = sqrt(real[k] * real[k] + imag[k] * imag[k]);
  }

                            // Finding the dominant frequency
  float maxMag = 0;
  int domIndex = 1;
  for (int k = 1; k < MAX_SAMPLES / 2; k++) {
    if (magnitude[k] > maxMag) {
      maxMag = magnitude[k];
      domIndex = k;
    }
  }

  float dominantFreq = (float)domIndex * currentSamplingRate / MAX_SAMPLES;
  float newSamplingRate = dominantFreq * 2.0;

                                                    // new sampling rate
  if (newSamplingRate < 0.5) newSamplingRate = 0.5;
  if (newSamplingRate > 4.0) newSamplingRate = 4.0;
  currentSamplingRate = newSamplingRate;

                                        // Deciding power mode between active, idle and power down
  String powerMode = "ACTIVE";
  if (avgVariation > 2.0) {
    powerMode = "ACTIVE";
    stableCycles = 0;
  } else if (avgVariation > 1.0) {
    powerMode = "IDLE";
    stableCycles = 0;
  } else {
    stableCycles++;
    if (stableCycles >= 5) {
      powerMode = "POWER_DOWN";
    } else {
      powerMode = "IDLE";
    }
  }

                                                                        // Output of results to the serail monitor
  Serial.print("Collecting temperature data for 1 minute...\n");
  Serial.print("Predicted Variation: ");
  Serial.print(avgVariation, 2);
  Serial.print(", Dominant Frequency: ");
  Serial.print(dominantFreq, 2);
  Serial.print(" Hz, New Sampling Rate: ");
  Serial.print(currentSamplingRate, 2);
  Serial.print(" Hz, Power Mode: ");
  Serial.println(powerMode);

  Serial.println("Time(s),Temp(C),Magnitude");

  for (int i = 0; i < MAX_SAMPLES; i++) {
    Serial.print(i);
    Serial.print(",");
    Serial.print(temperatureData[i]);
    Serial.print(",");
    Serial.println(magnitude[i]);
  }

  Serial.println("================================================");
}

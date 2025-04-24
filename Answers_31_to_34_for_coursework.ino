#include <math.h>

#define SampleSize 180                       // 1 sample a second for 3 minutes
#define FS 1                                  // Sampling frequency in Hertz

const int B = 4275000;  // B value of the thermistor
const int R0 = 100000;  // R0 = 100k
const int pinTempSensor = A0;  // Grove - Temperature Sensor connected to A0

float TemperatureData[SampleSize];            // Temperature data array

#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

// Function to collect the temperature data
void collect_temperature_data() {
    for (int i = 0; i < SampleSize; i++) {
        int a = analogRead(pinTempSensor);  // Read analog value from temp sensor

        float R = 1023.0 / a - 1.0;
        R = R0 * R;

        // Convert resistance to temperature in Celsius using Steinhart-Hart equation
        float temperature = 1.0 / (log(R / R0) / B + 1.0 / 298.15) - 273.15;

        TemperatureData[i] = temperature;  // Store temperature

        delay(1000);  // Wait for 1 second
    }
}

// Function to apply DFT and return mangitude array
float* apply_dft(float TemperatureData[], int N) {
    static float magnitude[SampleSize];
    float real[SampleSize];
    float imag[SampleSize];

    for (int k = 0; k < N; k++) {
        real[k] = 0;
        imag[k] = 0;

        for (int n = 0; n < N; n++) {
            real[k] += TemperatureData[n] * cos(2 * M_PI * k * n / N);
            imag[k] -= TemperatureData[n] * sin(2 * M_PI * k * n / N);
        }

        magnitude[k] = sqrt(real[k] * real[k] + imag[k] * imag[k]);
    }

    return magnitude;
}

// Function to calculate average freuqency from the magnitude array
float calculate_average_frequency(float* magnitude, int N) {
    float sum_frequency = 0;
    int num_frequencies = 0;

    for (int k = 0; k < N; k++) {
        float frequency = (float)k * FS / N;
        sum_frequency += frequency * magnitude[k];
        num_frequencies++;
    }

    return sum_frequency / num_frequencies;
}

// Function to decide the power mode
int decide_power_mode(float* magnitude, int N) {
    float average_frequency = calculate_average_frequency(magnitude, N);

    if (average_frequency > 0.5) {
        return 1;               // Active mode
    } else if (average_frequency > 0.1) {
        return 2;               // Idle mode
    } else {
        return 3;               // Power down mode
    }
}

// Function to send data to PC through the serial monitor
void send_data_to_pc(float temperatureData[], float* magnitude, int N) {
    Serial.println("Time,Temperature,Frequency,Magnitude");

    for (int t = 0; t < N; t++) {
        float time = t;
        float temperature = temperatureData[t];
        float frequency = (float)t * FS / N;

        Serial.print(time);
        Serial.print(",");
        Serial.print(temperature);
        Serial.print(",");
        Serial.print(frequency);
        Serial.print(",");
        Serial.println(magnitude[t]);
    }
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    collect_temperature_data();             //Collect temperature data

    float* magnitude = apply_dft(TemperatureData, SampleSize);            //DFT

    send_data_to_pc(TemperatureData, magnitude, SampleSize);            //Send CSV data to serial monitor

    int power_mode = decide_power_mode(magnitude, SampleSize);  //Decide mode from active, idle or power down

                                    //Print power mode to Serial monitor
    Serial.print("Power Mode: ");
    if (power_mode == 1) {
        Serial.println("ACTIVE");
    } else if (power_mode == 2) {
        Serial.println("IDLE");
 } else {
        Serial.println("POWER_DOWN");
    }

}
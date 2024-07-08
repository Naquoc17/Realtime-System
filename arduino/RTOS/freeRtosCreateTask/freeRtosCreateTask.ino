#include <Arduino_FreeRTOS.h>
#include <avr/pgmspace.h>
#include "SensorData.h"

// Shared variables
volatile float speed_kmh = 0.0;
volatile float acceleration_kms2 = 0.0;
volatile float distance_km = 0.0;

// Task prototypes
void TaskSensor(void *pvParameters);
void TaskAcc(void *pvParameters);
void TaskPos(void *pvParameters);
void TaskComm(void *pvParameters);

void setup() {
    // Initialize serial communication
    Serial.begin(115200);

    // Create tasks
    xTaskCreate(TaskSensor, "Sensor Task", 128, NULL, 1, NULL);
    xTaskCreate(TaskAcc, "Acc Task", 128, NULL, 1, NULL);
    xTaskCreate(TaskPos, "Pos Task", 128, NULL, 1, NULL);
    xTaskCreate(TaskComm, "Comm Task", 128, NULL, 1, NULL);
}

void loop() {
    // Nothing to do here
}

void TaskSensor(void *pvParameters) {
    (void) pvParameters;
    uint16_t sensorDataIndex = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 16 / portTICK_PERIOD_MS;

    for (;;) {
        if (sensorDataIndex < SENSOR_DATA_SIZE) {
            uint16_t sensorValue = pgm_read_word_near(SensorData + sensorDataIndex++);
            speed_kmh = (300.0 / 65535.0) * sensorValue;
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void TaskAcc(void *pvParameters) {
    (void) pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 32 / portTICK_PERIOD_MS;
    static float lastSpeed = 0.0;

    for (;;) {
        float currentSpeed = speed_kmh;
        acceleration_kms2 = (currentSpeed - lastSpeed) / (32 / 1000.0);
        lastSpeed = currentSpeed;

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void TaskPos(void *pvParameters) {
    (void) pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 48 / portTICK_PERIOD_MS;
    static float lastSpeed = 0.0;

    for (;;) {
        float currentSpeed = speed_kmh;
        distance_km += ((currentSpeed + lastSpeed) / 2) * (48 / 3600000.0);
        lastSpeed = currentSpeed;

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void TaskComm(void *pvParameters) {
    (void) pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 96 / portTICK_PERIOD_MS;

    for (;;) {
        float currentSpeed = speed_kmh;
        float currentAcceleration = acceleration_kms2;
        float currentDistance = distance_km;

        Serial.print("Speed (km/h): ");
        Serial.print(currentSpeed);
        Serial.print("\tAcceleration (km/s^2): ");
        Serial.print(currentAcceleration);
        Serial.print("\tDistance (km): ");
        Serial.println(currentDistance);

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

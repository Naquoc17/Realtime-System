#include <Arduino_FreeRTOS.h>
#include "SensorData.h"

// Shared variables
volatile float vehicleSpeed = 0.0;
volatile float vehicleAcceleration = 0.0;
volatile float vehicleDistance = 0.0;

unsigned long lastSpeedTime = 0;
unsigned long lastPosTime = 0;

// Function prototypes
void SensorTask(void* pvParameters);
void AccTask(void* pvParameters);
void PosTask(void* pvParameters);
void CommTask(void* pvParameters);

void setup() {
  Serial.begin(9600);

  // Create tasks
  xTaskCreate(
    SensorTask,           // Function that implements the task.
    "Sensor Task",        // Text name for the task.
    128,                  // Stack size in words, not bytes.
    NULL,                 // Parameter passed into the task.
    1,                    // Priority at which the task is created.
    NULL);  // Variable to hold the task's data structure.

  xTaskCreate(
    AccTask,
    "Acc Task",
    128,
    NULL,
    1,
    NULL);

  xTaskCreate(
    PosTask,
    "Pos Task",
    128,
    NULL,
    1,
    NULL);

  xTaskCreate(
    CommTask,
    "Comm Task",
    128,
    NULL,
    1,
    NULL);

  // Start the scheduler
  vTaskStartScheduler();
}

void SensorTask(void* pvParameters) {
  uint16_t sensorData;
  const TickType_t xDelay = pdMS_TO_TICKS(16); // 16ms delay
  int sensorIndex = 0;
  while (1) {
    // Read sensor data from the array in PROGMEM
    sensorData = pgm_read_word(&SensorData[sensorIndex]);
    // Convert to km/h
    vehicleSpeed = (float)sensorData * 300.0 / 0xFFFF;

    // Increment the sensor index
    sensorIndex++;
    if (sensorIndex >= SENSOR_DATA_SIZE) {
      sensorIndex = 0;
    }

    vTaskDelay(xDelay);
  }
}

void AccTask(void* pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(32);
  float lastSpeed = 0.0;
  while (1) {
    // Calculate acceleration in km/s^2
    vehicleAcceleration = (vehicleSpeed - lastSpeed) / (32.0 / 1000.0);
    lastSpeed = vehicleSpeed;

    vTaskDelay(xDelay);
  }
}

void PosTask(void* pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(48);
  while (1) {
    // Calculate distance in km
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastPosTime) / 1000.0;
    vehicleDistance += vehicleSpeed * (deltaTime / 3600.0); // km/h to km/s to km
    lastPosTime = currentTime;

    vTaskDelay(xDelay);
  }
}

void CommTask(void* pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(96);
  while (1) {
    // Send data over Serial
    Serial.print("Speed: ");
    Serial.print(vehicleSpeed);
    Serial.print(" km/h, Acceleration: ");
    Serial.print(vehicleAcceleration);
    Serial.print(" km/s^2, Distance: ");
    Serial.print(vehicleDistance);
    Serial.println(" km");

    vTaskDelay(xDelay);
  }
}

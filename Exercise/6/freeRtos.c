#include FreeRTOS
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdint.h>
#include <stdio.h>

// Constants
#define SENSOR_TASK_PERIOD_MS 16
#define ACC_TASK_PERIOD_MS 32
#define POS_TASK_PERIOD_MS 48
#define COMM_TASK_PERIOD_MS 96

// Shared data
float speed_kmh = 0;
float acceleration_kms2 = 0;
float distance_km = 0;

// Mutex for shared data
SemaphoreHandle_t dataMutex;

// Sensor data array (emulated)
#include "SensorData.h"

// Function to convert sensor data to km/h
float convertSensorData(uint16_t sensorValue) {
    return (sensorValue / (float)0xFFFF) * 300.0f;
}

// Sensor Task
void vSensorTask(void *pvParameters) {
    static uint16_t sensorIndex = 0;
    while (1) {
        uint16_t sensorValue = sensorData[sensorIndex];
        float speed = convertSensorData(sensorValue);

        xSemaphoreTake(dataMutex, portMAX_DELAY);
        speed_kmh = speed;
        xSemaphoreGive(dataMutex);

        sensorIndex = (sensorIndex + 1) % (sizeof(sensorData) / sizeof(sensorData[0]));
        vTaskDelay(pdMS_TO_TICKS(SENSOR_TASK_PERIOD_MS));
    }
}

// Acceleration Task
void vAccTask(void *pvParameters) {
    static float prevSpeed = 0;
    while (1) {
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        float currentSpeed = speed_kmh;
        acceleration_kms2 = (currentSpeed - prevSpeed) / (ACC_TASK_PERIOD_MS / 1000.0f);
        prevSpeed = currentSpeed;
        xSemaphoreGive(dataMutex);

        vTaskDelay(pdMS_TO_TICKS(ACC_TASK_PERIOD_MS));
    }
}

// Position Task
void vPosTask(void *pvParameters) {
    while (1) {
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        distance_km += (speed_kmh * (POS_TASK_PERIOD_MS / 3600000.0f)); // km/h to km per period
        xSemaphoreGive(dataMutex);

        vTaskDelay(pdMS_TO_TICKS(POS_TASK_PERIOD_MS));
    }
}

// Communication Task
void vCommTask(void *pvParameters) {
    while (1) {
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        printf("Speed: %.2f km/h, Acceleration: %.2f km/s^2, Distance: %.2f km\n", speed_kmh, acceleration_kms2, distance_km);
        xSemaphoreGive(dataMutex);

        vTaskDelay(pdMS_TO_TICKS(COMM_TASK_PERIOD_MS));
    }
}

int main(void) {
    // Initialize the FreeRTOS scheduler
    dataMutex = xSemaphoreCreateMutex();

    if (dataMutex == NULL) {
        // Mutex creation failed
        return -1;
    }

    // Create tasks with static stacks
    static StackType_t sensorTaskStack[configMINIMAL_STACK_SIZE];
    static StackType_t accTaskStack[configMINIMAL_STACK_SIZE];
    static StackType_t posTaskStack[configMINIMAL_STACK_SIZE];
    static StackType_t commTaskStack[configMINIMAL_STACK_SIZE];

    static StaticTask_t sensorTaskBuffer;
    static StaticTask_t accTaskBuffer;
    static StaticTask_t posTaskBuffer;
    static StaticTask_t commTaskBuffer;

    xTaskCreateStatic(vSensorTask, "Sensor Task", configMINIMAL_STACK_SIZE, NULL, 1, sensorTaskStack, &sensorTaskBuffer);
    xTaskCreateStatic(vAccTask, "Acc Task", configMINIMAL_STACK_SIZE, NULL, 1, accTaskStack, &accTaskBuffer);
    xTaskCreateStatic(vPosTask, "Pos Task", configMINIMAL_STACK_SIZE, NULL, 1, posTaskStack, &posTaskBuffer);
    xTaskCreateStatic(vCommTask, "Comm Task", configMINIMAL_STACK_SIZE, NULL, 1, commTaskStack, &commTaskBuffer);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here
    for (;;);

    return 0;
}

#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Arduino.h>
#include "SensorData.h"

QueueHandle_t speedQueue1, speedQueue2, speedQueue3;
QueueHandle_t accQueue, posQueue, commQueue;

void vApplicationMallocFailedHook() {
    Serial.println("Memory Allocation Failed");
}

void SensorTask(void *pvParameters) {
    uint16_t sensorIndex = 0;
    const TickType_t xDelay = 16 / portTICK_PERIOD_MS;
    float speed;

    while (1) {
        if (sensorIndex < SENSOR_DATA_SIZE) {
            uint16_t sensorValue = pgm_read_word_near(SensorData + sensorIndex);
            speed = (float)sensorValue * 300.0 / 0xFFFF;
            sensorIndex++;
        } else {
            sensorIndex = 0; // Reset to simulate continuous reading
        }

        xQueueSend(speedQueue1, &speed, 0);
        xQueueSend(speedQueue2, &speed, 0);
        xQueueSend(speedQueue3, &speed, 0);

        vTaskDelay(xDelay);
    }
}

void AccTask(void *pvParameters) {
    float prevSpeed = 0.0, currSpeed;
    float acceleration;
    const TickType_t xDelay = portMAX_DELAY;

    while (1) {
        if (xQueueReceive(speedQueue1, &currSpeed, xDelay)) {
            acceleration = (currSpeed - prevSpeed) / (16.0 / 1000.0);
            prevSpeed = currSpeed;
            xQueueSend(accQueue, &acceleration, 0);
        }
    }
}

void PosTask(void *pvParameters) {
    float speed, distance = 0.0;
    const TickType_t xDelay = portMAX_DELAY;

    while (1) {
        if (xQueueReceive(speedQueue2, &speed, xDelay)) {
            distance += speed * (16.0 / 3600000.0); // Convert speed to km and time to hours
            xQueueSend(posQueue, &distance, 0);
        }
    }
}

void CommTask(void *pvParameters) {
    float speed, acceleration, distance;
    float avgSpeed, avgAcc, avgPos;
    float sumSpeed = 0.0, sumAcc = 0.0, sumPos = 0.0;
    int countSpeed = 0, countAcc = 0, countPos = 0;
    const TickType_t xDelay = 96 / portTICK_PERIOD_MS;

    while (1) {
        if (xQueueReceive(speedQueue3, &speed, 0)) {
            sumSpeed += speed;
            countSpeed++;
        }

        if (xQueueReceive(accQueue, &acceleration, 0)) {
            sumAcc += acceleration;
            countAcc++;
        }

        if (xQueueReceive(posQueue, &distance, 0)) {
            sumPos += distance;
            countPos++;
        }

        if (countSpeed > 0) avgSpeed = sumSpeed / countSpeed;
        if (countAcc > 0) avgAcc = sumAcc / countAcc;
        if (countPos > 0) avgPos = sumPos / countPos;

        Serial.print("Avg Speed: ");
        Serial.print(avgSpeed);
        Serial.print(" km/h, Avg Acc: ");
        Serial.print(avgAcc);
        Serial.print(" km/s^2, Avg Pos: ");
        Serial.println(avgPos);

        sumSpeed = 0.0;
        sumAcc = 0.0;
        sumPos = 0.0;
        countSpeed = 0;
        countAcc = 0;
        countPos = 0;

        vTaskDelay(xDelay);
    }
}

void setup() {
    Serial.begin(115200);

    speedQueue1 = xQueueCreate(3, sizeof(float));
    speedQueue2 = xQueueCreate(3, sizeof(float));
    speedQueue3 = xQueueCreate(5, sizeof(float));
    accQueue = xQueueCreate(3, sizeof(float));
    posQueue = xQueueCreate(3, sizeof(float));

    if (speedQueue1 == NULL || speedQueue2 == NULL || speedQueue3 == NULL || accQueue == NULL || posQueue == NULL) {
        vApplicationMallocFailedHook();
    }

    xTaskCreate(SensorTask, "Sensor Task", 128, NULL, 1, NULL);
    xTaskCreate(AccTask, "Acc Task", 128, NULL, 1, NULL);
    xTaskCreate(PosTask, "Pos Task", 128, NULL, 1, NULL);
    xTaskCreate(CommTask, "Comm Task", 128, NULL, 1, NULL);
}

void loop() {
    // Empty. Tasks are running in FreeRTOS.
}

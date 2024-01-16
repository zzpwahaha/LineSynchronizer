#include <Arduino.h>
#include <elapsedMillis.h>

const uint8_t TRIG_INPUT_PIN = 2;
const uint8_t TRIG_OUTPUT_PIN = 3;
const uint8_t LED_PIN = 13;
const char TERMINATOR = '\n'; // terminator for serial communication
const uint16_t MAX_INPUT_SIZE = 512;
const uint16_t MAX_OUTPUT_SIZE = 512;

const char INPUT_CMD_ARM[] = "ARM";

volatile bool armed = false;
char serialInput[MAX_INPUT_SIZE];
char serialOutput[MAX_OUTPUT_SIZE];

// function declarations here:
void processData();
void trigger();

void setup()
{
    pinMode(TRIG_INPUT_PIN, INPUT);
    pinMode(TRIG_OUTPUT_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    attachInterrupt(TRIG_INPUT_PIN, trigger, RISING); // digitalPinToInterrupt(TRIG_INPUT_PIN)

    digitalWrite(TRIG_OUTPUT_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    delay(100);

    Serial.begin(115200); // 1500000
    Serial.setTimeout(1000);
}

void loop()
{
    if (Serial.available())
    {
        processData();
    }
}

// function definitions here:
void processData()
{
    elapsedMicros timer;
    memset(serialInput, 0, MAX_INPUT_SIZE);
    Serial.readBytesUntil(TERMINATOR, serialInput, MAX_INPUT_SIZE);
    bool equal = (strcmp(INPUT_CMD_ARM, serialInput) == 0);

    if (equal)
    {
        armed = true;
        Serial.print("ZYNQ armed in: ");
        Serial.print(timer);
        Serial.println(" us.");
        // sprintf(serialOutput, "ZYNQ armed in : %u us.", timer);
        // Serial.println(serialOutput);
    }
    else
    {
        armed = false;
        Serial.print("Error in receiving serial data after: ");
        Serial.print(timer);
        Serial.print(" us. The received data is: ");
        Serial.println(serialInput);
        // sprintf(serialOutput, "Error in receiving serial data after :%u us. The received data is %s",
        //         timer, serialInput);
        // Serial.println(serialOutput);
    }
}

void trigger()
{
    if (armed)
    {
        digitalWrite(TRIG_OUTPUT_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(TRIG_OUTPUT_PIN, LOW);
        armed = false;
    }
}
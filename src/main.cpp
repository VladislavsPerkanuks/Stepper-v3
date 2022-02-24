#include "internet.h"
#include "web_server.h"

#define LEDPIN 2
#define STEPPER_LINE1 15
#define STEPPER_LINE2 13
#define STEPPER_LINE3 12
#define STEPPER_LINE4 14
#define REACHED_TOP_LINE 5
#define REACHED_BOTTOM_LINE 4

static const uint8_t Q1[] = {LOW, HIGH, HIGH, LOW};
static const uint8_t Q2[] = {HIGH, LOW, LOW, HIGH};
static const uint8_t Q3[] = {LOW, LOW, HIGH, HIGH};
static const uint8_t Q4[] = {HIGH, HIGH, LOW, LOW};

unsigned long previousMillis = 0;
uint32_t reference;
uint8_t position = 0;
int i = 0;
unsigned long timeee = 0;
unsigned long currentMillis;
// bool finished;
// input inputs[15];
// bool submitted;
uint32_t passed_time = 0;
uint32_t total_passed_time = 0;

int ledState2 = LOW;
void setup()
{
  Serial.begin(74880);

  pinMode(LEDPIN, HIGH);
  pinMode(STEPPER_LINE1, OUTPUT);
  pinMode(STEPPER_LINE2, OUTPUT);
  pinMode(STEPPER_LINE3, OUTPUT);
  pinMode(STEPPER_LINE4, OUTPUT);

  pinMode(REACHED_TOP_LINE, INPUT);
  pinMode(REACHED_BOTTOM_LINE, INPUT);

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  if (!LittleFS.exists("/index.html"))
  {
    Serial.println("\nCould not find index.html");
    return;
  }

  if (!initialize_wifi())
  {
    Serial.println("\nCould not connect to WiFi");
    return;
  }

  initialize_server();
}

void loop()
{

  if (submitted)
  {
    int i = 0;
    while (inputs[i].speed && !finished)
    {
      unsigned long currentMillis = millis();
      // Serial.println(inputs[i].interval);
      if (currentMillis - previousMillis >= inputs[i].interval)
      {
        if (previousMillis)
          passed_time += (currentMillis - previousMillis);

        previousMillis = currentMillis;

        // Serial.println(String(passed_time));
        // passed_time2 = String(passed_time / 1000) + " seconds";

        ledState2 = ledState2 == HIGH ? LOW : HIGH;
        digitalWrite(LEDPIN, ledState2);

        if (passed_time >= (inputs[i].distance / inputs[i].speed))
        {
          i++;
          total_passed_time += passed_time;
          passed_time = 0;

        }
      }
    }
    finished = true;
  }
}

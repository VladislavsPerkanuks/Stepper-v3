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
 void my_loop();

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
  // my_loop();
}

// void my_loop()
// {
//   uint8_t i = 0;

//   while (inputs[i].speed || inputs[0].speed == 0)
//   {

//     unsigned long currentMillis = millis();
//     if (currentMillis - previousMillis >= inputs[i].interval)
//     {
//       timeee += currentMillis - previousMillis;
//       if (timeee >= inputs[i].speed / inputs[i].distance)
//       {
//         i++;
//         continue;
//       }
//       previousMillis = currentMillis;
//       position += inputs[i].direction;
//       uint8_t phase = position & 0x3;

//       if (inputs[i].direction)
//       {
//         digitalWrite(STEPPER_LINE1, Q1[phase]);
//         digitalWrite(STEPPER_LINE2, Q2[phase]);
//         digitalWrite(STEPPER_LINE3, Q3[phase]);
//         digitalWrite(STEPPER_LINE4, Q4[phase]);
//       }
//       else
//       { // turn off all lines
//         static const uint8_t stepper_off = LOW;
//         digitalWrite(STEPPER_LINE1, stepper_off);
//         digitalWrite(STEPPER_LINE2, stepper_off);
//         digitalWrite(STEPPER_LINE3, stepper_off);
//         digitalWrite(STEPPER_LINE4, stepper_off);
//       }
//     }
//   }
// }

void do_something(input x)
{
  currentMillis = millis();
  if (currentMillis - previousMillis >= inputs[i].interval)
  {
    timeee += currentMillis - previousMillis;
    Serial.println(timeee);

    if (timeee >=  (x.distance / x.speed)*1000000)
    {
      i++;
    }

    previousMillis = currentMillis;
    position += x.direction;
    uint8_t phase = position & 0x3;

    if (x.direction)
    {
      digitalWrite(STEPPER_LINE1, Q1[phase]);
      digitalWrite(STEPPER_LINE2, Q2[phase]);
      digitalWrite(STEPPER_LINE3, Q3[phase]);
      digitalWrite(STEPPER_LINE4, Q4[phase]);
    }
    else
    { // turn off all lines
      static const uint8_t stepper_off = LOW;
      digitalWrite(STEPPER_LINE1, stepper_off);
      digitalWrite(STEPPER_LINE2, stepper_off);
      digitalWrite(STEPPER_LINE3, stepper_off);
      digitalWrite(STEPPER_LINE4, stepper_off);
    }
  }
}

void loop()
{

  for (input x : inputs)
  {
    if (x.speed)
      do_something(x);
  }
}
// {

//   if (inputs[i].speed == 0)
//   {
//     delay(1000);
//   }

//   unsigned long currentMillis = millis();

//   if (currentMillis - previousMillis >= inputs[i].interval)
//   {

//     timeee += currentMillis - previousMillis;

//     if (timeee >= inputs[i].speed / inputs[i].distance)
//     {
//       i++;
//     }

//     previousMillis = currentMillis;
//     position += inputs[i].direction;
//     uint8_t phase = position & 0x3;

//     if (inputs[i].direction)
//     {
//       digitalWrite(STEPPER_LINE1, Q1[phase]);
//       digitalWrite(STEPPER_LINE2, Q2[phase]);
//       digitalWrite(STEPPER_LINE3, Q3[phase]);
//       digitalWrite(STEPPER_LINE4, Q4[phase]);
//     }
//     else
//     { // turn off all lines
//       static const uint8_t stepper_off = LOW;
//       digitalWrite(STEPPER_LINE1, stepper_off);
//       digitalWrite(STEPPER_LINE2, stepper_off);
//       digitalWrite(STEPPER_LINE3, stepper_off);
//       digitalWrite(STEPPER_LINE4, stepper_off);
//     }
//   }
// }

#include "web_server.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncWebServer server(80);
input inputs[15];

float calculate_pause_interval(uint32_t speed)
{
    return DISTANCE_NM_PER_ROTATION / speed;
}

void print_input()
{
    for (auto x : inputs)
    {
        if (!x.speed)
            break;
        Serial.print("Speed: ");
        Serial.print(x.speed);
        Serial.print(" Distance: ");
        Serial.print(x.distance);
        Serial.print(" Interval: ");
        Serial.print(x.interval);
        Serial.print(" Position: ");
        Serial.println(x.direction ? "Up" : "Down");
    }
    Serial.println("");
}

String processor(const String &var)
{
    int i = 0;
    while (inputs[i].speed)
    {
        if (var == "SPEED" + String(i))
        {
            return String(inputs[i].speed);
        }

        else if (var == "DIRECTION" + String(i))
        {
            return inputs[i].direction == 0 ? "Checked" : "";
        }

        else if (var == "DISTANCE" + String(i))
        {
            return String(inputs[i].distance);
        }
        i++;
    }
    return "";
}

void initialize_server()
{
    server.serveStatic("/images", LittleFS, "/images");
    server.serveStatic("/css", LittleFS, "/css");
    server.serveStatic("/js", LittleFS, "/js");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.html", String(), false, processor); });

    server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  StaticJsonDocument<256> doc;
                  AsyncWebParameter *p = request->getParam(0);
                  deserializeJson(doc, p->value());
                  JsonObject root = doc.as<JsonObject>();
                  int i = 0;

                    // reset all inputs
                  for (input x : inputs)
                  {
                      x.speed = 0;
                      x.direction = 0;
                      x.distance = 0;
                      x.interval = 0;
                  }

                  for (JsonPair kv : root)
                  {
                      // if parameter speed
                      if (kv.key().c_str()[2] == 'e')
                          inputs[i].speed = kv.value().as<int>();

                      // if parameter is distnace
                      else if (kv.key().c_str()[2] == 's')
                          inputs[i].distance = kv.value().as<int>();

                      else
                      {
                          inputs[i].direction = kv.value().as<int>();
                          inputs[i].interval = calculate_pause_interval(inputs[i].speed);
                          print_input();
                          i++;
                      }
                  }
                  request->send(200, "text/plain", "sucess");
              });
    server.begin();
}
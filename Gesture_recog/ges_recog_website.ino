#include <WiFi.h>
#include <WiFiProv.h>
#include <WebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Gesture_Recog_inferencing.h>

#define SERVICE_NAME "PROV_ESP32"  
#define POP "abcd1234"  

#define FREQUENCY_HZ 50
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))

// Create Objects
Adafruit_MPU6050 mpu;
WebServer server(80);

float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t feature_ix = 0;
String detectedGesture = "Idle"; 
static unsigned long last_interval_ms = 0;
void sysProvEvent(arduino_event_t *sys_event) {
    switch (sys_event->event_id) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("Connected! IP Address: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("Wi-Fi Disconnected! Restart ESP32 to provision again.");
            break;
        case ARDUINO_EVENT_PROV_START:
            Serial.println("BLE Provisioning started...");
            Serial.println("Use ESP BLE Provisioning App to configure Wi-Fi.");
            break;
        case ARDUINO_EVENT_PROV_CRED_RECV:
            break;
        case ARDUINO_EVENT_PROV_CRED_SUCCESS:
            break;
        case ARDUINO_EVENT_PROV_CRED_FAIL:
            Serial.println("Provisioning Failed! Check credentials and retry.");
            break;
        case ARDUINO_EVENT_PROV_END:
            Serial.println("Provisioning Ended.");
            break;
    }
}


void handleRoot() {
    String html = "<html><head><title>ESP32 Gesture Recognition</title>";
    html += "<style>";
    html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f7fa; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; color: #333;}";
    html += "h1 { font-size: 40px; color: #4CAF50; font-weight: 600; margin-bottom: 20px; text-align: center;}";
    html += ".gesture { font-size: 50px; color: #333; font-weight: 700; padding: 20px 40px; border-radius: 12px; background-color: #ffffff; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); margin-top: 20px; text-align: center;}";
    html += ".container { width: 90%; max-width: 600px; margin: 0 auto; padding: 20px; background-color: #fff; border-radius: 12px; box-shadow: 0 6px 12px rgba(0, 0, 0, 0.1); text-align: center;}";
    html += ".footer { font-size: 14px; color: #888; margin-top: 30px; text-align: center;}";
    html += "</style>";
    html += "<meta http-equiv='refresh' content='1'></head><body>";
    html += "<div class='container'>";
    html += "<h1>ESP32 Gesture Recognition</h1>";
    html += "<div class='gesture'>" + detectedGesture + "</div>";
    html += "</div>";
    html += "<div class='footer'>Powered by ESP32</div>";
    html += "</body></html>";

    server.send(200, "text/html", html); 
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

 
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) { delay(10); }
    }
    Serial.println("MPU6050 Found!");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    WiFi.onEvent(sysProvEvent);
    Serial.println("Starting BLE Provisioning...");
    uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf, 0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02};
    WiFiProv.beginProvision(NETWORK_PROV_SCHEME_BLE, NETWORK_PROV_SCHEME_HANDLER_FREE_BLE, NETWORK_PROV_SECURITY_1, POP, SERVICE_NAME, NULL, uuid, true);

   
    Serial.println("Scan this QR code in the ESP BLE Provisioning App:");
    WiFiProv.printQR(SERVICE_NAME, POP, "ble");

   
    server.on("/", HTTP_GET, handleRoot);
    server.begin();
}

void loop() {
    sensors_event_t a, g, temp;

    if (millis() > last_interval_ms + INTERVAL_MS) {
        last_interval_ms = millis();

        mpu.getEvent(&a, &g, &temp);

        features[feature_ix++] = a.acceleration.x;
        features[feature_ix++] = a.acceleration.y;
        features[feature_ix++] = a.acceleration.z;

        if (feature_ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
            
            signal_t signal;
            ei_impulse_result_t result;
            int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
            if (err != 0) {
                return;
            }

            EI_IMPULSE_ERROR res = run_classifier(&signal, &result, true);
            if (res != 0) return;
            for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
                if (result.classification[ix].value > 0.6) {
                    detectedGesture = result.classification[ix].label;
                }
            }
            feature_ix = 0;
        }
    }

    server.handleClient(); 
}
void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);
    if (r > 0) {
    }
}

#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

#define FREQUENCY_HZ        50
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))
#define ACC_RANGE           1 
#define CONVERT_G_TO_MS2    (9.81/(16384.0/(1.+ACC_RANGE))) 
static unsigned long last_interval_ms = 0;
MPU6050 imu;
int16_t ax, ay, az;
void setup() {
  
    Serial.begin(115200);
    Serial.println("Initializing I2C devices...");
    Wire.begin();
    imu.initialize();
    delay(10);
    delay(300);
    imu.setXAccelOffset(-1441);
    imu.setYAccelOffset(2324);
    imu.setZAccelOffset(1249);
    imu.setXGyroOffset(-71);
    imu.setYGyroOffset(-23);
    imu.setZGyroOffset(-20);
    imu.setFullScaleAccelRange(ACC_RANGE);
}

void loop() {

      if (millis() > last_interval_ms + INTERVAL_MS) {
        last_interval_ms = millis();
        imu.getAcceleration(&ax, &ay, &az);
        float ax_m_s2 = ax * CONVERT_G_TO_MS2;
        float ay_m_s2 = ay * CONVERT_G_TO_MS2;
        float az_m_s2 = az * CONVERT_G_TO_MS2;
        Serial.print(ax_m_s2); 
        Serial.print("\t");
        Serial.print(ay_m_s2); 
        Serial.print("\t");
        Serial.println(az_m_s2); 
      }
}
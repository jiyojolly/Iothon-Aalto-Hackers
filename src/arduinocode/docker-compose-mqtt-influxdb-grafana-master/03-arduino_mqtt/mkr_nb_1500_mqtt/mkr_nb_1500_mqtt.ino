/**

    This is the MKR NB 1500 MQTT sketch, configured
    to work with the IoThon 2019 VMs.

    Required libraries:
      - MKRNB
      - PubSubClient
**/
/***************************************************
 * Librariy Includes
 */
#include <MKRNB.h>
#include <PubSubClient.h>

//For Sensor setups
#include <Wire.h>

#include "SparkFunBME280.h"
#include "SparkFunCCS811.h"

/**************************************************
 * At minimum do change the following.
 *
 * If you don't change them, you will be sending your
 * data to the example server, at the example topic
 */
#define MQTT_TOPIC_TEMPERATURE "iothon/myteam/temperature"
#define MQTT_TOPIC_CO2        "iothon/myteam/co2"
#define MQTT_TOPIC_HUMIDITY       "iothon/myteam/humidity"
#define MQTT_TOPIC_PRESSURE       "iothon/myteam/pressure"
#define MQTT_TOPIC_STATE       "iothon/myteam/status"
#define MY_SERVER  "195.148.126.71"
/****************************************************/

#define MQTT_PUBLISH_DELAY        1000 // 60 seconds
#define MQTT_CLIENT_ID           "mkrnb1500iothon"

//CO2 sensor starting address
#define CCS811_ADDR 0x5B 

const char *MQTT_SERVER   = MY_SERVER;
const char *MQTT_USER     = "mqttuser";     // NULL for no authentication
const char *MQTT_PASSWORD = "mqttpassword"; // NULL for no authentication

//Setup SIM Card
const char PIN_NUMBER[] = "0000";
const char APN[] = ""; // "iot";


//Sensor object initialization 
BME280 mySensor;
CCS811 co2sensor(CCS811_ADDR);

//Modem object init
NB           nbAccess(true); // NB access: use a 'true' parameter to enable debugging
GPRS         gprsAccess;     // GPRS access
NBClient     tcpSocket;
PubSubClient mqttClient(tcpSocket);

/*********************************************************
  * Initialize variables
  */
  float Co2;
  float Humidity;
  float Pressure;
  float Temperature;
 /*********************************************************/

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (! Serial)
    ;
/*****************************************************
 * Setup Modem and connect to Network
 */
  Serial.println("MKR NB 1500 MQTT client starting.");

  Serial.println("Connect: NB-IoT / LTE Cat M1 network (may take several minutes)...");
  while (nbAccess.begin(PIN_NUMBER, APN) != NB_READY) {
    Serial.println("Connect: NB-IoT: failed.  Retrying in 10 seconds.");
    delay(10000);
  }
  Serial.println("Connect: NB-IoT: connected.");

  Serial.println("Acquire: PDP context...");
  while (gprsAccess.attachGPRS() != GPRS_READY) {
    Serial.println("Acquire: PDP context: failed.  Retrying in 10 seconds.");
    delay(10000);
  }
  Serial.println("Acquire: PDP context: acquired.");
#if 1 /* Using newer version of the library */
  Serial.print("Acquire: PDP Context: ");
  Serial.println(nbAccess.readPDPparameters());
#endif

  mqttClient.setServer(MQTT_SERVER, 1883);
  /*******************************************************
 /*******************************************************
  * Setup Environment Sensor and begin I2C communication 
  */
  Serial.println("Reading basic values from BME280 and ccs811");

  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
    CCS811Core::status returnCode = co2sensor.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    Serial.println(".begin() returned with an error.");
    while (1); //Hang if there was a problem.
  }
 
}

long lastMsgTime = 0;

void loop() {
  Serial.println("Looping: start...");
  if (!mqttClient.loop()) {
    mqttConnectIfNeeded();
  }

  long now = millis();
  if (now - lastMsgTime > MQTT_PUBLISH_DELAY) {
    lastMsgTime = now;
    
    Temperature = mySensor.readTempC();
    co2sensor.readAlgorithmResults();
    Co2 = co2sensor.getCO2();
    Humidity = mySensor.readFloatHumidity();
    Pressure = mySensor.readFloatPressure();
    Pressure = Pressure/100000;
    
    mqttPublish(MQTT_TOPIC_TEMPERATURE, Temperature);
    mqttPublish(MQTT_TOPIC_CO2, Co2);
    mqttPublish(MQTT_TOPIC_HUMIDITY, Humidity);
    mqttPublish(MQTT_TOPIC_PRESSURE, Pressure);
  }
  Serial.println("Looping: delay...");
  delay(1000);
  Serial.println("Looping: done.");
}

void mqttConnectIfNeeded() {
  while (!mqttClient.connected()) {
    Serial.println("Connect: starting...");

    // Attempt to connect
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD, MQTT_TOPIC_STATE, 1, true, "disconnected", false)) {
      Serial.println("Connect: connected.");

      // Once connected, publish an announcement...
      mqttClient.publish(MQTT_TOPIC_STATE, "connected", true);
    } else {
      Serial.print("Connect: failed, state=");
      Serial.print(mqttClient.state());
      Serial.println(". Trying again in 5 seconds.");
      delay(5000);
    }
  }
}

void mqttPublish(String topic, float payload) {
  Serial.print("Publish: ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);

  mqttClient.publish(topic.c_str(), String(payload).c_str(), true);
}

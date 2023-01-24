/*************************************************************

  This Sketch is the Firmware version for the Smart Environment
  Logger. The purpose of this Firmware is to make the logging 
  device usable with the Blynk IOT Service.

  This Environemnt Logger uses the BME860 sensor from Bosch to
  log, temperature, humidity and pressure. The CCS811 sensor
  is used to log C02 and TVOC levels. The BH1750 sensors is used
  to log the light levels and the GRV UV-A/B Sensor measures the UV
  A and UV B levels.

  Author: Hannes Suhr
  
  Firmmware version: 1.0

  Required Libraries: Wire.h, SparkfunCCS811.h,
                      Adafruit_Sensor.h, Adafruit_BME680.h,
                      ESP8266WiFi.h, BlynkSimpleEsp8266.h


  Hardware: The BME860, CCS811 and BH1750 are connected
            via I2C with pin 5 as SDA and pin 4 as SCL.
            The BME860 is connected to VCC = 3.3V from
            the micor controller. The other twos are 
            connected to 5V. The GRV UV-A/B is connected
            to A0.

  
 *************************************************************/


// importing Hardware Libraries 
#include <BH1750.h>
#include <Wire.h>
#include "SparkFunCCS811.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// Pin Definitions
#define SDA_PIN 5
#define SCL_PIN 4
#define ANALOG_IN A0

// Define I2C Addresses
#define CCS811_ADDR 0x5A

// Other definitions
#define SEALEVELPRESSURE_HPA (1013.25)

// Sensor Objects
Adafruit_BME680 bme;
CCS811 CCS_SENSOR(CCS811_ADDR);
BH1750 lightMeter;

// Define sensor variables
float temp = 0.0;
float humid = 0.0;
float pressur = 0.0;
float gasRes = 0.0;
float alt = 0.0;
float co2 = 0.0;
float tvoc = 0.0;
float uvab = 0.0;
float lux = 0.0;

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID   "TMPLryj6Wojb"
#define BLYNK_DEVICE_NAME   "Smart Environment Logger"
#define BLYNK_AUTH_TOKEN    "_F4GR7TUrYlNIq_Ff5OyNafR3Jt-yaqY"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// import Blynk libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// Your WiFi credentials.
// Set password to "" for open networks.
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WLAN-VEQX8Y";
char pass[] = "0719050360029229";

// Blynk Timer Object
BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // Send the sensor Data to the Blynk Cloud
  Blynk.virtualWrite(V2, millis() / 1000.0);
  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V4, humid);
  Blynk.virtualWrite(V5, pressur);
  Blynk.virtualWrite(V6, gasRes * 1000.0);
  Blynk.virtualWrite(V7, alt);
  Blynk.virtualWrite(V8, lux);
  Blynk.virtualWrite(V9, uvab); // returns the uv index
  Blynk.virtualWrite(V10, co2);
  Blynk.virtualWrite(V11, tvoc);

  // Check for boundary conditions and send events
  sendEvents();

  // print all sensor data in a console
  printAllSensorData();
}

// This function receives sensor data from all devices
void getAllSensorData () {

  // get ccs811 sensor data
  if (CCS_SENSOR.dataAvailable())
  {
    //If so, have the sensor read and calculate the results.
    //Get them later
    CCS_SENSOR.readAlgorithmResults();

    co2 = CCS_SENSOR.getCO2(); // in ppm
    tvoc = CCS_SENSOR.getTVOC(); // in ppb
  }

  // get bme860 sensor data
  temp = bme.temperature - 3.0; // in °C
  pressur = bme.pressure / 100.0; // in hPa
  humid = bme.humidity; // in %
  gasRes = bme.gas_resistance / 1000.0; // in kOhms
  alt =  bme.readAltitude(SEALEVELPRESSURE_HPA); // in m

  // get the bh1750 sensor data
  lux = lightMeter.readLightLevel();

  // get the UVA/UVB sensor data
  uvab = analogRead(ANALOG_IN);

  //delay(100);
}

// This function will print all the data into the serial monitor
void printAllSensorData() {

  Serial.print("Temperature in [°C]: \t\t"); Serial.println(temp);
  Serial.print("Humidity in [%]: \t\t"); Serial.println(humid);
  Serial.print("Pressure in [hPA]: \t\t"); Serial.println(pressur);
  Serial.print("Gas Resistance in [kOhm]: \t"); Serial.println(gasRes);
  Serial.print("Altitude in [m]: \t\t"); Serial.println(alt);
  Serial.print("CO2 in [ppm]: \t\t\t"); Serial.println(co2);
  Serial.print("TVOC in [ppm]: \t\t\t"); Serial.println(tvoc);
  Serial.print("UVAB in [-]: \t\t\t"); Serial.println(uvab);
  Serial.print("Light Level in [lux]: \t\t"); Serial.println(lux);
  Serial.println();

}

// This function will send events depending on boundary conditions
void sendEvents() {

  // Temperature
  if (temp < 20.0)
     Blynk.logEvent("too_low_temperature");
  if (temp > 30.0)
     Blynk.logEvent("too_high_temperature");

  // Humidity
  if (humid < 40.0)
     Blynk.logEvent("too_low_humidity");
  if (humid > 50.0)
     Blynk.logEvent("too_high_humidity");

  // CO2
  if (co2 > 1000.0)
     Blynk.logEvent("too_high_co2");

  // TVOC
  if (tvoc > 661.0)
     Blynk.logEvent("too_high_tvoc");
     
}

// Function to setup the BH1750 Sensor at the startup
void setupBH1750Sensor () {
  
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
  
}

// Function to setup the CCS811 Sensor at the startup
void setupCCSSensor () {
  
  Serial.println("Setting up CCS811 Sensor.");
  if (CCS_SENSOR.begin() == false)
  {
    Serial.print("CCS811 Sensor error. Please check wiring. Freezing...");
    while (1);
  }

}

// Function to setup the BME860 Sensor at the startup
void setupBME680Sensor () {

  // check if we can connect to the Sensor
  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

}

// Setup function running once at startup
void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // 
  Wire.begin(SDA_PIN, SCL_PIN); //Inialize I2C Hardware

  // Setup the sensor modules
  setupCCSSensor();
  setupBME680Sensor();
  setupBH1750Sensor();
}

// Loop function running all the time
void loop()
{
  // get the sensor data
  getAllSensorData();

  // Update Blynk
  Blynk.run();

  // Run Blynk Timer
  timer.run();

}

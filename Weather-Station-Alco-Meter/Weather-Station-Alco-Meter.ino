//Dx values represent the pin name on NodeMCU dev board
//Alco Tester
void checkReset();
void alcoLed(float alcoLvl);
void startAnim();
void alcoTester();
void readAlcohol();
void ledContrWeather(int rowsNo);
void weatherLedDisplay(String icon);

// Constants
#define SENSOR_PIN  0
#define BAC_START   410     // Beginning ADC value of BAC chart
#define BAC_END     960     // Last ADC value in BAC chart

// Alcometer
int TIME_UNTIL_WARMUP = 100; // Sensor warm up time
int TIME_UNTIL_RESET = 30;  // Defined reset time to allow the alco sensor return to initial position
int bacHistory = 7; // seconds until BAC value has been reset
unsigned long secs; //For time storage
unsigned long resetTime; //For time storage
unsigned long secsCurrent; //For time storage
unsigned long bacHistoryTime; //For time storage
int resetButton = 16; //D0 let sensor return to the initial state
int toggleButton = 5; //D1 Toggle between alcometer and weather station
int resetFlag = 0; // Reset button
int firstTime1 = 0; //Set time. Run for only 1 cycle
int firstTime2 = 0; //0 if haven't been run for the first time, otherwise 1
int sensor_read; // ADC value
int sensorReadForCal; // for calculations
float val = 0; //For BAC display

float bac;
float bac_chart[] =
{0.012, 0.015, 0.019, 0.023, 0.026, 0.030, 0.033, 0.036, 0.039, 0.043, 0.046, 0.048, 0.051, 0.054, 0.057, 0.059, 0.062, 0.064, 0.067, 0.069, 0.071, 0.074, 0.076, 0.078, 0.080, 0.082, 0.084, 0.086, 0.087, 0.089, 0.091, 0.092, 0.094, 0.096, 0.097, 0.098, 0.100, 0.101, 0.102, 0.104, 0.105, 0.106, 0.107, 0.108, 0.109, 0.110, 0.111, 0.112, 0.113, 0.114, 0.115, 0.116, 0.117, 0.117, 0.118, 0.119, 0.120, 0.120, 0.121, 0.121, 0.122, 0.123, 0.123, 0.124, 0.124, 0.125, 0.125, 0.126, 0.126, 0.126, 0.127, 0.127, 0.128, 0.128, 0.128, 0.129, 0.129, 0.129, 0.130, 0.130, 0.130, 0.130, 0.131, 0.131, 0.131, 0.131, 0.132, 0.132, 0.132, 0.132, 0.132, 0.133, 0.133, 0.133, 0.133, 0.133, 0.134, 0.134, 0.134, 0.134, 0.134, 0.135, 0.135, 0.135, 0.135, 0.135, 0.136, 0.136, 0.136, 0.136, 0.136, 0.137, 0.137, 0.137, 0.137, 0.137, 0.138, 0.138, 0.138, 0.138, 0.139, 0.139, 0.139, 0.139, 0.140, 0.140, 0.140, 0.140, 0.141, 0.141, 0.141, 0.142, 0.142, 0.142, 0.142, 0.143, 0.143, 0.144, 0.144, 0.144, 0.145, 0.145, 0.145, 0.146, 0.146, 0.147, 0.147, 0.147, 0.148, 0.148, 0.149, 0.149, 0.150, 0.150, 0.151, 0.151, 0.152, 0.152, 0.153, 0.153, 0.154, 0.154, 0.155, 0.155, 0.156, 0.156, 0.157, 0.157, 0.158, 0.159, 0.159, 0.160, 0.160, 0.161, 0.162, 0.162, 0.163, 0.164, 0.164, 0.165, 0.165, 0.166, 0.167, 0.168, 0.168, 0.169, 0.170, 0.170, 0.171, 0.172, 0.172, 0.173, 0.174, 0.175, 0.175, 0.176, 0.177, 0.178, 0.178, 0.179, 0.180, 0.181, 0.181, 0.182, 0.183, 0.184, 0.184, 0.185, 0.186, 0.187, 0.188, 0.188, 0.189, 0.190, 0.191, 0.192, 0.192, 0.193, 0.194, 0.195, 0.196, 0.197, 0.197, 0.198, 0.199, 0.200, 0.201, 0.202, 0.202, 0.203, 0.204, 0.205, 0.206, 0.207, 0.207, 0.208, 0.209, 0.210, 0.211, 0.212, 0.212, 0.213, 0.214, 0.215, 0.216, 0.217, 0.218, 0.218, 0.219, 0.220, 0.221, 0.222, 0.223, 0.223, 0.224, 0.225, 0.226, 0.227, 0.228, 0.228, 0.229, 0.230, 0.231, 0.232, 0.233, 0.234, 0.234, 0.235, 0.236, 0.237, 0.238, 0.239, 0.240, 0.240, 0.241, 0.242, 0.243, 0.244, 0.245, 0.246, 0.246, 0.247, 0.248, 0.249, 0.250, 0.251, 0.252, 0.253, 0.253, 0.254, 0.255, 0.256, 0.257, 0.258, 0.259, 0.260, 0.261, 0.262, 0.263, 0.264, 0.265, 0.266, 0.266, 0.267, 0.268, 0.269, 0.270, 0.271, 0.273, 0.274, 0.275, 0.276, 0.277, 0.278, 0.279, 0.280, 0.281, 0.282, 0.283, 0.285, 0.286, 0.287, 0.288, 0.289, 0.291, 0.292, 0.293, 0.295, 0.296, 0.297, 0.299, 0.300, 0.301, 0.303, 0.304, 0.306, 0.307, 0.309, 0.310, 0.312, 0.314, 0.315, 0.317, 0.319, 0.320, 0.322, 0.324, 0.326, 0.328, 0.330, 0.332, 0.334, 0.336, 0.338, 0.340, 0.342, 0.344, 0.346, 0.349, 0.351, 0.353, 0.356, 0.358, 0.361, 0.363, 0.366, 0.369, 0.371, 0.374, 0.377, 0.380, 0.383, 0.386, 0.389, 0.392, 0.395, 0.398, 0.402, 0.405, 0.409, 0.412, 0.416, 0.419, 0.423, 0.427, 0.431, 0.435, 0.439, 0.443, 0.447, 0.452, 0.456, 0.460, 0.465, 0.470, 0.474, 0.479, 0.484, 0.489, 0.494, 0.500, 0.505, 0.510, 0.516, 0.522, 0.527, 0.533, 0.539, 0.545, 0.551, 0.558, 0.564, 0.571, 0.577, 0.584, 0.591, 0.598, 0.605, 0.613, 0.620, 0.628, 0.636, 0.643, 0.652, 0.660, 0.668, 0.676, 0.685, 0.694, 0.703, 0.712, 0.721, 0.731, 0.740, 0.750, 0.760, 0.770, 0.780, 0.791, 0.801, 0.812, 0.823, 0.834, 0.846, 0.857, 0.869, 0.881, 0.893, 0.905, 0.918, 0.931, 0.944, 0.957, 0.970, 0.984, 0.998, 1.012, 1.026, 1.041, 1.055, 1.070, 1.086, 1.101, 1.117, 1.133, 1.149, 1.166, 1.182, 1.199, 1.217, 1.234, 1.252, 1.270, 1.288, 1.307, 1.326, 1.345, 1.365, 1.384, 1.405, 1.425, 1.446, 1.467, 1.488, 1.509, 1.531, 1.554, 1.576, 1.599, 1.622, 1.646, 1.670, 1.694, 1.719, 1.744, 1.769, 1.795, 1.821, 1.847, 1.874, 1.901, 1.928, 1.956, 1.984, 2.013, 2.042, 2.072, 2.101, 2.132, 2.162, 2.193, 2.225, 2.257, 2.289, 2.322, 2.355, 2.389, 2.423, 2.457, 2.492, 2.528, 2.564, 2.600, 2.637, 2.674, 2.712, 2.750, 2.789, 2.829, 2.868, 2.909, 2.949, 2.991, 3.033, 3.075, 3.118, 3.161, 3.205, 3.250};
//Values calculated according to the data sheets


//Weather Station
#include <Wire.h>
#include <Ticker.h>
#include "ssd1306_i2c.h"
#include "icons.h"
#include "leds.h"


#include <ESP8266WiFi.h>
#include "WeatherClient.h"

// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
SSD1306 display(0x3c, 14, 12); //Pins D6 & D5
WeatherClient weather;
Ticker ticker;

void drawFrame1(int x, int y);
void drawFrame2(int x, int y);
void drawFrame3(int x, int y);
void drawSpinner(int count, int active);
void setReadyForWeatherUpdate();
const char* getIconFromString(String icon);
void alcoTester();

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[3])(int x, int y) = {drawFrame1, drawFrame2, drawFrame3};

// how many frames are there?
int frameCount = 3;
// on frame is currently displayed
int currentFrame = 0;

// your network SSID (name)
char ssid[] = "xxxxxxxxxxxx";
// your network password
char pass[] = "xxxxxxxxxxxxxxxx";

// Go to forecast.io and register for an API KEY
String forecastApiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Coordinates of the place you want
// weather information for
double latitude = 56.968;
double longitude = 24.151;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;

//LEDS
// led values left for description purpose
//int led1 = 4; //Green D2
//int led2 = 0; //Green D3
//int led3 = 9; //Yellow SD2
//int led4 = 13; //Yellow D7
//int led5 = 15; //Red D8
//int led6 = 3; //Red RX
//int led7 = 1; //Red TX
int pins[7] = { 4, 0, 2, 13, 15, 3, 1};



//---------SETUP------------------

void setup() {
  // initialize dispaly
  display.init();
  // set the drawing functions
  display.setFrameCallbacks(3, frameCallbacks);
  // how many ticks does a slide of frame take?
  display.setFrameTransitionTicks(10);

  display.clear();
  display.display();

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawXbm(34, 10, 60, 36, WiFi_Logo_bits);
    display.setColor(INVERSE);
    display.fillRect(10, 10, 108, 44);
    display.setColor(WHITE);
    drawSpinner(3, counter % 3);
    display.display();
    counter++;
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // update the weather information every 10 mintues only
  // forecast.io only allows 1000 calls per day
  ticker.attach(60 * 10, setReadyForWeatherUpdate);

  //-------Pins-------
  pinMode(pins[0], OUTPUT); //LEDS
  pinMode(pins[1], OUTPUT);
  pinMode(pins[2], OUTPUT);
  pinMode(pins[3], OUTPUT);
  pinMode(pins[4], OUTPUT);
  pinMode(pins[5], OUTPUT);
  pinMode(pins[6], OUTPUT);

  pinMode(resetButton, OUTPUT); //Reset button. For attachInterrupt it should be OUTPUT, not INPUT. Not exactly sure why it works this way :)
  pinMode(toggleButton, INPUT); //Alco Sensor - Weather Switch

  digitalWrite(resetButton, 0); // Default reset pin value
  attachInterrupt(resetButton, checkReset, RISING); // Interrupt
}
//----------LOOP----------------------


void loop() {
  if (!digitalRead(toggleButton)) {   //Choose weather or alcometer mode
    if (readyForWeatherUpdate && display.getFrameState() == display.FRAME_STATE_FIX) {
      readyForWeatherUpdate = false;
      weather.updateWeatherData(forecastApiKey, latitude, longitude);
    }
    display.clear();
    display.nextFrameTick();
    display.display();
    firstTime1 =0; //Reset first time flags for the alco meter 
    firstTime2 =0; //
  }
  else { //Alcotester
    alcoTester();
    checkReset();
  }
  yield();
}

//---------WEATHER-----------------


// NOT My Code Starts Here
void setReadyForWeatherUpdate() {
  readyForWeatherUpdate = true;
}

void drawFrame1(int x, int y) {
  display.setFontScale2x2(false);
  display.drawString(65 + x, 8 + y, "Now");
  display.drawXbm(x + 7, y + 7, 50, 50, getIconFromString(weather.getCurrentIcon()));
  display.setFontScale2x2(true);
  display.drawString(64 + x, 20 + y, String(weather.getCurrentTemp()) + "C");
  weatherLedDisplay(weather.getCurrentIcon());
}

const char* getIconFromString(String icon) {
  //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
  if (icon == "clear-day") {
    return clear_day_bits;
  } else if (icon == "clear-night") {
    return clear_night_bits;
  } else if (icon == "rain") {
    return rain_bits;
  } else if (icon == "snow") {
    return snow_bits;
  } else if (icon == "sleet") {
    return sleet_bits;
  } else if (icon == "wind") {
    return wind_bits;
  } else if (icon == "fog") {
    return fog_bits;
  } else if (icon == "cloudy") {
    return cloudy_bits;
  } else if (icon == "partly-cloudy-day") {
    return partly_cloudy_day_bits;
  } else if (icon == "partly-cloudy-night") {
    return partly_cloudy_night_bits;
  }
  return cloudy_bits;
}

void drawFrame2(int x, int y) {
  display.setFontScale2x2(false);
  display.drawString(65 + x, 0 + y, "Today");
  display.drawXbm(x, y, 60, 60, xbmtemp);
  display.setFontScale2x2(true);
  display.drawString(64 + x, 14 + y, String(weather.getCurrentTemp()) + "C");
  display.setFontScale2x2(false);
  display.drawString(66 + x, 40 + y, String(weather.getMinTempToday()) + "C/" + String(weather.getMaxTempToday()) + "C");

}

void drawFrame3(int x, int y) {
  display.drawXbm(x + 7, y + 7, 50, 50, getIconFromString(weather.getIconTomorrow()));
  display.setFontScale2x2(false);
  display.drawString(65 + x, 7 + y, "Tomorrow");
  display.setFontScale2x2(true);
  display.drawString(64 + x, 20 + y, String(weather.getMaxTempTomorrow()) + "C");
}

void drawSpinner(int count, int active) {
  for (int i = 0; i < count; i++) {
    const char *xbm;
    if (active == i) {
      xbm = active_bits;
    } else {
      xbm = inactive_bits;
    }
    display.drawXbm(64 - (12 * count / 2) + 12 * i, 56, 8, 8, xbm);
  }
}
// NOT My Code Ends Here




void ledContrWeather(int rowsNo) {
  for (int a = 0; a < 7; a++) {
    digitalWrite(pins[a], weatherLeds[rowsNo][a]);
  }
}

void weatherLedDisplay(String icon) {      //Display weather symbol via LEDs                       //My LED Code. There are four types of led configuration for weather types Rain, Cloudy, Partly Cloudy and Rain/Snow
  if (icon == "clear-day") {
    ledContrWeather(0);
  } else if (icon == "clear-night") {
    ledContrWeather(0);
  } else if (icon == "rain") {
    ledContrWeather(3);
  } else if (icon == "snow") {
    ledContrWeather(3);
  } else if (icon == "sleet") {
    ledContrWeather(3);
  } else if (icon == "wind") {
    ledContrWeather(2);
  } else if (icon == "fog") {
    ledContrWeather(2);
  } else if (icon == "cloudy") {
    ledContrWeather(2);
  } else if (icon == "partly-cloudy-day") {
    ledContrWeather(1);
  } else if (icon == "partly-cloudy-night") {
    ledContrWeather(1);
  }
}
//---------ALCO DETECTOR-------------------

void alcoTester() {
  if (firstTime1 == 0 && firstTime2 == 0) { //check if first time. If so, warm up the sensor
    secsCurrent = millis();
    firstTime1 = 1;
  }
  if (millis() - secsCurrent < TIME_UNTIL_WARMUP * 1000 && firstTime2 == 0) // First time warmup
  {
    secs = (millis() - secsCurrent) / 1000;
    display.clear();
    secs = map(secs, 0, TIME_UNTIL_WARMUP, 0, 127);
    float secsFloat = float(secs)/127*100;
    display.drawRect(0, 0, 127, 30); //Empty Bar
    display.setColor(INVERSE);
    display.setFontScale2x2(true);
    display.drawString(8, 8, "Heating");
    display.fillRect(0, 0, secs, 30);
    display.drawString(35, 40, String(secsFloat,0) + "%");
    display.display();
    startAnim();
  } else
  {
    firstTime2 = 1;
    if (resetFlag == 0) {
      readAlcohol();  //Actual BAC test
    }
  }
}

void readAlcohol()
{
  // Read voltage
  sensor_read = analogRead(SENSOR_PIN);
  Serial.print("ADC: ");
  Serial.println(sensor_read);

  // Calculate ppm. Regression fitting from MQ-3 datasheet.
  // Equation using 5V max ADC and RL = 4.7k. "v" is voltage.
  // PPM = 150.4351049*v^5 - 2244.75988*v^4 + 13308.5139*v^3 -
  //       39136.08594*v^2 + 57082.6258*v - 32982.05333
  // Calculate BAC. See BAC/ppm chart from page 2 of:
  // http://sgx.cdistore.com/datasheets/sgx/AN4-Using-MiCS-Sensors-for-Alcohol-Detection1.pdf

  if ( sensor_read < BAC_START ) {
    bac = 0;
  } else if ( sensor_read > BAC_END ) {
    bac = 999;
  } else {
    sensorReadForCal = sensor_read * 1.7 - BAC_START;
    bac = bac_chart[sensorReadForCal];
    Serial.println("BAC");
    Serial.println(bac);
  }
  if (val < bac) {
    val = bac;
  }
  display.clear();
  display.setFontScale2x2(true);
  display.drawString(0, 0, "Blow!");
  display.drawRect(0, 22, 127, 21); //Empty Bar
  display.drawString(0, 27, "BAC:" + String(val, 2));
  display.setFontScale2x2(false);
  display.drawString(0, 55, "ADC:" + String(sensor_read));
  display.display();
  alcoLed(val);

 if (millis() - bacHistoryTime > bacHistory * 1000){  //reset history every "bacHistoryTime=7" seconds
  val=0; 
  bacHistoryTime=millis();
 }
}

void checkReset() //sensor reset function. let it clean up for the next blow
{
  if (digitalRead(resetButton))
  { resetFlag = 1;
    resetTime = millis();
    val = 0;
    digitalWrite(resetButton, 0);
  }
  if (millis() - resetTime < TIME_UNTIL_RESET * 1000 && resetFlag == 1)
  {
    secs = (millis() - resetTime) / 1000;
    display.clear();
    secs = map(secs, 0, TIME_UNTIL_RESET, 0, 100);
    display.drawRect(0, 0, 127, 30); //Empty Bar
    display.setColor(INVERSE);
    display.setFontScale2x2(true);
    display.drawString(8, 8, "Reset...");
    display.fillRect(0, 0, secs, 30);
    display.drawString(35, 40, String(secs) + "%");
    display.display();
    startAnim();
  } else {
    resetFlag = 0;
  }
}

void ledContr(int rowsNo) { //led controller for the alcometer
  for (int a = 0; a < 7; a++) {
    digitalWrite(pins[a], level[rowsNo][a]);
  }
}

void startAnim() { //animation during the warmup and reset
  for (int c = 0; c < 37; c++) {
    for (int b = 0; b < 7; b++) {
      digitalWrite(pins[b], startAnimation[c][b]);
    }
    delay(75);
  }
}

void alcoLed(float alcoLvl) { //display LEDs accordin to the alcohol concentration
  if (alcoLvl < 0.2) {
    ledContr(0);
  } else if (alcoLvl < 0.5) {
    ledContr(1);
  } else if (alcoLvl < 1) {
    ledContr(2);
  } else if (alcoLvl < 1.5) {
    ledContr(3);
  } else if (alcoLvl < 2) {
    ledContr(4);
  } else if (alcoLvl < 3) {
    ledContr(5);
  } else if (alcoLvl > 3.5) {
    ledContr(6);
  }
}


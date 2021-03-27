// This #include statement was automatically added by the Particle IDE.
#include <BH1750Lib.h>

//initialize the light sensor
BH1750Lib lightSensor;

// the value that we will consider full sunlight
int sunlight = 5000;
int lastThreeValues[3];
int nextIndex = 0;
bool wasDark = true;
// IFTTT event name
String event = "sunlight";
// IFTTT recipe data
String eventDataSunlight = "light";
String eventDataDarkness = "dark";
// how often to take a reading. Remember it's in millis!
int readingTime = 10000;

void setup(){
  Serial.begin(9600);
  // wait for serial to come online
  while (!Serial) {}
  
  Serial.print("Starting...");
  // initialise the light sensor
  lightSensor.begin(BH1750LIB_MODE_CONTINUOUSHIGHRES);
  Serial.println(" OK!");
}


void loop() {
  // take the reading
  int lux = lightSensor.lightLevel();
  // print to serial so you can keep track
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.print(Time.hourFormat12());
  Serial.print(":");
  Serial.print(Time.minute());
  Serial.print(":");
  Serial.println(Time.second());
  
  // we're going to average the last three values to combat
  // against a flash of light
  lastThreeValues[nextIndex] = lux;
  nextIndex++;
  
  if (nextIndex > 2) {
      nextIndex = 0;
      if (isSunlight(lux) && wasDark) {
          wasDark = false;
          // send notification that sunlight is on
          Particle.publish(event, eventDataSunlight);
      } else if (!isSunlight(lux) && !wasDark) {
          wasDark = true;
          // send notification that sunilght is off
          Particle.publish(event, eventDataDarkness);
      }
  }
  
  // Take a reading every specified time
  delay(readingTime);
}

// Function to return true if the sensor is sensing sunlight
bool isSunlight(int lux) {
    return lux >= sunlight;
}


#define PRINT_DEBUG_MESSAGES 1
#include <ThingSpeak.h>
#include <SPI.h>
#include <WiFi.h>

char ssid[] = "networkname";     //  your network SSID (name) 
char pass[]="networkpass";    // your network password

#define ELECTRICITY_SENSOR A0
int status = WL_IDLE_STATUS;
float amplitude_current;               //amplitude current
float effective_value;       //effective current 
float watts = amplitude_current/1000*12;

WiFiServer server(80);
WiFiClient client;


//enter ThingSpeak channel number and WriteAPIKey
unsigned long myChannelNumber = 377513;
const char * myWriteAPIKey = "WWK86IVR531UU6T7";

void setup() {
  // start serial port for debugging purposes
  Serial.begin(9600);
  


  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 2 seconds for connection:
    delay(2000);
    pins_init();
  } 
  ThingSpeak.begin(client);
  printWifiStatus();
}

void Grove ()
 {
        int sensor_max;
        sensor_max = getMaxValue();
        Serial.print("sensor_max = ");
        Serial.println(sensor_max);
        //the VCC on the Grove interface of the sensor is 5v
        amplitude_current=(float)sensor_max/1024*5/800*2000000;
        effective_value=amplitude_current/1.414;//minimum_current=1/1024*5/800*2000000/1.414=8.6(mA) 
                            //Only for sinusoidal alternating current
        Serial.println("The amplitude of the current is(in mA)");
        Serial.println(amplitude_current,1);//Only one number after the decimal point
        Serial.println("The effective value of the current is(in mA)");
        Serial.println(effective_value,1);
    }
    void pins_init()
    {
        pinMode(ELECTRICITY_SENSOR, INPUT);
    }
    /*Function: Sample for 1000ms and get the maximum value from the SIG pin*/
    int getMaxValue()
    {
        int sensorValue;             //value read from the sensor
        int sensorMax = 0;
        uint32_t start_time = millis();
        while((millis()-start_time) < 1000)//sample for 1000ms
        {
            sensorValue = analogRead(ELECTRICITY_SENSOR);
            if (sensorValue > sensorMax) 
            {
                /*record the maximum sensor value*/
                sensorMax = sensorValue;
            }
        }
        return sensorMax;
    }


void loop() {

          
  Grove ();
  Serial.println("Going to writeField now.");
  int r = ThingSpeak.writeField(myChannelNumber, 1, watts, myWriteAPIKey);
  Serial.println("writeField said ");
  Serial.println(r);
  delay(20000); // ThingSpeak will only accept updates every 15 seconds.
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



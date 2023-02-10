
#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define TOLERANCE 1
 
Adafruit_MPU6050 mpu;

const char* ssid = "Lol";//"Pixel_9010";
const char* password = "abcd1234";//"123456789";

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "ece180d/neil";
const char *sub = "ece180d/naz";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

 // Set software serial baud to 115200;
 Serial.begin(115200);

 MPU6050Setup();
 
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe

 while (true){
  String imuData = (String)MPU_call(); // Stream and wirelessly transmit MPU data
  client.publish(topic, imuData);
  client.publish(topic, "askjhda");
  Serial.print("sent topic");
  client.subscribe(sub);
 }
}

void callback(char *sub, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(sub);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
 }
 Serial.println();
 Serial.println("-----------------------");
}

void loop() {
 client.loop();
}

void MPU6050Setup(){
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
 
  Serial.println("Adafruit MPU6050 test!");
 
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
 
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
 
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
 
  Serial.println("");
  delay(100);  
}

String MPU_call(){
  String data = "";
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
  if (abs(g.gyro.x) > TOLERANCE && abs(g.gyro.y) > TOLERANCE){
    data = "Stir";
    Serial.println("Stir");
  }  
  else if (abs(g.gyro.x) > TOLERANCE){
    //Serial.println("Detected Motion in X-axis");
    data = "Push";
    Serial.println("Push");
  }
  else if (abs(g.gyro.y) > TOLERANCE){
    //Serial.println("Detected Motion in Y-axis");
    data = "Slide";
    Serial.println("Slide");
  }  
  else{
    data = "Idle";
    Serial.println("Idle");
  }  

  return data;
}

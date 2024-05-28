
// Import required libraries
#include <Arduino.h>
#include<math.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <WebSerial.h>
#include<SPI.h>
//innitial servo position is 90
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MPU6050.h>         // mpu6050
#include <Adafruit_Sensor.h>
// Replace with your network credentials
const char* ssid = "hexa";
const char* password = "12345678";
// const char* ssid = "TP-LINK_13";
// const char* password = "12341234";
Adafruit_MPU6050 mpu;
/////////////////////////////////////          variable to control servo
int pin=0;
Adafruit_PWMServoDriver pwm_r = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm_l = Adafruit_PWMServoDriver(0x41);
int ServoMin=100;
int ServoMax=600;
// variable to caculate mpu
static float trucXBase = 10.4,  trucYBase = 10.4;           // // Roll: góc nghiêng hai bên , Pitch: góc nghiêng trước sau
float RollBase[3] = {80.0, 85.0, 90.0}, PitchBase[3] = {80.0, 85.0, 90.0};             // //Cúi xuống: Pitch giảm // Ngửa lên: Pitch tăng // Nghiêng trái: Roll giảm // Nghiêng phải: Roll tăng
// trajectory
float x[ 160 ] = { 106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0,106.0 };
float y[ 160 ] = { -40.0,-39.9,-39.5,-38.8,-37.9,-36.8,-35.4,-33.8,-32.0,-29.9,-27.7,-25.3,-22.7,-20.0,-17.1,-14.2,-11.1,-8.0,-4.8,-1.6,1.6,4.8,8.0,11.1,14.2,17.1,20.0,22.7,25.3,27.7,29.9,32.0,33.8,35.4,36.8,37.9,38.8,39.5,39.9,40.0,40.0,37.9,35.9,33.8,31.8,29.7,27.7,25.6,23.6,21.5,19.5,17.4,15.4,13.3,11.3,9.2,7.2,5.1,3.1,1.0,-1.0,-3.1,-5.1,-7.2,-9.2,-11.3,-13.3,-15.4,-17.4,-19.5,-21.5,-23.6,-25.6,-27.7,-29.7,-31.8,-33.8,-35.9,-37.9,-40.0,40.0,37.9,35.9,33.8,31.8,29.7,27.7,25.6,23.6,21.5,19.5,17.4,15.4,13.3,11.3,9.2,7.2,5.1,3.1,1.0,-1.0,-3.1,-5.1,-7.2,-9.2,-11.3,-13.3,-15.4,-17.4,-19.5,-21.5,-23.6,-25.6,-27.7,-29.7,-31.8,-33.8,-35.9,-37.9,-40.0,-40.0,-39.9,-39.5,-38.8,-37.9,-36.8,-35.4,-33.8,-32.0,-29.9,-27.7,-25.3,-22.7,-20.0,-17.1,-14.2,-11.1,-8.0,-4.8,-1.6,1.6,4.8,8.0,11.1,14.2,17.1,20.0,22.7,25.3,27.7,29.9,32.0,33.8,35.4,36.8,37.9,38.8,39.5,39.9,40.0 };
float z[ 160 ] = { 145.0,141.8,138.6,135.4,132.3,129.3,126.4,123.6,121.0,118.5,116.2,114.0,112.1,110.4,108.9,107.6,106.6,105.8,105.3,105.0,105.0,105.3,105.8,106.6,107.6,108.9,110.4,112.1,114.0,116.2,118.5,121.0,123.6,126.4,129.3,132.3,135.4,138.6,141.8,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,145.0,141.8,138.6,135.4,132.3,129.3,126.4,123.6,121.0,118.5,116.2,114.0,112.1,110.4,108.9,107.6,106.6,105.8,105.3,105.0,105.0,105.3,105.8,106.6,107.6,108.9,110.4,112.1,114.0,116.2,118.5,121.0,123.6,126.4,129.3,132.3,135.4,138.6,141.8,145.0 };
float balanece_pos[3] = { 106.0, 0.0, 133.0};
// All function start from here
float RollTheta(float giatocXHientai)
{
    float gocHientai;
    gocHientai = acos(giatocXHientai /trucXBase)*180/PI;
    return gocHientai;
}
float PitchTheta(float giatocYHientai)
{
    float gocHientai;
    gocHientai = acos(giatocYHientai /trucYBase)*180/PI;
    return gocHientai;
}
float *change_axis(float theta, float x, float y, float *result){
  float c = cos(-theta);
  float s = sin(-theta);
  result[0] =  x*c - y*s;
  result[1] =  x*s + y*c;
  return result;
}
float * find_theta_23(int h, int L1, int L2, int L3, int x, int z, float *theta){
  float L = sqrt( (float) pow(x-L1,2) + (float) pow(z-h,2));
  float alpha = asin(float(abs(z-h)/L));
  float beta = acos(float((L3*L3 - L2*L2 - L*L) / (-2.0 * L2 * L))); 
  theta[0] =  beta - alpha;
  theta[1] = acos(float((L*L - L2*L2 - L3*L3) / (-2.0 * L2 * L3)));
  return theta;
}
float * inK(int x, int y, int z, float *euler){
  int coxa=37;    //L1
  int femur=60;   //L2
  int tibia=90;   //L3
  int h=36;
  euler[0] = atan(float(y)/float(x));
  float pos[2];
  change_axis( euler[0], x, y, pos);
  float theta23[2];
  find_theta_23(h, coxa, femur, tibia, pos[0], z, theta23);
  euler[1] = theta23[0];
  euler[2] = theta23[1];
  if (euler[0] > PI) 
    euler[0] -= 2*PI;
  else if (euler[0] < -PI)
    euler[0] += 2*PI;

  if (euler[1] > PI) 
    euler[1] -= 2*PI;
  else if (euler[1] < -PI)
    euler[1] += 2*PI;

  if (euler[2] > PI) 
    euler[2] -= 2*PI;
  else if (euler[2] < -PI)
    euler[2] += 2*PI;
  euler[0] = roundf(euler[0] * 180/PI);
  euler[1] = roundf(euler[1] * 180/PI);
  euler[2] = roundf(euler[2] * 180/PI);
  return euler;
}

float * ik_to_theta_control_right_leg(float theta1, float theta2, float theta3, float *value_right){
    value_right[0] = theta1+ 90;                                                       // các chân cùng pha ở bên phải
    value_right[1] = 90 - theta2;
    value_right[2] = theta3;

    return value_right;
}
float * ik_to_theta_control_left_leg(float theta1, float theta2, float theta3, float *value_left){
    value_left[0] =  90 - theta1;                                                     // các chân cùng pha ở bên trái
    value_left[1] = 90 + theta2;
    value_left[2] = 90 - (theta3 - 90);

    return value_left;
}
int angle(int ang){
  if (ang>180) 
    ang -= 180;
  else if (ang<-180) 
    ang += 180;
  
  int pulse=map(ang, 0,180, ServoMin, ServoMax);
  Serial.println(pulse);
  return pulse;
}
void standup(int alpha){
pwm_r.setPWM(0, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(1, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(2, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(4, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(5, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(6, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(8, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(9, 0,angle(alpha));
    delay(10);
    pwm_r.setPWM(10, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(0, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(1, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(2, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(4, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(5, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(6, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(8, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(9, 0,angle(alpha));
    delay(10);
    pwm_l.setPWM(10, 0,angle(alpha));
    delay(10);
}
void walking_blind(){
    pwm_r.setPWM(5, 0,angle(60));
    pwm_l.setPWM(4, 0,angle(120));
    pwm_l.setPWM(6, 0,angle(120));
    delay(100);
    // 
    pwm_r.setPWM(1, 0,angle(120));
    pwm_l.setPWM(0, 0,angle(60));
    pwm_l.setPWM(2, 0,angle(60));
    delay(100);
    //
    pwm_r.setPWM(5, 0,angle(90));
    pwm_l.setPWM(4, 0,angle(90));
    pwm_l.setPWM(6, 0,angle(90));
    delay(100);
    //
    pwm_r.setPWM(1, 0,angle(90));
    pwm_l.setPWM(0, 0,angle(90));
    pwm_l.setPWM(2, 0,angle(90));
    delay(100);
    //
    pwm_r.setPWM(4, 0,angle(60));
    pwm_r.setPWM(6, 0,angle(60));
    pwm_l.setPWM(5, 0,angle(120));
    delay(100);
    //
    pwm_r.setPWM(0, 0,angle(120));
    pwm_r.setPWM(2, 0,angle(120));
    pwm_l.setPWM(1, 0,angle(60));
    delay(100);
    //
    pwm_r.setPWM(4, 0,angle(90));
    pwm_r.setPWM(6, 0,angle(90));
    pwm_l.setPWM(5, 0,angle(90));
    delay(100);
    //
    pwm_r.setPWM(0, 0,angle(90));
    pwm_r.setPWM(2, 0,angle(90));
    pwm_l.setPWM(1, 0,angle(90));
    delay(100);


}

void walk_IK_no_balance(float rr1, float rr2, float rr3, float rl1, float rl2, float rl3, float lr1, float lr2, float lr3, float ll1, float ll2, float ll3 ){
  // 1 lefft 
  pwm_l.setPWM(1, 0, angle(rl1));
  pwm_l.setPWM(5, 0, angle(rl2));
  pwm_l.setPWM(9, 0, angle(rl3));
  //  2 right
  pwm_r.setPWM(0, 0, angle(rr1));
  pwm_r.setPWM(4, 0, angle(rr2));
  pwm_r.setPWM(8, 0, angle(rr3));
  // -----------
  pwm_r.setPWM(2, 0, angle(rr1));
  pwm_r.setPWM(6, 0, angle(rr2));
  pwm_r.setPWM(10, 0, angle(rr3));
  // 1 right 
  pwm_r.setPWM(1, 0, angle(lr1));
  pwm_r.setPWM(5, 0, angle(lr2));
  pwm_r.setPWM(9, 0, angle(lr3));
  //  2 left
  pwm_l.setPWM(0, 0, angle(ll1));
  pwm_l.setPWM(4, 0, angle(ll2));
  pwm_l.setPWM(8, 0, angle(ll3));
  // // -----------
  pwm_l.setPWM(2, 0, angle(ll1));
  pwm_l.setPWM(6, 0, angle(ll2));
  pwm_l.setPWM(10, 0, angle(ll3));

}
void control_IK_R0(float r1, float r2, float r3){
  pwm_r.setPWM(0, 0, angle(r1));
  pwm_r.setPWM(4, 0, angle(r2));
  pwm_r.setPWM(8, 0, angle(r3));
};
void control_IK_R1(float r1, float r2, float r3){
  pwm_r.setPWM(1, 0, angle(r1));
  pwm_r.setPWM(5, 0, angle(r2));
  pwm_r.setPWM(9, 0, angle(r3)); 
};
void control_IK_R2(float r1, float r2, float r3){
  pwm_r.setPWM(2, 0, angle(r1));
  pwm_r.setPWM(6, 0, angle(r2));
  pwm_r.setPWM(10, 0, angle(r3));
};
void control_IK_L0(float l1, float l2, float l3){
  pwm_l.setPWM(0, 0, angle(l1));
  pwm_l.setPWM(4, 0, angle(l2));
  pwm_l.setPWM(8, 0, angle(l3));
};
void control_IK_L1(float l1, float l2, float l3){
  pwm_l.setPWM(1, 0, angle(l1));
  pwm_l.setPWM(5, 0, angle(l2));
  pwm_l.setPWM(9, 0, angle(l3));
};
void control_IK_L2(float l1, float l2, float l3){
  pwm_l.setPWM(2, 0, angle(l1));
  pwm_l.setPWM(6, 0, angle(l2));
  pwm_l.setPWM(10, 0, angle(l3));
};

void walk_1side_right_IK(float r1, float r2, float r3, float l1, float l2, float l3){
  // 1 right 
  pwm_r.setPWM(1, 0, angle(r1));
  pwm_r.setPWM(5, 0, angle(r2));
  pwm_r.setPWM(9, 0, angle(r3));
  //  2 left
  pwm_r.setPWM(0, 0, angle(l1));
  pwm_r.setPWM(4, 0, angle(l2));
  pwm_r.setPWM(8, 0, angle(l3));
  // // -----------
  pwm_r.setPWM(2, 0, angle(l1));
  pwm_r.setPWM(6, 0, angle(l2));
  pwm_r.setPWM(10, 0, angle(l3));


}
/////////////////////////////////////


bool ledState = 0;
const int ledPin = 1;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>)rawliteral";

void notifyClients() {
  ws.textAll(String(ledState));
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  // Web Serrial
  WebSerial.begin(&server);
  // WebSerial.msgCallback(recvMsg);
  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start ElegantOTA
  AsyncElegantOTA.begin(&server);
  // Start server
  server.begin();
  ///////////////////////////////////////////////////////////////////////////
  
  Serial.println("GPIO test!");
  
  pwm_l.begin();
  pwm_r.begin();
  pwm_l.setPWMFreq(55); 
  pwm_r.setPWMFreq(55);
  Wire.setClock(400000);
  //set up for mpu
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
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //end up setup
  // standup(90);
  delay(2000);
}

void loop() {
  // ws.cleanupClients();
  // standup(90);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
// /////////////////al.print("//");
//     WebSerial.print(theta_1r[1]); WebSerial.print("//");
//     WebSerial.print(theta_2r[2]); WebSerial.println("//");
//     walk_IK_no_balance(theta_1r[0], theta_1r[1], theta_1r[2], theta_1l[0], theta_1l[1], theta_1l[2], theta_2r[0], theta_2r[1], theta_2r[2], theta_2l[0], theta_2l[1], theta_2l[2]);
//     // walk_1side_right_IK(theta_1r[0], theta_1r[1], theta_1r[2], theta_2r[0], theta_2r[1], theta_2r[2]); 
//   }
// //   // delay(50);

// //////////////////////////////////////////////         mpu balance
// // // Roll: góc nghiêng hai bên , Pitch: góc nghiêng trước sau
// // //Cúi xuống: Pitch giảm // Ngửa lên: Pitch tăng // Nghiêng trái: Roll giảm // Nghiêng phải: Roll tăng
//   float thetaRight0[3],thetaRight1[3],thetaRight2[3], thetaLeft0[3], thetaLeft1[3], thetaLeft2[3];
//   float roll = RollTheta(a.acceleration.x), pitch = PitchTheta(a.acceleration.y);
//   WebSerial.print(roll); WebSerial.print("//");WebSerial.print(pitch);
//   float ZdeltaRoll = 0 + roundf((-roll + RollBase)*2),
//         ZdeltaPitch = 0 + roundf((pitch  -  PitchBase)*2);
//   inK(balanece_pos[0], balanece_pos[1], balanece_pos[2] + ZdeltaRoll - ZdeltaPitch , thetaLeft2);                  // Left leg
//   inK(balanece_pos[0], balanece_pos[1], balanece_pos[2] + ZdeltaRoll , thetaLeft1);
//   inK(balanece_pos[0], balanece_pos[1], balanece_pos[2] + ZdeltaRoll + ZdeltaPitch , thetaLeft0);
//   inK(balanece_pos[0], balanece_pos[1], balanece_pos[2] - ZdeltaRoll - ZdeltaPitch  , thetaRight2);                  //Right Leg
//   inK(balanece_pos[0], balanece_pos[1], balanece_pos[2] - ZdeltaRoll , thetaRight1);
//   inK(balanece_pos[0], balanece_pos[1], balanece_pos[2] - ZdeltaRoll + ZdeltaPitch  , thetaRight0);
//   //// convert theta
//   ik_to_theta_control_left_leg(thetaLeft0[0], thetaLeft0[1], thetaLeft0[2], thetaLeft0);
//   ik_to_theta_control_left_leg(thetaLeft1[0], thetaLeft1[1], thetaLeft1[2], thetaLeft1);
//   ik_to_theta_control_left_leg(thetaLeft2[0], thetaLeft2[1], thetaLeft2[2], thetaLeft2);
//   ik_to_theta_control_right_leg(thetaRight0[0], thetaRight0[1], thetaRight0[2], thetaRight0);
//   ik_to_theta_control_right_leg(thetaRight1[0], thetaRight1[1], thetaRight1[2], thetaRight1);
//   ik_to_theta_control_right_leg(thetaRight2[0], thetaRight2[1], thetaRight2[2], thetaRight2);
//   //// start control movement
//   control_IK_L0(thetaLeft0[0], thetaLeft0[1], thetaLeft0[2]);
//   control_IK_L1(thetaLeft1[0], thetaLeft1[1], thetaLeft1[2]);
//   control_IK_L2(thetaLeft2[0], thetaLeft2[1], thetaLeft2[2]);
//   control_IK_R0(thetaRight0[0], thetaRight0[1], thetaRight0[2]);
//   control_IK_R1(thetaRight1[0], thetaRight1[1], thetaRight1[2]);
//   control_IK_R2(thetaRight2[0], thetaRight2[1], thetaRight2[2]);/////////////////////////////                    walking IK no_balance
/////////////////////////////////////////////// di chuyển có cân bằng
  float thetaRight0[3],thetaRight1[3],thetaRight2[3], thetaLeft0[3], thetaLeft1[3], thetaLeft2[3];
  for (int i = 0, j = (sizeof(z)/sizeof(float)) /2   ; i< (sizeof(z)/sizeof(float)) /2, j < sizeof(z)/sizeof(float); i++, j++ ){
    float roll = RollTheta(a.acceleration.x), pitch = PitchTheta(a.acceleration.y);
    // WebSerial.print(roll); WebSerial.print("//");WebSerial.print(pitch);
    float ZdeltaRoll, ZdeltaPitch;
    if ((roll < RollBase[0] )||(roll > RollBase[3]))  {                                               // so sánh góc hiện tại và góc cân bằng Roll
      ZdeltaRoll = roundf((-roll + RollBase[2])*1);
    }
    else {
      ZdeltaRoll = 0;
    }
    if ((pitch < PitchBase[0] )||(pitch > PitchBase[3]))  {                                         // so sánh góc hiện tại và góc cân bằng Pitch
      ZdeltaPitch = roundf((-pitch + PitchBase[2])*1);
    }
    else {
      ZdeltaPitch = 0;
    }
    WebSerial.print(ZdeltaRoll); WebSerial.print("//");WebSerial.println(ZdeltaPitch);
    delay(40);
    inK(x[i], y[i], z[i] + ZdeltaRoll + ZdeltaPitch , thetaLeft2);                  // Left leg
    inK(x[j], y[j], z[j]  + ZdeltaRoll , thetaLeft1);
    inK(x[i], y[i], z[i]  + ZdeltaRoll - ZdeltaPitch , thetaLeft0);
    inK(x[j], y[j], z[j]  - ZdeltaRoll + ZdeltaPitch  , thetaRight2);                  //Right Leg
    inK(x[i], y[i], z[i]  - ZdeltaRoll , thetaRight1);
    inK(x[j], y[j], z[j]  - ZdeltaRoll - ZdeltaPitch  , thetaRight0);
    //// convert theta
    ik_to_theta_control_left_leg(thetaLeft0[0], thetaLeft0[1], thetaLeft0[2], thetaLeft0);
    ik_to_theta_control_left_leg(thetaLeft1[0], thetaLeft1[1], thetaLeft1[2], thetaLeft1);
    ik_to_theta_control_left_leg(thetaLeft2[0], thetaLeft2[1], thetaLeft2[2], thetaLeft2);
    ik_to_theta_control_right_leg(thetaRight0[0], thetaRight0[1], thetaRight0[2], thetaRight0);
    ik_to_theta_control_right_leg(thetaRight1[0], thetaRight1[1], thetaRight1[2], thetaRight1);
    ik_to_theta_control_right_leg(thetaRight2[0], thetaRight2[1], thetaRight2[2], thetaRight2);
    //// start control movement
    control_IK_L0(thetaLeft0[0], thetaLeft0[1], thetaLeft0[2]);
    control_IK_L1(thetaLeft1[0], thetaLeft1[1], thetaLeft1[2]);
    control_IK_L2(thetaLeft2[0], thetaLeft2[1], thetaLeft2[2]);
    control_IK_R0(thetaRight0[0], thetaRight0[1], thetaRight0[2]);
    control_IK_R1(thetaRight1[0], thetaRight1[1], thetaRight1[2]);
    control_IK_R2(thetaRight2[0], thetaRight2[1], thetaRight2[2]);
// web Serial
  }
    WebSerial.println("//////////////////////////////////////");
  // WebSerial.println("Hello");
  delay(150);
}



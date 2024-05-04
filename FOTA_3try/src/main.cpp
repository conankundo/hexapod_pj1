
// Import required libraries
#include <Arduino.h>
#include<math.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <WebSerial.h>
#include<SPI.h>
//innitial position is 90
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// Replace with your network credentials
const char* ssid = "hexa";
const char* password = "12345678";
// const char* ssid = "TP-LINK_13";
// const char* password = "12341234";
/////////////////////////////////////
int pin=0;
Adafruit_PWMServoDriver pwm_r = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm_l = Adafruit_PWMServoDriver(0x41);
long ServoMin=100;
long ServoMax=600;

// int x[] = {98, 98};
// int y[] = {0, 20};
// int z[] = {133, 133}; 
int x[] = {106, 106,   106,  106,   106,  106, 106, 106, 106};
int y[] = {-30,  -26,  -19,  -9, 0, 9,  19,  26,  30};
int z[] = {133,		147,		155,		161,		161,		155,		147,		133};
// float theta1=-45.7;
// float theta2=7.1;
// float theta3=96.5;
// float theta1=68;
// float theta2=9.3;
// float theta3=85.07;
//int step=sizeof(theta1)/sizeof(float);
float *change_axis(float theta, float x, float y, float *result){
  float c = cos(-theta);
  float s = sin(-theta);
  // float pos[2] = { x*c - y*s, x*s + y*c};
  // return * pos;
  result[0] =  x*c - y*s;
  result[1] =  x*s + y*c;
  return result;
}
float * find_theta_23(int h, int L1, int L2, int L3, int x, int z, float *theta){
  float L = sqrt( (float) pow(x-L1,2) + (float) pow(z-h,2));
  float alpha = asin(float(abs(z-h)/L));
  // float beta = acos(float((L3^2-L2^2-L^2) / ((-2)*L*L2))); 
  // theta[0] = beta - alpha;
  // theta[1] =  acos(float((L^2-L2^2-L3^2) / ((-2)*L2*L3)));
  float beta = acos(float((L3*L3 - L2*L2 - L*L) / (-2.0 * L2 * L))); 
  theta[0] = beta - alpha;
  theta[1] = acos(float((L*L - L2*L2 - L3*L3) / (-2.0 * L2 * L3)));
  return theta;
}
float * inK(int x, int y, int z, float *euler){
  int coxa=37;    //1st
  int femur=60;   //2nd
  int tibia=90;
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

  return euler;
}
int angle(int ang){
  if (ang>360) 
    ang -= 360;
  else if (ang<-360) 
    ang += 360;
  
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
void walking(){
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
// void recvMsg(uint8_t *data, size_t len){
//   WebSerial.println("Received Data...");
//   String d = "";
//   for(int i=0; i < len; i++){
//     d += char(data[i]);
//   }
// }
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
  pwm_r.setPWMFreq(50);
  Wire.setClock(400000);
  //standup(90);
}

void loop() {
  ws.cleanupClients();
  //walking();
  //standup(90);
  float theta[3];
  for (int i = 0; i < sizeof(z)/sizeof(int); i++ ){
    
    inK(x[i], y[i], z[i], theta);
    pwm_r.setPWM(1, 0, angle(theta[0]*180/PI + 90));
    pwm_r.setPWM(5, 0, angle(90 - theta[1]*180/PI ));
    pwm_r.setPWM(9, 0, angle(theta[2]*180/PI));
    WebSerial.print(theta[0]*180/PI + 90); WebSerial.print("--");
    
    WebSerial.print(theta[1]*180/PI + 90); WebSerial.print("--");
    
    WebSerial.print(theta[2]*180/PI); WebSerial.println("--");
    
  }
  delay(50);
  WebSerial.println("Hello");
  
}
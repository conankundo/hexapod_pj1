// ///////////////////////////
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>
// #include <Update.h>
// const char* host = "NodeWifi32";
// const char* ssid = "hexa_로봇";
// const char* password = "12345678";
 
// //variabls to blink without delay:
// const int led = 2;
// unsigned long previousMillis = 0;        // will store last time LED was updated
// const long interval = 1000;           // interval at which to blink (milliseconds)
// int ledState = LOW;             // ledState used to set the LED
 
// WebServer server(80);
 
// /*
//  * Login page
//  */
 
// const char* loginIndex = 
//  "<form name='loginForm'>"
//     "<table width='20%' bgcolor='A09F9F' align='center'>"
//         "<tr>"
//             "<td colspan=2>"
//                 "<center><font size=4><b>ESP32 Login Page</b></font></center>"
//                 "<br>"
//             "</td>"
//             "<br>"
//             "<br>"
//         "</tr>"
//         "<td>Username:</td>"
//         "<td><input type='text' size=25 name='userid'><br></td>"
//         "</tr>"
//         "<br>"
//         "<br>"
//         "<tr>"
//             "<td>Password:</td>"
//             "<td><input type='Password' size=25 name='pwd'><br></td>"
//             "<br>"
//             "<br>"
//         "</tr>"
//         "<tr>"
//             "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
//         "</tr>"
//     "</table>"
// "</form>"
// "<script>"
//     "function check(form)"
//     "{"
//     "if(form.userid.value=='admin' && form.pwd.value=='admin')"
//     "{"
//     "window.open('/serverIndex')"
//     "}"
//     "else"
//     "{"
//     " alert('Error Password or Username')/*displays error message*/"
//     "}"
//     "}"
// "</script>";
 
// /*
//  * Server Index Page
//  */
 
// const char* serverIndex = 
// "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
// "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
//    "<input type='file' name='update'>"
//         "<input type='submit' value='Update'>"
//     "</form>"
//  "<div id='prg'>progress: 0%</div>"
//  "<script>"
//   "$('form').submit(function(e){"
//   "e.preventDefault();"
//   "var form = $('#upload_form')[0];"
//   "var data = new FormData(form);"
//   " $.ajax({"
//   "url: '/update',"
//   "type: 'POST',"
//   "data: data,"
//   "contentType: false,"
//   "processData:false,"
//   "xhr: function() {"
//   "var xhr = new window.XMLHttpRequest();"
//   "xhr.upload.addEventListener('progress', function(evt) {"
//   "if (evt.lengthComputable) {"
//   "var per = evt.loaded / evt.total;"
//   "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
//   "}"
//   "}, false);"
//   "return xhr;"
//   "},"
//   "success:function(d, s) {"
//   "console.log('success!')" 
//  "},"
//  "error: function (a, b, c) {"
//  "}"
//  "});"
//  "});"
//  "</script>";
 
// /*
//  * setup function
//  *///////////////////////////////////////////////////////////////////////////////////////
// #include<SPI.h>
// //innitial position is 90
// #include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>
// int pin=0;
// Adafruit_PWMServoDriver pwm_r = Adafruit_PWMServoDriver(0x40);
// Adafruit_PWMServoDriver pwm_l = Adafruit_PWMServoDriver(0x41);
// long ServoMin=100;
// long ServoMax=600;
// int angle(int ang){
//   int pulse=map(ang, 0,180, ServoMin, ServoMax);
//   Serial.println(pulse);
//   return pulse;
// } 
// void setup(void) {
//   Serial.begin(115200);
//   //###################### Connect to WiFi network    ####################
//   WiFi.begin(ssid, password);
//   Serial.println("");
//   // Wait for connection
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.print("Connected to ");
//   Serial.println(ssid);
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
//   /*use mdns for host name resolution*/
//   if (!MDNS.begin(host)) { //http://esp32.local
//     Serial.println("Error setting up MDNS responder!");
//     while (1) {
//       delay(1000);
//     }
//   }
//   Serial.println("mDNS responder started");
//   /*return index page which is stored in serverIndex */
//   server.on("/", HTTP_GET, []() {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/html", loginIndex);
//   });
//   server.on("/serverIndex", HTTP_GET, []() {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/html", serverIndex);
//   });
//   /*handling uploading firmware file */
//   server.on("/update", HTTP_POST, []() {
//     server.sendHeader("Connection", "close");
//     server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
//     ESP.restart();
//   }, []() {
//     HTTPUpload& upload = server.upload();
//     if (upload.status == UPLOAD_FILE_START) {
//       Serial.printf("Update: %s\n", upload.filename.c_str());
//       if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
//         Update.printError(Serial);
//       }
//     } else if (upload.status == UPLOAD_FILE_WRITE) {
//       /* flashing firmware to ESP*/
//       if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
//         Update.printError(Serial);
//       }
//     } else if (upload.status == UPLOAD_FILE_END) {
//       if (Update.end(true)) { //true to set the size to the current progress
//         Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
//       } else {
//         Update.printError(Serial);
//       }
//     }
//   });
//   server.begin();
//   //#############################################// end of web server ///////////////////////
  
  
//   Serial.begin(115200);
//   Serial.println("GPIO test!");
  
//   pwm_l.begin();
//   pwm_r.begin();
//   pwm_l.setPWMFreq(50); 
//   pwm_r.setPWMFreq(55);
//   Wire.setClock(400000);
  
//    for(int i=0;i<16;i++){
//     pwm_l.setPWM(i, 0,angle(70));
//     delay(10);
//     pwm_r.setPWM(i, 0,angle(110));       // turns pin fully on
//     delay(10);
//   }
// }

// void loop(void) {
 
// }
#include <Arduino.h>
#include<SPI.h>
//innitial position is 90
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
int pin=0;
Adafruit_PWMServoDriver pwm_r = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm_l = Adafruit_PWMServoDriver(0x41);
long ServoMin=100;
long ServoMax=600;
int angle(int ang){
  int pulse=map(ang, 0,180, ServoMin, ServoMax);
  Serial.println(pulse);
  return pulse;
}
void setup() {
  Serial.begin(115200);
  Serial.println("GPIO test!");
  
  pwm_l.begin();
  pwm_r.begin();
  pwm_l.setPWMFreq(50); 
  pwm_r.setPWMFreq(55);
  Wire.setClock(400000);
  
   for(int i=0;i<16;i++){
    pwm_l.setPWM(i, 0,angle(70));
    delay(10);
    pwm_r.setPWM(i, 0,angle(110));       // turns pin fully on
    delay(10);
  }
}
void loop() {
 
}


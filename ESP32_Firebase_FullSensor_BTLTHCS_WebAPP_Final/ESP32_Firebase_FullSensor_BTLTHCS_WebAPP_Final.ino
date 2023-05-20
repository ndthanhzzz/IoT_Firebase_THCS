#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "time.h"
#include <FirebaseESP32.h>
#include <WiFiClient.h>

#include <DHT.h>
#define DHTPIN 4              //PIN DHT11
#define DHTTYPE DHT11         //Loại Cảm biến DHT (DHT11, DHT21, DHT22)
DHT dht(DHTPIN, DHTTYPE);

#define SoundPIN 34           //PIN cảm biến âm thanh
#define LightPIN 35           //PIN cảm biến ánh sáng

#include "addons/TokenHelper.h"                                                 // Cung câp thông tin quy trình tạo Token         
#include "addons/RTDBHelper.h"                                                  // Cung cấp thông tin về Real Time DataBase và trợ giúp khác

#define WIFI_SSID "Phong 202"                                                       // Nhap Ten va Pass wifi
#define WIFI_PASSWORD "123456789"

#define API_KEY "AIzaSyDB9PsJCyzlacEKDUaRVJlyUwmxpmIbiDc"                       // Nhap Firebase project API Key
#define USER_EMAIL "nguyendangthanh20019@gmail.com"                             // Nhap Email va Password cai dat USER luc add user
#define USER_PASSWORD "12345678"
#define DATABASE_URL "https://btl-thcs-esp32-ptit-default-rtdb.asia-southeast1.firebasedatabase.app/"                           // Nhap dia chi URL DATA database

FirebaseData fbdo;                                  // Gán Firebase objects
FirebaseAuth auth;
FirebaseConfig config;

String uid;                                         // Khai báo UID
String databasePath;                               

String tempPath = "/temperature";                      // Khai báo và gán dữ liệu sẽ được hiển thị trên Firebase Database
String humPath = "/humidity";
String soundPath = "/sound";
String lightPath = "/light";
String timePath = "/timestamp";

// Nhóm dữ liệu update mỗi vòng lặp (Hàm loop)
String parentPath;

int timestamp;                                                            //Khai báo hàm thời gian
const char* ntpServer = "pool.ntp.org";                                   // Khai báo sever lấy dữ liệu thời gian

FirebaseJson json;                                                         // Khai báo đối tượng FirebaseJson

// Khai bao gia tri cam bien DHT11 
float t;
float h;

// Khai bao gia tri cam bien Anh sang va Am thanh
int Sound_val;
int Light_val;
int Light_fix;                                                              // Điều chỉnh lại thông thông số ánh sáng (từ nhở đến lớn)

// Khai bao thoi gian gui du lieu (1000ms = 1s)                              // Thoi gian delay lay du lieu
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 2000;

// Ham lay thoi gian tai thoi diem hien tai
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");                              // Báo lỗi khi không lấy được thời gian
    return(0);
  }
  time(&now);
  return now;
}

void setup(){
  Serial.begin(9600);
  delay(1000);

  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }
  Serial.println ("");
  Serial.println ("Da ket noi WiFi!");                      // Trạng thái đã kết nối wifi
  Serial.print (WIFI_SSID);                                 // Hiển thị tên wifi
  Serial.print (" | ");   
  Serial.print ("Dia chi IP: "); 
  Serial.println(WiFi.localIP());                           // Hien IP Wifi

  dht.begin();
  pinMode(SoundPIN, INPUT);                                 //Set chân PIN INPUT
  pinMode(LightPIN, INPUT);                                 //Set chân PIN INPUT

  configTime(0, 0, ntpServer);

  // Gán khoá API Firebase
  config.api_key = API_KEY;

  // Thông tin người dùng đăng nhập
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Gán địa chỉ Real Time DataBase
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Gán chức năng gọi lại token*/
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Gán số lần thử lại tối đa cho việc lấy token
  config.max_token_generation_retry = 5;

  // Khởi tạo thư viện với Firebase config và Firebase Auth
  Firebase.begin(&config, &auth);

  // In ra tên người dùng đang sử dụng
  Serial.print("User: ");
  Serial.println(USER_EMAIL);

  // Lấy và in ra User UID
  Serial.println("Dang lay User UID!");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);            
  }
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update đường dẫn cơ sở dữ liệu
  databasePath = "/UsersData/" + uid + "/readings";
}

void loop(){
    // Thoi gian delay gui du lieu len firebase (Dieu chinh tai TimerDelay)
    if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //Lấy dấu thời gian (thời gian được mã hoá)
    timestamp = getTime();
    Serial.print ("Thoi gian: ");
    Serial.println (timestamp);

    parentPath= databasePath + "/" + String(timestamp);

    h = dht.readHumidity();                               // Đọc độ ẩm
    t = dht.readTemperature();                            // Đọc nhiệt độ theo độ C
    Sound_val= analogRead(SoundPIN);                      // Đọc cường độ âm thanh
    Light_val= analogRead(LightPIN);                      // Đọc biến trở ánh sáng

    Light_fix= 4095 - Light_val;
    
    Serial.print("Nhiet do: ");
    Serial.print(t);
    Serial.print("*C | ");
    Serial.print("Do am: ");
    Serial.print(h);
    Serial.print("% | ");
    Serial.print("Am thanh: ");
    Serial.print(Sound_val);
    Serial.print(" | ");
    Serial.print("Anh sang: ");
    Serial.print(Light_fix);
    Serial.println(" ");
  
    json.set(tempPath.c_str(), String(t));
    json.set(humPath.c_str(), String(h));
    json.set(soundPath.c_str(), String(Sound_val));
    json.set(lightPath.c_str(), String(Light_fix));
    
    json.set(timePath, String(timestamp));
    
    Serial.printf("Gui du lieu len Firebase... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "Da gui!" : fbdo.errorReason().c_str());
  }
}

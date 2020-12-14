#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

/*******************WiFi连接信息************************/
#define ssid "aithinker"
#define password "aithinker888"

/*****************图片识别api的参数*********************/
#define api_key "uutPiTYKdP1jQb_tQ-6zguHSdn6E4WIb"
#define api_secret "UbBXZ-lSP1HM1FMgSuECE_zPXQgaalSV"
//HTTPS通讯的网站
const char *host = "api-cn.faceplusplus.com"; 
//HTTPS端口443
const int httpsPort = 443;

/*****************获取图片api的参数*********************/
const char*  get_url = "https://images.bemfa.com/cloud/v1/get/";
const char*  uid = "1823ec2d13e9d8c69b196d6822d8f17d";
const char*  topic = "esp32CAM";

String imageUrl = "";

/*******************WiFi初始化函数*********************/
bool init_wifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting"); Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
 
  return true;
}

void setup() {
  pinMode(4,OUTPUT);
  Serial.begin(9600);
  if(init_wifi()){
    Serial.println("WiFi connected");  
  }
}

long int tatoltime=0;

/*get图片url*/
string geturl()
{
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  http.begin(get_url+"?"+uid+"&"+topic);
  Serial.print("[HTTP] GET...\n");

  int httpResponseCode=http.GET();
  
  if(httpResponseCode==200){
    //获取get请求后的服务器响应信息
    String response =http.getString();
    Serial.print("Response Msg:");
    Serial.println(response);

    //json数据解析
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);
    const char* status = doc["status"]; 
    long code = doc["code"]; 
    const char* data_0_url = doc["data"][0]["url"]; 
    const char* data_0_time = doc["data"][0]["time"]; 
    Serial.println(data_0_url);

    http.end();

    return data_0_url;
  }  
  else{
    Serial.println("Get failed.")

    http.end();

    return "";
  }

}

/*物品识别*/
String httpsObject(){
  WiFiClientSecure httpsClient;

  Serial.println(host);

  //建立HTTPs连接，时好时坏，看脸
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);  
  delay(1000);
  Serial.println("HTTPS Connecting");Serial.println("");
  int r=0;
  while((!httpsClient.connect(objecthost, httpsPort)) && (r < 100)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r == 100) { 
    Serial.println("Connection failed");
    return;
  } else {
    Serial.println("Connected.");
  }
  Serial.print("requesting: ");
  Serial.println(host);

  // 拼接请求报文
  bool request =  String("POST /imagepp/beta/detectsceneandobject") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: keep-alive\r\n"+
                        "Content-Length: 545\r\n"+
                        "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n" +
                        "\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "Content-Disposition: form-data; name=\"image_url\"\r\n"+
                        "\r\n"+
                        imageUrl+"\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "Content-Disposition: form-data; name=\"api_key\"\r\n"+
                        "\r\n"+
                        api_key+"\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "Content-Disposition: form-data; name=\"api_secret\"\r\n"+
                        "\r\n"+
                        api_secret+"\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "\r\n";
  // 向服务器发送请求 
  httpsClient.print(request);
  Serial.println("request sent");
  // 检查服务器响应信息。
  if(httpsClient.connected()){
    Serial.println("Post success.")
  }
  else{
    Serial.println("Post failed.")
  }
 //解析响应信息
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
 //解析响应信息
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  const char*objects = doc[objects][0][value];
  httpsClient.stop();
  Serial.println("closing connection");

  if(objects == "books" || objects == "computer" || objects == "clothes" || objects == "bags"){
    return true;
  }
  else{
    return false;
  }
}

/*人体识别*/
bool httpsHuman(){
  WiFiClientSecure httpsClient;

  Serial.println(host);

  //建立HTTPs连接，时好时坏，看脸
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);  
  delay(1000);
  Serial.println("HTTPS Connecting");Serial.println("");
  int r=0;
  while((!httpsClient.connect(objecthost, httpsPort)) && (r < 100)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r == 100) { 
    Serial.println("Connection failed");
    return;
  } else {
    Serial.println("Connected.");
  }
  Serial.print("requesting: ");
  Serial.println(host);

  // 拼接请求报文
  bool request =  String("POST /humanbodypp/v1/detect") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: keep-alive\r\n"+
                        "Content-Length: 545\r\n"+
                        "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n" +
                        "\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "Content-Disposition: form-data; name=\"image_url\"\r\n"+
                        "\r\n"+
                        imageUrl+"\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "Content-Disposition: form-data; name=\"api_key\"\r\n"+
                        "\r\n"+
                        api_key+"\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "Content-Disposition: form-data; name=\"api_secret\"\r\n"+
                        "\r\n"+
                        api_secret+"\r\n"+
                        "----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"+
                        "\r\n";

  // 向服务器发送请求 
  httpsClient.print(request);
  Serial.println("request sent");

  // 检查服务器响应信息。
  if(httpsClient.connected()){
    Serial.println("Post success.")
  }
  else{
    Serial.println("Post failed.")
  }

 //解析响应信息
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  float confidence = doc["humanbodies"][2]["confidence"];
  httpsClient.stop();
  Serial.println("confidence = ",confidence)
  Serial.println("closing connection");

  //可信度大于70认为是人
  if(confidence >= 70){
    return true;
  }
  else{
    return false;
  }
}

long int time = 0;

void loop() {
  int passtime = millis()/1000;
  imageUrl = geturl();
  if(imageUrl){
    bool bookExist = httpsObject();
    bool humanExist = httpsPeople();
    if(humanExist && bookExist){
      time = time + 5;
    }
    else if(humanExist){
      time = 0;
    } 
    //控制亮灯 
    if(time >= 30*60*1000){
    digitalWrite(4,HIGH);
  }
  }
  delay(5*60*1000);
}
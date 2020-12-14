#include <HTTPClient.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Mine";           //WIFI名称
const char* password = "apecblue";     //WIFI密码
int capture_interval = 5*60*1000;        // 上传时间间隔，这里是5分钟一次
const char*  post_url = "http://images.bemfa.com/upload/v1/upimages.php"; 
const char*  uid = "1823ec2d13e9d8c69b196d6822d8f17d";   
const char*  topic = "esp32CAM"; 

long current_millis;
long last_capture_millis = 0;

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup()
{
  Serial.begin(115200);

  if (init_wifi()) { 

    Serial.println("Internet connected");
  }

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}


/********初始化WIFI*********/
bool init_wifi()
{
  int connAttempts = 0;
  Serial.println("\r\nConnecting to: " + String(ssid));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    if (connAttempts > 10) return false;
    connAttempts++;
  }
  return true;
}



/********推送图片*********/
static esp_err_t take_send_photo()
{
    //初始化相机并拍照
    Serial.println("Taking picture...");
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return ESP_FAIL;
    }
  
    HTTPClient http;
 
    http.begin(post_url);
    
    http.addHeader("Content-Type", "image/jpg");
    http.addHeader("Authorization", uid);
    http.addHeader("Authtopic", topic);
    
    int httpResponseCode = http.POST((uint8_t *)fb->buf, fb->len);
    
    if(httpResponseCode==200){
        //获取post请求后的服务器响应信息，json格式
        String response = http.getString();
        Serial.print("Response Msg:");
        Serial.println(response);

        //json数据解析
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, response);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
        }
        const char* url = doc["url"];
        Serial.print("Get URL:");
        Serial.println(url);//打印获取的URL
    
        
    }else{
        //错误请求
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }
   
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  
    //清空数据
    esp_camera_fb_return(fb);  

    http.end();
  
}

void loop()
{
  current_millis = millis();
  if (current_millis - last_capture_millis > capture_interval) { // 当前时间减去上次时间大于20S就执行拍照上传函数
    last_capture_millis = millis();
    take_send_photo(); //拍照上传函数
  }
}

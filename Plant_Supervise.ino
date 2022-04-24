#include <WiFi.h>
#include <WiFiClient.h>
#include <TridentTD_LineNotify.h>
#include <Wire.h>

// 修改成上述寄到登入郵箱的 Token權杖號碼
#define LINE_TOKEN "C8VXuQ3ubm2yV855mBjhb7fzw4m8ttwjqo7RmQ0q2i8"

// 設定無線基地台SSID跟密碼
const char* ssid     = "davidhome"; //"KSHS";
const char* password = "29960807"; //"KS=SERVE";

unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor
enum humidity_state 
{
  THIRSTY,
  JUST_FIT,
  TOO_MUCH,  
};
int state = 0;

int angry = 0;
int say = 0;

// Sensor
const int pinSensor = 34;
int nHumidity = 0;

char *g_strThristy[] = {"\n我需要水\U0001F975", 
                  "\n已經很久沒喝水了我感覺要死了！！\U00002620\U00002620\n救命啊～",
                  "\n我好渴喔\n原來這就是沒知覺的感覺嗎？",
                  "\n你忘記我了嗎？QQ\n那些回憶都是假的嗎？我好想你喔QQ",
                  "\n你還在嗎？\U0001F623\n我的葉子已經沒知覺了\U0001F622",
                  "\n你還愛我嗎？\U0001F97A\n我好渴喔\U0001F915",
                  "\n已經兩天以上沒澆水了\U0001F631\n已經兩天以上沒澆水了",
                  "\n君不見黃河之水天上來～～～我要渴死啦！\U0001F5FF\U0001F327",
                  "\n我要死了～口好渴～\U0001F525",
                  };
                  
char *g_strTooMuch[] = {"\n水太多了\U0001F616",
                  "\n謝謝你不過水好像有點太多了\U0001F62C\n好脹～\U0001F922",
                  "\n主人好關心我喔\U0001F92A\n只是試著調整一下水量有點太多了\U0000F92E\U0000F92E",
                  "\n水太多了啦～/U0001F92F",
                  "\n我要吐水出來了\U0001F922\n噁噁噁..\U0000F927\U0000F927",
                  "\n你還愛我嗎？\n我好渴喔",
                  };
                  
void setup(void)
{
  Serial.begin(115200);  // 設定速率 感測器


  WiFi.mode(WIFI_STA);
  // 連接無線基地台
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // 等待連線，並從 Console顯示 IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop(void)
{
  // 量測間等待至少 2 秒
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) 
  {
    // 將最後讀取感測值的時間紀錄下來 
    previousMillis = currentMillis;   
    // 顯示 Line版本
    Serial.println(LINE.getVersion());
 
    LINE.setToken(LINE_TOKEN);
    nHumidity = analogRead(pinSensor);
    randomSeed(nHumidity);
    Serial.println(nHumidity);
  
    if (nHumidity > 3800)
    {
      Serial.print("state = ");
      Serial.println(state);
      if (state != THIRSTY)
      {
        state = THIRSTY;
        LINE.notify(g_strThristy[0]);
        Serial.print(g_strThristy[0]);
        angry = 0;
      }
      else if (state == THIRSTY)
      {
          angry = angry + 1;
          Serial.print("Angry = ");
          Serial.println(angry);
          if (angry > 20)//在這裡設定～～～～～～～～～～～～～～～～～～～～～～～～～～～//在這裡設定～～～～～～～～～～～～～～～～～～～～～～～～～～～
          {
              
              say = random(1, 8);
              Serial.print("Say = ");
              Serial.println(say);
              if(say < sizeof(g_strThristy))
              {
                LINE.notify(g_strThristy[say]);
                Serial.print(g_strThristy[say]);
                angry = 0;
              }

           
          }
        } // else if (state == THIRSTY)
      }
      else if (nHumidity < 1800)
      {
        Serial.print("Say = ");
        Serial.println(say);
        if (state != TOO_MUCH)
        {
          state = TOO_MUCH;
          LINE.notify(g_strTooMuch[0]);
          Serial.println(g_strTooMuch[0]);
          angry = 0;
        }
        else if (state == TOO_MUCH)
        {
          angry = angry + 1;
          Serial.println(angry);
          
          if (angry > 20)//在這裡設定～～～～～～～～～～～～～～～～～～～～～～～～～～～//在這裡設定～～～～～～～～～～～～～～～～～～～～～～～～～～～
          {
            
            say = random(1, 5);
            Serial.print("Say = ");
            Serial.println(say);
            if(say < sizeof(g_strTooMuch))
            {
              LINE.notify(g_strTooMuch[say]);
              Serial.print(g_strTooMuch[say]);
              angry = 0;
            }
          }
      }
      else
      {
        if (state != JUST_FIT)
        {
          state = JUST_FIT;
          LINE.notify("\n這樣剛剛好\U0001F609");
          Serial.print("\n這樣剛剛好");
          angry = 0;
        }
        else if (state == JUST_FIT)
        {
          angry = angry ++;
          if (angry > 20)//在這裡設定～～～～～～～～～～～～～～～～～～～～～～～～～～～//在這裡設定～～～～～～～～～～～～～～～～～～～～～～～～～～～
          {
            angry = 0;
            say = random(1, 3);
            if (say == 1)
            {
              LINE.notify("\n主人對我好好喔謝謝你\U0001F61A");
              Serial.print("\n主人對我好好喔謝謝你");
              angry = 0;
            }
            else if (say == 2)
            {
              LINE.notify("\n今天要做什麼呢？/n今天心情好好");
              Serial.print("今天要做什麼呢？/n今天心情好好");
              angry = 0;
            }
            else if (say == 3)
            {
              LINE.notify("\n好想要快點長大");
              Serial.print("好想快點長大");
              angry = 0;
            }
          
          }
        }
      }
    }
  
  }
 delay(2000);
}

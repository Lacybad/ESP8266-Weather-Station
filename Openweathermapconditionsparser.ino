// simple Code for reading information from openweathermap.org 

  //#include <WiFi.h> //Library for ESP32
  #include <ESP8266WiFi.h> //Library for ESP8266
  #include <ArduinoJson.h>
  #include <Wire.h>
  #include <TFT_eSPI.h> // Hardware-specific library
  #include <SPI.h>
  #include <Adafruit_GFX.h>
  

  TFT_eSPI tft = TFT_eSPI();       // Invoke custom library


  #define TFT_GREY        0x5AEB
  #define TFT_BLACK       0x0000      
  #define TFT_NAVY        0x000F
  #define TFT_DARKGREEN   0x03E0      
  #define TFT_DARKCYAN    0x03EF      
  #define TFT_MAROON      0x7800      
  #define TFT_PURPLE      0x780F      
  #define TFT_OLIVE       0x7BE0      
  #define TFT_LIGHTGREY   0xC618      
  #define TFT_DARKGREY    0x7BEF      
  #define TFT_BLUE        0x001F  
  #define TFT_GREEN       0x07E0
  #define TFT_CYAN        0x07FF
  #define TFT_RED         0xF800
  #define TFT_MAGENTA     0xF81F      
  #define TFT_YELLOW      0xFFE0      
  #define TFT_WHITE       0xFFFF    
  #define TFT_ORANGE      0xFDA0      
  #define TFT_GREENYELLOW 0xB7E0  
  #define TFT_PINK        0xFC9F    

  // Don't change!!!
  #define TFT_DC  D3
  #define TFT_CS  D8
  #define TFT_MOSI D7
  #define TFT_SCK D5
  #define TFT_RST D4

  //For ESP32
  //#define TFT_MISO 19
  //#define TFT_MOSI 23
  //#define TFT_SCLK 18
  //#define TFT_CS    15  // Chip select control pin
  //#define TFT_DC    2  // Data Command control pin
  //#define TFT_RST   4  // Reset pin (could connect to RST pin)
  //#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is 

  /*#define ESP32_PARALLEL

  // The ESP32 and TFT the pins used for testing are:
  #define TFT_CS   33  // Chip select control pin (library pulls permanently low
  #define TFT_DC   15  // Data Command control pin - must use a pin in the range 0-31
  #define TFT_RST  32  // Reset pin, toggles on startup

  #define TFT_WR    4  // Write strobe control pin - must use a pin in the range 0-31
  #define TFT_RD    2  // Read strobe control pin

  #define TFT_D0   12  // Must use pins in the range 0-31 for the data bus
  #define TFT_D1   13  // so a single register write sets/clears all bits.
  #define TFT_D2   26  // Pins can be randomly assigned, this does not affect
  #define TFT_D3   25  // TFT screen update performance.
  #define TFT_D4   17
  #define TFT_D5   16
  #define TFT_D6   27
  #define TFT_D7   14*/




  const char* ssid     = "****************";                 // SSID of local network
  const char* password = "**********";                    // Password on network
  String APIKEY = "************************";                                 
  String CityID = "*********";                                 //Your City ID


  WiFiClient client;
  char servername[]="api.openweathermap.org";              // remote server we will connect to
  String result;

  int  counter = 10;                                      

  String weatherDescription ="";
  String weatherLocation = "";
  String Country;
  float Temperature;
  float Humidity;
  float Pressure;
  int Sunrise;
  int Sunset;
  float Speed;
  float Temp_min;
  float Temp_max;
  float Visibility;
  float Wind_angle;

const unsigned char wifiBitmap [] PROGMEM = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 31, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 128,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 15, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 254, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 127, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
 255, 255, 255, 255, 128, 0, 0, 0, 0, 0, 0, 0, 1, 255, 255, 255, 255, 192, 0, 0,
 0, 0, 0, 0, 0, 3, 255, 255, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 7,
 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 15, 255, 255, 255, 255, 248, 0, 0,
 0, 0, 0, 0, 0, 15, 255, 255, 255, 255, 252, 0, 0, 0, 0, 0, 0, 7, 255, 255,
 255, 255, 255, 255, 248, 0, 0, 0, 0, 0, 127, 255, 255, 255, 255, 255, 255, 255, 0, 0,
 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 192, 0, 0, 0, 3, 255, 255, 255, 255,
 255, 255, 255, 255, 224, 0, 0, 0, 7, 255, 255, 255, 255, 255, 128, 0, 1, 240, 0, 0, 
 0, 7, 255, 255, 255, 255, 255, 0, 0, 0, 120, 0, 0, 0, 15, 255, 255, 255, 255, 254,
 0, 0, 0, 60, 0, 0, 0, 31, 255, 255, 255, 255, 252, 0, 0, 0, 28, 0, 0, 0,
 31, 255, 255, 255, 255, 248, 0, 0, 0, 30, 0, 0, 0, 63, 255, 255, 255, 31, 240, 0,
 0, 30, 14, 0, 0, 0, 63, 7, 7, 6, 15, 240, 255, 255, 62, 14, 0, 0, 0, 63, 7,
 7, 6, 15, 224, 255, 255, 62, 7, 0, 0, 0, 127, 7, 7, 14, 15, 224, 255, 255, 62, 7, 0, 0, 0, 127, 135, 7, 15, 
 31, 224, 255, 255, 30, 7, 0, 0, 0, 127, 135, 7, 15, 255, 224, 255, 255, 0, 3, 0, 0, 0, 127,
 134, 6, 15, 255, 224, 252, 0, 30, 3, 0, 0, 0, 127, 130, 2, 14, 31, 224, 252, 0,
 62, 3, 0, 0, 0, 127, 130, 2, 30, 31, 224, 252, 0, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224,
 252, 0, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 194, 34, 30,
 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 194, 34, 30, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127,
 192, 32, 62, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 224, 32, 62, 31, 224, 255, 254, 62, 3, 0, 0,
 0, 127, 224, 96, 62, 31, 224, 255, 254, 62, 3, 0, 0, 0, 127, 224, 112, 62, 31, 224, 252, 0, 62, 3, 0, 0,
 0, 127, 224, 112, 62, 31, 224, 252, 0, 62, 3, 0, 0, 0, 127, 224, 112, 126, 31, 224,
 252, 0, 62, 3, 0, 0, 0, 127, 240, 112, 126, 31, 192, 252, 0, 62, 7, 0, 0, 0, 127, 240, 240, 126, 31, 192,
 252, 0, 62, 7, 0, 0, 0, 63, 255, 255, 255, 255, 192, 248, 0, 62, 7, 0, 0, 0, 63, 255, 255, 255, 255,
 192, 0, 0, 0, 14, 0, 0, 0, 63, 255, 255, 255, 255, 128, 0, 0, 0, 14, 0, 0, 0, 31, 255, 255, 255, 255,
 128, 0, 0, 0, 30, 0, 0, 0, 31, 255, 255, 255, 255, 0, 0, 0, 0, 28, 0, 0, 0, 15, 255, 255, 255,
 255, 0, 0, 0, 0, 56, 0, 0, 0, 7, 255, 255, 255, 254, 0, 0, 0, 0, 248, 0, 0, 0, 7, 255, 255, 255, 252, 
 0, 0, 0, 1, 240, 0, 0, 0, 1, 255, 255, 255, 255, 255, 255, 255, 255, 224, 0, 0, 0, 0, 255, 255, 255,
 255, 255, 255, 255, 255, 192, 0, 0, 0, 0, 63, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0,
 0, 0, 7, 255, 255, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 7, 255, 255, 255, 255, 248, 0, 0, 0,
 0, 0, 0, 0, 3, 255, 255, 255, 255, 240, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 255, 255, 224, 0, 0, 0, 0,
 0, 0, 0, 1, 255, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 128, 0, 0, 0, 0, 0,
 0, 0, 0, 63, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 255, 255, 254, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 7, 255, 255, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 255, 255, 224, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 127, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  void setup()
  {
  Serial.begin(115200);
  int cursorPosition=0;
  tft.init();
  tft.fillScreen(TFT_BLACK );
  tft.setRotation(0); //2
  tft.setTextWrap(0);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawBitmap(70, 120, wifiBitmap, 100, 70, TFT_WHITE);
  delay(5000);
  
  //tft.init();
  tft.fillScreen(TFT_BLACK );
  tft.drawRect(0,0,240,320,TFT_GREEN);
  tft.drawRect(1,1,238,318,TFT_GREEN);
  tft.drawLine(2,30,240,30, TFT_GREEN);
  tft.drawLine(2,70,240,70, TFT_GREEN);
  tft.drawLine(2,110,240,110, TFT_GREEN);
  tft.drawLine(2,153,240,153, TFT_GREEN);
  tft.drawLine(110,110,110,152, TFT_GREEN);//
  tft.drawLine(2,180,240,180, TFT_GREEN);
  tft.drawLine(2,207,240,207, TFT_GREEN);
  tft.drawLine(2,270,240,270, TFT_GREEN);

  

  
  tft.setCursor(10,10);
  tft.print("Connecting");  
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  
             while (WiFi.status() != WL_CONNECTED) 
            {
            {
            delay(1000);
            tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(180,10);
            tft.print(cursorPosition); 
            tft.print(".");
            cursorPosition++;
            }

             
  }
  
  tft.setCursor(10,10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("Connected to Wifi!");
  Serial.println("Connected to WIFI!");
  delay(1000);
    }

    void loop()
    
    {
    if(counter == 10)                                 //Get new data every 10 minutes
    {
       
      counter = 0;
      displayGettingData();
      delay(1000);
      getWeatherData();
      delay(1000);
    }else
    {
      counter++;
      displayWeather(weatherLocation,weatherDescription);
       delay(1000);
      displayConditions(Temperature,Humidity,Pressure);
      delay(1000);
      displayWindSpeed(Speed , Temp_min , Temp_max, Visibility, Wind_angle);
      delay(1000);
      
     
{
      tft.drawCircle(220,305,10,TFT_WHITE);
      tft.fillCircle(220,305,8,TFT_GREEN);
      delay(6000);
      tft.fillCircle(220,305,8,TFT_BLACK);

      long rssi = WiFi.RSSI();

      tft.setTextSize(1);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(160,295);
      tft.print (rssi);

      tft.setTextSize(1);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(180,295);
      tft.print ("dbm");
      //delay(3000);
      }
    }
    }

    void getWeatherData()                                //client function to send/receive GET request data.
   {
   if (client.connect(servername, 80))   
          {                                         //starts client connection, checks for connection
          client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
          client.println("Host: api.openweathermap.org");
          client.println("User-Agent: ArduinoWiFi/1.1");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         result = result+c;
       }

  client.stop();                                      //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);
  char jsonArray [result.length()+1];
  result.toCharArray(jsonArray,sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

  String location = root["name"];
  String country = root["sys"]["country"];
  float temperature = root["main"]["temp"];
  float humidity = root["main"]["humidity"];
  String weather = root["weather"]["main"];
  String description = root["weather"]["description"];
  float pressure = root["main"]["pressure"];
  float sunrise = root["sys"]["sunrise"];
  float sunset = root["sys"]["sunset"];
  float temp_min = root["main"]["temp_min"];
  float temp_max = root["main"]["temp_max"];
  float speed = root["wind"]["speed"];
  float visibility = root["visibility"];
  float wind_angle = root["wind"]["deg"];
  weatherDescription = description;
  weatherLocation = location;
  Country = country;
  Temperature = temperature;
  Humidity = humidity;
  Pressure = pressure;
  Sunrise = sunrise;
  Sunset = sunset;
  Speed = speed;
  Temp_min = temp_min;
  Temp_max = temp_max;
  Visibility = visibility;
  Wind_angle = wind_angle;
  

 
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10,276);
  tft.print("IP:");
  tft.print(WiFi.localIP());
  
  
  delay(1000);
 }

  void displayWeather(String location,String description)
 {
    tft.setTextSize(2);
  tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
  tft.setCursor(15,52);
  tft.print(location);
  tft.print(", ");
  tft.print(Country);
  tft.setCursor(10,93);
  tft.print(description);
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.setCursor(45,75);
  tft.print("Description");
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.setCursor(30,35);
  tft.print("Location:");

  tft.setTextSize(2);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.setCursor(150,35);
  tft.print(CityID);
  delay(1000);
 }

  void displayConditions(float Temperature,float Humidity, float Pressure)
 {
 
 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(30,115);//Printing Temperature
 tft.print("Temp"); 
 
 tft.setTextSize(2);
 tft.setTextColor(TFT_YELLOW, TFT_BLACK);
 tft.setCursor(20,133);//Printing Temperature
 tft.print(Temperature,1);
 tft.print("°C "); 

 tft.setCursor(120,115);  
 tft.setTextSize(2);
 tft.setTextColor(TFT_BLUE, TFT_BLACK);                                      
 tft.print("Humidity"); //Printing Humidity
 
 tft.setCursor(145,133);  
 tft.setTextSize(2);
 tft.setTextColor(TFT_BLUE, TFT_BLACK);
 tft.print(Humidity,1);
 tft.print(" %"); 

 tft.setTextSize(2);
 tft.setTextColor(TFT_CYAN, TFT_BLACK);
 tft.setCursor(10,160);                     //Printing Pressure
 tft.print("P: ");
 tft.print(Pressure,1);
 tft.print(" hPa");
 delay(1000);
 }

 void displayGettingData()
 {

  
  
  tft.setTextSize(1);
 tft.setCursor(10,295);
 tft.setTextColor(TFT_GREEN, TFT_BLACK); 
 tft.print(ssid);

 

 tft.setTextSize(1);
 tft.setCursor(10,305);
 tft.setTextColor(TFT_GREEN, TFT_BLACK); 
 tft.print(APIKEY);
 delay(1000);
 }

 void displayWindSpeed(float Speed , float Temp_min, float Temp_max , float Visibility , float Wind_angle)
 {


 tft.setTextSize(2);
 tft.setCursor(10,187);
 tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
 tft.print("Wind:");
 
 tft.setTextSize(2);
 tft.setCursor(95,187);
 tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
 tft.print(Speed,1);
 
 tft.setTextSize(2);
 tft.setCursor(160,187);
 tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
 tft.print("m/s");

 tft.setTextSize(2);
 tft.setCursor(10,210);
 tft.setTextColor(TFT_BLUE, TFT_BLACK); 
 tft.print("Tmin:");
 tft.print(Temp_min);
 tft.print("°C");

 tft.setTextSize(2);
 tft.setCursor(10,231);
 tft.setTextColor(TFT_RED, TFT_BLACK); 
 tft.print("Tmax:");
 tft.print(Temp_max);
 tft.print("°C");

 tft.setTextSize(2);
 tft.setCursor(10,252);
 tft.setTextColor(TFT_MAGENTA, TFT_BLACK); 
 tft.print("Vis:");
 tft.print(Visibility,0);
 tft.print(" meters");

 tft.setTextSize(2);
 tft.setCursor(165,210);
 tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
 tft.print("Angle");
 
 tft.setTextSize(2);
 tft.setCursor(170,231);
 tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
 tft.print(Wind_angle,0);
 tft.print("°");
 
 delay(1000);
 }

      

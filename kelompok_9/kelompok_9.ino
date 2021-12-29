//Memasukkan library yang digunakan
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Adafruit_MLX90614.h>
#define trig 12 
#define echo 13
#define buzzer 18
#define ledmerah 26
#define ledhijau 27

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Memasukkan SSID dan Password pada wifi yang digunakan agar dapat mengirim data ke telegram
const char* ssid= "Gratis"; //nama wifi
const char* password = "12345678910"; //password wifi

//Memasukkan Bot token dan Id Bot dari bot Telegram yang dibuat
#define BOTtoken "2144711229:AAFZXJS5p02jiwTXoXPs9hUOiaLo7txCvAs" //token bot telegram
#define idChat "1294087766" //idbot
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup(){
  //melakukan setup pada sensor dan aktuator yang digunakan
  Serial.begin(9600);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(ledmerah, OUTPUT);
  pinMode(ledhijau, OUTPUT);
  pinMode(buzzer,OUTPUT);
  mlx.begin();
  //menghubungkan ke wifi
  Serial.print("\nConnecting Wifi: ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(50000);
}

void loop() {
  //Mengukur jarak dari wadah minuman untuk mengetahui ada atau tidaknya minuman yang diukur suhunya
  digitalWrite(trig,LOW);
  delay(10);
  digitalWrite(trig,HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  long durasi;
  float jarak;

  durasi = pulseIn(echo,HIGH);
  jarak = durasi * 0.034/2;
  //Menampilkan jarak dan suhu dari minuman
  Serial.print("\nJarak : ");
  Serial.print(jarak);
  Serial.println(" cm");
  Serial.print("Suhu saat ini : ");
  Serial.print(mlx.readObjectTempC());
  Serial.println(" C");
  //memberikan notifikasi suhu pada telegram
  Serial.println("Mengirim data sensor ke telegram");
  bot.sendMessage(idChat,"Suhu Minuman anda " + String (mlx.readObjectTempC()) + " C");
  if (jarak<4){
    //Ketika jarak<4 dan suhu yang terbaca <= 50 maka akan membunyikan buzzer dan menyalakan lampu led berwarna hijau
    if (mlx.readObjectTempC()<=50){
      bot.sendMessage(idChat, "Minuman anda siap diminum!!!!\n", "");
      digitalWrite(buzzer,HIGH);
      digitalWrite(ledmerah, LOW);
      digitalWrite(ledhijau, HIGH);
    }
    //Ketika jarak<4 dan suhu yang terbaca > 50 maka lampu led merah menyala
    else{
      digitalWrite(buzzer,LOW);
      digitalWrite(ledhijau, LOW);
      digitalWrite(ledmerah, HIGH);
    }
  }
  //Ketika jarak>4 maka buzzer mati dan lampu led tidak ada yang menyala 
  else{
     digitalWrite(ledhijau, LOW);
     digitalWrite(ledmerah, LOW);
     digitalWrite(buzzer,LOW);
  }
  delay(5000);
} 

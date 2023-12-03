#include <SoftwareSerial.h>

// Motor Sürücüleri
int motorA1 = 3;
int motorA2 = 4;
int motorB1 = 5;
int motorB2 = 6;

// Kodlayıcı
int encoderA = 2;
int encoderB = 7;

// İlk Pozisyonu
int posA = 0;
int posB = 0;

// Kodlayıcıya Ait Hareket Tipi
volatile char aDirection = '0';
volatile char bDirection = '0';

// Aracın Yolu
int aracYolu[100];
int yolSayaci = 0;

void setup() {
 // Seri Monitörü Başlat
 Serial.begin(9600);

 // Motor Sürücüleri
 pinMode(motorA1, OUTPUT);
 pinMode(motorA2, OUTPUT);
 pinMode(motorB1, OUTPUT);
 pinMode(motorB2, OUTPUT);

 // Kodlayıcı Pinleri
 pinMode(encoderA, INPUT);
 pinMode(encoderB, INPUT);

 // Kodlayıcı Pinleri için Hareket Algılayıcılarını Ayarla
 attachInterrupt(digitalPinToInterrupt(encoderA), aEncoderInterrupt, CHANGE);
 attachInterrupt(digitalPinToInterrupt(encoderB), bEncoderInterrupt, CHANGE);
}

void loop() {
 // Kodlayıcıya Ait Hareketleri Seri Monitörde Görüntüle
 Serial.print("Hareket A: ");
 Serial.print(aDirection);
 Serial.print(" Hareket B: ");
 Serial.println(bDirection);

 // Kodlayıcıların Dönme Miktarını Ölç ve Yolu Kaydet
 int adonme = 0;
 int bdonme = 0;
 if (aDirection == 'F') {
    adonme = posA;
 } else if (aDirection == 'B') {
    adonme = -posA;
 }
 if (bDirection == 'F') {
    bdonme = posB;
 } else if (bDirection == 'B') {
    bdonme = -posB;
 }
 aracYolu[yolSayaci] = adonme;
 yolSayaci++;
 aracYolu[yolSayaci] = bdonme;
 yolSayaci++;

 // Hareketi Sonlandır ve Aracı Durdur
 motorHareketiDurdur();

 // Sonraki Hareket İçin Bekle
 delay(1000);
}

void aEncoderInterrupt() {
 // Kodlayıcı A Pinini Okur ve Hareketi Algılar
 int aState = digitalRead(encoderA);
 if (aState == LOW) {
    aDirection = digitalRead(encoderB) == LOW ? 'F' : 'B';
    posA++;
 } else {
    aDirection = digitalRead(encoderB) == LOW ? 'B' : 'F';
    posA--;
 }
}

void bEncoderInterrupt() {
 // Kodlayıcı B Pinini Okur ve Hareketi Algılar
 int bState = digitalRead(encoderB);
 if (bState == LOW) {
    bDirection = digitalRead(encoderA) == LOW ? 'F' : 'B';
    posB++;
 } else {
    bDirection = digitalRead(encoderA) == LOW ? 'B' : 'F';
    posB--;
 }
}

void motorHareketiDurdur() {
 // Motor Sürücülerini Kapatır ve Hareketi Sonlandırır
 digitalWrite(motorA1, LOW);
 digitalWrite(motorA2, LOW);
 digitalWrite(motorB1, LOW);
 digitalWrite(motorB2, LOW);
}
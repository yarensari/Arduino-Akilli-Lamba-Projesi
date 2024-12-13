#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define ldrSensor A0
#define sesSensor 5
#define role      4

const byte satir = 4;
const byte sutun = 3;

char tuslar[satir][sutun] {
  {'1', '2', '3',},
  {'4', '5', '6',},
  {'7', '8', '9',},
  {'*', '0', '#',},
};

byte satirPin[satir] = {9, 8, 7, 6};
byte sutunPin[sutun] = {10, 11, 12};

char gelenKarakter;
const byte sifreBoy = 4;
char gelenSifre[sifreBoy];
const char belirlenenSifre[sifreBoy] = "1999";
byte sayac = 0;
int kontrol = -1;
byte kaliciSifreDeger = 1;
float ldrDeger;
byte sesDeger;
bool sesDurum = true;

void kullaniciGiris(void);
int sifreKontrol(void);

Keypad tusTakimi(makeKeymap(tuslar), satirPin, sutunPin, satir, sutun);
LiquidCrystal_I2C lcd(0x20, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  pinMode(ldrSensor, INPUT);
  pinMode(sesSensor, INPUT);
  pinMode(role, OUTPUT);

  digitalWrite(role, HIGH);

  Serial.println("Şifre giriniz ");
  lcd.setCursor(0, 0);
  lcd.print("Sifre giriniz    ");
  
}

void loop() {
  // kullanıcının girdiği 4 haneli şifre burada kaydedilir.
  kullaniciGiris();

  kontrol = sifreKontrol();

  if (kontrol == 1) {
    Serial.println("\nŞifre doğru");
    lcd.setCursor(0, 0);
    lcd.print("Sifre dogru      ");
    lcd.setCursor(0, 1);
    lcd.print("                 ");
    
    while (kaliciSifreDeger) {
      ldrDeger = analogRead(ldrSensor);
      Serial.println(ldrDeger );
      if (ldrDeger < 975) {
        sesDeger = digitalRead(sesSensor);
        if (sesDeger == LOW) {
          delay(20);
          Serial.println("!SES VAR!");
          lcd.setCursor(0, 0);
          lcd.print("    !SES VAR!     ");
          if (sesDurum == true) {
            Serial.println("!!!!!RÖLE AKTİF!!!!!");
            digitalWrite(role, LOW); // röle aktif
            lcd.setCursor(0, 1);
            lcd.print("  !ROLE AKTIF!   ");
            delay(20);
            sesDurum = false; 
          } else if (sesDurum == false) {
            Serial.println("!RÖLE PASİF!");
            digitalWrite(role, HIGH); // röle aktif değil
            lcd.setCursor(0, 1);
            lcd.print("  !ROLE PASIF!   ");
            delay(20);
            sesDurum = true;
          }
        }
      }
      else
      {
        lcd.setCursor(0, 0);
        lcd.print("Ortam aydinlik   ");
      }
    }
  } else if (kontrol == 0) {
    Serial.println("\nGirilen şifre doğru değil.");
    Serial.println("Tekrar deneyiniz.");
    lcd.setCursor(0, 0);
    lcd.print("Sifre yanlis     ");
    lcd.setCursor(0, 1);
    lcd.print("Tekrar deneyiniz ");
    delay(700);
    lcd.setCursor(0, 0);
    lcd.print("Sifre giriniz    ");
    lcd.setCursor(0, 1);
    lcd.print("                 ");
  }
}

void kullaniciGiris(void) {
  gelenKarakter = tusTakimi.getKey();
  if(gelenKarakter)
  {  
    if(gelenKarakter == '#')
    {
      if(sayac < sifreBoy)
      {
        Serial.println("\nEksik karakter");
        Serial.println("Tekrar deneyiniz");
        lcd.setCursor(0, 0);
        lcd.print("Eksik karakter   ");
        lcd.setCursor(0, 1);
        lcd.print("Tekrar deneyiniz ");
        delay(700);
        lcd.setCursor(0, 0);
        lcd.print("Sifre giriniz    ");
        lcd.setCursor(0, 1);
        lcd.print("                 ");
        sayac = 0;
      }
      else if(sayac > sifreBoy)
      {
        Serial.println("\nFazla karakter");
        Serial.println("Tekrar deneyiniz");
        lcd.setCursor(0, 0);
        lcd.print("Fazla karakter   ");
        lcd.setCursor(0, 1);
        lcd.print("Tekrar deneyiniz ");
        delay(700);
        lcd.setCursor(0, 0);
        lcd.print("Sifre giriniz    ");
        lcd.setCursor(0, 1);
        lcd.print("                 ");
        sayac = 0;
      }
    }
    else
    {
      gelenSifre[sayac] = gelenKarakter;
      Serial.print(gelenSifre[sayac]);
      sayac++;
      for(int i = 0; i < sayac; i++)
      {
        lcd.setCursor(i, 1);
        lcd.print(gelenSifre[i]);
      }
    }
  }
}

int sifreKontrol(void) {
  if(sifreBoy == sayac && gelenKarakter == '#')
  {
    for (int i = 0; i < sifreBoy; i++) 
    {
      if (gelenSifre[i] != belirlenenSifre[i]) {
        sayac = 0;
        return 0;
      }
    }
    return 1;
  }
  return -1;
}

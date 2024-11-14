#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin = 2; 
const int buzzerPin = 9; 
int score = 0; 
int jumpHeight = 0;  
bool isJumping = false; 

// Pola karakter pesawat
byte pesawat[8] = {
  0b00100,
  0b00111,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b01010,
};

// Pola karakter "<"
byte awan[8] = {
  0b00000,
  0b00010,
  0b00100,
  0b01000,
  0b00100,
  0b00010,
  0b00000,
  0b00000,
};

bool gameOver = false;  

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(buzzerPin, OUTPUT);      
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, pesawat);    // Buat karakter pesawat
  lcd.createChar(1, awan);       // Buat karakter "<"

  Serial.begin(9600); 
}

void loop() {
  // Cek apakah permainan berakhir
  if (gameOver) {
    lcd.clear();
    
    // Animasi untuk tampilan "Try again" di tengah
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(2, 0);    
      lcd.print("^^Try Again^^");
      delay(300);
      lcd.clear();
      delay(300);
    }
    
    lcd.setCursor(3, 0);      
    lcd.print(">> Loser <<");  
    
    lcd.setCursor(5, 1);      
    lcd.print(" Score: ");
    lcd.print(score);

    tone(buzzerPin, 1000, 500); 

    while (digitalRead(buttonPin) == HIGH) {
      // Menunggu tombol ditekan
    }

    delay(500);
    
    score = 0;         
    gameOver = false;
    jumpHeight = 0;    
    delay(500);        
  } else {
    // loop awan bergerak
    for (int position = 15; position >= 0; position--) {
      lcd.clear(); 

      // Cek jika tombol ditekan untuk membuat pesawat melompat
      if (digitalRead(buttonPin) == LOW && !isJumping) {
        isJumping = true; 
        tone(buzzerPin, 1000, 200); 
      }

      // Logika lompat max 3
      if (isJumping) {
        if (jumpHeight < 3) {
          lcd.setCursor(3, 0);  
          lcd.write((byte)0);   // Tampilkan pesawat di baris pertama saat melompat
          jumpHeight++;    
        } else {
          lcd.setCursor(3, 1);   
          lcd.write((byte)0);    
          jumpHeight = 0;        
          isJumping = false;     
          noTone(buzzerPin);     
        }
      } else {
        lcd.setCursor(3, 1);   
        lcd.write((byte)0);    
      }

      if(position == 4) {
        score += 1;
      }

      lcd.setCursor(13, 0);         
      if(score < 10) {
        lcd.print("  ");  
      } else if (score < 100) {
        lcd.print(" ");   
      }
      lcd.print(score);  

      lcd.setCursor(position, 1);    
      lcd.write((byte)1);            

      if (position == 3 && !isJumping) {
        gameOver = true;
        break; 
      }

      delay(200); 
    }
  }
}

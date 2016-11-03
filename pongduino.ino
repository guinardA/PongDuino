    /*
    This Code has extra features
    including a XY positioning function on Display
    and a Line Draw function on Nokia 3310 LCD
    It is modded from the original
    http://playground.arduino.cc/Code/PCD8544
    */
 
   #include "TrueRandom.h" 
 
    #define PIN_SCE   7  // LCD CS  .... Pin 3
    #define PIN_RESET 6  // LCD RST .... Pin 1
    #define PIN_DC    5  // LCD Dat/Com. Pin 5
    #define PIN_SDIN  4  // LCD SPIDat . Pin 6
    #define PIN_SCLK  3  // LCD SPIClk . Pin 4
                         // LCD Gnd .... Pin 2
                         // LCD Vcc .... Pin 8
                         // LCD Vlcd ... Pin 7

    #define LCD_C     LOW
    #define LCD_D     HIGH

    #define LCD_X     84
    #define LCD_Y     48
    #define LCD_CMD   0

    //---Global parameters
    int fps = 30;
    boolean difficulty = true;
    boolean sound = true;
    boolean alive = true;

    //---Ball parameters
    int posXBall = 1;
    int posYBall;
    byte byteBall;
    byte ballPositions[] = {0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0};
    boolean leftToRight = true;
    boolean topToBottom = true;
    
    //---Vector movement
    int vectorX;
    int vectorY;
    int cptX = 0;
    int cptY = 0;
    
    //---Joystick parameters
    int yPin = A1;
    int xPin = A0;
    int buttonPin = 8;
    int buttonState = 0;

    //---Racket parameters    
    byte racketByte1 = 0xF0;
    byte racketByte2 = 0x0F;
    int posY1Racket = 2;
    int posY2Racket = 3;
    
    //Buzzer
    int buzerPin = 12;
    
    static const byte ASCII[][5] =
    {
     {0x00, 0x00, 0x00, 0x00, 0x00} // 20 
    ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
    ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
    ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
    ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
    ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
    ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
    ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
    ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
    ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
    ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
    ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
    ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
    ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
    ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
    ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
    ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
    ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
    ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
    ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
    ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
    ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
    ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
    ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
    ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
    ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
    ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
    ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
    ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
    ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
    ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
    ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
    ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
    ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
    ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
    ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
    ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
    ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
    ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
    ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
    ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
    ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
    ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
    ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
    ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
    ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
    ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
    ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
    ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
    ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
    ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
    ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
    ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
    ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
    ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
    ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
    ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
    ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
    ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
    ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
    ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
    ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
    ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
    ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
    ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
    ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
    ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
    ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
    ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
    ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
    ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
    ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
    ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
    ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
    ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
    ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
    ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
    ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
    ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
    ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
    ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
    ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
    ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
    ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
    ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
    ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
    ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
    ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
    ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
    ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
    ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
    ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
    ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
    ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
    ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
    ,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f →
    };

    //Write a charcter on lcd screen
    void LcdCharacter(char character)
    {
      LcdWrite(LCD_D, 0x00);
      for (int index = 0; index < 5; index++)
      {
        LcdWrite(LCD_D, ASCII[character - 0x20][index]);
      }
      LcdWrite(LCD_D, 0x00);
    }

    //Clear lcd screen
    void LcdClear(void)
    {
      for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
      {
        LcdWrite(LCD_D, 0x00);
      }
    }

    //Game initialisation
    void LcdInitialise(void)
    {
      pinMode(PIN_SCE,   OUTPUT);
      pinMode(PIN_RESET, OUTPUT);
      pinMode(PIN_DC,    OUTPUT);
      pinMode(PIN_SDIN,  OUTPUT);
      pinMode(PIN_SCLK,  OUTPUT);

      digitalWrite(PIN_RESET, LOW);
     // delay(1);
      digitalWrite(PIN_RESET, HIGH);

      LcdWrite( LCD_CMD, 0x21 );  // LCD Extended Commands.
      LcdWrite( LCD_CMD, 0xBf );  // Set LCD Vop (Contrast). //B1
      LcdWrite( LCD_CMD, 0x04 );  // Set Temp coefficent. //0x04
      LcdWrite( LCD_CMD, 0x14 );  // LCD bias mode 1:48. //0x13
      LcdWrite( LCD_CMD, 0x0C );  // LCD in normal mode. 0x0d for inverse
      LcdWrite(LCD_C, 0x20);
      LcdWrite(LCD_C, 0x0C);
    }

    //Write a string on lcd screen
    void LcdString(char *characters)
    {
      while (*characters)
      {
        LcdCharacter(*characters++);
      }
    }

    void LcdWrite(byte dc, byte data)
    {
      digitalWrite(PIN_DC, dc);
      digitalWrite(PIN_SCE, LOW);
      shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
      digitalWrite(PIN_SCE, HIGH);
    }

    // gotoXY routine to position cursor
    // x - range: 0 to 84
    // y - range: 0 to 5
    void gotoXY(int x, int y)
    {
      LcdWrite( 0, 0x80 | x);  // Column.
      LcdWrite( 0, 0x40 | y);  // Row. 

    }

    void setup(void)
    {

      Serial.begin(9600);
      
      LcdInitialise();
      LcdClear();
      
      gotoXY(11,2);
      LcdCharacter('p');
      LcdCharacter('o');
      LcdCharacter('n');
      LcdCharacter('g');
      LcdCharacter('d');
      LcdCharacter('u');
      LcdCharacter('i');
      LcdCharacter('n');
      LcdCharacter('o');
      delay(1000);
      LcdClear();
      
      //---Initialise Ball
      posYBall = TrueRandom.random(0,6);
      byteBall = ballPositions[TrueRandom.random(0,7)];
      
      gotoXY(posXBall,posYBall);
      LcdWrite(1,byteBall);
      
      //---Initialise Vector
      vectorX = TrueRandom.random(1,4);
      vectorY = TrueRandom.random(1,4);
      
      
      //---Initialise racket
      pinMode(xPin, INPUT);
      pinMode(yPin, INPUT);
      
      gotoXY(0,posY1Racket);
      LcdWrite(1,racketByte1);
      
      gotoXY(0,posY2Racket);
      LcdWrite(1,racketByte2);
      
      //---Initialise buzzer
      pinMode(buzerPin, OUTPUT);

    }

    void loop(void)
    {
      
      if(alive){
      initialiseBallPosition();
      
      initialiseRacketlPosition();
      
      delay(fps);
      }
      else{
        LcdClear();
        gotoXY(20,2);
        LcdCharacter('P');
        LcdCharacter('E');
        LcdCharacter('R');
        LcdCharacter('D');
        LcdCharacter('U');
        LcdCharacter('!');
        delay(700);
        LcdClear();
        delay(700);
      }

    }
    
    //Clean screen on position x and y
    void cleanPosition(int x, int y){
      gotoXY(x,y);
      LcdWrite(1,0x00);
    }
    
    //Ball initialisation
    void initialiseBallPosition(){
      
      //Clear old ball position
      cleanPosition(posXBall,posYBall);
      cleanPosition(posXBall+1,posYBall);
      
      //Calculate new ball position
      calculateXPosition();
      calculateYPosition();
      
      //Write ball position on the screen
      gotoXY(posXBall,posYBall);
      LcdWrite(1,byteBall);
      gotoXY(posXBall+1,posYBall);
      LcdWrite(1,byteBall);
      
      if(cptX >= vectorX && cptY >= vectorY){
       cptX = 0;
       cptY = 0; 
      }
    }
    
    //Move ball on axis X
    void calculateXPosition(){
      
      if(cptX < vectorX){
        
         cptX = cptX + 1;
         
         if(leftToRight){
           posXBall = posXBall+1;
         }
         else{
           posXBall = posXBall-1;
         }
         
         //If the ball touch left wall, change the vector movement
         if(posXBall == 0 && !leftToRight){
           leftToRight = true;
           vectorX = TrueRandom.random(1,4);
           vectorY = TrueRandom.random(1,4);
           touchBall();
         }
         
         //If the ball touch right wall, change the vector movement
         if(posXBall+1 == LCD_X && leftToRight){
           leftToRight = false;
           vectorX = TrueRandom.random(1,4);
           vectorY = TrueRandom.random(1,4);
          
           //fps = fps -2;
         }         
       }
     }
      
     //Move ball on axis Y
     void calculateYPosition(){
        
        if(cptY < vectorY){
          
          cptY = cptY + 1;
          
          switch(byteBall){
            
            case 0x03:
               if(posYBall != 0){
                if(topToBottom){
                  byteBall = 0x06;
                }
                else{
                 byteBall = 0xC0;
                 posYBall = posYBall-1;
                }
              }
              else{
                byteBall = 0x06;
                topToBottom = true;
              }   
              break;
              
            case 0x06:
              if(topToBottom){
                byteBall = 0x0C;
              }
              else{
               byteBall = 0x03;
              }
              break;
              
            case 0x0C:
              if(topToBottom){
                byteBall = 0x18;
              }
              else{
               byteBall = 0x06;
              }
              break;
              
            case 0x018:
              if(topToBottom){
                byteBall = 0x30;
              }
              else{
               byteBall = 0x0C;
              }
              break;
              
            case 0x30:
              if(topToBottom){
                byteBall = 0x60;
              }
              else{
               byteBall = 0x18;
              }
              break;
              
            case 0x60:
              if(topToBottom){
                byteBall = 0xC0;
              }
              else{
               byteBall = 0x30;
              }
              break;
              
            case 0xC0:
              if(posYBall != 5){
                if(topToBottom){
                  byteBall = 0x03;
                  posYBall = posYBall+1;
                }
                else{
                 byteBall = 0x60;
                }
              }
              else{
                byteBall = 0x60;
                topToBottom = false;
              }
            }
        }
      }
      
      //Initialise racket position
      void initialiseRacketlPosition(){
      
      //Clear old racket position
      cleanPosition(0, posY1Racket);
      cleanPosition(0, posY2Racket);
      
      //Calculate new racket position
      calculateRacketPosition(analogRead(yPin));
      
      //Write racket position on the screen
      gotoXY(0,posY1Racket);
      LcdWrite(1,racketByte1);
      
      gotoXY(0,posY2Racket);
      LcdWrite(1,racketByte2);

    }
      
      //Calculate new position of racket
      void calculateRacketPosition(int pos){
        
        int racketTopToBottom = 0;
        
        //If the direction joystick is on top
        if(pos >= 0 && pos < 300){
          racketTopToBottom = 1;
        }
        //If the direction jostick is on bottom
        else if(pos > 800 && pos <= 1024){
          racketTopToBottom = 2;
        }
        
         switch(racketByte1){
           
           case 0xFF:
             if(racketTopToBottom == 1){
                racketByte1 = 0xFE;
                racketByte2 = 0x01;
             }
             else if(racketTopToBottom == 2){
               if(posY1Racket!= 0){
                 posY2Racket = posY1Racket;
                 posY1Racket = posY1Racket-1;
                 racketByte2 = 0x7F;
                 racketByte1 = 0x80;
               }
             }
            break;
            
            case 0xFE:
              if(racketTopToBottom == 1){
                racketByte1 = 0xFC;
                racketByte2 = 0x03;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xFF;
                racketByte2 = 0x00;
             }
            break;
            
            case 0xFC:
              if(racketTopToBottom == 1){
                racketByte1 = 0xF8;
                racketByte2 = 0x07;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xFE;
                racketByte2 = 0x01;
             }
            break;
            
            case 0xF8:
              if(racketTopToBottom == 1){
                racketByte1 = 0xF0;
                racketByte2 = 0x0F;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xFC;
                racketByte2 = 0x03;
             }
            break;
            
            case 0xF0:
              if(racketTopToBottom == 1){
                racketByte1 = 0xE0;
                racketByte2 = 0x1F;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xF8;
                racketByte2 = 0x07;
             }
            break;
            
            case 0xE0:
              if(racketTopToBottom == 1){
                racketByte1 = 0xC0;
                racketByte2 = 0x3F;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xF0;
                racketByte2 = 0x0F;
             }
            break;
            
            case 0xC0:
              if(racketTopToBottom == 1){
                racketByte1 = 0x80;
                racketByte2 = 0x7F;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xE0;
                racketByte2 = 0x1F;
             }
            break;
            
            case 0x80:
              if(racketTopToBottom == 1){
                racketByte1 = 0x00;
                racketByte2 = 0xFF;
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0xC0;
                racketByte2 = 0x3F;
             }
            break;
            
            case 0x00:
              if(racketTopToBottom == 1){
                if(posY2Racket != 5){
                 posY1Racket = posY2Racket;
                 posY2Racket = posY2Racket+1;
                 racketByte1 = 0xFE;
                 racketByte2 = 0x01;
               }
             }
             else if(racketTopToBottom == 2){
                racketByte1 = 0x80;
                racketByte2 = 0x7F;
             }
            break;   
         }
      }

    //Event touch ball on the racket
    void touchBall(){
     
     boolean racketPosition = false;
     Serial.println("Position : ");
     Serial.println(posYBall);
     Serial.println(posY1Racket);
      Serial.println(posY2Racket);
     
     if(posYBall == posY1Racket){
      
       switch(byteBall){
         
         case 0x03:
           if(racketByte1 == 0xFF || racketByte1 == 0xFE){
            Serial.println("Tap");
           touch(); 
           }
           else{
             Serial.println("Miss"); 
             miss();
           }
         break;
         
         case 0x06:
            if(racketByte1 == 0xFF || racketByte1 == 0xFE || racketByte1 == 0xFC){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss");
            miss(); 
           }
         break;
         
         case 0x0C:
           if(racketByte1 == 0xFF || racketByte1 == 0xFE || racketByte1 == 0xFC || racketByte1 == 0xF8){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           }
         break;
         
         case 0x18:
            if(racketByte1 == 0xFF || racketByte1 == 0xFE || racketByte1 == 0xFC || racketByte1 == 0xF8 || racketByte1 == 0xF0){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           }
         break;
         
         case 0x30:
            if(racketByte1 == 0xFF || racketByte1 == 0xFE || racketByte1 == 0xFC || racketByte1 == 0xF8 || racketByte1 == 0xF0 || racketByte1 == 0xE0){
            Serial.println("Tap");
           touch(); 
           }
           else{
             Serial.println("Miss"); 
             miss();
           }
         break;
         
         case 0x60:
            if(racketByte1 == 0xFF || racketByte1 == 0xFE || racketByte1 == 0xFC || racketByte1 == 0xF8 || racketByte1 == 0xF0 || racketByte1 == 0xE0 || racketByte1 == 0xC0){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           }   
         break;
         
         case 0xC0:  
         if(racketByte1 == 0xFF || racketByte1 == 0xFE || racketByte1 == 0xFC || racketByte1 == 0xF8 || racketByte1 == 0xF0 || racketByte1 == 0xE0 || racketByte1 == 0xC0  || racketByte1 == 0x80){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss");
            miss(); 
           }      
         break;
       }
     }
     
     else if(posYBall == posY2Racket){
      
       switch(byteBall){
         
         case 0x03:
            if(racketByte2 == 0xFF || racketByte2 == 0x7F || racketByte2 == 0x3F || racketByte2 == 0x1F || racketByte2 == 0x0F  || racketByte2 == 0x07 || racketByte2 == 0x03  || racketByte2 == 0x01){
            Serial.println("Tap");
           touch(); 
           }
           else{
             Serial.println("Miss"); 
             miss();
           }
         break;
         
         case 0x06:
           if(racketByte2 == 0xFF || racketByte2 == 0x7F || racketByte2 == 0x3F || racketByte2 == 0x1F || racketByte2 == 0x0F  || racketByte2 == 0x07 || racketByte2 == 0x03){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           }  
         break;
         
         case 0x0C:
            if(racketByte2 == 0xFF || racketByte2 == 0x7F || racketByte2 == 0x3F || racketByte2 == 0x1F || racketByte2 == 0x0F  || racketByte2 == 0x07){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           } 
         break;
         
         case 0x18:
            if(racketByte2 == 0xFF || racketByte2 == 0x7F || racketByte2 == 0x3F || racketByte2 == 0x1F || racketByte2 == 0x0F){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss");
            miss(); 
           }
         break;
         
         case 0x30:
           if(racketByte2 == 0xFF || racketByte2 == 0x7F || racketByte2 == 0x3F || racketByte2 == 0x1F){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           }   
         break;
         
         case 0x60:
           if(racketByte2 == 0xFF || racketByte2 == 0x7F || racketByte2 == 0x3F){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss");
            miss(); 
           } 
         break;
         
         case 0xC0:
           if(racketByte2 == 0xFF || racketByte2 == 0x7F){
            Serial.println("Tap"); 
            touch();
           }
           else{
             Serial.println("Miss"); 
             miss();
           }  
         break;
       }
     }
     else{
       Serial.println("Outside");
       miss();
     }
      
 }
   
  //Action ball touch racket 
  //more speed and buzzer on
  void touch(){
    if(sound){
      tone(buzerPin, 3322, 50);
    }
    if(fps >= 10 && difficulty){
       fps = fps - 5; 
      }
  }

  //Action ball miss racket
  //Buzzer on
  void miss(){
    if(sound){
      tone(buzerPin, 1397, 150);
    }
    alive = false;
  }

 

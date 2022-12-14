//使用旋转编码器和 LCD 上的显示角度控制伺服电机

const int SW_PIN = 4;//定义旋转编码开关的引脚
const int PIN_A =2;
const int PIN_B =3;

#include <Encoder.h> /*******************************/

// 将这两个数字更改为连接到编码器的引脚。
// 最佳性能：两个引脚都有中断能力
// 良好的性能：只有第一个引脚具有中断能力
// 低性能：两个引脚都没有中断能力
Encoder myEnc(PIN_A, PIN_B);
//  

const int homePosition = 0; //初始位置
const int stepValue = 3;//转动旋钮时舵机的旋转速度
const int servoPin = 9;//~必须是标有~的引脚舵机引脚
#include <Servo.h> /*******************************/

Servo myservo;  //创建伺服对象来控制伺服
int servoAngel =homePosition;


#include <Wire.h>/*******************************/
#include <LiquidCrystal_I2C.h>/*******************************/

// 将 LCD 地址设置为 0x27 或 0x3F 以实现 16 个字符和 2 行显示

LiquidCrystal_I2C lcd(0x27, 16, 2); //地址初始

void setup() {
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
  Serial.println("bianmaqi");
   myservo.attach(servoPin);  // 将伺服连接到引脚
   myservo.write(servoAngel);//将舵机移动到初始位置

   
//LCD屏幕显示的文字角度信息 
  lcd.begin();
  lcd.backlight();     
  lcd.print("c");
  lcd.setCursor (0,1); 
  lcd.print("Servo Test");   
  delay(2000);   
   lcd.clear();
  lcd.print("Find Order"); //LCD屏幕显示的文字角度信息 
  lcd.setCursor (0,1); 
  lcd.print("Angel: ");  //LCD屏幕显示的文字角度信息 
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {

    if(newPosition >  oldPosition)
    {
    int newStep = abs(newPosition - oldPosition);
      Serial.print("Angle ");
      Serial.println(servoAngel);      
      servoAngel -= stepValue;
      if(servoAngel <0)
          servoAngel =0;      
      myservo.write(servoAngel); //将伺服移动到
      lcdAngel(servoAngel);//在液晶屏上打印
    }

    if(newPosition <  oldPosition )
    {
    int newStep = abs(newPosition - oldPosition);
      Serial.print("Angle ");
      Serial.println(servoAngel);        
      servoAngel += stepValue;
      if(servoAngel >180)
          servoAngel =180;
      myservo.write(servoAngel); //
      lcdAngel(servoAngel);//
    }
   oldPosition = newPosition;//记住新位置
  }

  if( digitalRead(SW_PIN) == LOW)
  {
    Serial.print("Home: ");
    Serial.println(homePosition);
    servoAngel =homePosition;
      myservo.write(servoAngel); //
      lcdAngel(servoAngel);//
  
  }

  delay(200);
}

void lcdAngel(int angel)
{
  
  int startChar =7;
  for(int i=startChar; i<16; i++)
  {
   lcd.setCursor (i,1);    
    lcd.print(" ");   
  }
 
   lcd.setCursor (startChar,1); //
   lcd.print(angel); 
   lcd.print((char)223);
}


/**
   角度参数说明
  steps=5.68888  旋转1度
  steps=170.666  旋转30度
  steps=256      旋转45度
  steps=341.333  旋转60度
  steps=512      旋转90度
  steps=1024     旋转180度
  steps=2048     旋转360度
*/
#include<Stepper.h>
// 步进电机旋转一圈的步数
const int STEPS = 256;
// 初始化步进电机要使用的Arduino的引脚编号
Stepper myStepper(STEPS, 8 , 9  , 10, 11   );

/**

 */
int a = 512; //电机角度 默认逆时针90°
int b ;//用来转换a的值
int c ;//停止
int d = 10; //超声波空值的距离 默认值是10cm
int i ;//用户是否修改串口判断
int r ;//判断用户选择的时针方向选择
int jd;//用户修改角度的值的判断
int ledjd;//led显示的角度

/**
   超声波
*/
#define led 4//led灯端口
// 发声口
const int TrigPin = 2;
//回声口
const int EchoPin = 3;
//距离
float distance;


/**
   lcd显示屏
*/
#include <Wire.h>  //加载Wire通讯库
#include <LiquidCrystal_I2C.h>  //加载液晶屏库
LiquidCrystal_I2C lcd(0x27, 16, 2); // 设 LCD的地址为0x27，16列2行


//声明用到的函数
int updateDJ(int r);//串口修改电机
void jvliBycsb();//超声波
void initLcd(float distance, int jd); //初始化lcd
void initDJ(int a );//初始化电机



void setup()
{
  // 设置转速，单位r/min
  myStepper.setSpeed(20);//脉冲r/min
  Serial.begin(9600);// 初始化串口

  //超声波操作
  pinMode(TrigPin, OUTPUT);//  发声输出
  pinMode(EchoPin, INPUT);  //  回声输入
  pinMode(led, OUTPUT);//小灯输入口

  //lcd操作
  lcd.begin();     //初始化LCD init
  lcd.backlight(); //打开背光
}

void loop()
{


  initDJ(a);
}


//lcd显示实现 distance：要显示的距离参数，jd:要显示的角度参数
void initLcd(float distance, int jd) {
  lcd.clear();
  //第一排
  lcd.setCursor(0, 0);
  lcd.print("DT:");
  lcd.setCursor(4, 0);
  lcd.print(distance);

  //第二排
  lcd.setCursor(0, 1);
  lcd.print("AG:");

  lcd.setCursor(4, 1);
  lcd.print(ledjd);
  lcd.setCursor(9, 1);
  lcd.print("Order?");
}


//超声波测距函数实现
int  jvliBycsb(float distance, int a1) {
  int getjd;
  //设置
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  pinMode(led, OUTPUT);

  //距离逻辑
  distance = pulseIn(EchoPin, HIGH) / 58.0;
  int c;//记录传来的步数
  if (a1 != 0) {
    c = a1;
  }
  if (distance <= d) {
    a1 = 0;
    getjd = 0;
    digitalWrite(led, LOW);//关闭灯光
  } else {
    //电机初始化
    a1 = c;
    digitalWrite(led, HIGH);
  }

  Serial.print("The current distance detected by the sensor is：【");
  Serial.print(distance);
  Serial.print("】cm");
  Serial.println();


  if (a1 == 256) {
    getjd = 45;
  }
  else if (a1 == -256) {
    getjd = -45;
  }
  else if (a1 == 512) {
    getjd = 90;
  }
  else if (a1 == -512) {
    getjd = -90;
  }
  else if (a1 == 1024) {
    getjd = 180;
  }
  else if (a1 == -1024) {
    getjd = -180;
  }
  else if (a1 == 2048) {
    getjd = 360;
  }
  else if (a1 == -2048) {
    getjd = -360;
  }

  ledjd = (ledjd + getjd);
  initLcd( distance, ledjd ); //显示lcd
  delay(1000);

  return a1;
}



//实现修改电机角度函数
int updateDJ(int r) {
  //  int jd;//定义选择的度数
  Serial.print("int volume of R:");
  Serial.println(r);

  //第一个开关 r 判断用户选择顺时针还是逆时针 以及 保存数据
  while (true) {

    r = Serial.parseInt();//默认值是0（int）
    if (r == 0) {
      Serial.println("Please select the angle you want to modify");
      Serial.println("【1 is clockwise】");
      Serial.println("【2 is counter-clockwise】");
      Serial.println("【3 is the sensor】");
      Serial.println("【4 Save and exit】");
    }
    //顺势针
    else if (r == 1) {
      while (true) {
        //第二个开关 jd 判断用户选择的角度
        jd = Serial.parseInt();//默认值是0（int）
        if (jd == 0) {
          Serial.println("Please select the clockwise angle you want to modify");
          Serial.println("【1-45°】");
          Serial.println("【2-90°】");
          Serial.println("【3-180°】");
          Serial.println("【4-360°】");
        }
        else if (jd == 1) {
          //第三个开关,给a赋值
          while (true) {
            a = 256;
            Serial.println("The current motor angle has been modified to-45°");
            break;//退出当前循环
          }
          break;

        }
        else if (jd == 2) {
          //第三个开关,给a赋值
          while (true) {
            a = 512;
            Serial.println("The current motor angle has been modified to-90°");
            break;//退出当前循环
          }   break;
        }


        else if (jd == 4) {
          //第三个开关,给a赋值
          while (true) {
            a = 1024;
            Serial.println("The current motor angle has been modified to-180°");
            break;//退出当前循环
          }   break;
        }
        else if (jd == 4) {
          //第三个开关,给a赋值
          while (true) {
            a = 1024;
            Serial.println("The current motor angle has been modified to-360°");
            break;//退出当前循环
          }   break;
        }

      }

    }

    //逆时针
    else if (r == 2) {

      //第二个开关 jd 判断用户选择的角度
      while (true) {
        jd = Serial.parseInt();//默认值是0（int）
        if (jd == 0) {
          Serial.println("Please select the counterclockwise angle you want to modify");
          Serial.println("【1-45°】");
          Serial.println("【2-90°】");
          Serial.println("【3-180°】");
          Serial.println("【4-360°】");
        }
        else if (jd == 1) {
          //第三个开关,给a赋值
          while (true) {
            a = -256;
            Serial.println("The current motor angle has been modified to-45°");
            break;//退出当前循环
          }
          break;

        }
        else if (jd == 2) {
          //第三个开关,给a赋值
          while (true) {
            a = -512;
            Serial.println("The current motor angle has been modified to-90°");
            break;//退出当前循环
          }  break;
        }
        else if (jd == 3) {
          //第三个开关,给a赋值
          while (true) {
            a = -1024;
            Serial.println("The current motor angle has been modified to-180°");
            break;//退出当前循环
          }  break;
        }
        else if (jd == 4) {
          //第三个开关,给a赋值
          while (true) {
            a = -1024;
            Serial.println("The current motor angle has been modified to-360°");
            break;//退出当前循环
          }  break;
        }
      }

    }


    else if (r == 3) {
      //第二个开关 jd 判断用户选择的角度
      while (true) {
        d = Serial.parseInt();//默认值是0（int）
        if (d > 0) {
          Serial.print("The distance of the currently set sensor is");
          Serial.println(d);
          break;
        } else {
          Serial.println("Please select the distance of the sensor you want to modify");
          Serial.print("The distance of the currently set sensor is");
          Serial.println(d);
        }
      }
    }
    //保存退出
    else {

      //第4个开关，传值
      while (true) {
        initDJ(a);
      }
    }
  }
}







//初始化电机 a:电机的旋转度数
void initDJ(int a ) {
  //调用超声波测距
  a = jvliBycsb(distance, a );

  Serial.print("Now a：【");
  Serial.print(a);
  Serial.println("】Milliseconds");
  Serial.print("The distance of the currently set sensor is【");
  Serial.print(d);
   Serial.println("】cm");
  //打印电机当前度数的状态
  if (a == 256) {
    Serial.print("Current motor running state clockwise【45°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == -256) {
    Serial.print("Current motor running state counterclockwise【45°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == 512) {
    Serial.print("Current motor running state clockwise【90°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == -512) {
    Serial.print("Current motor running state counterclockwise【90°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == 1024) {
    Serial.print("Current motor running state clockwise【180°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == -1024) {
    Serial.print("Current motor running state counterclockwise【180°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == 2048) {
    Serial.print("Current motor running state clockwise【360°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }
  else if (a == -2048) {
    Serial.print("Current motor running state counterclockwise【360°】rotation delay");
    Serial.print(a);
    Serial.println("Milliseconds");
  }




  // 电机运行
  myStepper.step(a);
  //判断a的正负
  if (a < 0) {
    b = -a;
  } else {
    b = a;
  }
  delay(b);  //设置延迟为a的值

  //串口输入判断
  //判断用户是否想要修改电机的旋转角度
  Serial.println("To modify the system, enter【R】");
  //  Serial.print("退出系统，请输入N");
  i = Serial.read();
  //  对串口数据进行操作
  //输入了R，进行修改
  if (i == 'R') {
    updateDJ(i);
  }

}

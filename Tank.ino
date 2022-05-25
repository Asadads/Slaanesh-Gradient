

/*Полностью работающий код. Реализованы три степени свободы.
Во время актиивации первой осуществляется движение моторов. Предусмотрено 8 положений джостика:
Движение вперед, назад, на месте направо, налево, вперед и назад только правым или левым мотором.
Во время активации второй степени свободы осуществляется движение башни и пушки двумя слайдерами.
Башня поворачивается на 180 градусов, пушка поднимается на 90.
Во время активации третьей степени свободы осуществляется стрельба лазером по кнопке.
Индикация степени свободы осуществляется с помощью трех светодиодов: зеленый, желтый и красный. */


#define REMOTEXY_MODE__HARDSERIAL

#include <RemoteXY.h>
#include <Servo.h>
#include <AFMotor.h>

AF_DCMotor motor1(1); 
AF_DCMotor motor2(2); 

#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_RX 1
#define REMOTEXY_SERIAL_TX 0
#define REMOTEXY_SERIAL_SPEED 9600

 
#pragma pack(push, 1)
unsigned char RemoteXY_CONF[] =   // 52 bytes
  { 255,6,0,0,0,45,0,16,31,0,5,1,60,9,30,30,2,26,31,4,
  128,25,45,69,11,2,26,4,0,33,5,11,33,2,26,1,0,4,44,12,
  12,2,31,0,3,3,5,6,8,22,2,26 };
struct {

    // input variables
  signed char joystick_1_x; // oт -100 до 100  
  signed char joystick_1_y; // oт -100 до 100  
  unsigned char slider_1; // =-100..100 положение слайдера 
  unsigned char slider_2; // =0..100 положение слайдера 
  unsigned char button_1; // =1 если кнопка нажата, иначе =0 
  unsigned char switch_1; // =0 если переключатель в положении A, =1 если в положении B, =2 если в положении C, ... 

    // other variable
  unsigned char connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop) 

Servo myservo1; 
Servo myservo2;

#define PIN_BUTTON_1 14

#define PIN_SWITCH_1 19

#define PIN_SERVO_VERT 9
#define PIN_SERVO_GOR 10

#define PIN_ZVUK 18

#define PIN_KRAS_D 15
#define PIN_ZELEN_D 16
#define PIN_GELT_D 17

void setup() 
{
  RemoteXY_Init (); 

  pinMode (PIN_ZVUK, OUTPUT);

  pinMode (PIN_KRAS_D, OUTPUT);
  pinMode (PIN_ZELEN_D, OUTPUT);
  pinMode (PIN_GELT_D, OUTPUT);

  pinMode (PIN_SERVO_VERT, OUTPUT);
  pinMode (PIN_SERVO_GOR, OUTPUT);

  pinMode (PIN_ZVUK, OUTPUT);
  
  myservo1.attach(PIN_SERVO_VERT);
  myservo2.attach(PIN_SERVO_GOR);
  
  RemoteXY.slider_1 = 50;
  RemoteXY.slider_2 = 0; 

  pinMode (PIN_BUTTON_1, OUTPUT);

  pinMode (PIN_SWITCH_1, OUTPUT);

  RemoteXY.switch_1 = 0;

}

void loop() 
{ 
  RemoteXY_Handler ();
  
int ms1 = RemoteXY.slider_1*20+500;
int ms2 = RemoteXY.slider_2*10+500;

int A = RemoteXY.joystick_1_y;
int B = RemoteXY.joystick_1_x;

switch (RemoteXY.switch_1) {
  case 0:
    digitalWrite(16, HIGH);
    digitalWrite(17, LOW);
    digitalWrite(15, LOW);

    if((B > - 20)&&(B < 20)&&(A > 60)){
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor1.setSpeed(225);
      motor2.setSpeed(225);
    }
    else if((B > - 20)&&(B < 20)&&(A < - 60)){
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor1.setSpeed(225);
      motor2.setSpeed(225);
    }
    else if((B > 60)&&(A > - 20)&&(A < 20)){
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      motor1.setSpeed(225);
      motor2.setSpeed(225);
    }
    else if((B < - 60)&&(A > - 20)&&(A < 20)){
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      motor1.setSpeed(225);
      motor2.setSpeed(225);
    }
    else if((B > 50)&&(A > 50)){
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor1.setSpeed(0);
      motor2.setSpeed(225);
    }
    else if((B < - 50)&&(A > 50)){
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor1.setSpeed(225);
      motor2.setSpeed(RELEASE);
    }
    else if((B < - 50)&&(A < - 50)){
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor1.setSpeed(225);
      motor2.setSpeed(0);
    }
    else if((B > 50)&&(A < - 50)){
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor1.setSpeed(0);
      motor2.setSpeed(225);
    }
    else{
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor1.setSpeed(RELEASE);
      motor2.setSpeed(RELEASE);
    }
    break;

  case 1: 
    digitalWrite(16, LOW);
    digitalWrite(17, HIGH);
    digitalWrite(15, LOW);
    myservo1.writeMicroseconds(ms1); 
    myservo2.writeMicroseconds(ms2);
    
    break;
    
  case 2:
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    //digitalWrite(15, HIGH);
    
    digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1==0)?LOW:HIGH);
    digitalWrite(PIN_ZVUK, (RemoteXY.button_1==0)?LOW:HIGH);
    digitalWrite(PIN_KRAS_D, (RemoteXY.button_1==0)?LOW:HIGH);
    
    break;
  }

}

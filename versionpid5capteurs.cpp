#include <Arduino.h>
//motor right(in1 avant et in2 arriere):
#define in2 4
#define in1 2
#define in4 8
#define in3 7
#define enA 5
#define enB 6
// enA et enB need PWM pin
#define sensor_1 A0  //sensor 1
#define sensor_2 A1   //sensor 2
#define sensor_3 A2   //sensor 3
#define sensor_4 A3  //sensor 4
#define sensor_5 A4   //sensor 5
#define avglspeed A5 //vitesse normale avancer left
#define avgrspeed 50 // vitesse normale avancer right 
#define button 12
int i=1;
int v=80;
int j=0;
int delaypid=0;
int max1=0;
int max2=0,max3=0,max4=0,max5=0;
void setup(){
  //motors :
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);
  Serial.begin(115200);
  //sensors:
  pinMode(sensor_1, INPUT);
  pinMode(sensor_2, INPUT);
  pinMode(sensor_3, INPUT);
  pinMode(sensor_4, INPUT);
  pinMode(sensor_5, INPUT);
  pinMode(button,INPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,0);
}
float kp=25.0;
float ki=0.0;
float kd=0.0;
int last_error=0;
float integral=0; 
int x=0;
void move(float,float);
void afficher_analog_sensors();
int read_sensors();
void calibrationblanc();
void calibrationnoire();
int h=0;
int ok=0;
int calculererror();
int pid(int);
void loop()
{
    unsigned long startTime = 0;
    int a=digitalRead(button);
    if(a==1){
      h++;
    }
    if(h==1 &&  ok==0){
        startTime = millis();
        digitalWrite(13,1);
        calibrationblanc();
        ok=1;
        digitalWrite(13,0);

    }
    else if(h==2 && ok==1){
        digitalWrite(13,1);
        calibrationnoire();
        digitalWrite(13,0);
        ok=0;
    }
  //looooooop:
  else if(h>=3){
  int s=read_sensors();
  if(s!=11111 && (i==1||i==2))i++;
  if(j==3) i++;
    if(s==11111){
    if(i==0){ 
          move(v,v);
          delay(100);
          s=read_sensors();
          if(s!=11111)i++;
      }
    else if(i==1 || i==2){
      move(1.5*v,0.75*v);
      delay(60);
    }
    /*else if (i==2){
      move(2*v,-1*v);
      delay(10);
    }*/
    
      
  }
  else if (s==1111){
    move(2*v,-0.5*v);
    delay(300);
    i++;
  }
  else {
  int error=calculererror();
  int correction=pid(error);
  move(v-correction,v+correction);
  delay(delaypid);
  }
  
  }

}
int calibrer2(int);
int calibrer3(int);
int calibrer4(int);
void move(float speed_right,float speed_left){
  digitalWrite(in1,speed_left<0);
  digitalWrite(in2,speed_left>0);
  digitalWrite(in3,speed_right<0);
  digitalWrite(in4,speed_right>0);
  analogWrite(enA,abs(speed_left));
  analogWrite(enB,abs(speed_right));
}
int inverser(int n){
  int t[5];
  for(int w=4;w>=0;w--){
    if(n%10==0)t[w]=1;
    else t[w]=0;
    n=n/10;
  }
  return(t[4]+10*t[3]+100*t[2]+1000*t[1]+10000*t[0]);
}
int calculererror(){
  
  int error=0;
  int s=read_sensors();
  switch (s) {
    case 1110:
      return 0;    // Perfectly centered
    case 1100:
      return 1;    // left
    case 110:
      return -1;   // right
    case 1000:
      return 3;    // left
    case 10:
      return -3;   //  right
    case 111:
    return -4; // right
    case 11100:
    return 4; //left
    case 11000:
    return 6; //left
    case 11:
    return -6;//right
    case 1: //right
    return -8;
    case 10000:     
    return 8;//left
    default:
      return 0;    // No line detected, or lost, treat as no error
  }
}
int pid(int error){
  float p=kp*error;
  integral+=error;
  float i=ki*integral;
  float d=kd*(error-last_error);
  last_error=error;
  return p+i+d;
}
void afficher_analog_sensors(){
  Serial.print(analogRead(sensor_1));Serial.print("*");
  Serial.print(analogRead(sensor_2));Serial.print("*");
  Serial.print(analogRead(sensor_3));Serial.print("*");
  Serial.print(analogRead(sensor_4));Serial.print("*");
  Serial.print(analogRead(sensor_5));Serial.println("*");
  delay(1000);
}
int calibrer1(int s){
int res=analogRead(s);
if(res<max1){
  return 0;
}
else return 1;
}
int calibrer2(int s){
int res=analogRead(s);
if(res<max2){
  return 0;
}
else return 1;
}
int calibrer3(int s){
int res=analogRead(s);
if(res<max3){
  return 0;
}
else return 1;
}
int calibrer4(int s){
int res=analogRead(s);
if(res<max4){
  return 0;
}
else return 1;
}
int calibrer5(int s){
int res=analogRead(s);
if(res<max5){
  return 0;
}
else return 1;
}
int read_sensors(){
  int s1= calibrer1(sensor_1);
  int s2 = calibrer2(sensor_2);
  int s3 = calibrer3(sensor_3);
  int s4 = calibrer4(sensor_4);
  int s5= calibrer5(sensor_5);
  int res=s5+10*s4+100*s3+1000*s2+10000*s1;
  return(res);
}
int read_line_position() {
  int s2 = analogRead(sensor_2);
  int s4 = analogRead(sensor_4);
  int position = (s2 * 1)+(s4 * -1);
  return position;
}
int calibrernoire(int sensor){
  unsigned long start=millis();
  int s=1023;
  while(millis()-start<500){
    if(analogRead(sensor)<s)
    s=analogRead(sensor);
    
  }
  return s;

}
int calibrerblanc(int sensor){
  unsigned long start=millis();
  int   s=0;
  while(millis()-start<500){
    if(analogRead(sensor)>s)
    s=analogRead(sensor);
  }
  return s;}
void calibrationblanc(){
    int s1,s2,s3,s4,s5;
    s1=calibrerblanc(sensor_1);
    s2=calibrerblanc(sensor_2);
    s3=calibrerblanc(sensor_3);
    s4=calibrerblanc(sensor_4);
    s5=calibrerblanc(sensor_5);
    max1=s1;
    max2=s2;
    max3=s3;
    max4=s4;
    max5=s5;
}
void calibrationnoire(){
    int s1,s2,s3,s4,s5;
    s1=calibrernoire(sensor_1);
    s2=calibrernoire(sensor_2);
    s3=calibrernoire(sensor_3);
    s4=calibrernoire(sensor_4);
    s5=calibrernoire(sensor_5);
    max1=max1+s1;
    max2=max2+s2;
    max3=max3+s3;
    max4=max4+s4;
    max5=max5+s5;
    max1=max1/2;
    max2=max2/2;
    max3=max3/2;
    max4=max4/2;
    max5=max5/2;
    
}

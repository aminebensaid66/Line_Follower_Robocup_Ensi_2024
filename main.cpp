#include <Arduino.h>
//motor right(in1 avant et in2 arriere):
#define in1 2
#define in2 4
#define in3 7
#define in4 8
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
int i=0;
int avg=60;
int max1;
int max2,max3,max4,max5;
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
}

int x=0;
void move(float,float);
void afficher_analog_sensors();
int read_sensors();
void loop()
{
  
  int s=read_sensors();
  afficher_analog_sensors();
  switch(s){
    case 0:move(0,0);
        break;
    case 1:move(avg,0);
        break;
    case 10:move(0,0);
        break;
    case 11:move(avg,-0.5*avg);
        break;
    case 100:move(0,0);
        break;
    case 101:move(0,0);
        break;
    case 110:move(avg,avg*0.5);
        break;
    case 111:move(0,0);
        break;
    case 1000:move(0,0);
        break;
    case 1001:move(0,0);
        break;
    case 1010:move(0,0);
        break;
    case 1011:move(0,0);
        break;
    case 1100:move(avg*0.5,avg);
        break;
    case 1101:move(0,0);
        break;
    case 1110:move(avg,avg);
        break;
    case 1111:move(0,0);
        break;
    case 10000:move(-0.5*avg,1.4*avg);
        break;
    case 10001:move(0,0);
        break;
    case 10010:move(0,0);
        break;
    case 10011:move(0,0);
        break;
    case 10100:move(0,0);
        break;
    case 10101:move(0,0);
        break;
    case 10110:move(0,avg);
        break;
    case 10111:move(0,0);
        break;
    case 11000:move(-0.5*avg,avg);
        break;
    case 11001:move(0,0);
        break;
    case 11010:move(0,0);
        break;
    case 11011:move(0,0);
        break;
    case 11100:move(0,1.4*avg);
        break;
    case 11101:move(0,0);
        break;
    case 11110:move(0,avg);
        break;
    case 11111:move(0,0);
        break;


    
    /*case 11111 :
    //'Hexagone
    if(i==0 || i==1){
      //Chemin De La Sageusse
      droite120deg();
      i++;
    }
    //sorite de Le Repos des Anciens
    if(i==4){
      avantsimple();
      i++;
    }
    //le passage de cercle 
    if (i==5){
      droite120deg();
      i++;
    }
    case 0 :
     //sentier Brisé 
  //ligne discontinue
  //00000 
  avantsimple();i++;
    case 1111:
    case 11110 : 
     //petite tire a eviter
     if(i==2){
    avantsimple();
    i++;}
    //sortie depassage au cercle 
    if(i==6){
      droite90deg();
      i++;
    }
    if(i==0){
      droite90deg();
      i++;
    }
    case 10001 :
    // Le Repos des Anciens
    if(i==3){
    avantmilieu();
    i++;
    }
    //Le Chemin Sinusoïdal
    if(i==7){
      inverser(s);
      i++;
    }
    if(i==8){
      inverser(s);
      i++;
    }
    ////Escaliers du Défi :
    case 10000 :retourligneescalierdroite();
    case 1:retourligneescalierguache();
     


  }*/
}

}
void move(float speed_left,float speed_right){
  digitalWrite(in1,speed_left>0);
  digitalWrite(in2,speed_left<0);
  digitalWrite(in3,speed_right>0);
  digitalWrite(in4,speed_right<0);
  analogWrite(enA,abs(speed_left));
  analogWrite(enB,abs(speed_right));
}
int inverser(int n){
  int t[5];
  for(int i=4;i>=0;i--){
    if(n%10==0)t[i]=1;
    else t[i]=0;
    n=n/10;
  }
  return(t[4]+10*t[3]+100*t[2]+1000*t[1]+10000*t[0]);
}
void avantsimple(){
move(75,50);
}
void droite120deg(){
}
void afficher_analog_sensors(){
  Serial.print(analogRead(sensor_1));Serial.print("*");
  Serial.print(analogRead(sensor_2));Serial.print("*");
  Serial.print(analogRead(sensor_3));Serial.print("*");
  Serial.print(analogRead(sensor_4));Serial.print("*");
  Serial.print(analogRead(sensor_5));Serial.println("*");
  delay(1000);
}
int calibrer(int s){
int res=analogRead(s);
if(res<250){
  return 0;
}
else return 1;
}
int read_sensors(){

  int s1= calibrer(sensor_1);
  int s2 = calibrer(sensor_2);
  int s3 = calibrer(sensor_3);
  int s4 = calibrer(sensor_4);
  int s5= calibrer(sensor_5);
  int res=s5+10*s4+100*s3+1000*s2+10000*s1;
  return(res);
}
void droite90deg(){

}
void avantmilieu(){
//le robot doit avancer au milieu et attendre 5 seconde sans sortir de la loop 

}
void calibrergauche(){
move(avglspeed,avgrspeed-30);
// le robot a sortis de la ligne a gauche (ndawrouh imin chwya )
}
void calibrerdroite(){
  move(avglspeed-30,avgrspeed);
// le robot a sortis de la ligne a droite (ndawrouh isar chwya )

}


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
}
int calibrerblanc(int sensor){
  unsigned long start=millis();
  int   s=0;
  while(millis()-start<1000){
    if(analogRead(sensor)>s)
    s=analogRead(sensor);
  }
  return s;}

int calibrernoire(int sensor){
  unsigned long start=millis();
  int s=1023;
  while(millis()-start<1000){
    if(analogRead(sensor)<s)
    s=analogRead(sensor);
    
  }
  return s;

}
/*void droite120deg(){
  move(avglspeed*0.3,avgrspeed*0.5);
  //rotation droite 120 degree 
}
void droite90deg(){
  move(avglspeed*-0.25,avgrspeed*0.5);
}
void retourligneescalierguache(){

}
void retourligneescalierdroite(){

}*/
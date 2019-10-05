//================== Código para Servomotor ===================//
#include <Servo.h>

#define dd  A11
#define d   A12
#define c   A13
#define e   A14
#define ee  A15

#define M1  6
#define M2  9

#define limite 590
#define valIn 1500       //0-255
#define vel 200

#define Kp 600
#define Ki 200   // A soma desses valores não pode
#define Kd 300   //ultrapassar 11000

Servo direita, esquerda;

unsigned long t1, t2;
int P, erroAnterior, velMax = valIn + 4*(Kp+Ki+Kd) + vel, velMin = valIn - 4*(Kp+Ki+Kd) - vel;
bool cont;
long PID;

int erro(){
  int DD = analogRead(dd), D = analogRead(d),
      C = analogRead(c), E = analogRead(e), EE = analogRead(ee);
  Serial.print(DD); Serial.print(" - "); Serial.print(D); Serial.print(" - ");
  Serial.print(C); Serial.print(" - "); Serial.print(E); Serial.print(" - ");
  Serial.print(EE);
  if(DD > limite && D > limite && C > limite && E > limite && EE > limite) return 6;
  if(DD < limite && D < limite && C < limite && E < limite && EE < limite) return 5;
  if(DD < limite && D < limite && C < limite && E < limite && EE > limite) return 4;
  if(DD < limite && D < limite && C < limite && E > limite && EE > limite) return 3;
  if(DD < limite && D < limite && C < limite && E > limite && EE < limite) return 2;
  if(DD < limite && D < limite && C > limite && E > limite && EE < limite) return 1;
  if(DD < limite && D < limite && C > limite && E < limite && EE < limite) return 0;
  if(DD < limite && D > limite && C > limite && E < limite && EE < limite) return -1;
  if(DD < limite && D > limite && C < limite && E < limite && EE < limite) return -2;
  if(DD > limite && D > limite && C < limite && E < limite && EE < limite) return -3;
  if(DD > limite && D < limite && C < limite && E < limite && EE < limite) return -4;
}

void calcularPID(){
  P = erro(); Serial.print("      Erro: "); Serial.print(P);
  if(P < 5){
    int I = I + P,
        D = P - erroAnterior;
    PID = (Kp*P)+(Ki*I)+(Kd*D);
    erroAnterior = P;
  }
}

void setup(){
  Serial.begin(9600);
  direita.attach(M1); esquerda.attach(M2);
}

void loop(){
  calcularPID();
  if(P == 6) pararMotor();
  if(P == 5){
    PID = 0; mover();
  }
  if(P < 5) mover();
}

void pararMotor(){
  if(cont){
    direita.writeMicroseconds(valIn); Serial.println();
    esquerda.writeMicroseconds(valIn);
  }
  else{
    direita.writeMicroseconds(valIn - vel); Serial.println();
    esquerda.writeMicroseconds(valIn + vel); cont++;
  }
}

void mover(){
  cont = 0;
  //PID = map(PID, 0, 4*(Kp+Kd+Ki), 0, valIn/3);
  long velM1, velM2;
  velM1 = valIn + vel - PID;
  velM2 = valIn - vel - PID;
  direita.writeMicroseconds(velM1);
  esquerda.writeMicroseconds(velM2);
  Serial.print("  -  PID: "); Serial.print(PID);  
  Serial.print("  -  velM1: "); Serial.print(velM1);
  Serial.print("  -  velM2: "); Serial.println(velM2);
  Serial.println();
}

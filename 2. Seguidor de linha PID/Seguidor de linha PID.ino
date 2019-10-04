#define dd    A0
#define d     A1
#define c     A2
#define e     A3
#define ee    A4
#define M1_F  5
#define M1_T  6
#define M2_T  10
#define M2_F  11
#define limite 580
#define valIn 120       //0-255
#define tempoMax 25

#define Kp 600
#define Ki 300
#define Kd 500

unsigned long t1, t2;
int P, PID, erroAnterior, velMax = 2*valIn, cont = 0;

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
  P = erro(); Serial.print("     - Erro: "); Serial.print(P);
  if(P < 5){
    int I = I + P,
        D = P - erroAnterior;
    PID = (Kp*P)+(Ki*I)+(Kd*D);
    erroAnterior = P;
  }
  else cont = 0;
}

void setup(){
  pinMode(M1_F, OUTPUT); pinMode(M1_T, OUTPUT);
  pinMode(M2_F, OUTPUT); pinMode(M2_T,OUTPUT);
  t1 = millis(); t2 = millis(); Serial.begin(9600);
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
  digitalWrite(M1_F, LOW); Serial.println();
  digitalWrite(M2_F, LOW); cont = 0;
}

void mover(){
  if(!cont){
    analogWrite(M1_F, 255);
    analogWrite(M2_F, 255); //Torque máximo para partida
    delay(4); cont++;
  }
  //PID = PID+4*(Kp+Kd+Ki);
  PID = map(PID, 0, 4*(Kp+Kd+Ki), 0, valIn);
  int velM1, velM2;
  if(!PID){
    velM1 = valIn;
    velM2 = valIn;
  }
  else{
    velM1 = valIn + PID;
    velM2 = valIn - PID;
  }
  
  /*********** PWM Arduino ************/ 
    velM1 = map(velM1, 0, velMax, 0, valIn);
    velM2 = map(velM2, 0, velMax, 0, valIn);
    analogWrite(M1_F, velM1);
    analogWrite(M2_F, velM2);
    Serial.print("  -  PID: "); Serial.print(PID);  
    Serial.print("  -  velM1: "); Serial.print(velM1);
    Serial.print("  -  velM2: "); Serial.println(velM2);
    Serial.println();

  /********** PWM pelo tempo s/ delay *********/
    /*velM1 = map(velM1, 0, velMax, 0, tempoMax);
    velM2 = map(velM2, 0, velMax, 0, tempoMax);
    if(millis()-t1 < velM1) analogWrite(M1_F, 255);
      else if(millis()-t1 > velM1) analogWrite(M1_F, 0);
    if(millis()-t1 > tempoMax) t1 = millis();
    if(millis()-t2 < velM2) analogWrite(M2_F, 255);
      else if(millis()-t2 > velM2) analogWrite(M2_F, 0);
    if(millis()-t2>tempoMax) t2 = millis();
    Serial.print("  -  PID: "); Serial.print(PID);  
    Serial.print("  -  velM1: "); Serial.print(velM1);
    Serial.print("  -  velM2: "); Serial.println(velM2);
    Serial.println();*/

    /********** PWM pelo tempo c/ delay *********/
    /*velM1 = map(velM1, 0, velMax, 0, tempoMax) - P;
    velM2 = map(velM2, 0, velMax, 0, tempoMax) - P;
    analogWrite(M1_F, 255); delay(velM1); analogWrite(M1_F, 0);
    analogWrite(M2_F, 255); delay(velM2); analogWrite(M2_F, 0);
    Serial.print("  -  PID: "); Serial.print(PID);  
    Serial.print("  -  velM1: "); Serial.print(velM1);
    Serial.print("  -  velM2: "); Serial.println(velM2);
    Serial.println();*/
}

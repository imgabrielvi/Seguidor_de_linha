#define dd    A0
#define d     A1
#define c     A2
#define e     A3
#define ee    A4
#define M1_F  5
#define M1_T  6
#define M2_T  10
#define M2_F  11
#define limite 600
#define valIn 10
#define tempoMax 600

#define Kp 20
#define Ki 10
#define Kd 7

unsigned long tempo;
int PID, erroAnterior, velMax = valIn + 4*(Kp+Ki+Kd);

byte erro(){
  int DD = analogRead(dd), D = analogRead(d),
      C = analogRead(c), E = analogRead(e), EE = analogRead(ee);
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
  int P = erro(),
      I = I + P,
      D = erroAnterior - P;
  PID = (Kp*P)+(Ki*I)+(Kd*D);
  erroAnterior = P;
}

void setup() {
  pinMode(M1_F, OUTPUT); pinMode(M1_T, OUTPUT);
  pinMode(M2_F, OUTPUT); pinMode(M2_T,OUTPUT);
  tempo = millis();
}

void loop() {
  calcularPID();
  int velM1 = valIn + PID,
      velM2 = valIn - PID;
      
  /*********** PWM Arduino ************/ 
    velM1 = map(velM1, 0, velMax, 0, 255);
    velM2 = map(velM2, 0, velMax, 0, 255);
    analogWrite(M1_F, velM1);
    analogWrite(M2_F, velM2);

  /********** PWM pelo tempo  *********/
   /* velM1 = map(velM1, 0, velMax, 0, tempoMax);
    velM2 = map(velM2, 0, velMax, 0, tempoMax);
    if(millis()-tempo < velM1) digitalWrite(M1_F, HIGH);
      else if(millis()-tempo < tempoMax) digitalWrite(M1_F, LOW);
    if(millis()-tempo < velM2) digitalWrite(M2_F, HIGH);
      else if(millis()-tempo < tempoMax) digitalWrite(M2_F, LOW);*/
}
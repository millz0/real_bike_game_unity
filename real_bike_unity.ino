const int pinA = 2;//A相 割り込み0
const int pinB = 3;//B相 割り込み1
const int pinZ = 10;//Z相
volatile short enc_count = 0;
volatile short rot = 0;
volatile int flag = 0;
char buf[5];
void setup()
{
  Serial.begin(9600);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinZ, INPUT);
  attachInterrupt(0, enc_changedPinA, CHANGE); //pinAの信号変化に合わせて割り込み処理
  attachInterrupt(1, enc_changedPinB, CHANGE); //pinBの信号変化に合わせて割り込み処理
}
void loop()
{

  //シリアルコンソールに現在の値を出力する
  rot = enc_count;
  if (rot < 0) {
    rot = rot * (-1);
    rot = rot + 10000;
  }
  byte low = lowByte(rot);
  byte high = highByte(rot);


  flag =  Serial.read();
  if (flag == 50) {
    Serial.write(low);
    Serial.write(high);
  }


  //Serial.print("\t");
  delayMicroseconds(10000);

}
void count_inc()
{
  if (digitalRead(pinZ) == 1) enc_count = 0;
  else --enc_count; enc_count = enc_count % 6000;
}
void count_dec()
{
  if (digitalRead(pinZ) == 1) enc_count = 0;
  else ++enc_count; enc_count = enc_count % 6000;
}
//pinAの割り込み処理
void enc_changedPinA()
{
  if (digitalRead(pinA))
  {
    if (digitalRead(pinB)) count_dec();
    else count_inc();
  } else {
    if (digitalRead(pinB)) count_inc();
    else count_dec();
  }
}
//pinBの割り込み処理
void enc_changedPinB()
{
  if (digitalRead(pinB))
  {
    if (digitalRead(pinA)) count_inc();
    else count_dec();
  } else {
    if (digitalRead(pinA)) count_dec();
    else count_inc();
  }
}

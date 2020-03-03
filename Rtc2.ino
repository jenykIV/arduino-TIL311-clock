#include <DS3231.h>
#include <Wire.h>
DS3231 clock;
RTCDateTime datumCas;

byte mode = 1;

byte pomocna;//pomocne pro zistovani casu
byte jp;
byte dp;

byte jm;//pomocne pro ukazani setupu
byte jh;
byte dh;
byte dm;
byte js;
byte ds;

//byte j[2] = {js,jm};
//byte d[2] = {ds,dm};
byte value;//hodnota
byte ButM;//tlacitko mode
byte ButS;//tlacitko set

//byte latchPiny[6] = {8,9,10,11,12,13}; //puvodni nastaveni lpinu
byte latchPiny[6] = {13,12,11,10,9,8};
byte piny[4] = {2,3,4,5};
byte cisla[13][4] = {
  {0,0,0,0},
  {0,0,0,1},
  {0,0,1,0},
  {0,0,1,1},
  {0,1,0,0},
  {0,1,0,1},
  {0,1,1,0},
  {0,1,1,1},
  {1,0,0,0},
  {1,0,0,1},
  {1,1,1,0},
  {1,1,0,0},
  {1,1,0,1}
};
void napis(byte is, byte xs, byte im, byte xm, byte ih, byte xh) {//vypise (jednotky,desitky,stovky.tisice,destidice,stacisice)
  digitalWrite (latchPiny[0], LOW);
  cislice(is);
  digitalWrite (latchPiny[0], HIGH);
  digitalWrite (latchPiny[1], LOW);
  cislice(xs);
  digitalWrite (latchPiny[1], HIGH);
  digitalWrite (latchPiny[2], LOW);
  cislice(im);
  digitalWrite (latchPiny[2], HIGH);
  digitalWrite (latchPiny[3], LOW);
  cislice(xm);
  digitalWrite (latchPiny[3], HIGH);
  digitalWrite (latchPiny[4], LOW);
  cislice(ih);
  digitalWrite (latchPiny[4], HIGH);
  digitalWrite (latchPiny[5], LOW);
  cislice(xh);
  digitalWrite (latchPiny[5], HIGH);
  
  }

  void cislice(byte cislo){//vypise cislo
    for(int j = 0; j < 4; j++){
    if(cisla[cislo][j] == 1){
      digitalWrite(piny[j], HIGH);
    }else{
      digitalWrite(piny[j], LOW);
    }
}
  }

void setup() {
   //pinMode (6, OUTPUT);
pinMode(7, INPUT);
   
  clock.begin();//init. casu
  //clock.setDateTime(__DATE__, __TIME__);
  
  for(int i = 0; i < 4; i++){//piny pro ABCD
  pinMode (piny[i], OUTPUT);
  }
  for (int i = 0; i < 6; i++){
  pinMode (latchPiny[i], OUTPUT);//latche
  digitalWrite (latchPiny[i], HIGH);
  }
  
  Serial.begin(9600);
  
  
}
void loop(){
  ButM = digitalRead(7);//nastavovani modu
  if(ButM == HIGH){
    mode++;
    if(mode == 9){
      mode = 1;
    }
    delay(1000);
  }
  

  if(mode == 2){//nastavovani sek,
    napis(value,10,10,10,10,10);
    ButS = digitalRead(6);
    if(ButS == HIGH){
      value++;
      if(value == 10){
        value = 0;
      }
      delay(500);
    }
    js = value;
  }else if(mode == 3){//des.sek,
    napis(js,value,10,10,10,10);
    ButS = digitalRead(6);
    if(ButS == HIGH){
      value++;
      if(value == 6){
        value = 0;
      }
      delay(500);
    } 
    ds = value;    
  }else if(mode == 4){//minut
   
  
    napis(js,ds,value,10,10,10);
    ButS = digitalRead(6);
    if(ButS == HIGH){
      value++;
      delay(500);
      if(value == 10){
        value = 0;
      }
    }
    jm = value; 
  }else if(mode == 5){//mod des.minut
    
    napis(js,ds,jm,value,10,10);
    ButS = digitalRead(6);
    if(ButS == HIGH){
      value++;
      if(value == 6){
        value = 0;
      }
      delay(500);
    }
    dm = value;    
  }else if(mode == 6){//mod hodin
    
    napis(js,ds,jm,dm,value,10);
    ButS = digitalRead(6);
    if(ButS == HIGH){
      value++;
      if(value == 4){
        value = 0;
      }
      delay(500);
    }
    jh = value;
  }else if(mode == 7){//mod des.hodin
    
    napis(js,ds,jm,dm,jh,value);
    ButS = digitalRead(6);
    if(ButS == HIGH){
      value++;
      if(value == 3){
        value = 0;
      }
      delay(500);
    }
    dh = value;
  }else if(mode == 8){//pocita a zapisuje do RTC
    napis(js,ds,jm,dm,jh,dh);
    delay(100);
    napis(10,12,1,7,0,0);
    delay(1000);
        byte sec;
        byte mins;
        byte hrs;
        byte math;
        math = 10 * ds;
        sec = js + math;
        math = 10 * dm;
        mins = jm + math;
        math = 10 * dh;
        hrs = jh + math;
        clock.setDateTime(2000,1,1,hrs,mins,sec);
        napis(4,10,5,10,7,0);
        delay(1000);
        mode = 0;
  }else if(mode == 1){//vypise cas
  datumCas = clock.getDateTime();
  pomocna = datumCas.second;
  jp = 0;
  dp = 0;
  for (int count = 0; count < 60; count++) {
    if(count == pomocna){
      js = jp;
      ds = dp;
      napis(js, ds, jm, dm, jh, dh);
    }else{
      jp++;
      if(jp == 10){
        jp = 0;
        dp++;
      }
    }
  }


  pomocna = datumCas.minute;
  datumCas = clock.getDateTime();
  jp = 0;
  dp = 0;
  for (int count = 0; count < 60; count++) {
    if(count == pomocna){
      jm = jp;
      dm = dp;
       napis(js, ds, jm, dm, jh, dh);
    }else{
      jp++;
      if(jp == 10){
        jp = 0;
        dp++;
      }
    }
  }

  pomocna = datumCas.hour;
  
  datumCas = clock.getDateTime();
  jp = 0;
  dp = 0;
  for (int count = 0; count < 24; count++) {
    if(count == pomocna){
      jh = jp;
      dh = dp;
       napis(js, ds, jm, dm, jh, dh);
    }else{
      jp++;
      if(jp == 10){
        jp = 0;
        dp++;
      }
    }
  }
}
}

  

  


  

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>     //LiquidCrystal 라이브러리 추가 
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//블루투스
int bluetoothTx = 2;
int bluetoothRx = 3;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
//LED제어
int LED1 = 4; int LED2 = 5; int LED3 = 6; int LED4 = 7;
//piezo부저
int piezo = 8;
//카운트
int count=1;
//비밀번호 저장변수
char pass1='\0';
char pass2='\0';
char pass3='\0';
char pass4='\0';
//비밀번호 설정
char success1 = '9';
char success2 = '0';
char success3 = '8';
char success4 = '0';
//잠금
char lock1 = 'A';
int lock = 0;

//솔레노이드
int solenoidPin=9;

//키패드 
const byte ROWS = 4;    // 행(rows) 개수
const byte COLS = 4;    // 열(columns) 개수
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte rowPins[ROWS] = {0, A2, 10, 11};   // R1, R2, R3, R4 단자가 연결된 아두이노 핀 번호
byte colPins[COLS] = {12,13,A0,A1};   // C1, C2, C3, C4 단자가 연결된 아두이노 핀 번호
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//잠금,잠금해제 1버튼
int flagcount = 0;
int flag = 0;
int key = A3;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
delay(100);
bluetooth.begin(9600);
pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT); pinMode(LED3, OUTPUT); pinMode(LED4, OUTPUT); pinMode(piezo, OUTPUT);
pinMode(solenoidPin, OUTPUT);
pinMode(key, INPUT);
digitalWrite(solenoidPin, HIGH);   //솔레노이드 기본상태설정(잠금)
lcd.init();                      // initialize the lcd 
lcd.backlight();
lcdset();
}


void loop() {
char cmd;
if(bluetooth.available()) {
  cmd = bluetooth.read();
  if(cmd) {

//입력초기화
if(cmd == '*') {  
  Serial.println("입력 초기화");
  bluetooth.println("입력 초기화");
  lcd.clear();            // 글자를 모두 지워라.
  ledlow();
  count=0;
  pass1='\0';
  pass2='\0';
  pass3='\0';
  pass4='\0';
}
    count=count+1;
    Serial.print("Command = ");
    Serial.println(cmd);
  }
if(count==2){
  digitalWrite(LED1,HIGH);
  ton();
  pass1 = cmd;
}
if(count==3){
  digitalWrite(LED2,HIGH);
  ton();
  pass2 = cmd;
}
if(count==4){
  digitalWrite(LED3,HIGH);
  ton();
  pass3 = cmd;
}
if(count==5){
  digitalWrite(LED4,HIGH);
  ton();
  pass4 = cmd;
  delay(700);
  ledlow();
}
lcd.clear();
lcd.setCursor(0, 0);    // 커서를 5, 0에 가져다 놓아라. (열, 행)
lcd.print("Command = "); 
lcd.setCursor(11, 0);    // 비번1자리
lcd.print(pass1); 
lcd.setCursor(12, 0);    // 비번2자리
lcd.print(pass2);
lcd.setCursor(13, 0);    // 비번3자리
lcd.print(pass3); 
lcd.setCursor(14, 0);    // 비번4자리
lcd.print(pass4);

delay(500);

//잠그기
if(cmd == lock1) { 
  count=1;
  lock = 0;
  pass1='\0';
  pass2='\0';
  pass3='\0';
  pass4='\0';
  Serial.println("잠김");
  bluetooth.println("잠김");
  lcd.clear();            // 글자를 모두 지워라.
  lcd.setCursor(4, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print("Door Lock"); // Codingrun을 입력해라.
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);
  solenoid_OFF();
  LCSound();
  ledlow();
}

//열기
if(pass1 == success1 && pass2 == success2 && pass3 == success3 && pass4 == success4) {  // pwd변수와 내가 설정한 비밀번호를 비교하는 조건문
  Serial.println("열림");
  bluetooth.println("열림");
  lcd.clear();
  lcd.setCursor(4, 0);   
  lcd.print("Door Open");
  digitalWrite(LED1,HIGH); digitalWrite(LED2,HIGH); digitalWrite(LED3,HIGH); digitalWrite(LED4,HIGH);
  solenoid_ON();
  OPSound();  
  ledlow();
  lock = 1;
  if(count=5){
      count=0;
    }
}
}

char key = keypad.getKey();
if (key) {
  //입력초기화
if(key == '*') {  
  Serial.println("입력 초기화");
  bluetooth.println("입력 초기화");
  lcd.clear();            // 글자를 모두 지워라.
  ledlow();
  count=0;
  pass1='\0';
  pass2='\0';
  pass3='\0';
  pass4='\0';
}
  
  count=count+1;
  Serial.print("Command = ");
  Serial.println(key);

if(count==2){
  digitalWrite(LED1,HIGH);
   ton();
   pass1 = key;
}
if(count==3){
  digitalWrite(LED2,HIGH);
  ton();
  pass2 = key;
}
if(count==4){
  digitalWrite(LED3,HIGH);
  ton();
  pass3 = key;
}
if(count==5){
  digitalWrite(LED4,HIGH);
  ton();
  pass4 = key;
 delay(700);
  ledlow();
if(count=5){
  count=1;
  }
}
lcd.clear();
lcd.setCursor(0, 0);    // 커서를 5, 0에 가져다 놓아라. (열, 행)
  lcd.print("Command = ");     // 5, 0에 Hi ^^를 출력해라.
  lcd.setCursor(11, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print(pass1); // Codingrun을 입력해라.
  lcd.setCursor(12, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print(pass2); // Codingrun을 입력해라.
  lcd.setCursor(13, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print(pass3); // Codingrun을 입력해라.
  lcd.setCursor(14, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print(pass4); // Codingrun을 입력해라.
  delay(500);

//열기
if(pass1 == success1 && pass2 == success2 && pass3 == success3 && pass4 == success4) {  // pwd변수와 내가 설정한 비밀번호를 비교하는 조건문
  Serial.println("열림");
  bluetooth.println("열림");
  lcd.clear();            // 글자를 모두 지워라.
  lcd.setCursor(4, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print("Door Open"); // Codingrun을 입력해라.
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);
  solenoid_ON();
  OPSound();
  ledlow();
  lock = 1;
  if(count=5){
      count=1;
    }
}

//잠그기
if(lock == 1) {
if(key == lock1) { 
  Serial.println("잠김");
  bluetooth.println("잠김");
  lcd.clear();            // 글자를 모두 지워라.
  lcd.setCursor(4, 0);    // 커서를 3, 1로 가져다 놓아라. (열, 행)
  lcd.print("Door Lock"); // Codingrun을 입력해라.
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);
  solenoid_OFF();
  LCSound();
  ledlow();
  count=0;
  lock = 0;
  pass1='\0';
  pass2='\0';
  pass3='\0';
  pass4='\0';
}
}
}
}

void ledlow(){
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);

  //led를 다 꺼주는 함수
}

void ton() {
  tone(piezo, 700);
  delay(100);
  noTone(piezo);
}

void OPSound() {
  tone(piezo, 392, 500);
  delay(100);
  tone(piezo, 440, 500);
  delay(100);
  tone(piezo, 494, 500);
}

void LCSound() {
  tone(piezo, 494, 500);
  delay(100);  
  tone(piezo, 440, 500);
  delay(100);
  tone(piezo, 392, 500);
}

void solenoid_ON(){
 digitalWrite(solenoidPin, LOW);   //솔레노이드 모터를 작동시킨다.
}

void solenoid_OFF(){
  digitalWrite(solenoidPin, HIGH);    //솔레노이드 모터를 멈춘다.
}
//lcd 초기화면
void lcdset() {
lcd.setCursor(4, 0);
lcd.print("Arduino");
lcd.setCursor(4, 1);
lcd.print("DoorLock");
}


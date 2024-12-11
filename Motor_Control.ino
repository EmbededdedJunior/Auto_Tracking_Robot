//motor port
const int M_L_PWM = 2;
const int M_L_1 = 3;
const int M_L_2 = 4;

const int M_R_1 = 5;
const int M_R_2 = 6;
const int M_R_PWM = 7;

const int Buzzer = 8;

int Motor_Speed = 50;

char pidata; //pi에서오는데이터 저장 변수
char Pondata; //폰에서오는데이터 저장 변수
char ATmega; //128에서 오는데이터 저장 변수
int color_tracking=0; //0=전진 1=좌 2=우 회전 
int mode_s=0; // 0=정지&수동 1=컬러찾기 
int k=0;

void setup() {

  Serial.begin(9600); //pc 프로그램용 

  Serial1.begin(9600);  //라즈베리파와 통신 시리얼통신
  Serial2.begin(9600);  //핸드폰앱이랑통신 BT
  Serial3.begin(9600);  //ATmega128 이랑통신 BT

  pinMode(M_L_1, OUTPUT);   
  pinMode(M_L_2, OUTPUT); 

  pinMode(M_R_1, OUTPUT);  
  pinMode(M_R_2, OUTPUT); 

  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

  digitalWrite(M_L_1, LOW);
  digitalWrite(M_L_2, LOW);
  analogWrite(M_L_PWM, 0);

  digitalWrite(M_R_1, LOW);
  digitalWrite(M_R_2, LOW);
  analogWrite(M_R_PWM, 0); 
  delay(100);
}

void loop() {

  if (Serial1.available()) {  
    pidata = Serial1.read();
    //Serial.print("\t| pidata:\t");
    //Serial.print(pidata); 

    if(pidata=='l'){
      color_tracking=1; 
      //완쪽가장 
    }

    if(pidata=='r'){
      color_tracking=2;
      //오른쪽가장
    }   

    if(pidata=='c'){
      color_tracking=0;
      //전진하장
    }

    if(pidata=='n'){
      color_tracking=3;
      //검출안되면 정지하장
    }

    if(pidata=='k'){
      color_tracking=4;
      //너무가까우니까 정지하장
    } 

    if(pidata=='g'){
      color_tracking=5;
      //너무가까우니까 후진하자
    }    

    if((pidata=='g')||(pidata=='k')){
       while(Serial1.available() > 0){
        Serial1.read();
       }
    }

  }
  

  Serial.print("\t| mod:\t");  
  Serial.println(color_tracking); 
  //delay(400);

  if(mode_s==1){
    pidata=0;
      if(color_tracking==0){
        forward();
      }
      if(color_tracking==1){
        turnleft();
      }
      if(color_tracking==2){
        turnright();
      }
      if(color_tracking==3){
        robotstop();
      }
      if(color_tracking==4){
        robotstop();
      }
      if(color_tracking==5){

        backward();
        delay(1000);

        robotstop();

        

      }
  }


  if (Serial2.available()) {

    Pondata = Serial2.read();

    if(Pondata=='j'){
      mode_s=0;    //수동모드,정지
      robotstop();
    }
    if(Pondata=='k'){
      mode_s=1;    //자동,컬러찾기 
      robotstop();
    }
    if((mode_s==0)&&(Pondata=='a')){
      forward();
    }
    if((mode_s==0)&&(Pondata=='b')){
      turnleft();
    }
    if((mode_s==0)&&(Pondata=='c')){
      turnright();
    }
    if((mode_s==0)&&(Pondata=='d')){
      backward();
    }
    if(Pondata=='e'){
      mode_s=0;    //수동모드,정지
      robotstop();
    } 
    if(Pondata=='f'){
      Motor_Speed=Motor_Speed+5;
      if(Motor_Speed>=250)Motor_Speed=250;
    } 
    if(Pondata=='g'){
      Motor_Speed=Motor_Speed-5;
      if(Motor_Speed<=50)Motor_Speed=50;
    } 
             
  }

  if (Serial3.available()) {

    ATmega = Serial3.read();

    if(ATmega=='a'){
      mode_s=0;    //수동모드,정지
      robotstop();
      digitalWrite(Buzzer, HIGH);
      delay(200);
      digitalWrite(Buzzer, LOW);
      delay(100);
      ATmega='c';
    }
    if(ATmega=='b'){
      digitalWrite(Buzzer, LOW);
      ATmega='c';
    }
             
  }  

////end loop
} /////end


void forward()
{
  digitalWrite(M_L_1, LOW);
  digitalWrite(M_L_2, HIGH);
  analogWrite(M_L_PWM, Motor_Speed);

  digitalWrite(M_R_1, LOW);
  digitalWrite(M_R_2, HIGH);
  analogWrite(M_R_PWM, Motor_Speed); 
}

void backward()
{
  digitalWrite(M_L_1, HIGH);
  digitalWrite(M_L_2, LOW);
  analogWrite(M_L_PWM, Motor_Speed);

  digitalWrite(M_R_1, HIGH);
  digitalWrite(M_R_2, LOW);
  analogWrite(M_R_PWM, Motor_Speed); 
}

void turnleft()
{
  digitalWrite(M_L_1, HIGH);
  digitalWrite(M_L_2, LOW);
  analogWrite(M_L_PWM, Motor_Speed);

  digitalWrite(M_R_1, LOW);
  digitalWrite(M_R_2, HIGH);
  analogWrite(M_R_PWM, Motor_Speed); 
}

void turnright()
{
  digitalWrite(M_L_1, LOW);
  digitalWrite(M_L_2, HIGH);
  analogWrite(M_L_PWM, Motor_Speed);

  digitalWrite(M_R_1, HIGH);
  digitalWrite(M_R_2, LOW);
  analogWrite(M_R_PWM, Motor_Speed); 
}

void turn_left()
{
  digitalWrite(M_L_1, LOW);
  digitalWrite(M_L_2, LOW);
  analogWrite(M_L_PWM, Motor_Speed);

  digitalWrite(M_R_1, LOW);
  digitalWrite(M_R_2, HIGH);
  analogWrite(M_R_PWM, Motor_Speed); 
}

void turn_right()
{
  digitalWrite(M_L_1, LOW);
  digitalWrite(M_L_2, HIGH);
  analogWrite(M_L_PWM, Motor_Speed);

  digitalWrite(M_R_1, LOW);
  digitalWrite(M_R_2, LOW);
  analogWrite(M_R_PWM, Motor_Speed); 
}

void robotstop()
{
  digitalWrite(M_L_1, LOW);
  digitalWrite(M_L_2, LOW);
  analogWrite(M_L_PWM, 0);

  digitalWrite(M_R_1, LOW);
  digitalWrite(M_R_2, LOW);
  analogWrite(M_R_PWM, 0); 
}

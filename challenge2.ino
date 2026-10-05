/*
  Fade (Custom Software PWM Implementation using digitalWrite & delayMicroseconds)
*/

int led = 7;           // LED가 연결된 핀
int d = 0;          // 현재 듀티비 (0 ~ 100 %)
int p = 10000;     // 주기 (us), 눈에 깜빡임이 안 보이도록 5000us (5ms) 정도로 설정 추천
int brightness = 0;    // 현재 밝기 단계 (0 ~ 100)
int fadeAmount = 1;    // 밝기가 변하는 증감 폭

void set_duty(int duty){
  d = duty;            // 매개변수 값을 전역 변수 duty에 대입하도록 수정
}

void set_period(int period){
  p = period/2;          // 매개변수 값을 전역 변수 period에 대입하도록 수정
}

// the setup routine runs once when you press reset:
void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  set_duty(10);         // 초기 듀티비 0%
  set_period(10000);    // 주기 설정 (5000us = 5ms)
}

// the loop routine runs over and over again forever:
void loop() {
  // 1. 1주기(period) 동안 켜짐(HIGH)과 꺼짐(LOW)을 비율(duty)에 맞춰 반복 출력
  // digitalWrite와 delayMicroseconds를 이용해 소프트웨어 PWM 1회 수행
  if (d > 0) {
    unsigned long high_time = (unsigned long)p * d / 100;
    unsigned long low_time = p - high_time;

    digitalWrite(led, HIGH);
    delayMicroseconds(high_time);
    
    digitalWrite(led, LOW);
    delayMicroseconds(low_time);
  } else {
    // 0%일 때는 완전히 끄고 주기만큼 대기
    digitalWrite(led, LOW);
    delayMicroseconds(p);
  }

  // 2. 밝기(brightness) 값 서서히 변경 (삼각 패턴: 0 -> 100 -> 0)
  brightness += fadeAmount;

  // 밝기가 0이 되거나 100이 되면 증감 방향을 반대로 뒤집음
  if (brightness <= 0 || brightness >= 100) {
    fadeAmount = -fadeAmount;
  }

  // 변경된 밝기를 duty에 반영 (0~100%)
  set_duty(brightness);
}

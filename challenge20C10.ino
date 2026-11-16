#include <math.h>

const int NUM_DATA = 7;
float dist[NUM_DATA] = {0.0, 5.0, 10.0, 15.0, 20.0, 25.0, 30.0};
float volt[NUM_DATA];     // 원본 입력값 (예: 635, 480...)
float norm_volt[NUM_DATA]; // 0~1 범위로 정규화한 값

// 지수 감소 모델 계수: y = a * exp(-b * x_norm) + c
float a = 30.0, b = 1.0, c = 0.0;

float alpha = 0.01;
long n_iter = 20000;
const float SCALE = 1000.0; // 입력값 스케일링 기준 (635 -> 0.635)

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("=== 적외선 센서 지수 감소 모델 (정규화 적용) ===");
  Serial.println("모델 수식: y = a * exp(-b * (x / 1000)) + c\n");
}

void loop() {
  // 1. 7지점 값 입력받기
  for (int i = 0; i < NUM_DATA; i++) {
    Serial.print((int)dist[i]);
    Serial.print("cm 지점의 측정값을 입력하세요: ");
    volt[i] = readFloatFromSerial();
    norm_volt[i] = volt[i] / SCALE; // 발산 방지를 위해 1000으로 나눔
    Serial.println(volt[i], 2);
  }

  // 2. 학습률(@) 및 반복 횟수(n) 입력
  Serial.print("\n학습률(@, 권장: 0.01)을 입력하세요: ");
  alpha = readFloatFromSerial();
  Serial.println(alpha, 6);

  Serial.print("반복 횟수(n, 권장: 30000)를 입력하세요: ");
  n_iter = (long)readFloatFromSerial();
  Serial.println(n_iter);

  // 계수 초기화
  a = 30.0;
  b = 5.0;
  c = 0.0;

  Serial.println("\n경사하강법 학습 중...");

  // 3. 경사하강법 실행
  for (long iter = 0; iter < n_iter; iter++) {
    float grad_a = 0.0;
    float grad_b = 0.0;
    float grad_c = 0.0;

    for (int i = 0; i < NUM_DATA; i++) {
      float x = norm_volt[i];
      float y = dist[i];

      float exp_val = exp(-b * x);
      float y_pred = a * exp_val + c;
      float error = y_pred - y;

      grad_a += error * exp_val;
      grad_b += error * (-a * x * exp_val);
      grad_c += error;
    }

    a -= alpha * (2.0 / NUM_DATA) * grad_a;
    b -= alpha * (2.0 / NUM_DATA) * grad_b;
    c -= alpha * (2.0 / NUM_DATA) * grad_c;

    if (isnan(a) || isinf(a) || isnan(b) || isinf(b) || isnan(c) || isinf(c)) {
      Serial.println("\n[오류] 값이 발산했습니다! 학습률(@)을 더 작게 설정해주세요.");
      break;
    }
  }

  // 4. 결과 출력 (원래 x 스케일로 환산된 b_real 함께 표시)
  if (!isnan(a) && !isinf(a) && !isnan(b) && !isinf(b)) {
    float b_real = b / SCALE;

    Serial.println("\n=== 학습 완료 ===");
    Serial.print("최종 수식: y = ");
    Serial.print(a, 4);
    Serial.print(" * exp(-");
    Serial.print(b_real, 6);
    Serial.print(" * x) + (");
    Serial.print(c, 4);
    Serial.println(")");

    Serial.println("\n[실제 거리 vs 예측 거리]");
    for (int i = 0; i < NUM_DATA; i++) {
      float pred = a * exp(-b_real * volt[i]) + c;
      Serial.print("측정값: ");
      Serial.print(volt[i], 1);
      Serial.print(" -> 실제: ");
      Serial.print(dist[i], 1);
      Serial.print("cm / 예측: ");
      Serial.print(pred, 2);
      Serial.println("cm");
    }
  }

  Serial.println("\n----------------------------------------");
  Serial.println("새로 측정하려면 아무 키나 입력 후 엔터를 누르세요.");
  while (Serial.available() == 0);
  while (Serial.available() > 0) Serial.read();
  Serial.println();
}

float readFloatFromSerial() {
  while (Serial.available() == 0);
  String input = Serial.readStringUntil('\n');
  input.trim();
  return input.toFloat();
}

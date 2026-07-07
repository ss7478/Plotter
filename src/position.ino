//Программа для работы робота ЕТЗ, 01.2026

#define TEETH 17.0    // количество зубьев ведущей шестерни
#define MOD   2.0     // модуль ведущей шестерни

// ТЕСТ ТОЧНОСТИ:
// это результаты теста
#define Y_TEST_STEPS    15000.0
#define X_TEST_STEPS    4600.0

#define Y_TEST_S        149.5
#define X_TEST_S        153.15

#define X_MARKER_OFFSET 0.8
#define Y_MARKER_OFFSET 36.6

float position_offset = 0;
const float dividing_circle = MOD * TEETH;  // диаметр делительной окружности
//const float x_mm_to_steps = (1.0 / (dividing_circle * PI)) * 3200.0; // константа для перевода расстояний из миллиметров в шаги
const float x_mm_to_steps = X_TEST_STEPS / X_TEST_S;
const float y_mm_to_steps = Y_TEST_STEPS / Y_TEST_S;
float max_speed = 20;  // переменная для хранения значения максимальной разрешенной скорости движения

void run(int spd_x, int spd_y) // функция для движения робота с определенной скоростью в шагах в секунду
{
  x_motor(spd_x);
  y_motor(spd_y);
}

void stop() // функция для остановки движения робота и блокировки двигателей
{
  run(0, 0);
}

void run_mms(float spd_x, float spd_y) // функция для движения робота с определенной скоростью в миллиметрах в секунду
{
  run(spd_x * x_mm_to_steps, spd_y * y_mm_to_steps);
}

void set_speed(int val) // функция для установки максимальной разрешенной скорости в значение val
{
  max_speed = val;
}

float get_speed()
{
  return max_speed;
}

void move(float sx, float sy) // функция для относительного перемещения на расстояние s в миллиметрах
{
  if(abs(sx) < 0.1)sx = 0;
  if(abs(sy) < 0.1)sy = 0;
  int32_t x_steps_start = x_safe_steps();
  int32_t x_steps_stop = x_steps_start + (int32_t)(sx * x_mm_to_steps);
  int32_t y_steps_start = y_safe_steps();
  int32_t y_steps_stop = y_steps_start + (int32_t)(sy * y_mm_to_steps);

  float x_moving_speed = 0;
  float y_moving_speed = 0;
  sx = abs(sx);
  sy = abs(sy);
  if (sy > sx){
    y_moving_speed = max_speed;
    x_moving_speed = max_speed * (sx / sy);
  }
  else {
    x_moving_speed = max_speed;
    y_moving_speed = max_speed * (sy / sx);
  }
  int8_t dir_x = (x_steps_start < x_steps_stop) ? 1 : -1;
  int8_t dir_y = (y_steps_start < y_steps_stop) ? 1 : -1;
  run_mms(x_moving_speed * dir_x, y_moving_speed * dir_y);
  Serial.print(x_steps_start);
  Serial.print("\t");
  Serial.print(y_steps_start);
  Serial.print("\t");
  Serial.print(x_steps_stop);
  Serial.print("\t");
  Serial.println(y_steps_stop);
  if (dir_x == 1)while(x_safe_steps() < x_steps_stop);
  else while(x_safe_steps() > x_steps_stop);
  if (dir_y == 1)while(y_safe_steps() < y_steps_stop);
  else while(y_safe_steps() > y_steps_stop);  
  stop();
}

int grey_real_x = 0;
int grey_real_y = 0;

int get_grey()
{
  return grey_real_x;
}

int32_t x_step_offset = 0;
int32_t y_step_offset = 0;

void calibration()  // функция для калибровки - определения значения границы серого, выравнивания датчика по краю линии и получения смещения в шагах от точки нуля
{
  int min_value = 1024;
  int max_value = 0;

  servo_up();
  run_mms(0, -20);
  while(endstop());
  move(0, 100);
  run_mms(20, 0);
  while(line() < GREY_DEFAULT)
  {
    min_value = min(min_value, line());
    max_value = max(max_value, line());
  }
  uint32_t timercalib = millis();
  while(millis() - timercalib < 2500)
  {
    min_value = min(min_value, line());
    max_value = max(max_value, line());
  }
  set_speed(20);
  stop();
  delay(200);
  grey_real_x = (min_value + max_value) / 2;
  run_mms(-20, 0);
  while(line() < grey_real_x);
  run_mms(20, 0);
  stop();
  delay(500);
  x_step_offset = x_safe_steps();

  move(25, 0);
  delay(200);
  run_mms(0, -20);
  min_value = 1024;
  max_value = 0;
  while(line() < GREY_DEFAULT)
  {
    min_value = min(min_value, line());
    max_value = max(max_value, line());
  }
  timercalib = millis();
  while(millis() - timercalib < 600)
  {
    min_value = min(min_value, line());
    max_value = max(max_value, line());
  }
  stop();
  grey_real_y = (min_value + max_value) / 2;
  move(0, 25);
  run_mms(0, -20);
  while(line() < grey_real_y);
  stop();
  delay(500);
  y_step_offset = y_safe_steps();
}

double sensor_x()  // функция, возвращающая текущую координату маркера
{
  return (double)(x_safe_steps() - x_step_offset) / x_mm_to_steps;
}

double sensor_y()
{
  return (double)(y_safe_steps() - y_step_offset) / y_mm_to_steps;
}

double marker_x()
{
  return (double)(x_safe_steps() - x_step_offset) / x_mm_to_steps + X_MARKER_OFFSET;
}

double marker_y()
{
  return (double)(y_safe_steps() - y_step_offset) / y_mm_to_steps + Y_MARKER_OFFSET;
}


void sensor_goto(float x, float y) // функция, отправляющая робота(маркер) в указанную координату
{
  float dx = x - sensor_x();
  float dy = y - sensor_y();
  move(dx, dy);
}

void marker_goto(float x, float y)
{
  float dx = x - marker_x();
  float dy = y - marker_y();
  move(dx, dy);
}

void home()
{
  servo_up();
  marker_goto(-100, 0);
  run_mms(0, -20);
  while(endstop());
  move(0, 10);
}

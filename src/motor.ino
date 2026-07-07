//Программа для работы робота ЕТЗ, 01.2026

#include <TimerOne.h>   // подключение библиотек для работы с таймерами
#include <TimerThree.h>
#include <TimerFour.h>

#define DIR1  9  // определение пинов для подключения драйверов двигателей
#define STEP1 8
#define DIR2  6
#define STEP2 5
#define DIR3  3
#define STEP3 2

volatile bool step_state1 = 0; // глобальные переменные для работы функций управления моторами
volatile bool step_state2 = 0;
volatile bool step_state3 = 0;

volatile int8_t x_dir = 0;
volatile int8_t y_dir = 0;

volatile int32_t x_steps = 0; // переменная, хранящая количество шагов двигателей
volatile int32_t y_steps = 0;

static inline int32_t x_safe_steps() {  // функция для получения текущего значения шагов
  noInterrupts();                     // с защитой от неатомарного чтения
  int32_t v = x_steps;                  // (срабатывание прерывания не может помешать чтению)
  interrupts();
  return v;
}

static inline int32_t y_safe_steps() {
  noInterrupts();
  int32_t v = y_steps;
  interrupts();
  return v;
}

void init_motors() // функция инициализации всех пинов, связанных с управлением шаговиками
{
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(STEP3, OUTPUT);
  pinMode(DIR3, OUTPUT);
  
  digitalWrite(STEP1, 0);
  digitalWrite(DIR1, 0);
  digitalWrite(STEP2, 0);
  digitalWrite(DIR2, 0);
  digitalWrite(STEP3, 0);
  digitalWrite(DIR3, 0);
}

void step_isr1() {  // функция прерывания для генерации шагов первого двигателя
  step_state1 = !step_state1;
  x_steps += x_dir * step_state1;
  digitalWrite(STEP1, step_state1);
}

void step_isr2() {  // функция прерывания для генерации шагов второго двигателя
  step_state2 = !step_state2;
  digitalWrite(STEP2, step_state2);
}

void step_isr3() {
  step_state3 = !step_state3;
  y_steps += y_dir * step_state3;
  digitalWrite(STEP3, step_state3);
}

void motor1(int spd) // функция управления скоростью первого двигателя
{
  static bool wsrng = 0;
  
  if (spd == 0)
  {
    if (wsrng)
    {
      Timer1.stop();
      wsrng = 0;
    }
    return;
  }
  x_dir = (spd > 0) ? 1 : -1;
  digitalWrite(DIR1, (spd > 0) ? 1 : 0);
  spd = abs(spd);

  uint32_t period = (uint32_t)500000 / (uint32_t)spd;
  // T = 1000000 / (2 * spd);

  if (!wsrng) {
    Timer1.initialize(period);
    Timer1.attachInterrupt(step_isr1);
    Timer1.start();
    wsrng = 1;
  }
  else {
    Timer1.stop();
    Timer1.initialize(period);
    Timer1.start();
  }
}

void motor2(int spd) // функция управления скоростью второго двигателя
{
  static bool wsrng = 0;
  
  if (spd == 0)
  {
    if (wsrng)
    {
      Timer3.stop();
      wsrng = 0;
    }
    return;
  }
  
  digitalWrite(DIR2, (spd > 0) ? 0 : 1);
  spd = abs(spd);

  uint32_t period = (uint32_t)500000 / (uint32_t)spd;
  // T = 1000000 / (2 * spd);
  
  if (!wsrng) {
    Timer3.initialize(period);
    Timer3.attachInterrupt(step_isr2);
    Timer3.start();
    wsrng = 1;
  }
  else {
    Timer3.stop();
    Timer3.initialize(period);
    Timer3.start();
  }
}


void motor3(int spd) // функция управления скоростью второго двигателя
{
  static bool wsrng = 0;
  
  if (spd == 0)
  {
    if (wsrng)
    {
      Timer4.stop();
      wsrng = 0;
    }
    return;
  }
  
  y_dir = (spd > 0) ? 1 : -1;
  digitalWrite(DIR3, (spd > 0) ? 0 : 1);
  spd = abs(spd);

  uint32_t period = (uint32_t)500000 / (uint32_t)spd;
  // T = 1000000 / (2 * spd);
  
  if (!wsrng) {
    Timer4.initialize(period);
    Timer4.attachInterrupt(step_isr3);
    Timer4.start();
    wsrng = 1;
  }
  else {
    Timer4.stop();
    Timer4.initialize(period);
    Timer4.start();
  }
}


void x_motor(int spd) // функция движения моторов оси x
{
  motor1(spd);
  motor2(spd);
}

void y_motor(int spd)
{
  motor3(spd);
}

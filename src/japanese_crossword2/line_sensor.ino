//Программа для работы робота ЕТЗ, 01.2026

#define LINE_PIN            A1      // пин датчика линии
#define GREY_DEFAULT        250     // приблизительное значение серого для изначального движения

#define ENDSTOP_PIN         24
int line() // функция, возвращающая значение датчика линии
{
  return analogRead(LINE_PIN);
}

bool endstop()
{
  return digitalRead(ENDSTOP_PIN);
}

void init_endstop()
{
  pinMode(ENDSTOP_PIN, INPUT);
}

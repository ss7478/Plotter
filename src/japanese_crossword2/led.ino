//Программа для работы робота ЕТЗ, 01.2026

#define LED_PIN 10 // пин сигнального светодиода

void init_led() // функция инициализации светодиода
{
  pinMode(LED_PIN, OUTPUT);
}

void led_on() // функция включения светодиода
{
  digitalWrite(LED_PIN, 1);
}

void led_off() // функция выключения светодиода
{
  digitalWrite(LED_PIN, 0);
}

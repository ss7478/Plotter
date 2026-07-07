//Программа для работы робота ЕТЗ, 01.2026

#include <Servo.h>  // подключение библиотеки для работы с сервоприводом
Servo marker; // экземпляр класса Servo для работы с сервоприводом

#define SERVO_PIN 13 // пин, в который подключен сервопривод

bool servo_down_flag = 0;

void init_servo() // функция инизиализации сервопривода
{
  pinMode(SERVO_PIN, OUTPUT);
  marker.attach(SERVO_PIN);
}

void servo_up() // функция поднятия маркера
{
//  marker.attach(SERVO_PIN);
  marker.write(120);
  delay(300);
  servo_down_flag = 0;
//  marker.detach();
}

void servo_down() // функция опускания маркера
{
//  marker.attach(SERVO_PIN);
  marker.write(30);
  delay(100);
  servo_down_flag = 1;
//  marker.detach();
}

bool getservo()
{
  return servo_down_flag;
}

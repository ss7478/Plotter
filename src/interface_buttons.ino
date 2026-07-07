//Программа для работы робота ЕТЗ, 01.2026

#define SELECTBTN 27 // пин кнопки выбора задачи
#define STARTBTN  29 // пин кнопки запуска задачи

void init_buttons() // функция инициализации кнопок
{
  pinMode(SELECTBTN, INPUT);
  pinMode(SELECTBTN, INPUT);
}

bool select_value() // функция получения значения кнопки выбора задачи
{
  return !digitalRead(SELECTBTN);
}

bool start_value() // функция получения значения кнопки запуска задачи
{
  return !digitalRead(STARTBTN);
}

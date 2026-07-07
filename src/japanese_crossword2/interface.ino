//Программа для работы робота ЕТЗ, 01.2026

int8_t select_mission() // функция выбора задачи для выполнения
{
  int8_t mission_value = 1;
  while(1)
  {
    if (start_value())return mission_value;
    if (select_value())
    {
      mission_value++;
      while(select_value());
    }
    if (mission_value > 9)mission_value = 0;
    displayDigit(mission_value);
  }
}

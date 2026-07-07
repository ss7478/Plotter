  //Программа для работы робота ЕТЗ, 01.2026
#include "header.h"

void setup() {    // инициализация:
  init_motors();  // моторов
  init_servo();   // сервопривода
  init_plugs();   // разъемов
  init_7seg();    // 7-сегментного индикатора
  init_buttons(); // кнопок
  init_led();     // светодиодов
  init_endstop(); // концевика
  servo_up();     // поднятие маркера перед перемещениями
  Serial.begin(115200);
}

//функции задач:

// функция для сканирования "японского кроссворда"
int scan(int x, int y)
{
  bool flag_top = 0;
  bool flag_bottom = 0;
  sensor_goto(x, y);
  move(5, 0);
  if(line() > get_grey())flag_top = 1;
  move(-10, 0);
  if(line() > get_grey())flag_bottom = 1;
  if(!flag_top && !flag_bottom){
    displayDigit(0);
    return 0;
  }
  if(flag_top && !flag_bottom){
    displayDigit(2);
    return 2;
  }
  if(!flag_top && flag_bottom){
    displayDigit(1);
    return 1;
  } 
  displayDigit(3);
  return 3;
}



bool match_col(int table[][3], int i, int a, int b, int c) {
  return table[i][0] == a && table[i][1] == b && table[i][2] == c;
}

bool match_row(int table[][3], int i, int a, int b, int c) {
  return table[0][i] == a && table[1][i] == b && table[2][i] == c;
}

// функции задач, запускаемых из интерфейса по кнопкам
void task1()
{
  calibration();
  int table[3][3] = {
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1}
  };
  int scan_col[3] = {0, 0, 0};
  int scan_row[3] = {0, 0, 0};
  
  scan_col[2] = scan(50, 30);
  scan_col[1] = scan(80, 30);
  scan_col[0] = scan(110, 30);
  scan_row[0] = scan(136, 50);
  scan_row[1] = scan(136, 80);
  scan_row[2] = scan(136, 110);

  for(int i = 0; i < 3; i++) {
    if(scan_col[i] == 3) {
      table[i][0] = 1;
      table[i][1] = 1;
      table[i][2] = 1;
    }
  }
  
  for(int i = 0; i < 3; i++) {
    if(scan_row[i] == 3) {
      table[0][i] = 1;
      table[1][i] = 1;
      table[2][i] = 1;
    }
  }

  for(int i = 0; i < 3; i++)
  {
    if(scan_col[i] == 1)
    {
      bool flag1 = 0;
      for(int j = 0; j < 3; j++)
        if(table[i][j] == 1)
          flag1 = 1;
      if(flag1)
      {  for(int j = 0; j < 3; j++)
          if(table[i][j] == -1)table[i][j] = 0;}
      else{
        int countzeros = 0;
        for(int j = 0; j < 3; j++)
          countzeros += (table[i][j] == 0);
        if (countzeros == 2)
          for(int j = 0; j < 3; j++)
            if(table[i][j] == -1)table[i][j] = 1;
      }
    }
  }

  for(int i = 0; i < 3; i++)
  {
    if(scan_row[i] == 1)
    {
      bool flag1 = 0;
      for(int j = 0; j < 3; j++)
        if (table[j][i] == 1)
          flag1 = 1;
      if(flag1)
      {  for(int j = 0; j < 3; j++)
          if (table[j][i] == -1) table[j][i] = 0;}
      else {
        int countzeros = 0;
        for(int j = 0; j < 3; j++)
          countzeros += (table[j][i] == 0);
        if (countzeros == 2)
          for(int j = 0; j < 3; j++)
            if(table[j][i] == -1) table[j][i] = 1;
      }   
    }
  }

   for(int i = 0; i < 3; i++)
  {
    if(scan_col[i] == 2)
    {
      bool countones = 0;
      for(int j = 0; j < 3; j++)
        countones += (table[i][j] == 1);
      if(countones == 2)
      {  for(int j = 0; j < 3; j++)
          if(table[i][j] == -1)table[i][j] = 0;}
      else{
        int countzeros = 0;
        for(int j = 0; j < 3; j++)
          countzeros += (table[i][j] == 0);
        if (countzeros == 1)
          for(int j = 0; j < 3; j++)
            if(table[i][j] == -1)table[i][j] = 1;
      }
    }
  }

     for(int i = 0; i < 3; i++)
  {
    if(scan_row[i] == 2)
    {
      bool countones = 0;
      for(int j = 0; j < 3; j++)
        countones += (table[j][i] == 1);
      if(countones == 2)
      {  for(int j = 0; j < 3; j++)
          if(table[j][i] == -1)table[j][i] = 0;}
      else{
        int countzeros = 0;
        for(int j = 0; j < 3; j++)
          countzeros += (table[j][i] == 0);
        if (countzeros == 1)
          for(int j = 0; j < 3; j++)
            if(table[j][i] == -1)table[j][i] = 1;
      }
    }
  }


  
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
    {
      if(table[i][j] == 1) draw_circle(110 - 30 * i, 50 + 30 * j, 3);
    }
}

void task2()
{
  calibration();
  draw_circle(100, 100, 5);
  draw_circle(30, 70, 10);
  draw_circle(70, 30, 15);
  draw_circle(70, 70, 25);
  draw_circle(90, 90, 50); 
}

void task3()
{
  calibration();
  draw_line(30, 30, 80, 30);
  draw_arc_90(80, 30, 80, 40);
  draw_line(90, 40, 90, 110);
  draw_arc_90(90, 110, 80, 110);
  draw_line(80, 120, 30, 120);
  draw_arc_90(30, 120, 30, 110);
  draw_line(20, 110, 20, 40);
  draw_arc_90(20, 40, 30, 40);
}

void task4()
{
  calibration();
}

void task5()
{
  calibration();
}

// основной исполняемый блок программы с вызовом подзадач:
void loop() {
  int i = 0;
  while(1)
  {
    i = select_mission(); // выбор задачи
    // запуск функций задач в зависимости от выбранной
    led_on();
    if(i == 0)task0();
    else if (i == 1)task1();
    else if (i == 2)task2();
    else if (i == 3)task3();
    else if (i == 4)task4();
    else if (i == 5)task5();
    delay(500);
    home();  
    led_off();
    delay(500);
  }
}


void task0() // код конкретно для выполнения тестового задания
{            // технического допуска
  calibration();
  draw_line(160, 5, 10, 5);
  draw_line(10, 5, 10, 155);
}

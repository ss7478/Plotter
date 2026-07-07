//Программа для работы робота ЕТЗ, 01.2026
void draw_point(float x, float y)  // функция рисования точки в указанной координате
{
  marker_goto(x, y);
  servo_down();
  servo_up();
}

void draw_line(float x1, float y1, float x2, float y2, bool lift_marker)  // рисование линии из одной точки в другую
{
  if(lift_marker || (abs(marker_x() - x1) > 0.1 && abs(marker_y() - y1) > 0.1))servo_up();
  marker_goto(x1, y1);
  if(lift_marker || getservo() == 0)servo_down();
  marker_goto(x2, y2);
  if(lift_marker)servo_up();
}

void draw_circle(float cx, float cy, float r) // рисование окружности с центром в точке и определенным радиусом
{
  int segments = (int)(r * 5);
  float prev_x = cx + r;
  float prev_y = cy;
  float angle_step = (2.0 * PI) / segments;
  for (int i = 1; i <= segments; i++)
  {
    float angle = angle_step * i;

    float x = cx + r * cos(angle);
    float y = cy + r * sin(angle);

    draw_line(prev_x, prev_y, x, y);
    prev_x = x;
    prev_y = y;
  }
  servo_up();
}

void draw_arc_90(float x1, float y1, float cx, float cy) // рисование дуги
{
  float r = sqrt(sq(x1-cx) + sq(y1 - cy));
  int segments = (int)(r * 1.25);
  float prev_x = x1;
  float prev_y = y1;
  float angle_step = 0.5*PI / segments;
  float start_angle = atan2(y1-cy, x1-cx);
  float end_angle = start_angle + 0.5 * PI;
  for(int i = 1; i <= segments; i++)
  {
    float angle = start_angle + angle_step * i;
    float x = cx + r * cos(angle);
    float y = cy + r * sin(angle);
    draw_line(prev_x, prev_y, x, y);
    prev_x = x;
    prev_y = y;
  }
}

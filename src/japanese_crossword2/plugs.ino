//Программа для работы робота ЕТЗ, 01.2026

#define PORT_A A9  // пин аналогового входа порта А
#define PORT_B A11 // пин цифрового входа порта Б

// void init_plugs() // инициализация пинов аналогового и цифрового входов с разъемов
// {
//   pinMode(PORT_A, INPUT);
//   pinMode(PORT_B, INPUT);
// }

void init_plugs()
{
  pinMode(PORT_A, INPUT_PULLUP);
  pinMode(PORT_B, OUTPUT);
}


// int port_a_value() // значение аналогового входа с разъема А
// {
//   return analogRead(PORT_A);
// }

int port_a_value()
{
  return digitalRead(PORT_A);
}

bool port_b_value() // значение цифрового входа с разъема Б
{
  return digitalRead(PORT_B);
}

void light_portb()
{
  digitalWrite(PORT_B, 1);
}

void dark_portb()
{
  digitalWrite(PORT_B, 0);
}

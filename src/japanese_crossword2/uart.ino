// Функция расчета CRC8 (простое XOR)
uint8_t getCRC8(uint8_t *data, uint8_t len) {
  uint8_t crc = 0x00;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
  }
  return crc;
}

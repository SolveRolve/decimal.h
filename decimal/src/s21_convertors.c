#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  // Очищаем структуру dst перед заполнением новых данных
  int status = 0;
  unsigned int max_int = 0b11111111111111111111111111111111;
  if ((unsigned int)src > max_int) status = 1;
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = 0;
  }
  // Записываем знак числа (бит 31) в dst
  dst->bits[3] = (src < 0) ? MINUS : 0;
  // Получаем абсолютное значение числа
  unsigned int abs_src = (src < 0) ? -src : src;
  // Записываем целую часть числа в dst (младшие 32 бита в bits[0])
  dst->bits[0] = abs_src;

  return status;
}
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = 0;
  unsigned int max_int = 0b11111111111111111111111111111111;
  if (src <= max_int || src >= 1e-28) {
    s21_decimal zero = {0};
    *dst = zero;
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src *= -1;
    }

    int integer = (int)src;
    float remainder = src - integer;
    int counter = 0;

    for (int i = 0; i <= 15; i++) {
      if (integer != 0) {
        counter++;
      }
      integer = integer / 10;
    }
    integer = (int)src;
    for (int i = counter - 8; i > 0; i--) {
      integer = integer / 10;
    }
    for (int i = 7 - counter; i >= 0; i--) {
      integer = integer * 10;
    }
    for (int i = 7 - counter; i >= 0; i--) {
      remainder = remainder * 10;
    }
    integer = integer + remainder;
    int round = integer % 10;
    integer = integer / 10;
    if (round >= 5) {
      integer++;
    }
    set_bit(dst, 127, sign);
    int exp = 7 - counter;
    // printf("(%d)", exp);
    if (exp >= 0) set_bit_scale(dst, exp);
    dst->bits[0] = integer;
  } else
    status = 1;
  return status;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = 0;
  int num = find_first_bit(src) / 3 + 1;
  for (int i = num - 10; i > 1; i--) {
    s21_div_10(&src);
  }
  // for(int i = 3;i>=0;i--){print_binary(src.bits[i],32);}printf("\n");
  long double result = 0;
  int integer = src.bits[0];

  int scale = get_bit_scale(src);
  result = (long double)integer;
  result /= pow(10, scale);
  int k = (int)(result * pow(10, scale + 1)) % 10;
  if (k >= 5) result += (long double)1 / pow(10, scale + 1);
  int sign = get_bit(src, 127);
  if (sign) result *= -1;
  *dst = result;

  return status;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_truncate(src, &src);
  int sign = get_bit(src, 127);
  int status = 0;
  if (src.bits[1] != 0 || src.bits[2] != 0) status = 1;
  *dst = src.bits[0];
  if (sign) *dst *= -1;
  return status;
}

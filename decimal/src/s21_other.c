#include "s21_decimal.h"

/// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
/// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int status = 0;  // Инициализируем флаг статуса значением "успех"

  if (result) {  // Проверка на нулевой указатель на результат

    s21_decimal zero = {0};
    *result = zero;

    while (get_bit_scale(value) > 0) {
      s21_div_10(&value);
    }

    *result = value;
  } else {
    status = 1;  // Устанавливаем флаг статуса на "ошибка"
  }

  return status;  // Возвращаем флаг статуса выполнения
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int status = 0;  // Инициализируем флаг статуса значением "успех"

  if (result) {  // Проверка на нулевой указатель на результат
    *result = value;
    result->bits[3] ^= MINUS;
  } else {
    status = 1;  // Устанавливаем флаг статуса на "ошибка"
  }

  return status;  // Возвращаем флаг статуса выполнения
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int status = 0;  // Инициализируем флаг статуса значением "успех"

  if (result) {  // Проверка на нулевой указатель на результат
    if (get_bit_scale(value) != 0) {
      s21_decimal buff_1 = value;
      s21_decimal buff_2 = value;

      s21_truncate(value, &buff_1);
      printf("\n");
      for (int i = 3; i >= 0; i--) {
        print_binary(buff_1.bits[i], 32);
      }
      printf("\n");

      s21_mul_10(&buff_1);
      s21_vspom_truncate(value, &buff_2);
      printf("\n");
      for (int i = 3; i >= 0; i--) {
        print_binary(buff_1.bits[i], 32);
      }
      printf("\n");

      s21_decimal diff = {0};
      minus_mashina(buff_2, buff_1, &diff);
      s21_div_10(&buff_1);
      s21_decimal one = {0};
      one.bits[0] = 1;

      if (diff.bits[0] >= 5) {
        simple_addition(buff_1, one, result);
      } else {
        *result = buff_1;
      }
    } else {
      *result = value;
    }
  } else {
    status = 1;  // Устанавливаем флаг статуса на "ошибка"
  }

  return status;  // Возвращаем флаг статуса выполнения
}

int s21_vspom_truncate(s21_decimal value, s21_decimal *result) {
  s21_decimal zero = {0};
  *result = zero;

  while (get_bit_scale(value) == 1) {
    s21_div_10(&value);
  }
  *result = value;

  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = 0;  // Инициализируем флаг статуса значением "успех"
  int sign = get_bit(value, 127);
  if (!result) {  // Проверка на нулевой указатель на результат
    status = 1;  // Устанавливаем флаг статуса на "ошибка"
  } else {
    s21_decimal buff_1 = value;
    s21_decimal buff_2 = value;

    s21_truncate(value, &buff_1);
    s21_mul_10(&buff_1);
    s21_vspom_truncate(value, &buff_2);

    s21_decimal diff = {0};
    minus_mashina(buff_2, buff_1, &diff);
    s21_div_10(&buff_1);
    s21_decimal one = {0};
    one.bits[0] = 1;

    if (diff.bits[0] > 0 && sign) {
      s21_sub(buff_1, one, result);
    } else {
      *result = buff_1;
    }
  }

  return status;  // Возвращаем флаг статуса выполнения
}

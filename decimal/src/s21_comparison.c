#include <math.h>

#include "s21_decimal.h"
//сравнение 1 если равны и 0 если нет
int s21_is_equal(s21_decimal dec_1, s21_decimal dec_2) {
  norm_func(&dec_1, &dec_2);
  int bit_0 = 0, bit_1 = 0, bit_2 = 0, bit_3 = 0, res = 0;
  bit_0 = dec_1.bits[0] - dec_2.bits[0];
  bit_1 = dec_1.bits[1] - dec_2.bits[1];
  bit_2 = dec_1.bits[2] - dec_2.bits[2];
  bit_3 = dec_1.bits[3] - dec_2.bits[3];
  if ((bit_0 == 0) && (bit_1 == 0) && (bit_2 == 0) && (bit_3 == 0))
    res = 1;
  else
    res = 0;
  return res;
}
// если дец_1<дец_2 тогда 1 если нет то 0
int s21_is_less(s21_decimal dec_1, s21_decimal dec_2) {
  norm_func(&dec_1, &dec_2);
  printf("\n");
  for (int i = 3; i >= 0; i--) {
    print_binary(dec_1.bits[i], 32);
  }
  printf("  \n");
  for (int i = 3; i >= 0; i--) {
    print_binary(dec_2.bits[i], 32);
  }
  printf("\n");
  int result = 0;
  int sign_1 = get_bit(dec_1, 127);
  int sign_2 = get_bit(dec_2, 127);

  for (int j = 95; j >= 0; j--) {
    if (get_bit(dec_1, j) > get_bit(dec_2, j)) {
      result = 0;
      j = -1;
    }
    if (j >= 0) {
      if (get_bit(dec_1, j) < get_bit(dec_2, j)) {
        result = 1;
        j = -1;
      }
    }
  }
  int flag_res = 0;
  if (sign_1 > sign_2 && !result && flag_res == 0) {
    result = 1;
    flag_res = 1;
  }
  if (sign_1 < sign_2 && result && flag_res == 0) {
    result = 0;
    flag_res = 1;
  }
  if (sign_1 && sign_2 && result && flag_res == 0) {
    result = 0;
    flag_res = 1;
  }  //
  if (sign_1 && sign_2 && !result && flag_res == 0) {
    result = 1;
    flag_res = 1;
  }  //-3 -
  if (simpl_is_equal(dec_1, dec_2) && sign_1 && flag_res == 0) {
    result = 1;
    flag_res = 1;
  }
  if (simpl_is_equal(dec_1, dec_2) && sign_2 && flag_res == 0) {
    result = 0;
    flag_res = 1;
  }

  if (chek_for_zero(dec_1) && sign_2 && result && flag_res == 0) {
    result = 0;
    flag_res = 1;
  }

  if (chek_for_zero(dec_2) && sign_1 && !result && flag_res == 0) {
    result = 1;
    flag_res = 1;
  }
  return result;
}
// если дец_1<=дец_2 тогда 1 если нет то 0
int s21_is_less_or_equal(s21_decimal dec_1, s21_decimal dec_2) {
  return (s21_is_less(dec_1, dec_2) || s21_is_equal(dec_1, dec_2));
}
// если дец_1>дец_2 тогда 1 если нет то 0
int s21_is_greater(s21_decimal dec_1, s21_decimal dec_2) {
  return (!s21_is_less(dec_1, dec_2) && !s21_is_equal(dec_1, dec_2));
}
// если дец_1>=дец_2 тогда 1 если нет то 0
int s21_is_greater_or_equal(s21_decimal dec_1, s21_decimal dec_2) {
  return (!s21_is_less(dec_1, dec_2) || s21_is_equal(dec_1, dec_2));
}
// если дец_1 != дец_2 тогда 1 если нет то 0
int s21_is_not_equal(s21_decimal dec_1, s21_decimal dec_2) {
  return (!s21_is_equal(dec_1, dec_2));
}

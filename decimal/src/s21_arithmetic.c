#include "s21_decimal.h"
// сложение децималов нужно обработать все ретурны
int s21_add(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
  s21_decimal check = {0};
  norm_func(&dec_1, &dec_2);
  // printf("\n");for(int i =
  // 3;i>=0;i--){print_binary(dec_1.bits[i],32);}printf("\n"); for(int i =
  // 3;i>=0;i--){print_binary(dec_2.bits[i],32);}printf("\n");
  int flag_add = 0;
  int value_eror = 0, flag = 0;
  int exp = get_bit_scale(dec_1);
  int sign_1 = get_bit(dec_1, 127), sign_2 = get_bit(dec_2, 127);

  if (!sign_1 && !sign_2 && flag_add == 0) {  // dec_1 +  dec_2 = result
    flag += simple_addition(dec_1, dec_2, result);
    set_bit_scale(result, exp);
    flag_add = 1;
  }

  if (sign_1 && s21_is_less_simpl(dec_1, dec_2) &&
      flag_add == 0) {                    // dec_2 - dec_1 =result (dec_2>dec_1)
    minus_mashina(dec_2, dec_1, result);  // exp
    set_bit_scale(result, exp);
    flag_add = 1;
  }
  if (sign_1 && s21_is_less_simpl(dec_2, dec_1) &&
      flag_add == 0) {  // dec_1 - dec_2 = - result (dec_2<dec_1)
    minus_mashina(dec_1, dec_2, result);  // exp
    set_bit_scale(result, exp);
    set_bit(result, 127, 1);
  }
  if (sign_1 && sign_2) {  //-dec_1 -  dec_2 = -result
    flag += simple_addition(dec_1, dec_2, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 1);
    flag_add = 1;
  }
  if (sign_2 && s21_is_less_simpl(dec_2, dec_1) &&
      flag_add == 0) {                    // dec_1 - dec_2 =result (dec_1>dec_2)
    minus_mashina(dec_1, dec_2, result);  // exp
    set_bit_scale(result, exp);
    flag_add = 1;
  }
  if (sign_2 && s21_is_less_simpl(dec_1, dec_2) &&
      flag_add == 0) {                    // dec_1 - dec_2 =result (dec_1>dec_2)
    minus_mashina(dec_2, dec_1, result);  // exp
    set_bit_scale(result, exp);
    set_bit(result, 127, 1);
    flag_add = 1;
  }

  if (flag > 0) value_eror = 1;
  if (exp > 28) {
    check = *result;
    for (int i = 0; i < 29; i++) {
      s21_div_10(&check);
    }
    if (chek_for_zero(check)) {
      value_eror = 2;  //
    }
  }
  if (value_eror == 1 && get_bit(*result, 127)) {
    value_eror = 2;
  }

  return value_eror;
}
//   вычетание  децималов нужно обработать все ретурны и проверить всю логику
int s21_sub(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
  s21_decimal check = {0};
  norm_func(&dec_1, &dec_2);
  int exp = get_bit_scale(dec_1), sign_1 = get_bit(dec_1, 127),
      sign_2 = get_bit(dec_2, 127);
  int value_eror = 0, flag = 0;
  if (sign_1 && !sign_2) {  //- dec_1  -  +dec_2 = -result
    flag += simple_addition(dec_1, dec_2, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 1);
  }

  if (!sign_1 && sign_2) {  // dec_1  -  -dec_2 = +result
    flag += simple_addition(dec_1, dec_2, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 0);
  }

  if (!sign_1 && !sign_2 &&
      s21_is_less_simpl(dec_1,
                        dec_2)) {  //+dec_2 -  +dec_1 = -result dec_1<dec_2
    minus_mashina(dec_2, dec_1, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 1);
  }
  if (!sign_1 && !sign_2 &&
      s21_is_less_simpl(dec_2,
                        dec_1)) {  //+dec_1 -  +dec_2 = -result dec_1<dec_2
    minus_mashina(dec_1, dec_2, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 0);
  }
  if (sign_1 && sign_2 &&
      s21_is_less_simpl(dec_2,
                        dec_1)) {  //+dec_1 -  +dec_2 = -result dec_1<dec_2
    minus_mashina(dec_1, dec_2, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 1);
  }
  if (sign_1 && sign_2 &&
      s21_is_less_simpl(dec_1,
                        dec_2)) {  //+dec_1 -  +dec_2 = -result dec_1<dec_2
    minus_mashina(dec_2, dec_1, result);
    set_bit_scale(result, exp);
    set_bit(result, 127, 0);
  }

  if (flag > 0) value_eror = 1;
  if (exp > 28) {
    check = *result;
    for (int i = 0; i < 29; i++) {
      s21_div_10(&check);
    }
    if (chek_for_zero(check)) {
      value_eror = 2;  //
    }
  }
  if (value_eror == 1 && get_bit(*result, 127)) {
    value_eror = 2;
  }
  return value_eror;
}
// деление децималов ретурны рабтают
int s21_div(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
  int value_eror = 0;
  if (chek_for_zero(dec_2)) value_eror = 3;
  norm_func(&dec_1, &dec_2);
  if (dec_1.bits[2] == S21_MAX_UINT && dec_2.bits[0] == 0) value_eror = 1;

  int sign_1 = get_bit(dec_1, 127), sign_2 = get_bit(dec_2, 127);

  while (s21_only_less_simpl(dec_1, dec_2)) {
    s21_mul_10(&dec_1);
  }

  int exp = get_bit_scale(dec_1) - get_bit_scale(dec_2);
  s21_decimal buffer_dec = dec_1, buffer_dec_div = dec_2, check = {0};
  int num_ = find_first_bit(buffer_dec) - find_first_bit(buffer_dec_div);
  for (int i = num_; i >= 0; i--) {
    decimal_shift_left(&buffer_dec_div, i);
    if (s21_is_less_simpl(buffer_dec_div, buffer_dec)) {
      minus_mashina(buffer_dec, buffer_dec_div, &buffer_dec);
      set_bit(result, i, 1);
    } else {
      set_bit(result, i, 0);
    }
    buffer_dec_div = dec_2;
  }
  int flag_sign = 0;
  if (sign_1 && sign_2 && flag_sign == 0) {
    set_bit(result, 127, 0);
    flag_sign = 1;
  }
  if (sign_1 != sign_2 && flag_sign == 0) {
    set_bit(result, 127, 1);
    flag_sign = 1;
  }
  if (!sign_1 && !sign_2 && flag_sign == 0) {
    set_bit(result, 127, 0);
    flag_sign = 1;
  }
  if (exp >= 0)
    set_bit_scale(result, exp);
  else {
    for (int i = exp; i <= 0; i++) {
      if (s21_mul_10(result) != 0) value_eror = 1;
    }
    set_bit_scale(result, 0);
  }
  if (exp > 28) {
    check = *result;
    for (int i = 0; i < 29; i++) {
      s21_div_10(&check);
    }
    if (chek_for_zero(check)) {
      value_eror = 2;  //
    }
  }
  if (value_eror == 1 && get_bit(*result, 127)) {
    value_eror = 2;
  }
  for (int i = 3; i >= 0; i--) {
    print_binary(result->bits[i], 32);
  }
  printf("\n");
  return value_eror;
}
//умножение децималов ретурны рабтают
int s21_mul(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
  int value_eror = 0;
  int flag_overflow = 0;

  int sign_1 = get_bit(dec_1, 127), sign_2 = get_bit(dec_2, 127);
  // norm_func(&dec_1,&dec_2);

  int exp = get_bit_scale(dec_1) + get_bit_scale(dec_2);
  s21_decimal bufer_sum = {0}, shift_dec_2 = dec_2, check = {0};
  for (int i = 0; i < 96; i++) {
    if (get_bit(dec_1, i)) {
      flag_overflow += decimal_shift_left(&shift_dec_2, i);

      check = bufer_sum;
      flag_overflow += simple_addition(bufer_sum, shift_dec_2, &bufer_sum);

      shift_dec_2 = dec_2;
    }
  }
  *result = bufer_sum;
  set_bit_scale(result, exp);
  int flag_sign = 0;
  if (sign_1 && sign_2 && flag_sign == 0) {
    set_bit(result, 127, 0);
    flag_sign = 1;
  }
  if (sign_1 != sign_2 && flag_sign == 0) {
    set_bit(result, 127, 1);
    flag_sign = 1;
  }
  if (!sign_1 && !sign_2 && flag_sign == 0) {
    set_bit(result, 127, 0);
    flag_sign = 1;
  }
  if (flag_overflow > 0) value_eror = 1;
  printf("\n");
  for (int i = 3; i >= 0; i--) {
    print_binary(result->bits[i], 32);
  }
  printf("\n");
  if (exp > 28) {
    check = bufer_sum;
    for (int i = 0; i < 29; i++) {
      s21_div_10(&check);
    }
    if (chek_for_zero(check)) {
      value_eror = 2;  //
    }
  }
  if (value_eror == 1 && get_bit(*result, 127)) {
    value_eror = 2;
  }
  return value_eror;
}
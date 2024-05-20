#ifndef C5_S21_DECIMAL_1_S21_DECIMAL_H
#define C5_S21_DECIMAL_1_S21_DECIMAL_H
#define is_nan(x) __builtin_isnan(x)
#define is_inf(x) __builtin_isinf(x)
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define S21_MAX_UINT 4294967295
#define MINUS 0x80000000  // 10000000 00000000 00000000 00000000

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} big_decimal;

int set_bit(s21_decimal *dec, int position, int value);
int get_bit_bd(big_decimal dec, int position);
int get_bit(s21_decimal dec, int position);
int set_bit_bd(big_decimal *dec, int position, int value);
int get_bit_scale(s21_decimal dec);
void set_bit_scale(s21_decimal *dec, int exp);
int get_bit_scale_bd(big_decimal dec);
void set_bit_scale_bd(big_decimal *dec, int exp);
void print_binary(unsigned int value, int dec_bits);
void decimal_shift_left_bd(big_decimal *dec, int shift);
void decimal_shift_right_bd(big_decimal *dec, int shift);
int decimal_shift_left(s21_decimal *dec, int shift);
void decimal_shift_right(s21_decimal *dec, int shift);
int s21_mul_10(s21_decimal *dec);
int s21_is_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_less(s21_decimal dec_1, s21_decimal dec_2);
int chek_for_zero(s21_decimal dec);
void s21_div_10(s21_decimal *dec);
void norm_func(s21_decimal *dec_1, s21_decimal *dec_2);
void norm_func(s21_decimal *dec_1, s21_decimal *dec_2);
void min_max_scale_dec(s21_decimal dec_1, s21_decimal dec_2,
                       s21_decimal *min_scale_dec, s21_decimal *max_scale_dec);
int simple_addition(s21_decimal dec_1, s21_decimal dec_2,
                    s21_decimal *dec_result);
int simpl_is_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_less_or_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_greater(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_greater_or_equal(s21_decimal dec_1, s21_decimal dec_2);
void minus_mashina(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result);
int s21_is_less_simpl(s21_decimal dec_1, s21_decimal dec_2);
int find_first_bit(s21_decimal dec);
int s21_add(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result);
int s21_div(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result);
int s21_only_less_simpl(s21_decimal dec_1, s21_decimal dec_2);
int s21_mul(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_sub(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_vspom_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_is_not_equal(s21_decimal dec_1, s21_decimal dec_2);
#endif  // C5_S21_DECIMAL_1_S21_DECIMAL_H

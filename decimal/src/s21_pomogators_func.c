
#include "s21_decimal.h"
// Функция для получения значения бита в заданной позиции
int get_bit(s21_decimal dec, int position) {
  // Проверяем корректность позиции (допустимы значения от 0 до 127)
  if (position < 0 || position > 127) {
    return -1;  // Возвращаем -1 в случае ошибки
  }
  int arr_index = position / 32;  // Номер массива bits
  int bit_index = position % 32;  // Номер бита внутри массива
  unsigned int bit = dec.bits[arr_index];  //Здесь мы получаем значение
                                           // 32-битного числа из массива bits,
  return (bit >> bit_index) & 1;  // Возвращаем значение бита (0 или 1)
}
// Функция для установки значения бита в заданной позиции
int set_bit(s21_decimal *dec, int position, int value) {
  // Проверяем корректность позиции (допустимы значения от 0 до 127)
  if (position < 0 || position > 127) {
    return -1;
  }
  int arr_index = position / 32;  // Номер массива bits
  int bit_index = position % 32;  // Номер бита внутри массива
  if (value == 0) {
    // Сбрасываем бит на позиции position (устанавливаем в 0)
    dec->bits[arr_index] &= ~(1u << bit_index);
  } else if (value == 1) {
    // Устанавливаем бит на позиции position (устанавливаем в 1)
    dec->bits[arr_index] |= (1u << bit_index);
  } else {
    return -1;
  }
  return (dec->bits[arr_index] >> bit_index) & 1;
}

//узнать степень в децемале
int get_bit_scale(s21_decimal dec) {
  int exponent =
      (dec.bits[3] >> 16) &
      0x0000007F;  //сдвиг до первого значащего бита и чтобы 127 не учитывался
  return exponent;
}

//установить степень в децемале
void set_bit_scale(s21_decimal *dec, int exp) {
  int bit_sign = get_bit(*dec, 127);
  int bit_ = get_bit(*dec, 126);
  dec->bits[3] = (exp << 16);
  set_bit(dec, 127, bit_sign);
  set_bit(dec, 126, bit_);
}
//печать децела по количеству битов
void print_binary(unsigned int value, int dec_bits) {
  for (int i = dec_bits - 1; i >= 0; i--) {
    putchar((value & (1u << i)) ? '1' : '0');
  }
}
// двигаем децела <<
int decimal_shift_left(s21_decimal *dec, int shift) {
  int flag = 0;
  for (int i = 0; i < shift; i++) {
    int dec_31 = get_bit(*dec, 31);
    int dec_63 = get_bit(*dec, 63);
    int dec_95 = get_bit(*dec, 65);
    dec->bits[0] <<= 1;
    dec->bits[1] <<= 1;
    set_bit(dec, 32, dec_31);
    dec->bits[2] <<= 1;
    set_bit(dec, 64, dec_63);
    if (dec_95) flag = 1;
  }
  return flag;
}
// двигаем децела >>
void decimal_shift_right(s21_decimal *dec, int shift) {
  for (int i = 0; i < shift; i++) {
    int dec_32 = get_bit(*dec, 32);
    int dec_64 = get_bit(*dec, 64);
    dec->bits[0] >>= 1;
    dec->bits[1] >>= 1;
    set_bit(dec, 31, dec_32);
    dec->bits[2] >>= 1;
    set_bit(dec, 63, dec_64);
  }
}
// умножает мантису на 10 и увеличивает экспаненту на 1
int s21_mul_10(s21_decimal *dec) {
  s21_decimal buffer_1 = *dec, buffer_2 = *dec;
  int k = 0;
  k += decimal_shift_left(&buffer_2, 3);  //<< 3
  k += decimal_shift_left(&buffer_1, 1);  //<<1
  simple_addition(buffer_1, buffer_2, dec);  //сложение полученных мантис
  int scale = get_bit_scale(*dec) + 1;  //ну и добавление к экспаненте +1
  set_bit_scale(dec, scale);
  return k;
}
//проверка мантисы на ноль, возврвщает 1 если нулевая и 0 если не нулевая
int chek_for_zero(s21_decimal dec) {
  int result = dec.bits[0] == 0 && dec.bits[2] == 0 && dec.bits[1] == 0;
  return result;
}
// делит мантису на 10 и уменьшает экспаненту на 1  
void s21_div_10(s21_decimal *dec) {
  int scale = get_bit_scale(*dec) - 1;
  int sign = get_bit(*dec, 127);
  s21_decimal ten = {0}, buffer_dec = *dec, result = {0};
  int num_ = find_first_bit(buffer_dec) - 3;
  for (int i = num_; i >= 0; i--) {
    {
      ten.bits[0] = 10;
      ten.bits[1] = 0;
      ten.bits[2] = 0;
      ten.bits[3] = 0;
    }

    decimal_shift_left(&ten, i);
    if (s21_is_less_simpl(ten, buffer_dec)) {
      minus_mashina(buffer_dec, ten, &buffer_dec);
      set_bit(&result, i, 1);
    } else {
      set_bit(&result, i, 0);
    }
  }
  *dec = result;

  if (scale >= 0) set_bit_scale(dec, scale);
  set_bit(dec, 127, sign);
}
// вроде как починил и приводит к нужной степени тоесть к наибольшей и местами
// не меняет числа
void norm_func(s21_decimal *dec_1, s21_decimal *dec_2) {
  s21_decimal min_scale_dec = {0}, max_scale_dec = {0};
  min_max_scale_dec(*dec_1, *dec_2, &min_scale_dec, &max_scale_dec);
  unsigned int buf = min_scale_dec.bits[2];

  int delta_scale =
      abs(get_bit_scale(*dec_1) -
          get_bit_scale(*dec_2));  // находим дельту между экспанентами

  while (get_bit_scale(min_scale_dec) !=
         get_bit_scale(max_scale_dec)) {  // проводим операции пока не станут
                                          // одинаковыми экспаненты
    buf = min_scale_dec.bits[2] / 10000000;
    if (get_bit_scale(min_scale_dec) + delta_scale < 29 &&
        (buf < 43)) {  //сначала всегда стараемся умножать прежде чем делить
      s21_mul_10(&min_scale_dec);
    } else {
      s21_div_10(&max_scale_dec);
    }  // делим только в случае когда мантиса переполнена или экспанента больше
       // 29

    delta_scale = abs(get_bit_scale(*dec_1) -
                      get_bit_scale(*dec_2));  // пересчитываем дельту экспанент
  }
  if (get_bit(max_scale_dec,
              126)) {  // после миксования по максимальной экспапненте
    set_bit(&max_scale_dec, 126, 0);
    // возвращем децел с маркированным 126 битом
    // и конечно же обратно делаем из этого бита ноль
    *dec_1 = max_scale_dec;
    *dec_2 = min_scale_dec;
  }
  if (get_bit(min_scale_dec, 126)) {  //та же история просто зеркальная
    set_bit(&min_scale_dec, 126, 0);
    *dec_1 = min_scale_dec;
    *dec_2 = max_scale_dec;
  }
}
//расперделяет децелы по экспаненте маркируя первый децел еденичкой на 126
//бите!!нужно будет затирать единичку
void min_max_scale_dec(s21_decimal dec_1, s21_decimal dec_2,
                       s21_decimal *min_scale_dec, s21_decimal *max_scale_dec) {
  if (get_bit_scale(dec_1) < get_bit_scale(dec_2)) {
    *min_scale_dec = dec_1;
    *max_scale_dec = dec_2;
    set_bit(min_scale_dec, 126, 1);
  }
  if (get_bit_scale(dec_1) > get_bit_scale(dec_2)) {
    *min_scale_dec = dec_2;
    *max_scale_dec = dec_1;
    set_bit(max_scale_dec, 126, 1);
  }
}
//простое сложение мантис при переполнении возвращает 1
int simple_addition(s21_decimal dec_1, s21_decimal dec_2,
                    s21_decimal *dec_result) {
  int num_buffer_1 = 0, num_buffer_2 = 0, buff_number = 0;
  int error = 0;
  for (int i = 0; i < 96; i++) {
    num_buffer_1 = get_bit(dec_1, i);
    num_buffer_2 = get_bit(dec_2, i);
    if (i == 95 && num_buffer_1 && num_buffer_2) {
      error = 1;
    }
    if (!num_buffer_1 && !num_buffer_2 && !buff_number)
      set_bit(dec_result, i, 0);  //если все нули то ставим в децел ноль
    if ((num_buffer_1 != num_buffer_2) && !buff_number)
      set_bit(dec_result, i,
              1);  //если хоть один из битов 1 и нет доп бита то единица

    if (num_buffer_1 && num_buffer_2 &&
        buff_number) {  //если есть все три бита ==1 тогда ставим единицу и
                        //запоминаем единицу
      set_bit(dec_result, i, 1);
      buff_number = 1;
    }

    if (!num_buffer_1 && !num_buffer_2 &&
        buff_number) {  //если оба бита нули тогда и есть единица ставим еденицу
      set_bit(dec_result, i, 1);
      buff_number = 0;
    }
    if (num_buffer_1 && num_buffer_2 &&
        !buff_number) {  //если оба бита == 1 то ставим 0 и закидываем единицу
      set_bit(dec_result, i, 0);
      buff_number = 1;
    }
    if ((num_buffer_1 != num_buffer_2) &&
        buff_number) {  //если оба бита == 1 то ставим 0 и закидываем единицу
      set_bit(dec_result, i, 0);
      buff_number = 1;
      if (i == 95) error = 1;
    }
  }
  return error;
}
//сравнение мантисы 1 если == и 0 если !=
int simpl_is_equal(s21_decimal dec_1, s21_decimal dec_2) {
  norm_func(&dec_1, &dec_2);
  int bit_0 = 0, bit_1 = 0, bit_2 = 0, res = 0;
  bit_0 = dec_1.bits[0] - dec_2.bits[0];
  bit_1 = dec_1.bits[1] - dec_2.bits[1];
  bit_2 = dec_1.bits[2] - dec_2.bits[2];
  if ((bit_0 == 0) && (bit_1 == 0) && (bit_2 == 0))
    res = 1;
  else
    res = 0;
  return res;
}
//выкидывает номер первой единице в децеле начиная с слева на право
int find_first_bit(s21_decimal dec) {
  int number_bit = 0;
  for (int i = 95; i >= 0; i--) {
    if (!number_bit && get_bit(dec, i)) {
      number_bit = i;
    }
  }
  return number_bit;
}
// dec_1 - dec_2 = result
void minus_mashina(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
  for (int i = 0; i < 96; i++) {
    if (get_bit(dec_1, i) && get_bit(dec_2, i)) set_bit(result, i, 0);  // 1-1=0
    if (!get_bit(dec_1, i) && get_bit(dec_2, i)) {  // 0-1= 0
      int j = i, flag = 0;
      while (!get_bit(dec_1, j) && flag != 1) {
        set_bit(&dec_1, j, 1);
        j += 1;
        if (get_bit(dec_1, j)) {
          set_bit(&dec_1, j, 0);
          flag = 1;
        }
        set_bit(result, i, 1);
      }
    }
    if (get_bit(dec_1, i) && !get_bit(dec_2, i))
      set_bit(result, i, 1);  // 1-0=1
    if (!get_bit(dec_1, i) && !get_bit(dec_2, i))
      set_bit(result, i, 0);  // 0-0=0 простое закончилось
  }
}
// возвращает единицу если дес_1 <= дес_2
int s21_is_less_simpl(s21_decimal dec_1, s21_decimal dec_2) {
  int result = 1;
  for (int j = 95; j >= 0; j--) {
    if (get_bit(dec_1, j) >
        get_bit(dec_2, j)) {  //перебирает единицы у кого встретиться старше
                              //когда у другого там ноль
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
  return result;
}
int s21_only_less_simpl(s21_decimal dec_1, s21_decimal dec_2) {
  int result = 0;
  for (int j = 95; j >= 0; j--) {
    if (get_bit(dec_1, j) >
        get_bit(dec_2, j)) {  //перебирает единицы у кого встретиться старше
                              //когда у другого там ноль
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
  return result;
}
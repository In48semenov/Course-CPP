#include "numbers.h"

namespace mse {

std::size_t sum_digits(std::int32_t number) {
  size_t sum = 0;

  while (number != 0) {
    if (number > 0) {
      sum += number % 10;
    } else {
      sum -= number % 10;
    }
    number /= 10;
  }

  return sum;
}

void shift_left_by_one(int *m, size_t curr_idx, size_t &curr_size) {
  for (; curr_idx < curr_size; ++curr_idx) {
    if ((curr_idx + 1) < curr_size) {
      m[curr_idx] = m[curr_idx + 1];
    }
  }
  --curr_size;
}

std::size_t count_primes(std::uint32_t limit) {
  if (limit < 2) {
    return 0;
  }

  size_t result_count = limit - 2;
  int *massive = new int[result_count];

  // insert elements in massive
  for (uint32_t i = 0; i < result_count; ++i) {
    massive[i] = i + 2;
  }

  for (size_t i = 0; i < result_count; ++i) {
    for (size_t j = i + 1; j < result_count; ++j) {
      if (massive[j] % massive[i] == 0) {
        shift_left_by_one(massive, j, result_count);
        --j;
      }
    }
  }

  return result_count;
}

std::int32_t divide(std::int32_t a, std::int32_t b) {
  // I'm sorry ;(
  if (a == std::numeric_limits<int>::min() && b == -1)
    return std::numeric_limits<int>::max();

  if (a == 0) return 0;

  bool negative = false;
  if ((a > 0 && b < 0) || (a < 0 && b > 0)) {
    negative = true;
  }

  unsigned int a_positive = (a < 0) ? -a : a;
  unsigned int b_positive = (b < 0) ? -b : b;

  int power_max = 0;
  int b_even = ((b_positive % 2) == 0) ? b_positive : b_positive - 1;
  for (int i = 0; i <= 31; ++i) {
    if (((1 << i) & b_even) != 0) {
      power_max = i;
    }
  }

  int result = 0;
  for (int i = 31 - power_max; i >= 0; --i) {
    if ((b_positive << i) <= a_positive) {
      result += (1 << i);
      a_positive -= (b_positive << i);
    }
  }

  if (negative) {
    return -result;
  } else {
    return result;
  }
}

int32_t dict_roman_int(char symbol) {
  switch (symbol) {
  case 'I':
    return 1;
  case 'V':
    return 5;
  case 'X':
    return 10;
  case 'L':
    return 50;
  case 'C':
    return 100;
  case 'D':
    return 500;
  case 'M':
    return 1000;
  default:
    return 0;
  }
}

std::int32_t roman_to_integer(const std::string &number) {
  int32_t value_integer = 0;
  for (uint64_t i = 0; i < number.length(); i++) {
    if (dict_roman_int(number[i + 1]) <= dict_roman_int(number[i])) {
      value_integer += dict_roman_int(number[i]);
    } else {
      value_integer -= dict_roman_int(number[i]);
    }
  }
  return value_integer;
}

} // namespace mse

#include "arrays.h"

namespace mse {

// for function sort
void find_insertion_index(std::vector<std::int32_t> &elements,
                          int valueToInsert, int &insertIndexint) {
  for (size_t idx = 0; idx < elements.size(); ++idx) {
    if (elements[idx] > valueToInsert) {
      insertIndexint = (int)idx;
      break;
    }
  }
}

// for function sort
void insert(std::vector<std::int32_t> &elements, int insertIndexAt,
            int insertIndexFrom) {
  int tmp = elements[insertIndexAt];
  elements[insertIndexAt] = elements[insertIndexFrom];

  for (; insertIndexFrom > insertIndexAt; insertIndexFrom--) {
    elements[insertIndexFrom] = elements[insertIndexFrom - 1];
  }

  elements[insertIndexAt + 1] = tmp;
}

// Сортировка вставками. Сложность O(n^2), память O(1)
void sort(std::vector<std::int32_t> &elements) {
  size_t sortedRangeEndIndex = 1;
  int insertIndexint;

  while (sortedRangeEndIndex < elements.size()) {

    if (elements[sortedRangeEndIndex] < elements[sortedRangeEndIndex - 1]) {
      find_insertion_index(elements, elements[sortedRangeEndIndex],
                           insertIndexint);
      insert(elements, insertIndexint, sortedRangeEndIndex);
    }
    ++sortedRangeEndIndex;
  }
}

std::int32_t trace(const std::vector<std::vector<std::int32_t>> &matrix) {
  int32_t result = 0;
  for (size_t idx_raw = 0; idx_raw < matrix.size(); ++idx_raw) {
    if (idx_raw < matrix[idx_raw].size()) {
      result += matrix[idx_raw][idx_raw];
    }
  }
  return result;
}

std::int32_t find_unique_element(const std::vector<std::int32_t> &elements) {
  if (elements.size() == 1) {
    return elements[0];
  }
  for (size_t idx = 0; idx < elements.size(); ++idx) {
    for (size_t jdx = 0; jdx < elements.size(); ++jdx) {
      if (idx != jdx) {
        if (elements[idx] == elements[jdx]) {
          break;
        } else if (jdx == elements.size() - 1) {
          return elements[idx];
        }
      }
    }
  }
  return 0; // т.к. до сюда не дойдет (Эта задача предполагает, что все элементы
            // входного массива, кроме одного, имеют пару (см. список тест.
            // кейсов).)
}

// for function find_max_square
size_t find_square(const std::vector<std::vector<std::int8_t>> &matrix,
                   size_t max_len_square, size_t start_raw, size_t start_col) {
  bool find_zero = false;
  if (max_len_square > 1) {
    for (size_t idx_raw = 0; idx_raw < max_len_square; ++idx_raw) {
      if (!find_zero) {
        for (size_t idx_col = 0; idx_col < max_len_square; ++idx_col) {
          if (matrix[idx_raw + start_raw][idx_col + start_col] == 0) {
            find_zero = true;
            break;
          }
        }
      } else {
        break;
      }
    }
  } else {
    size_t idx_col = 0;
    while (idx_col < max_len_square) {
      if (matrix[start_raw][idx_col + start_col] == 0) {
        find_zero = true;
      } else if (matrix[start_raw][idx_col + start_col] == 1) {
        find_zero = false;
        break;
      }
      ++idx_col;
    }
  }

  return find_zero ? (size_t)0 : (size_t)(max_len_square * max_len_square);
}

// So far implemented without recurrence relations
std::size_t
find_max_square(const std::vector<std::vector<std::int8_t>> &matrix) {

  if (matrix.size() == 0) {
    return (size_t)0;
  }

  size_t result = 0;

  size_t max_len_square =
      matrix.size() > matrix[0].size() ? matrix[0].size() : matrix.size();

  while (max_len_square > 0) {
    size_t start_raw = 0;
    while (start_raw + max_len_square <= matrix.size()) {
      size_t start_col = 0;
      if (result != 0) {
        break;
      }
      while (start_col + max_len_square <= matrix[0].size()) {
        result = find_square(matrix, max_len_square, start_raw, start_col);
        if (result != 0) {
          break;
        }
        ++start_col;
      }
      ++start_raw;
    }
    --max_len_square;
  }

  return result;
}

} // namespace mse

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Int2dArray {
  explicit Int2dArray(size_t raw, size_t col)
      : raw_(raw), col_(col), data_(new double *[raw]) {
    data_[0] = new double[raw_ * col_];
    for (size_t idx = 1; idx != raw_; ++idx) {
      data_[idx] = data_[idx - 1] + col_;
    }
    for (size_t raw_idx = 0; raw_idx != raw_; ++raw_idx) {
      for (size_t col_idx = 0; col_idx != col_; ++col_idx) {
        data_[raw_idx][col_idx] = (double)0;
      }
    }
  }

  Int2dArray(Int2dArray const &a)
      : raw_(a.raw_), col_(a.col_), data_(new double *[raw_]) {
    data_[0] = new double[raw_ * col_];
    for (size_t idx = 1; idx != raw_; ++idx) {
      data_[idx] = data_[idx - 1] + col_;
    }
    for (size_t raw_idx = 0; raw_idx != raw_; ++raw_idx) {
      for (size_t col_idx = 0; col_idx != col_; ++col_idx) {
        data_[raw_idx][col_idx] = (double)a.data_[raw_idx][col_idx];
      }
    }
  }

  ~Int2dArray() {
    delete[] data_[0];
    delete[] data_;
  }

  Int2dArray &operator=(Int2dArray const &a) {
    if (this != &a)
      Int2dArray(a).swap_(*this);
    return *this;
  }

  void swap_(Int2dArray &a) {
    swap(raw_, a.raw_);
    swap(col_, a.col_);
    swap(data_, a.data_);
  }

  size_t raw() const { return raw_; }

  size_t col() const { return col_; }

  double get(size_t raw_idx, size_t col_idx) const {
    return data_[raw_idx][col_idx];
  }

  void set_element(size_t raw_idx, size_t col_idx, double element) {
    data_[raw_idx][col_idx] = element;
  }

private:
  size_t raw_;
  size_t col_;
  double **data_;
};

void print_result(Int2dArray matrices) {
  cout << matrices.raw() << " " << matrices.col() << endl;
  for (size_t i = 0; i != matrices.raw(); ++i) {
    for (size_t j = 0; j != matrices.col(); ++j) {
      if (j == (matrices.col() - 1)) {
        cout << matrices.get(i, j) << endl;
      } else {
        cout << matrices.get(i, j) << " ";
      }
    }
  }
}

Int2dArray add(Int2dArray m1, Int2dArray m2) {
  for (size_t raw_idx = 0; raw_idx != m1.raw(); ++raw_idx) {
    for (size_t col_idx = 0; col_idx != m1.col(); ++col_idx) {
      m1.set_element(raw_idx, col_idx,
                  m1.get(raw_idx, col_idx) + m2.get(raw_idx, col_idx));
    }
  }
  return m1;
}

Int2dArray mult(Int2dArray m1, Int2dArray m2) {
  Int2dArray tmp_m(m1.raw(), m2.col());
  size_t idx_raw_1 = 0;
  while (idx_raw_1 < m1.raw()) {
    for (size_t col_idx_2 = 0; col_idx_2 != m2.col(); ++col_idx_2) {
      double tmp_value = 0;
      for (size_t col_idx_1 = 0; col_idx_1 != m1.col(); ++col_idx_1) {
        tmp_value +=
            m1.get(idx_raw_1, col_idx_1) * m2.get(col_idx_1, col_idx_2);
      }
      tmp_m.set_element(idx_raw_1, col_idx_2, tmp_value);
    }
    ++idx_raw_1;
  }
  return tmp_m;
}

int checker_command(char *path) {
  const char *cmd_add = "--add";
  const char *cmd_mult = "--mult";
  if (strcmp(path, cmd_add) == 0) {
    return 0;
  } else if (strcmp(path, cmd_mult) == 0) {
    return 1;
  } else {
    cout << "Ошибка: аргументы командной строки не соответствую формату!"
         << endl;
    return -1;
  }
}

int checker_open(char *path, size_t &row, size_t &col) {
  fstream fs;
  fs.open(path, fstream::in);
  if (!fs.is_open()) {
    cout << "Ошибка открытия файла!" << endl;
    fs.close();
    return -1;
  } else {
    int idx = 0;
    while (!fs.eof()) {
      if (idx == 0) {
        fs >> row;
      } else if (idx == 1) {
        fs >> col;
      } else {
        break;
      }
      ++idx;
    }
    fs.close();
    return 0;
  }
}

Int2dArray parse_file(char *path, size_t row, size_t col) {
  fstream fs;
  fs.open(path, fstream::in);
  size_t tmp_raw;
  size_t tmp_col;
  double tmp_el;
  Int2dArray mat(row, col);
  size_t idx = 0;
  while (!fs.eof()) {
    if (idx == 0) {
      fs >> tmp_raw;
    } else if (idx == 1) {
      fs >> tmp_col;
    } else if (idx < (row * col + 2)) {
      fs >> tmp_el;
      mat.set_element((size_t)((idx - 2) / col), (size_t)((idx - 2) % col), tmp_el);
    } else {
      break;
    }
    ++idx;
  }
  fs.close();
  return mat;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  /* command_idx: -1 - None, 0 - add, 1 - mult */
  if (argc % 2 == 0) {
    int curr_command = -1;
    Int2dArray main_matrices(1, 1);
    for (auto i = 1; i < argc; ++i) {
      Int2dArray curr_matrices(1, 1);
      if ((argv[i][0] == '-') && (i % 2 == 0)) {
        curr_command = checker_command(argv[i]);
        cout << curr_command << endl;
        if (curr_command == -1) {
          return -1;
        }
      } else if (i % 2 == 1) {
        size_t row = 0;
        size_t col = 0;
        if (checker_open(argv[i], row, col) == 0) {
          if (i == 1) {
            main_matrices = parse_file(argv[i], row, col);
          } else {
            curr_matrices = parse_file(argv[i], row, col);
            if (curr_command == 0) {
              if ((main_matrices.raw() == curr_matrices.raw()) &&
                  (main_matrices.col() == curr_matrices.col())) {
                main_matrices = add(main_matrices, curr_matrices);
              } else {
                return -1;
              }
            } else {
              if (main_matrices.col() == curr_matrices.raw()) {
                main_matrices = mult(main_matrices, curr_matrices);
              } else {
                return -1;
              }
            }
          }
        } else {
          return -1;
        }
      } else {
        return -1;
      }
    }

    print_result(main_matrices);
    return 0;
  } else {
    return -1;
  }
}
#ifndef MATRIX_ARRAY_MATRIX_ARRAY_H
#define MATRIX_ARRAY_MATRIX_ARRAY_H

#include <iostream>
#include <istream>
#include <ostream>
//#include <util/constants.h>

class MatrixArrayIsDegenerateError : public std::runtime_error {
public:
    MatrixArrayIsDegenerateError() : std::runtime_error("MatrixArrayIsDegenerateError") {
    }
};

class MatrixArrayOutOfRange : public std::out_of_range {
public:
    MatrixArrayOutOfRange() : std::out_of_range("MatrixArrayOutOfRange") {
    }
};

template <class T, size_t N, size_t M>
struct MatrixArray {
private:
public:
    T matrix[N][M];
    MatrixArray();
    // MatrixArray(std::initializer_list<T> l);
    // MatrixArray(std::initializer_list<std::initializer_list<T>> l);
    size_t RowsNumber() const;
    size_t ColumnsNumber() const;
    T& operator()(size_t idx_row, size_t idx_col);
    const T& operator()(size_t idx_row, size_t idx_col) const;
    T& At(size_t idx_row, size_t idx_col);
    T At(size_t idx_row, size_t idx_col) const;
    MatrixArray<T, M, N> GetTransposed() const;
    MatrixArray<T, N, M> operator-() const;
    MatrixArray<T, N, M>& operator+=(const MatrixArray<T, N, M>& other);
    MatrixArray<T, N, M>& operator-=(const MatrixArray<T, N, M>& other);
    MatrixArray<T, N, M>& operator*=(const MatrixArray<T, M, M>& other);
    MatrixArray<T, N, M>& operator*=(const T num);
    MatrixArray<T, N, M>& operator/=(const T num);
};

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>::MatrixArray() {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            matrix[i][j] = 0;
            // matrix[i][j] = kZero<T>;
        }
    }
}

// template <class T, size_t N, size_t M>
// MatrixArray<T, N, M>::MatrixArray(std::initializer_list<T> l) {
//     size_t j = 0;
//     for (auto y : l) {
//         matrix[0][j] = y;
//         ++j;
//     }
// }

// template <class T, size_t N, size_t M>
// MatrixArray<T, N, M>::MatrixArray(std::initializer_list<std::initializer_list<T>> l) {
//     size_t i = 0;
//     for (auto x : l) {
//         size_t j = 0;
//         for (auto y : x) {
//             matrix[i][j] = y;
//             ++j;
//         }
//         ++i;
//     }
// }

template <class T, size_t N, size_t M>
size_t MatrixArray<T, N, M>::RowsNumber() const {
    return N;
}

template <class T, size_t N, size_t M>
size_t MatrixArray<T, N, M>::ColumnsNumber() const {
    return M;
}

template <class T, size_t N, size_t M>
T& MatrixArray<T, N, M>::operator()(size_t idx_row, size_t idx_col) {
    return matrix[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
const T& MatrixArray<T, N, M>::operator()(size_t idx_row, size_t idx_col) const {
    return matrix[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
T& MatrixArray<T, N, M>::At(size_t idx_row, size_t idx_col) {
    if (idx_row >= N || idx_col >= M) {
        throw MatrixArrayOutOfRange{};
    }
    return matrix[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::At(size_t idx_row, size_t idx_col) const {
    if (idx_row >= N || idx_col >= M) {
        throw MatrixArrayOutOfRange{};
    }
    return matrix[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
MatrixArray<T, M, N> MatrixArray<T, N, M>::GetTransposed() const {
    MatrixArray<T, M, N> transposed;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            transposed(j, i) = matrix[i][j];
        }
    }
    return transposed;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> MatrixArray<T, N, M>::operator-() const {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = -matrix[i][j];
        }
    }
    return result;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator+=(const MatrixArray<T, N, M>& other) {
    if (other == *this) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                matrix[i][j] *= 2;
            }
        }
    } else {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                matrix[i][j] += other(i, j);
            }
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator-=(const MatrixArray<T, N, M>& other) {
    *this += (-other);
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator*=(const MatrixArray<T, M, M>& other) {
    MatrixArray<T, N, M> copy;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            copy(i, j) = matrix[i][j];
        }
    }
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < M; ++col) {
            // T sum = kZero<T>;
            T sum = 0;
            for (size_t ind = 0; ind < M; ++ind) {
                sum += copy(row, ind) * other(ind, col);
            }
            matrix[row][col] = sum;
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator*=(const T num) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            matrix[i][j] *= num;
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator/=(const T num) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            matrix[i][j] /= num;
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator+(const MatrixArray<T, N, M>& x, const MatrixArray<T, N, M>& y) {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = x(i, j) + y(i, j);
        }
    }
    return result;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator-(const MatrixArray<T, N, M>& x, const MatrixArray<T, N, M>& y) {
    return x + (-y);
}

template <class T, size_t N, size_t M, size_t K>
MatrixArray<T, N, K> operator*(const MatrixArray<T, N, M>& x, const MatrixArray<T, M, K>& y) {
    MatrixArray<T, N, K> result;
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < K; ++col) {
            // T sum = kZero<T>;
            T sum = 0;
            for (size_t ind = 0; ind < M; ++ind) {
                sum += x(row, ind) * y(ind, col);
            }
            result(row, col) = sum;
        }
    }
    return result;
}

template <class T, class S = T, size_t N, size_t M>
MatrixArray<T, N, M> operator*(const MatrixArray<T, N, M>& x, const S num) {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = x(i, j) * num;
        }
    }
    return result;
}

template <class T, class S = T, size_t N, size_t M>
MatrixArray<T, N, M> operator*(const S num, const MatrixArray<T, N, M>& x) {
    return x * num;
}

template <class T, class S = T, size_t N, size_t M>
MatrixArray<T, N, M> operator/(const MatrixArray<T, N, M>& x, const S num) {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = x(i, j) / num;
        }
    }
    return result;
}

template <class T, size_t N, size_t M>
bool operator==(const MatrixArray<T, N, M>& x, const MatrixArray<T, N, M>& y) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (x.matrix[i][j] != y.matrix[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <class T, size_t N, size_t M>
bool operator!=(const MatrixArray<T, N, M>& x, const MatrixArray<T, N, M>& y) {
    return !(x == y);
}

template <class T, size_t N, size_t M>
std::istream& operator>>(std::istream& is, MatrixArray<T, N, M>& matr) {
    T item;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            is >> item;
            matr.matrix[i][j] = item;
        }
    }
    return is;
}

template <class T, size_t N, size_t M>
std::ostream& operator<<(std::ostream& os, const MatrixArray<T, N, M>& matr) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            os << matr.matrix[i][j];
            if (j != M - 1) {
                os << ' ';
            }
        }
        os << '\n';
    }
    return os;
}

#endif  // MATRIX_ARRAY_MATRIX_ARRAY_H

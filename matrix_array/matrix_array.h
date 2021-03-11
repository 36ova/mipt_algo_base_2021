#ifndef MATRIX_ARRAY_MATRIX_ARRAY_H
#define MATRIX_ARRAY_MATRIX_ARRAY_H

#include <iostream>
#include <istream>
#include <ostream>
#include <util/constants.h>

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
class MatrixArray {
private:
public:
    T array[N][M];

    size_t RowsNumber() const;
    size_t ColumnsNumber() const;
    T& operator()(const size_t idx_row, const size_t idx_col);
    const T& operator()(const size_t idx_row, const size_t idx_col) const;
    T& At(const size_t idx_row, const size_t idx_col);
    T At(const size_t idx_row, const size_t idx_col) const;
    MatrixArray<T, M, N> GetTransposed() const;
    MatrixArray<T, N, M> operator-() const;
    MatrixArray<T, N, M>& operator+=(const MatrixArray<T, N, M>& other);
    MatrixArray<T, N, M>& operator-=(const MatrixArray<T, N, M>& other);
    MatrixArray<T, N, M>& operator*=(const MatrixArray<T, M, M>& other);
    MatrixArray<T, N, M>& operator*=(const T number);
    MatrixArray<T, N, M>& operator/=(const T number);
};

template <class T, size_t N, size_t M>
size_t MatrixArray<T, N, M>::RowsNumber() const {
    return N;
}

template <class T, size_t N, size_t M>
size_t MatrixArray<T, N, M>::ColumnsNumber() const {
    return M;
}

template <class T, size_t N, size_t M>
T& MatrixArray<T, N, M>::operator()(const size_t idx_row, const size_t idx_col) {
    return array[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
const T& MatrixArray<T, N, M>::operator()(const size_t idx_row, const size_t idx_col) const {
    return array[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
T& MatrixArray<T, N, M>::At(const size_t idx_row, const size_t idx_col) {
    if (idx_row >= N || idx_col >= M) {
        throw MatrixArrayOutOfRange{};
    }
    return array[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
T MatrixArray<T, N, M>::At(const size_t idx_row, const size_t idx_col) const {
    if (idx_row >= N || idx_col >= M) {
        throw MatrixArrayOutOfRange{};
    }
    return array[idx_row][idx_col];
}

template <class T, size_t N, size_t M>
MatrixArray<T, M, N> MatrixArray<T, N, M>::GetTransposed() const {
    MatrixArray<T, M, N> transposed;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            transposed(j, i) = array[i][j];
        }
    }
    return transposed;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> MatrixArray<T, N, M>::operator-() const {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = -array[i][j];
        }
    }
    return result;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator+=(const MatrixArray<T, N, M>& other) {
    if (other == *this) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                array[i][j] *= 2;
            }
        }
    } else {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                array[i][j] += other(i, j);
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
            copy(i, j) = array[i][j];
        }
    }
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < M; ++col) {
            T sum = kZero<T>;
            for (size_t ind = 0; ind < M; ++ind) {
                sum += copy(row, ind) * other(ind, col);
            }
            array[row][col] = sum;
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator*=(const T number) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            array[i][j] *= number;
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M>& MatrixArray<T, N, M>::operator/=(const T number) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            array[i][j] /= number;
        }
    }
    return *this;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator+(const MatrixArray<T, N, M>& first, const MatrixArray<T, N, M>& second) {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = first(i, j) + second(i, j);
        }
    }
    return result;
}

template <class T, size_t N, size_t M>
MatrixArray<T, N, M> operator-(const MatrixArray<T, N, M>& first, const MatrixArray<T, N, M>& second) {
    return first + (-second);
}

template <class T, size_t N, size_t M, size_t K>
MatrixArray<T, N, K> operator*(const MatrixArray<T, N, M>& first, const MatrixArray<T, M, K>& second) {
    MatrixArray<T, N, K> result;
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < K; ++col) {
            T sum = kZero<T>;
            for (size_t ind = 0; ind < M; ++ind) {
                sum += first(row, ind) * second(ind, col);
            }
            result(row, col) = sum;
        }
    }
    return result;
}

template <class T, class S = T, size_t N, size_t M>
MatrixArray<T, N, M> operator*(const MatrixArray<T, N, M>& first, const S number) {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = first(i, j) * number;
        }
    }
    return result;
}

template <class T, class S = T, size_t N, size_t M>
MatrixArray<T, N, M> operator*(const S number, const MatrixArray<T, N, M>& first) {
    return first * number;
}

template <class T, class S = T, size_t N, size_t M>
MatrixArray<T, N, M> operator/(const MatrixArray<T, N, M>& first, const S number) {
    MatrixArray<T, N, M> result;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            result(i, j) = first(i, j) / number;
        }
    }
    return result;
}

template <class T, size_t N, size_t M>
bool operator==(const MatrixArray<T, N, M>& first, const MatrixArray<T, N, M>& second) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (first.array[i][j] != second.array[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <class T, size_t N, size_t M>
bool operator!=(const MatrixArray<T, N, M>& first, const MatrixArray<T, N, M>& second) {
    return !(first == second);
}

template <class T, size_t N, size_t M>
std::istream& operator>>(std::istream& is, MatrixArray<T, N, M>& matr) {
    T item;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            is >> item;
            matr.array[i][j] = item;
        }
    }
    return is;
}

template <class T, size_t N, size_t M>
std::ostream& operator<<(std::ostream& os, const MatrixArray<T, N, M>& matr) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            os << matr.array[i][j];
            if (j != M - 1) {
                os << ' ';
            }
        }
        os << '\n';
    }
    return os;
}

#endif  // MATRIX_ARRAY_MATRIX_ARRAY_H

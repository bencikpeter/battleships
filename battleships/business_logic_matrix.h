#ifndef BUSSINESS_LOGIC_MATRIX
#define BUSSINESS_LOGIC_MATRIX
#include <ostream>
namespace logic{
enum CellType { SHIP_NOT_SHOT, SHIP_SHOT, WATER_NOT_SHOT, WATER_SHOT, CLICKABLE, NOT_CLICKABLE };

inline std::ostream& operator<<(std::ostream & os, CellType & t)
{
  switch (t) {
  case SHIP_NOT_SHOT:
      os << 'S';
      break;
  case SHIP_SHOT:
      os << '#';
      break;
  case WATER_NOT_SHOT:
      os << '.';
      break;
  case WATER_SHOT:
      os << 'x';
      break;
  case CLICKABLE:
      os << 'C';
      break;
  case NOT_CLICKABLE:
      os << 'N';
      break;
  }
  return os;
}

class Matrix{
    CellType** matrix;
public:
    Matrix() = delete;
    Matrix(CellType** a): matrix(new CellType*[10]) {
        for (int i = 0; i < 10; i++){
            matrix[i] = new CellType[10];
            for (int j = 0; j < 10; j++){
                matrix[i][j] = a[i][j];
            }
        }
    }
    Matrix(const Matrix& o): matrix(new CellType*[10]) {
        for (int i = 0; i < 10; i++){
            matrix[i] = new CellType[10];
            for (int j = 0; j < 10; j++){
                matrix[i][j] = o.matrix[i][j];
            }
        }
    }

    CellType** get(){return matrix;}

    const Matrix& operator= (Matrix o){
        std::swap(o.matrix, matrix);
        return *this;
    }

    ~Matrix(){
        for (int i = 0; i < 10; i++){
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m){
        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++){
                os << m.matrix[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }
};


}
#endif // BUSSINESS_LOGIC_MATRIX


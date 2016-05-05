#ifndef BUSSINESS_LOGIC_MATRIX
#define BUSSINESS_LOGIC_MATRIX
namespace logic{
enum CellType { SHIP_NOT_SHOT, SHIP_SHOT, WATER_NOT_SHOT, WATER_SHOT, CLICKABLE, NOT_CLICKABLE };
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
};
}
#endif // BUSSINESS_LOGIC_MATRIX


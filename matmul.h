#include <cstddef>
/*
2d matrix class;
*/
template <typename T, std::size_t R, std::size_t C>
class matrix
{
    T *data; // can be changed to unique_ptr in C++1z
public:
    matrix() : data(new T[R * C]) {}

    ~matrix() { delete[] data; }

    inline T *operator[](int row)
    {
        return &data[row * C];
    }

    inline const T *operator[](int row) const
    {
        return &data[row * C];
    }

    /*
    Method for adding elements to matrix entry-wise
    */
    void addElement(int row, int col, T val)
    {
        data[row][col] = val;
    }

    /*
    Matrix addition
    */
    void add(matrix B)
    {
    }

    /*
     Matrix multiplication
     */
    void multiply(matrix B)
    {
    }
};

/*
Two approaches:
    - have methods for matrix multiplication in matrix class
    OR
    - write separate struct w accompnaying methods for matrix multiplication
*/
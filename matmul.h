#include <cstddef>
#include "iostream"

// TO-DO: figure out if using template class or float class and remove other (generally clean up)

/*
2d float matrix class;
*/
class matrix
{
public:
    int numRows;
    int numCols;
    float *data;

    matrix(int R, int C)
    {
        numRows = R, numCols = C;
        data = new float[R * C]; // Allocate memory for the matrix
    }

    // Destructor
    ~matrix()
    {
        delete[] data; // Free the allocated memory
    }

    // Copy Constructor
    matrix(const matrix &other) : numRows(other.numRows), numCols(other.numCols)
    {
        data = new float[numRows * numCols];                         // Allocate new memory
        std::copy(other.data, other.data + numRows * numCols, data); // Copy the data
    }

    // Assignment Operator
    matrix &operator=(const matrix &other)
    {
        if (this == &other)
            return *this; // Handle self-assignment

        // Free existing memory
        delete[] data;

        // Allocate new memory and copy data
        numRows = other.numRows;
        numCols = other.numCols;
        data = new float[numRows * numCols];
        std::copy(other.data, other.data + numRows * numCols, data);

        return *this;
    }

    inline float *operator[](int row)
    {
        return &data[row * numCols];
    }

    inline const float *operator[](int row) const
    {
        return &data[row * numCols];
    }

    /*
    Method for adding elements to matrix entry-wise
    */
    void addElement(int row, int col, float val)
    {
        data[row * numCols + col] = val; // Correct index calculation
    }

    // Method to get an element
    float getElement(int row, int col) const
    {
        return data[row * numCols + col]; // Correct index calculation
    }

    /*
    Matrix addition
    */
    void add(matrix B)
    // entry-wise addition
    // not sure if dims need to exactly align?
    {
    }

    /*
     Matrix multiplication
     */
    // matrix multiply(matrix B)
    // {
    matrix multiply(matrix B)
    {
        // check dims
        if (this->numCols != B.numRows)
        {
            std::cout << "err";
        }
        // Create a product matrix with the appropriate dimensions
        // matrix<T, R, BCols> product;
        matrix product(this->numRows, B.numCols);

        // multiply by B, return result
        // numCols of this needs to = numRows of B
        // for each col in B
        //  for each row in A
        //      fill col of C (result)
        for (int i = 0; i < B.numCols; i++)
        {
            // each val at
            for (int j = 0; j < this->numRows; j++)
            {
                product.addElement(j, i, this->dotRowCol(j, B, i, numCols));
            }
        }

        return product;
    }

    float dotRowCol(int row, matrix B, int col, int dim)
    {
        float product = 0; // Initialized to 0 for dot product sumfor (int i = 0; i < dim; i++)
        for (int i = 0; i < dim; i++)
        {
            product += this->getElement(row, i) * B.getElement(i, col);
        }
        return product;
    }

    /*
    Print method to display the matrix
    */
    void print() const
    {
        std::cout << "\n";
        for (int i = 0; i < numRows; i++)
        {
            for (int j = 0; j < numCols; j++)
            {
                std::cout << (*this)[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "\n";
    }
};

// OLD TEMPLATE IMPLEMENTATION
// template <typename T, std::size_t R, std::size_t C>
// class matrix
// {
// public:
//     int numRows = R;
//     int numCols = C;
//     std::shared_ptr<T[]> data;
//     matrix() : data(std::shared_ptr<T[]>(new T [R * C])) {};

//     //     // Destructor
//     ~matrix()
//     {
//         delete[] data; // Free the allocated memory
//     }

//     inline T *operator[](int row)
//     {
//         return &data[row * C];
//     }

//     inline const T *operator[](int row) const
//     {
//         return &data[row * C];
//     }

//     /*
//     Method for adding elements to matrix entry-wise
//     */
//     void addElement(int row, int col, T val)
//     {
//         data[row * numCols + col] = val;
//     }

//     T getElement(int row, int col)
//     {
//         return data[row * numCols + col];
//     }

//     /*
//     Matrix addition
//     */
//     void add(matrix B)
//     // entry-wise addition
//     // not sure if dims need to exactly align?
//     {
//     }

//     /*
//      Matrix multiplication
//      */
//     // matrix multiply(matrix B)
//     // {
//     template <std::size_t BCols>
//     matrix<T, R, BCols> multiply(matrix<T, C, BCols>& B){
//         // Create a product matrix with the appropriate dimensions
//         matrix<T, R, BCols> product;

//         // dims will be this.numRows by B.numCols
//         // need to instantiate class w different type var?
//         // matrix product = matrix<T, numRows, BCols>();

//         // multiply by B, return result
//         // numCols of this needs to = numRows of B
//         // for each col in B
//         //  for each row in A
//         //      fill col of C (result)
//         for (int i = 0; i < BCols; i ++)
//         {
//             // each val at
//             for (int j = 0; j < numRows; j ++)
//             {
//                 product.addElement(i, i, this->dotRowCol(j, B, i, numCols));
//             }
//         }

//         return product;
//     }

//     float dotRowCol(int row, matrix B, int col, int dim){
//         float product = 0; // Initialized to 0 for dot product sumfor (int i = 0; i < dim; i++)
//         for (int i = 0; i < dim; i ++)
//         {
//             product += this->getElement(row, i) * B.getElement(i, col);
//         }
//         return product;
//     }

//     /*
//     Print method to display the matrix
//     */void print() const
//     {
//         for (int i = 0; i < numRows; i++)
//         {
//             for (int j = 0; j < numCols; j++)
//             {
//                 std::cout << (*this)[i][j] << " ";
//             }
//             std::cout << std::endl;
//         }
//     }
// };
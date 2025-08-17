#include "hw1.h"

using Matrix = std::vector<std::vector<double>>;

size_t algebra::rownum(const Matrix& matrix)
{
    if (matrix.empty()) 
    {
        return 0;
    }
    return matrix.size();
}

size_t algebra::colnum(const Matrix& matrix)
{
    if (matrix.empty()) 
    {
        return 0;
    }
    return matrix[0].size();
}

std::vector<double> algebra::getcol(const Matrix& matrix,const size_t j)
{
    std::vector<double> col;
    for(const std::vector<double>& row : matrix)
    {
        col.push_back(row[j]);
    }
    return col;
}

double algebra::dotproduct(const std::vector<double>& vec1,const std::vector<double>& vec2)
{
    double dotproduct=0.0;
    if(vec1.size()!=vec2.size())
    {
        throw std::logic_error("Vector sizes do not match for dot product!");
    }
    for(size_t i=0;i<vec1.size();i++)
    {
        dotproduct+=vec1[i]*vec2[i];
    }
    return dotproduct;    
}
    
Matrix algebra::adjoint(const Matrix& matrix)
{
    Matrix adjjointmatrix=matrix;
    for (size_t i = 0; i < rownum(matrix); i++)
    {
        for (size_t j = 0; j < colnum(matrix);j++)
        {
            int sign=1;
            if((i+j)%2)
            {
                sign=-1;
            }
            adjjointmatrix[i][j]=sign*determinant(minor(matrix,j,i));
        }
    }
    return adjjointmatrix;
}    

//----------------------------------------------------------------------

Matrix algebra::zeros(size_t n, size_t m)
{
    Matrix matrix(n);
    for (std::vector<double>& row : matrix)
    {
        row=std::vector<double>(m,0);
    }
    return matrix;    
}

Matrix algebra::ones(size_t n, size_t m)
{
    Matrix matrix(n);
    for (std::vector<double>& row : matrix)
    {
        row=std::vector<double>(m,1);
    }
    return matrix;    
}

Matrix algebra::random(size_t n, size_t m, double min, double max)
{
    //logic error
    if (min > max) 
    {
        throw std::logic_error("min can't be greater than max!");
    }
    
    // 创建一个随机数设备作为种子
    std::random_device rd;
    // 使用 rd 的输出作为 mt19937 引擎的种子
    std::mt19937 gen(rd());
    // 定义一个在 [min,max] 范围内均匀分布的浮点数
    std::uniform_real_distribution<> distrib(min,max);

    Matrix matrix;
    for (int i = 0; i < n; i++) 
    {
        std::vector<double> row ;
        for (int j = 0; j < m; j++) 
        {
            row.push_back(distrib(gen));
        }
        matrix.push_back(row);
    }
    return matrix;
}

void algebra::show(const Matrix& matrix)
{
    for (const std::vector<double>& row : matrix)
    {
        for(const double& element : row)
        {
            std::cout<<std::setw(7)<<std::fixed<<std::setprecision(3)<<element;
        }
        std::cout<<std::endl;
    }
}

Matrix algebra::multiply(const Matrix& matrix, double c)
{
    Matrix mulmatrix;
    for(const std::vector<double>& row : matrix)
    {
        std::vector<double> mulrow;
        for(const double& element : row)
        {
            mulrow.push_back(element*c);
        }
        mulmatrix.push_back(mulrow);
    }
    return mulmatrix;
}

Matrix algebra::multiply(const Matrix& matrix1, const Matrix& matrix2) 
{
    
    if (colnum(matrix1) != rownum(matrix2)) 
    {
        throw std::logic_error("these two matrix can't be multiplied!");
    }

    Matrix mulmatrix;
    for(size_t i=0 ; i<rownum(matrix1) ; i++)
    {
        std::vector<double> mulrow;
        for (size_t j = 0; j < colnum(matrix2); j++)
        {
            mulrow.push_back(dotproduct(matrix1[i],getcol(matrix2,j)));
        }
        mulmatrix.push_back(mulrow);
    }    
    return mulmatrix;
}

Matrix algebra::sum(const Matrix& matrix, double c)
{
    Matrix addmatrix=zeros(rownum(matrix),colnum(matrix));
    for(size_t i=0;i<rownum(matrix);i++)
    {
        for(size_t j=0;j<colnum(matrix);j++)
        {
            addmatrix[i][j]=matrix[i][j]+c;
        }
    }
    return addmatrix;
}

Matrix algebra::sum(const Matrix &matrix1, const Matrix &matrix2)
{
    if(rownum(matrix1)!=rownum(matrix2)||colnum(matrix1)!=colnum(matrix2))
    {
        throw std::logic_error("matrixs with two different dimension can't be sumed!");
    }
    Matrix addmatrix=zeros(rownum(matrix1),colnum(matrix1));
    for(size_t i=0;i<rownum(matrix1);i++)
    {
        for(size_t j=0;j<colnum(matrix2);j++)
        {
            addmatrix[i][j]=matrix1[i][j]+matrix2[i][j];
        }
    }
    return addmatrix;
}

Matrix algebra::transpose(const Matrix &matrix)
{
    Matrix transmatrix;
    for(size_t i=0;i<colnum(matrix);i++)
    {
        transmatrix.push_back(getcol(matrix,i));
    }
    return transmatrix;
}

Matrix algebra::minor(const Matrix& matrix, size_t n, size_t m) 
{
    Matrix minormatrix=matrix;

    auto it=minormatrix.begin()+n;
    minormatrix.erase(it);

    Matrix t_minormatrix=transpose(minormatrix);
    it=t_minormatrix.begin()+m;
    t_minormatrix.erase(it);

    minormatrix=transpose(t_minormatrix);
    return minormatrix;    
}

double algebra::determinant(const Matrix &matrix)
{
    //logic error
    if (colnum(matrix) != rownum(matrix)) 
    {
        throw std::logic_error("input matrix is not a square!");
    }

    if(matrix.empty())
    {
        return 1;
    }

    if(rownum(matrix)==1)
    {
        return matrix[0][0];
    }

    double det=0.0;
    for(size_t i=0;i<colnum(matrix);i++)
    {
        double sign=1.0;
        if(i%2)
        {
            sign=-1.0;
        }
        det+=sign*matrix[0][i]*algebra::determinant(minor(matrix,0,i));
    }
    return det;
}

Matrix algebra::inverse(const Matrix& matrix)
{
    if (matrix.empty()) 
    {
        return matrix;
    }

    //logic error
    if (colnum(matrix) != rownum(matrix)) 
    {
        throw std::logic_error("non-square matrix can't be inversed!");
    }
    double det=determinant(matrix);
    if (det == 0) 
    {
        throw std::logic_error("the determinant of input matrix can't be inversed");
    }
    
    return multiply(adjoint(matrix),1/det);
}

Matrix algebra::concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) 
{
    Matrix catmatrix;
    if(axis==0)
    {
        //logic error
        if (colnum(matrix1) != colnum(matrix2)) 
        {
            throw std::logic_error("colNum of two input matrixs are different when concatenated up and down");
        }

        catmatrix=matrix1;
        for(const std::vector<double>& row : matrix2)
        {
            catmatrix.push_back(row);
        }  
    }
    else 
    {
        //logic error
        if (rownum(matrix1) != rownum(matrix2)) 
        {
            throw std::logic_error("rowNum of two input matrixs are different when concatenated alongside");
        }

        for(size_t i=0;i<rownum(matrix1);i++)
        {
            std::vector<double> row=matrix1[i];
            row.insert(row.end(),matrix2[i].begin(),matrix2[i].end());
            catmatrix.push_back(row);
        }
    }
    return catmatrix;
}

Matrix algebra::ero_swap(const Matrix &matrix, size_t r1, size_t r2) 
{
    if (r1 >= rownum(matrix)  || r2 >= rownum(matrix)) 
    {
        throw std::logic_error("toSwap row number is larger than matrix size!");
    }

    Matrix swapedmatrix;
    for (size_t i = 0; i < rownum(matrix); i++)
    {
        if(i==r1)
        {
            swapedmatrix.push_back(matrix[r2]);
        }
        else if (i==r2)
        {
            swapedmatrix.push_back(matrix[r1]);
        }
        else 
        {
            swapedmatrix.push_back(matrix[i]);
        }
    }
    return swapedmatrix;
}

Matrix algebra::ero_multiply(const Matrix &matrix, size_t r, double c)
{
    Matrix mulmatrix=matrix;
    for(double& element : mulmatrix[r])
    {
        element*=c;
    }
    return mulmatrix;
}

Matrix algebra::ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2)
{
    Matrix summatrix=matrix;
    for(size_t i=0;i<colnum(matrix);i++)
    {
        summatrix[r2][i]+=summatrix[r1][i]*c;
    }
    return summatrix;
}

Matrix algebra::upper_triangular(const Matrix& matrix)
{
    if (colnum(matrix) != rownum(matrix)) 
    {
        throw std::logic_error("input matrix is not a square!");
    }
    if (matrix.empty()) 
    {
        return matrix;
    }
    
    Matrix transmatrix=matrix;
    for(size_t k=0;k<colnum(transmatrix);k++)
    {
        if(transmatrix[k][k]==0)
        {
            size_t swap_index=k;
            for (size_t i = k+1; i < rownum(transmatrix); i++)
            {
                if(transmatrix[i][k]!=0)
                {
                    swap_index=i;
                    break;
                }
            }
            if(swap_index!=k)
            {
                transmatrix=ero_swap(transmatrix,k,swap_index);
            }
            else continue;            
        }

        for(size_t i=k+1;i<rownum(matrix);i++)
        {
            if (transmatrix[i][k]!=0)
            {
                double c=transmatrix[i][k]/transmatrix[k][k];
                transmatrix=ero_sum(transmatrix,k,-c,i);
            }   
        }
    }

    return transmatrix;    
}
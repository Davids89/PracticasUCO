#ifndef _MATRIX_H
#define _MATRIX_H
//
//Version 2.17b October 2013
//2.17  Added dual determination of useable rank to restore historical performance. 
//2.17a Changed a parameter in tracking of small sing. values in splits2 in rmatrix.h. 
//2.17b Some minor code refactoring and/or recoding. 
//      Some rare problems may experience a small change to solutions.
//      It is arguable which method may be best.
//
//matrix.h contains a library of matrix creation and manipulation 
//routines for any shape matrix, and a suite of standard solvers
//based on a pseudo-inverse (a.k.a. truncated SVD) treatment when needed.
//matrix.h also contains slightly modified versions of decomposition routines provided
//by Mathworks, NIST, and the Template Numerical Toolkit web site.
//
//rejtrix.h contains a suite of manually- and automatically-regularized solvers
//plus code to support solution of three-way systems:
//least squares          (A*x=b); 
//equality constraints   (E*x==f); and
//inequality constraints (G*x>=h). 
//
//sparse.h contains the Node and Sparse classes that support creation and manipulation
//of sparse matrices, and the code for the GMRES solver.  Sparse.h requires matrix.h.
//Recently we added eigenvalue/eigenvector routines to sparse.h.
//
//All these packages are freely distributed from the author's web site at
//www.rejonesconsulting.com.
//See that site for usage instructions and tutorials.
//
//Class diagram...
//
//    [-----Matrix------]  [---Diagonal---]     (base classes)
//       ^      ^    ^          ^    ^
//       |      .    |          .    .           dashed lines mean "derives from"
//       |      .    |          .    .           dotted lines mean "references" 
//       |      .    |          .    .
//    [-Row-]   .  [---Vector-----]  .          (limited to one row or column)
//              .         ^          .
//              .         .          .
//    [-Node-]  .         .          .          (a Node is a row of a Sparse matrix)
//       ^      .         .          .
//       .      .         .          .
//    [-----------------Sparse-----------]      (sparse matrix class)
//                        ^ 
//                        .
//                        .
//         [---------SparseEig--------]         (for sparse eigenvalues & vectors)
//
//
//
//------Licensing Notice---------------------------------------
//Copyright (c) 2006, 2008, 2011 Rondall E. Jones, Albuquerque NM, USA.
//As of January 1, 2012, we grant completely free usage of all these
//software packages to any users and all purposes.
//(The included NIST decomposition routines are unlimited release.)
//We do ask that you not remove the author's identification and notes above,
//and when appropriate, reference the author's web site in publications.
//
//------Warning Notice-----------------------------------------
//Warning: This software is provided AS IS!  
//THERE IS NO WARRANTY OF ANY SORT WHATSOEVER!
//The algorithms contained herein are partly heuristic and/or experimental.
//Inevitably this package contains bugs/errors which are as yet unknown,
//as well as known deficiencies.
//The user must determine if the results of any calculation using this package
//are appropriate for the user's purposes.
//
//------Technical note on coding style------------------------
//In matrix.h and rejtrix.h we have emphasized code re-use over efficiency 
//in lower level utility routines.  
//This is an attempt to minimize errors,
//maximize the utility of code testing, 
//and improve reliability of the code.
//  
//Specifically, Row and Vector inherit many operations from Matrix,
//at less than optimum efficiency, in order to reuse that one implementation.
//Or, see how Matrix::operator+() uses Matrix::operator+=()
//or how Matrix::prepend_columns() leverages off Matrix::append_columns()
//to make a trivial implementation of prepend_columns at a cost in efficiency.
//
//In sparse.h, efficiency is paramount and key algorithms are 
//coded with that in mind.
//
//Efficiency of the higher level algorithm usually far
//exceeds in impact any such minor issue of efficiency in low level routines.

//amatrix.h-----------------------------------------------------------
#include <stdlib.h>
#include <math.h>

//Recent compilers may need the .h removed in the following three includes.
//The form with .h is deprecated, but needed by older compilers.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

typedef int I;
typedef double T;
static const T BIG  =1.0E30;
static const T BIG2 =2.0E30;

T absf(T x);
I absf(I x);

I minf(I x,I y);
T minf(T x,T y);

I maxf(I x,I y);
T maxf(T x,T y);

I minf(I x,I y,I z);
T minf(T x,T y,T z);

I maxf(I x,I y,I z);
T maxf(T x,T y,T z);

I minf(I x,I y,I z,I p);

T limitabsf(T x, T limit);

T squaref(T x);
T pif();
T signumf(T x);

void prompt();
void separate();
void separateX();
void skipline();

bool ask_the_user(const char * msg);

//******************************************************************
//The Matrix class.
//******************************************************************

class Matrix 
{ 
  private:
    I m_;
    I n_;
    T* data_;
    T** v_;  

    friend class Row;
    friend class Vector;
    
    //internal use only
    void checkdim(I m, I n) { if (m<0 || n<0) Matrix::xerror(4,"Matrix::checkdim"); return; }
    void setup2(I m, I n);
    void setup() { setup2(m_,n_); }

    void normalize_them(Matrix &B, Matrix &E, I i, T rownorm);

  public:

    //static utilities

    //error reporting routine
    static void xerror(I m, const char* who)
      {   
        cerr << "Error in routine " << who << endl;
             if (m==1)  cerr << "Reference Out-of-Bounds!" << endl; 
        else if (m==2)  cerr << "Dimensions do not match!" << endl;
        else if (m==3)  cerr << "Operation on an empty matrix!" << endl;
        else if (m==4)  cerr << "Invalid dimensions!" << endl;
        else if (m==5)  cerr << "Taking vector norm of non-vector! Use matrix norm?" << endl;
        else if (m==6)  cerr << "Divide by zero!" << endl;
        else if (m==7)  cerr << "Invalid input parameter" << endl;
        else if (m==8)  cerr << "Algorithm error" << endl;
        else if (m==9)  cerr << "Prohibited operation for Rows and Vectors!" << endl;
        else if (m==10) cerr << "Given row is too long for matrix!" << endl;
        else if (m==11) cerr << "Invalid argument vector!" << endl;
        else if (m==12) cerr << "Problem is too large for current limits!" << endl;
        else            cerr << "Miscellaneous error: " << m << endl;
        prompt();
        exit(1);
      }

    //find the value of roundoff versus 1.0
    static T roundoff()
      {
        static bool ok=false;
        static T round=1.0e-9;
        if (ok) return round;
        
        int j=0;
        T one=1.0;
        T two=1.0;
        T three;
        for (int i=0; i<=100; i++)
          { j=i; three=one+two; if (three==one) break; two/=2.0; }
        round = two*2.0; //went one too far
        if (j>=100) round = 1.0E-9;
        ok = true;
        return round;
      }

    //generate a random value between zero and one
    static T myrandom(I reset=0) 
      {
        static int seed=13*13*13;
        if (reset!=0) seed=reset%16384;
        seed=seed*13;       //scramble
        seed=seed%16384;    //chop to 16 bits
        return T(seed)/16384.0;
      }

    //generate an approximately Gaussian random value, mean 0, sigma 1
    static T mygauss() 
      {
         T sum=0.0;
         //for (int i=0; i<9; i++)  sum += (myrandom()-0.5)*2.0*1.732;
         //rms is about 7% too large often... why???  so reduce it...
         for (int i=0; i<9; i++)  sum += (myrandom()-0.5)*2.0*1.62;
         return sum/3.0; 
      }

    //count the number of arguments less than BIG
    static I countargs(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10)
      { 
        if  (t2>BIG) return 1;
        if  (t3>BIG) return 2;
        if  (t4>BIG) return 3;
        if  (t5>BIG) return 4;
        if  (t6>BIG) return 5;
        if  (t7>BIG) return 6;
        if  (t8>BIG) return 7;
        if  (t9>BIG) return 8;
        if (t10>BIG) return 9; 
        return 10;       
      }

    //constructors---------------------------------------------------

    //default constructor: 0 by 0 matrix
    Matrix () : m_(0), n_(0) {}
      
    //construct an m by n matrix (zero filled)
    explicit Matrix (int m, int n) { setup2(m,n); }
 
    //construct an m by n matrix filled with the value x
    explicit Matrix (int m, int n, T x); 

    //construct an m by n matrix; copy the contents from the array a[m][ndim] 
    Matrix (int m, int n, int ndim, const T *a);
    
    //copy constructor
    Matrix (const Matrix &A); 

    //destructors----------------------------------------------------

    //delete all data and set size to 0 by 0
    void clear()
      {
         if (m_>0 && n_>0) { delete [] data_; delete [] v_; }
         m_ = 0;
         n_ = 0; 
      }

     ~Matrix () { clear(); } 

    //assignment-----------------------------------------------------

    //supports for example B = 3.14;
    Matrix operator=(const T x);

    //supports for example B = A;
    Matrix operator=(const Matrix &A); 

    //accessors------------------------------------------------------

    //get the row dimension
    int dim1() const { return m_; }

    //get the column dimension
    int dim2() const { return n_; }

    //get the smaller dimension
    int dmin() const { return m_ < n_ ? m_ : n_ ; }

    //get the larger dimension
    int dmax() const { return m_ > n_ ? m_ : n_ ; }

    //get the 2-D size
    int dsize() const { return m_ * n_ ; }

    //get the 2-dimensional array representing the matrix
    void get_array(T* A) { int sz=m_*n_; for (int i=0; i<sz; i++) A[i]=data_[i]; }

    //see if the two matrices have matching dimensions ... A.matching(B)
    bool matches(const Matrix &B) const
      { return m_==B.m_ && n_==B.n_; }    

    //index----------------------------------------------------------

    inline T* operator[] (I i) 
      { 
        if (i < 0 || i >= m_) { Matrix::xerror(1,"operator[]"); }; //DELETE for no debug
        return v_[i]; 
      }

    inline const T* operator[] (I i) const
      { 
        if (i < 0 || i >= m_) { Matrix::xerror(1,"operator[]"); }; //DELETE for no debug
        return v_[i]; 
      }

    //Alternative index form... A(i,j) rather than A[i][j].
    //This checks both the indices for proper range.
    //(The A[i][j] form can only check the first index.)

    T& operator() (I i, I j)
      { 
        if (i < 0 || i >= m_ || j < 0 || j >= n_) Matrix::xerror(1,"operator(,)"); 
        return v_[i][j]; 
      }

    const T& operator() (I i, I j) const
      { 
        if (i < 0 || i >= m_ || j < 0 || j >= n_) Matrix::xerror(1,"operator(,)"); 
        return v_[i][j]; 
      }

    //equivalence operations-----------------------------------------

    //supports A==B
    bool operator==(const Matrix &B) const;

    //supports A!=B
    bool operator!=(const Matrix &B) const;

    //approximately equal test.
    //Two values are considered approx. equal if they differ by
    //less than tolerance.
    //So this is an absolute test, not relative.
    bool approximate(const Matrix &B, T tolerance) const;

    //element-wise Matrix operations---------------------------------

    //these operations support for example, A+=2.0, A-=2.0, A*=2.0, A/=2.0  
    Matrix operator+= (T x) 
      {int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]+=x; return *this; }
    Matrix operator-= (T x) 
      {int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]-=x; return *this; }
    Matrix operator*= (T x) 
      {int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]*=x; return *this; }
    Matrix operator/= (T x) 
      {int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]/=x; return *this; }

    //these operations support for example, A+2.0, A-2.0, A*2.0, A/2.0  
    Matrix operator+  (T x) const { Matrix C(*this); C+=x; return C; }
    Matrix operator-  (T x) const { Matrix C(*this); C-=x; return C; }
    Matrix operator*  (T x) const { Matrix C(*this); C*=x; return C; }
    Matrix operator/  (T x) const { Matrix C(*this); C/=x; return C; }
  
    //unary minus--- for B = -A; for example
    Matrix operator- ();

    //these operations support A+=B, A-=B, A*=B, A/=B, which are all element-wise.
    //A and B must have exactly the same shape.
    Matrix operator+= (const Matrix &B);
    Matrix operator-= (const Matrix &B);  
    Matrix operator*= (const Matrix &B);   

    //these operations support A+B  and A-B, which are all element-wise.
    //A and B must have exactly the same shape.
    Matrix operator+ (const Matrix &B) const;
    Matrix operator- (const Matrix &B) const;

    //the following provides the matrix product A*B, 
    //where A's second dimension must equal B's first dimension
    Matrix operator* (const Matrix &B) const;

    //--------

    //the following scales each row of the matrix to unit norm,
    //and carries along B and E. (B usually the RHS; E the error est.)
    void normalize_rows(Matrix &B, Matrix &E);

    //the following scales each row of the matrix to unit norm,
    //and carries along B
    void normalize_rows(Matrix &B) 
      { Matrix E(m_,1); normalize_rows(B,E); } 

    //the following scales each row of the matrix to unit norm
    void normalize_rows() 
      { Matrix B(m_,1); Matrix E(m_,1); normalize_rows(B,E); } 

    //--------

    //the following scales each row of the matrix to max element of 1.0
    //and carries along B and E. (B usually the RHS; E the error est.)
    void normalize_rows_max1(Matrix &B, Matrix &E);

    //the following scales each row of the matrix to max element of 1.0,
    //and carries along B
    void normalize_rows_max1(Matrix &B)
      { Matrix E(m_,1); normalize_rows_max1(B,E); } 

    //the following scales each row of the matrix to max element of 1.0
    void normalize_rows_max1() 
      { Matrix B(m_,1); Matrix E(m_,1); normalize_rows_max1(B,E); } 

    //element-wise operations----------------------------------------

    //replaces each element with its absolute value
    void mabs();

    //replaces each element with the square root of its absolute value
    void msqrt();

    //replaces each element with its square
    void msquare();

    //Replaces each element with the base 10 log of its absolute value
    //log(A.maxabs())-30.0 is used for zero elements.
    void mlog10();

    //Replaces each element a with 10^a.
    //That is, with the antilog10 of a.
    void mpow10();

    //makes each element at least x
    void at_least(T x);

    //truncates each number to n digits
    void keep_digits(I n);

    //In the following integer utilities, the values are computed as integers,
    //but the TYPE remains floating point.

    //truncates each element to integer                  -2.6 --> -2.0    2.6 --> 2.0
    void trunc();

    //rounds each element to the nearest integer         -2.6 --> -3.0    2.6 --> 3.0
    void round(); 

    //rounds each element toward +infinity               -2.6 --> -2.0    2.6 --> 3.0
    void ceil();    

    //rounds each element to +1 or -1; zero goes to +1   -2.6 --> -1.0    2.6 --> 1.0
    void signum();      

    //rounds each element to +1 or -1; zero stays as 0   -2.6 --> -1.0    2.6 --> 1.0
    void trinity();

    //convert each column to percentages based on the sum of the
    //(absolute values of the) elements of the column
    void to_percentages();

    //min/max/sum functions------------------------------------------------
      
    //returns the element which is algebraically largest
    T maxval() const; 

    //returns the element which is algebraically smallest
    T minval() const;

    //returns the (absolute value of the) element which is largest in absolute value
    T maxabs() const; 

    //returns the (absolute value of the) element which is smallest in absolute value
    T minabs() const;

    //returns the smallest element greater than zero
    T minpos() const;

    //returns (imax,jmax) position of element with largest abs value
    void ijmaxabs(I &imax, I &jmax) const;

    //returns the sum of the elements
    T sum() const;

    //returns the sum of the absolute values of the elements
    T sumabs() const;

    //returns the average of the values of the elements
    T average() const { return sum()/(m_*n_); }

    //returns the average of the absolute values of the elements
    T averageabs() const { return sumabs()/(m_*n_); }

    //find a neglible value for *this
    T epsilon() const { return maxabs()*8.0*Matrix::roundoff(); }

    //count the number of non-zero elements
    I num_non_zero() const ;

    //cout the number of non-negative elements
    I num_non_negative() const;

    //1-D norms------------------------------------------------------
    //These methods require that the object be 1-dimensional.
    //That is, a Row, a Vector, or a Matrix of size 1 by n, or m by 1.
    //For a row v, norm(v) = sqrt(v * v').

    //returns square(norm(*this)) 
    T norm2() const; 
 
    //returns norm(*this)
    T norm () const { return sqrt(norm2()); }

    //returns root-mean-square(*this)
    T rms  () const { return sqrt(norm2()/T(maxf(m_,n_,1))); }

    //returns the population standard deviation
    T popstddev() const { T a=average(); Matrix d=*this - a; return d.rms(); }

    //returns the sample standard deviation
    T samstddev() const 
      { if (m_<2) return 0.0; return popstddev()*sqrt(T(n_)/T(n_-1)); }

    //norms of the elements of the matrix as if it were 1-D ---------
    //These methods NO NOT require that the object be 1-dimensional.

    //returns the sum of the squares of all the elements
    T norm2_as_vector() const;   

    //returns the square root of the sum of the squares of the elements
    T norm_as_vector() const { return sqrt(norm2_as_vector()); }

    //Frobenius norm is another name for our norm_as_vector 
    T Frobenius() const { return sqrt(norm2_as_vector()); }

    //returns root-mean-square of the matrix elements
    T rms_as_vector() const { return sqrt(norm2_as_vector()/T(m_*n_)); } 
    
    //row/column operations-------------------------------------------

    //returns the dot product of two rows of *this
    T rowdot(I i,I k) const;

    //dot product of two equal-length 1-dimensional matrices.
    //dot tolerates any two equal length 1-D matrices: row.row, row.col, col.col
    T dot (Matrix &B) const;

    //returns a row of *this
    Matrix get_row (I i) const;

    //returns a column of *this
    Matrix get_column (I j) const;

    //sets all values in row i to val
    void set_row (I i, T val);    

    //sets all values in column j to val
    void set_column (I j, T val);

    //sets row i from a row matrix.  The row sizes must match.
    void set_row (I i, Matrix A);    

    //sets column i from a column matrix.  The columns sizes must match.
    void set_column (I j, Matrix A);

    //sets all values in row i to zero
    void set_row_zero(I i);

    //sets all values in column j to zero
    void set_column_zero(I j);

    //matrix shape operations----------------------------------------

    //transposes *this
    Matrix t();              

    //resize to smaller or larger
    //keeps upper left content as far as possible; fills with zero
    void resize(I m,I n);   

    //deletes row r; decreases matrix size!                     
    void del_row(I r);

    //deletes column c; decreases matrix size!      
    void del_column(I c);
    
    //add m new rows at the bottom of *this; zero filled
    void add_rows(I m) { resize(m_ + m, n_); }       

    //append Matrix B to bottom of *this
    void append_rows(const Matrix &B );

    //prepend the Matrix B to the top of *this
    void prepend_rows(const Matrix &B);
 
    //add n new columns at the right side of *this
    void add_columns(I n) { resize(m_, n_ + n); };

    //append Matrix B to right side of *this
    void append_columns(const Matrix &B ); 

    //prepend the Matrix B to the left of *this
    void prepend_columns(const Matrix &B);

    //common matrices------------------------------------------------

    void zeros();   //set *this to all zeros     
    void ones();    //set *this to all ones   
    void identity();//set *this to identity matrix     
    void iota();    //set *this[i][j] = i + j + 1.  In a row that's 1, 2, 3, ...
    void iotazero();//set *this[i][j] = i + j.  In a row that's 0, 1, 2, ...
    void random();  //set *this to random values in (0,1)
    void gauss();   //set *this to random Gaussian, mean 0, standard deviation 1  
    void hilbert(); //set *this[i][j] = 1/(i+j+1)
    void heat();    //set to an example heat equation style kernel
    void laplace(); //set to an example inverse Laplace style kernel
    void cusp();    //set to one positive cusp of a sine function

    //following static methods create a Matrix of the given size and
    //call the appropriate routine above to define the elements of the Matrix.

    static Matrix zeros   (I m, I n) { Matrix A(m,n); A.zeros();    return A; }
    static Matrix ones    (I m, I n) { Matrix A(m,n); A.ones();     return A; }
    static Matrix identity(I m, I n) { Matrix A(m,n); A.identity(); return A; }
    static Matrix iota    (I m, I n) { Matrix A(m,n); A.iota();     return A; }
    static Matrix iotazero(I m, I n) { Matrix A(m,n); A.iotazero(); return A; }
    static Matrix random  (I m, I n) { Matrix A(m,n); A.random();   return A; }
    static Matrix gauss   (I m, I n) { Matrix A(m,n); A.gauss();    return A; }
    static Matrix hilbert (I m, I n) { Matrix A(m,n); A.hilbert();  return A; }
    static Matrix heat    (I m, I n) { Matrix A(m,n); A.heat();     return A; }
    static Matrix laplace (I m, I n) { Matrix A(m,n); A.laplace();  return A; }

    //displays-------------------------------------------------------

    //print a rectangular layout with default width
    void print() const;       
 
    //print by row in narrow (less than 80 column) format
    void print_by_row() const;      

    //print by column in narrow format
    void print_by_column() const;

    //print a glimpse of the matrix, in an 80-column wide format        
    void printA() const;

    //print a glimpse of the matrix and the right hand side vector, b, 
    //in an 80-column wide format
    void printAb(const Matrix &b) const;

    //print a glimpse of the matrix and the right hand side vector, b, 
    //and an error estimate vector e, in an 80-column wide format
    void printAbe(const Matrix &b, const Matrix &e) const;

    //print a glimpse of the matrix, the solution, x, and the right hand side vector, b,
    //in an 80-column wide format.
    //x and b must be single column or single row matrices, or a Row or Vector.
    //By default, up to 25 rows will be printed.
    void printAxb(const Matrix &x, const Matrix &b, I maxrows=25) const;
 
    //Compute for each element an Order of Magnitude of 1 to 16 or so.
    //This arrangement follows the way stars are classified: 
    //magnitude 1 is from largest to about 1/10th that;
    //magnitude 2 is from 1/10th of the largest to about 1/100th of the largest; 
    //etc
    Matrix compute_star_magnitudes() const;

    //show each element as Order of Magnitude 1 to 9 or blank for smaller than 9th magnitude
    void print_star_magnitudes() const;

}; //end class Matrix

Matrix transpose (const Matrix& A);

//supports 2.0+A for example...returns element[i][j] = 2.0+A[i][j]
Matrix operator + (T x, const Matrix &A);

//supports 2.0*A for example...returns element[i][j] = 2.0*A[i][j]
Matrix operator * (T x, const Matrix &A);

//supports 2.0-A for example...returns element[i][j] = 2.0-A[i][j]
Matrix operator - (T x, const Matrix &A);

//returns the transpose of the object.
//Note that the object itself is NOT modified.  To modify the object, use A.t()
Matrix transpose (const Matrix& A);

T pilof();
T pihif();

//******************************************************************
//The Diagonal Matrix class.
//This is coded completely separately to avoid any possible
//mix of Matrix functionality with Diagonal functionality.
//******************************************************************
class Diagonal
{
  private:

    I m_;
    I n_;
    I mm_;
    T* data_;

    void checkdim(I m, I n) { if (m<0 || n<0) Matrix::xerror(4,"Diagonal::checkdim"); return; }
    void setupd(I m, I n);

  public:

    //constructors---------------------------------------------------

    //default constructor: 0 length diagonal
    Diagonal() { setupd(0,0); }

    //construct a Diagonal of size m by m, zero filled
    explicit Diagonal(int m) { setupd(m,m); }

    //construct a Diagonal of size m by n, zero filled
    explicit Diagonal(int m, int n) { setupd(m,n); }

    //construct a Diagonal of size m by n, with every diagonal element set to x
    Diagonal(int m, int n, T x);

    //construct a Diagonal of size m by n, with diagonal from array a
    Diagonal (int m, int n, const T *a);

    //copy constructor
    Diagonal(const Diagonal &D);

    //copy constructor from D with shape change to smaller or larger
    Diagonal(int m, int n, const Diagonal &D);

    //construct a Diagonal:
    // 1. from a row, with D[i] = R[i]
    // 2. from a column, with D[i] = C[i]
    // 3. of the same shape as matrix A, with D[i] = A[i][i]
    explicit Diagonal(const Matrix &A);

    //destructors

    //delete all data and set size to 0 by 0
    void clear()
      {
        if (m_>0 && n_>0) { delete [] data_; }
        m_ = n_ = mm_ = 0;
      }

    ~Diagonal() { clear(); }

    //assignment-----------------------------------------------------

    //supports D = x
    Diagonal operator=(const T x);

    //supports D = D
    Diagonal operator=(const Diagonal &D);

    //resize to smaller or larger
    //keeps upper left content as far as possible; fills with zero
    void resize(I m,I n);

    //create a Diagonal of the same shape as A,
    //with diagonal elements taken from A's diagonal: D[i] = A[i][i]
    Diagonal operator=(const Matrix &A);

    //accessors------------------------------------------------------

    //get the row dimension
    inline int dim1() const { return m_; }

    //get the column dimension
    inline int dim2() const { return n_; }

    //get the smaller dimension
    inline int dmin() const { return m_ < n_ ? m_ : n_ ; }

    //get the larger dimension
    inline int dmax() const { return m_ > n_ ? m_ : n_ ; }

    //index----------------------------------------------------------
    inline T& operator[] (I i)
      {
        if (i < 0 || i >= mm_) { Matrix::xerror(1,"Diagonal::operator[]"); };
        return data_[i];
      }

    inline const T& operator[] (I i) const
      {
        if (i < 0 || i >= mm_) { Matrix::xerror(1,"Diagonal::operator[]"); };
        return data_[i];
      }

    //alternative index form... D(i,i) rather than D[i].
    //This checks the indices for proper range
    inline T& operator() (I i, I j)
      {
        if (i < 0 || i >= mm_ || i!=j ) Matrix::xerror(1,"Diagonal::operator(,)");
        return data_[i];
      }

    //equivalence operations-----------------------------------------

    //supports D1==D2
    bool operator ==(const Diagonal &D) const;

    //supports D1!=D2
    bool operator !=(const Diagonal &D) const { return !((*this)==D); }

    //approximate equality, called as D.approximate(D2,0.00000001);
    //Any absolute difference greater than the given scalar causes a return of false.
    bool approximate(const Diagonal &D, T tolerance) const;

    //element-wise operations----------------------------------------

    //these operations support D+=2.0  D-=2.0   D*=2.0  D/=2.0  for example
    Diagonal operator += (T x) { for (int i=0; i<mm_; i++) data_[i]+=x; return *this; }
    Diagonal operator -= (T x) { for (int i=0; i<mm_; i++) data_[i]-=x; return *this; }
    Diagonal operator *= (T x) { for (int i=0; i<mm_; i++) data_[i]*=x; return *this; }
    Diagonal operator /= (T x) { for (int i=0; i<mm_; i++) data_[i]/=x; return *this; }

    //these operations support D+2.0  D-2.0   D*2.0  D/2.0  for example
    Diagonal operator +  (T x) const { Diagonal C(*this); C+=x; return C; }
    Diagonal operator -  (T x) const { Diagonal C(*this); C-=x; return C; }
    Diagonal operator *  (T x) const { Diagonal C(*this); C*=x; return C; }
    Diagonal operator /  (T x) const { Diagonal C(*this); C/=x; return C; }

    //operate and assign
    Diagonal operator += (const Diagonal &D);
    Diagonal operator -= (const Diagonal &D);

    //unary minus--- for B = -A; for example
    Diagonal operator - () const;

    //these operations support A+B  and A-B.  A and B must have exactly the same shape
    Diagonal operator + (const Diagonal &D) const;
    Diagonal operator - (const Diagonal &D) const;

    //the following allows A*B, where A's second dimension must equal B's first dimension
    Diagonal operator * (const Diagonal &D) const;

    //replaces each element with its absolute value
    void mabs();

    //replaces each element with the square root of its absolute value
    void msqrt();

    //replaces each element with its square
    void msquare();

    //Replaces each element with the base 10 log of its absolute value.
    //log(A.maxabs())-30.0 is used for zero elements.
    void mlog10();

    //Replaces each element a with 10^a.
    //That is, with the antilog10 of a.
    void mpow10();

    //makes each element at least x
    void at_least(T x);

    //min / max -----------------------------------------------------

    //returns the (absolute value of the) element which is largest in absolute value
    T maxabs() const;

    //returns the (absolute value of the) element which is smallest in absolute value
    T minabs() const;

    //returns the index of the maximum absolute value in the Diagonal
    I imaxabs() const;

    //returns the index of the minimum absolute value in the Diagonal
    I iminabs() const;

    //returns the index of the last non-zero in the Diagonal
    I ilastnz() const;

    //returns the trace of the matrix, which is the sum of the diagonal elements.
    T trace() const;

    //find a neglible value for *this
    T epsilon() const;

    //Determine the number of approximately equal singular
    //values at the end of the list of singular values,
    //not counting the first one at that level.
    //In other words, compute p in Algorithm 12.3.1 See Golub and Van Loan, 2nd Ed.
    //The point is that S(k-p-1,k-p-1) should be signifcantly larger than S(k-p,k-p)
    //where k=number of singular values in S.
    //Ideally, p should be 0.
    I plateau() const;

    //transposes *this
    void t() { I t = n_; n_ = m_; m_ = t; }

    //builders
    void zeros();   //set *this to all zeros
    void ones();    //set *this to all ones
    void identity();//set *this to identity matrix
    void iota();    //set *this[i][j] = i + j + 1.  In a row that's 1, 2, 3, ...
    void random();  //set *this to random values in (0,1)
    void gauss();   //set *this to random Gaussian, mean 0, standard deviation 1

    void print() const;

};  //class Diagonal

ostream& operator<<(ostream& os, const Diagonal &D);

//Free functions for Diagonal-------------------------------

//supports 2.0+D for example...returns element[i][j] = 2.0+D[i][j]
Diagonal operator + (T x, const Diagonal &D);

//supports 2.0*D for example...returns element[i][j] = 2.0*D[i][j]
Diagonal operator * (T x, const Diagonal &D);

//supports 2.0-D for example...returns element[i][j] = 2.0-D[i][j]
Diagonal operator - (T x, const Diagonal &D);

//returns a Diagonal which is the transpose of the argument.
//The argument is not changed.
Diagonal transpose (const Diagonal &D);

//returns the pseudo-inverse of a Diagonal matrix D.
//Values below or near roundoff times the largest (magnitude) diagonal
//element are considered to be zero
Diagonal pseudoinverse(const Diagonal &D);

//returns the smoothed (regularized) pseudo-inverse of a Diagonal matrix D.
//Values below or near roundoff times the largest (magnitude) diagonal
//element are considered to be zero
Diagonal smoothinverse(const Diagonal &S, T lambda);

//Tikhonov regularize the Matrix A with given lambda.
//Seldom necessary. Use the above when possible.
Diagonal regularize(const Diagonal &S, T lambda);

//returns the condition number of a Diagonal matrix D.
T condition_number(const Diagonal &D);

//returns the condition number of a Diagonal matrix ignoring zero or near-zero values
T condition_number_nonzero(const Diagonal &D);

//Matrix * Diagonal
Matrix operator * (const Matrix &A, const Diagonal &D);

//Diagonal * Matrix
Matrix operator * (const Diagonal &D , const Matrix &B);

  //create a full Matrix of the same shape as D
  //with diagonal elements taken from D and zeros otherwise
  Matrix full(const Diagonal &D);


//******************************************************************
//Row and Vector classes.
//******************************************************************

class Row : public Matrix
{
  public:
    //constructors

    //default constructor: 0 length row
    Row() : Matrix(0,0) {}

    //construct a row of length m, zero filled
    explicit Row(int m) : Matrix(1,m) {}

    //construct a row of length m, with data from array a
    explicit Row(int m, T x) : Matrix(1,m,x) {}

    //construct a row of length m, with data from 1D array a
    explicit Row(int m, const T* a);

    //copy constructor
    Row (const Row &R) : Matrix(R) {}

    //construct a Row from a 1-row Matrix
    Row (const Matrix &A) : Matrix(A)
      { if (A.dim1()>1) Matrix::xerror(2,"Row(Matrix)"); }

    //construct a Row from a Diagonal
    explicit Row(const Diagonal &D);

    //construct a row from a list of at least 2 values (must be less than BIG)
    explicit Row(T t1, T t2, T t3=BIG2, T t4=BIG2, T t5=BIG2, T t6=BIG2,
                             T t7=BIG2, T t8=BIG2, T t9=BIG2, T t10=BIG2);

    //supports for example R = 3.14;
    Row operator=(T x);

    //assignment ... R = A, where A is a 1-row matrix
    Row operator=(const Matrix &A);

    //get the primary dimension
    int dim() const { return n_; }
    int size() const { return n_; }

    //index
    T& operator[] (I i)
      {
        if (i < 0 || i >= n_) { Matrix::xerror(1,"Row::operator[]"); }; //DELETE for no debug
        return data_[i];
      }

    inline const T& operator[] (I i) const
      {
        if (i < 0 || i >= n_) { Matrix::xerror(1,"Row::operator[]"); }; //DELETE for no debug
        return data_[i];
      }

    //limitations
    void resize(I n) { Matrix::resize(1,n); }
    void resize(I m, I n)
       { if (m==1) Matrix::resize(m,n);
         else Matrix::xerror(9,"Row::resize to Matrix");}
    void append_columns(const Matrix &B )
      { if (B.dim1()==1) Matrix::append_columns(B);
        else Matrix::xerror(2,"Vector::append_columns()"); }
    void prepend_columns(const Matrix &B)
      { if (B.dim1()==1) Matrix::prepend_columns(B);
        else Matrix::xerror(2,"Vector::prepend_columns()"); }

    //prohibitions
    private:
    void t()                              { Matrix::xerror(9,"Row::t()"); }
    void del_row(I r)                     { Matrix::xerror(9,"Row::del_row()"); }
    void add_rows(I m)                    { Matrix::xerror(9,"Row::add_row()"); }
    void append_rows(const Matrix &B )    { Matrix::xerror(9,"Row::append_rows()"); }
    void prepend_rows(const Matrix &B)    { Matrix::xerror(9,"Row::prepend_rows()"); }

};  //end class Row


//-------------------------------------------------------------------
class Vector : public Matrix
{
  public:
    //constructors

    //default constructor: 0 length
    Vector() : Matrix(0,0) {}

    //construct a Vector of length m, zero filled
    explicit Vector(int m) : Matrix(m,1) {}

    //construct a Vector of length m, filled with the value x
    explicit Vector(int m, T x) : Matrix(m,1,x) {}

    //construct a Vector of length m, with data from 1D array a
    explicit Vector(int m, const T* a);

    //copy constructor
    Vector(const Vector &V) : Matrix(V) {}

    //construct a Vector from a 1-column Matrix
    Vector(const Matrix &A) : Matrix(A)
      {
        if (A.dim2()>1) Matrix::xerror(2,"Vector(Matrix)");
      }

    //construct a Vector from a Diagonal
    explicit Vector(const Diagonal &D);

    //construct a Vector from a list of values (each less than BIG)
    explicit Vector(T t1, T t2, T t3=BIG2, T t4=BIG2, T t5=BIG2, T t6=BIG2,
                                T t7=BIG2, T t8=BIG2, T t9=BIG2, T t10=BIG2);

    //supports for example R = 3.14;
    Vector operator=(T x);

    //supports V = A, where A is a 1-column matrix
    Vector operator =(const Matrix &A);

    //copy a vector from the diagonal elements of a Diagonal matrix
    Vector operator = (const Diagonal &D);

    //get the primary dimension
    int dim() const { return m_; }
    int size() const { return m_; }

    //index
    T& operator[] (I i)
      {
        if (i < 0 || i >= m_) { Matrix::xerror(1,"Vector::operator[]"); }; //DELETE for no debug
        return data_[i];
      }

    inline const T& operator[] (I i) const
      {
        if (i < 0 || i >= m_) { Matrix::xerror(1,"Vector::operator[]"); }; //DELETE for no debug
        return data_[i];
      }

    //normalize this vector to unit norm, if possible
    void normalize() { T a=norm(); if (a>0.0) operator*=(1.0/a); }

    //returns the index of the algebraically maximum value in the Vector.
    I imax() const;

    //returns the index of the algebraically minimum value in the Vector.
    I imin() const;

    //returns the index of the maximum absolute value in the Vector.
    I imaxabs() const;

    //returns the index of the minimum absolute value in the Vector.
    I iminabs() const;

    //create a vector from elements i through j of *this: Vector V2=V1.range(3:9);
    Vector range(I i, I j);

    //stack operations
    //Note that these operations are intended as a convenience,
    //not an optimally efficient mechanism.
    //Every push and pop requires a resize.

    //add a value to the front of the vector (becomes the new (*this)[0])
    void push_front(T value);

    //remove the first value in the vector (remove (*this)[0])
    void pop_front();

    //add a value to the end of the vector
    void push_end(T value);

    //remove the fast value in the vector
    void pop_end();

    //sort the vector into increasing order
    void sort();

    //sort the Vector into increasing order and carry along the integer array p
    void sort(int* p);

    //return the median value; for even lengths, return average of the two in the middle
    T median() const;

    //return the moving average of the elements of the vector (slow/accurate method)
    Vector moving_average(I w);

    //return the moving average of the elements of the vector (fast/inaccurate method)
    Vector moving_average_fast(I w);

    //limitations
    void resize(I m) { Matrix::resize(m,1); }
    void resize(I m, I n)
      { if (n==1) Matrix::resize(m,n);
        else Matrix::xerror(9,"Vector::resize to Matrix");}
    void append_rows(const Matrix &B )
      { if (B.dim2()==1) Matrix::append_rows(B);
        else Matrix::xerror(2,"Vector::append_rows()"); }
    void prepend_rows(const Matrix &B)
      { if (B.dim2()==1) Matrix::prepend_rows(B);
        else Matrix::xerror(2,"Vector::prepend_rows()"); }

    //prohibitions
    private:
    void t()                              { Matrix::xerror(9,"Vector::t()"); }
    void del_column(I r)                  { Matrix::xerror(9,"Vector::del_rcolumn()"); }
    void add_columns(I m)                 { Matrix::xerror(9,"Vector::add_columns()"); }
    void append_columns(const Matrix &B ) { Matrix::xerror(9,"Vector::append_columns()"); }
    void prepend_columns(const Matrix &B) { Matrix::xerror(9,"Vector::prepend_columns()"); }

    private:
    //sort elements v[m] to v[n] in increasing order.
    //w must be a work array at least as large as v.
    //If carry is true then the integer array p is carried along.
    //The array p must be the same length as v, and
    //the integer work array q must be at least as large as p.
    static void sort(T *v, T *w, bool carry, I *p, I *q, I m, I n);
};

//******************************************************************
//Column is typedef'd to "Vector"
//******************************************************************
typedef Vector Column;

//******************************************************************
//Miscellaneous Free functions
//******************************************************************

//Householder operations -- See Golub and Van Loan, 2nd Ed, Sec 5.1.3

//Create a Householder matrix of size m by m that zeroes all but
//the first element of the subcolumn or subrow given in vector x.
//NOTE: this is a quick-and-dirty way to implement Householder,
//and is not appropriate to use for matrix decompositions.
//Its purpose in rejtrix.h is for special situations such as T.L.S.
//See Golub and Van Loan for proper way to implement Householder
//for use in decompositions.

Matrix householder(I m, const Vector &x);

//******************************************************************
//Simple "printer plot" routines for demos.
//******************************************************************

//plot yy versus xx; xx and yy must be columns or rows
void plot(const Matrix &xx, const Matrix &yy);

//dmatrix.h-----------------------------------------------------------
//***********************************************************************
//Decomposition routines adapted from Template Numerical Toolkit.
//***********************************************************************
//Following matrix decomposition routines for SVD, QR, and Eigenvalue
//are adapted from http://math.nist.gov/tnt/index.html.
//These are public domain routines.
//My thanks to the authors for this nice public domain package.

T hypot2(T a, T b);

//the SVD decomposition----------------------------------------------
static void svd (const Matrix& Arg, Matrix& U, Diagonal& s, Matrix& V);

//---Class implementation of QR--------------------------------------
//Classical QR Decomposition:
//For an m-by-n matrix A with m >= n, the QR decomposition is an m-by-n
//orthogonal matrix Q and an n-by-n upper triangular matrix R so that A = Q*R.
//
//The QR decomposition always exists, even if the matrix does not have
//full rank, so the constructor will never fail.  The primary use of the
//QR decomposition is in the least squares solution of nonsquare systems
//of simultaneous linear equations.
//This will fail if isFullRank() returns false.
//
//The Q and R factors can be retrived via the getQ() and getR() methods.
//Furthermore, a solve() method is provided to find the
//least squares solution of Ax=b using the QR factors.
//
//(Adapted from JAMA, a Java Matrix Library, developed by jointly
//by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).
class QR {
private:
  Matrix QR_;
  int m, n;
  Diagonal Rdiag;

public:
  //construct a QR decomposition of A
  QR(const Matrix &A)
    {
      m = 0;
      n = 0;
      if (A.dim1()<1 || A.dim2()<1) Matrix::xerror(3,"QR::construct()");
      if (A.dim1()<A.dim2()) Matrix::xerror(4,"QR::construct()");
      m = A.dim1();
      n = A.dim2();
      QR_ = A;
      Rdiag = Diagonal(n);
	int i=0, j=0, k=0;
      for (k = 0; k < n; k++)
      {
        // Compute 2-norm of k-th column without under/overflow.
        T nrm = 0;
        for (i = k; i < m; i++) nrm = hypot2(nrm,QR_[i][k]);
        if (nrm != 0.0)
        {
           // Form k-th Householder vector.
           if (QR_[k][k] < 0) nrm = -nrm;
           for (i = k; i < m; i++) QR_[i][k] /= nrm;
           QR_[k][k] += 1.0;

           // Apply transformation to remaining columns.
           for (j = k+1; j < n; j++)
             {
               T s = 0.0;
               for (i = k; i < m; i++) s += QR_[i][k]*QR_[i][j];
               s = -s/QR_[k][k];
               for (i = k; i < m; i++)  QR_[i][j] += s*QR_[i][k];
             }
         }
         Rdiag[k] = -nrm;
      }
    }

  //Return true if matrix is full rank, false otherwise.
  bool isFullRank() const
    {
      //for (int j = 0; j < n; j++) if (Rdiag[j] == 0) return false;
      T eps=QR_.epsilon();  //REJ 2011
      for (int j = 0; j < n; j++) if (absf(Rdiag[j]) <= eps) return false; //REJ 2011
      return true;
    }

  //Return the upper triangular factor, R, of the QR factorization
  Matrix getR() const
   {
      if (m<1 || n<1) Matrix::xerror(3,"QR::getR()");
      Matrix R(n,n);
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
           if (i < j) {
              R[i][j] = QR_[i][j];
           } else if (i == j) {
              R[i][j] = Rdiag[i];
           } else {
              R[i][j] = 0.0;
           }
        }
      return R;
    }

  //Generate and return the orthogonal factor Q
  Matrix getQ() const
    {
      if (m<1 || n<1) Matrix::xerror(3,"QR::getQ()");
	int i=0, j=0, k=0;
	Matrix Q(m,n);
      for (k = n-1; k >= 0; k--)
      {
         for (i = 0; i < m; i++) Q[i][k] = 0.0;
         Q[k][k] = 1.0;
         for (j = k; j < n; j++) {
            if (QR_[k][k] != 0) {
               T s = 0.0;
               for (i = k; i < m; i++) s += QR_[i][k]*Q[i][j];
               s = -s/QR_[k][k];
               for (i = k; i < m; i++) Q[i][j] += s*QR_[i][k];
            }
         }
      }
      return Q;
   }

  //Least squares solution of A*x = b
  //B m-length array (vector).
  //return the Vector x that minimizes the two norm of Q*R*x-b
  Vector solve(Vector &b) const
  {
    if (m<1 || n<1)    Matrix::xerror(3,"QR::solve");
    if (b.dim1() != m) Matrix::xerror(4,"QR::solve");
    if (!isFullRank()) Matrix::xerror(6,"QR::solve");
    Vector x = b;

    // Compute Y = transpose(Q)*b
    for (int k = 0; k < n; k++)
    {
      T s = 0.0;
      for (int i = k; i < m; i++) s += QR_[i][k]*x[i];
      s = -s/QR_[k][k];
      for (int i = k; i < m; i++) x[i] += s*QR_[i][k];
    }

    // Solve R*x = Y;
    for (int k = n-1; k >= 0; k--)
    {
      x[k] /= Rdiag[k];
      for (int i = 0; i < k; i++) x[i] -= x[k]*QR_[i][k];
    }

    //temp work space has to be deleted
    Vector xx(n);
    for (int i=0; i<n; i++) xx[i] = x[i];
    return xx;
  }
};  //end class QR

//------------------------------------------------------------------
//The following eigenvalue routines are not needed by the Matrix solvers.
//They are provided as a convenience for the users of the Matrix package.
//------------------------------------------------------------------

//Computes eigenvalues and eigenvectors of a real symmetric matrix
//If A is symmetric, then A = V*D*V' where the eigenvalue matrix D is
//diagonal and the eigenvector matrix V is orthogonal. That is,
//the diagonal values of D are the eigenvalues, and
//V*V' = I, where I is the identity matrix.  The columns of V
//represent the eigenvectors in the sense that A*V = V*D.
//(Adapted from JAMA, a Java Matrix Library, developed by jointly
//by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).

void tred2(Matrix &V, Vector &d, Vector &e);

// Symmetric tridiagonal QL algorithm.

void tql2 (Matrix &V, Vector &d, Vector &e);

//gmatrix.h-----------------------------------------------------------
//******************************************************************
//General Matrix solvers.
//******************************************************************

//Computes the eigenvectors and eigenvalues of a real, square, symmetric matrix.
//To assure the matrix is symmetric only the upper right triange of A is used.
//The lower left triangle is copied from the upper right triangle.

//Note the eigenvalues are returned in a Vector.
//The user may want a Diagonal instead.
//In that case, just construct Diagonal D(d);

void sym_eig(const Matrix &A, Matrix &V, Vector &d);


//returns a "compact" SVD of A
void mysvd(const Matrix &A, Matrix &U, Diagonal &S, Matrix &V);

//returns a "compact" SVD of A with reduced rank
void mysvdrank(const Matrix &A, I rank, Matrix &U, Diagonal &S, Matrix &V);

//Matrix norm == s[0] ... use carefully due to expense
T matrix_norm(const Matrix &A);

//return condition number = largest singular value / smallest non-zero singular value
T condition_number(const Matrix &A);

//returns V * S+ * transpose(U)
Matrix pseudoinverse(const Matrix &A);


//general pseudoinverse solver... solves A*x = B.
//Note that B can have multiple columns.
Matrix solve(const Matrix &A, const Matrix &B);

//Allows user to say either x=solve(A,B) or x=B/A
//Note that B can have multiple columns.
Matrix operator / (const Matrix &B, const Matrix &A) ;

//power of a Matrix
Matrix power(const Matrix &A, I p);

//This routine returns an m by k matrix, where k is the number of
//separate linear dependencies among the rows of A.
//For example, if there is one set of dependencies,
//the returned matrix will be an m x 1 matrix.
//If that one dependency is between rows 3, 9, and 12,
//then the returned matrix will contain the values
// 3.0, 9.0, 12.0, -1.,-1., ... , -1.
//(Meaningless elements are set to -1.)
//If there are two separate depencies, then the matrix would
//be of size m x 2, and the second column might contain
// 4.0, 5.0, 15.0, 16.0, -1., -1., ... , -1.
Matrix get_linear_dependencies (const Matrix &A);

#endif
//end of MATRIX_H

//implementations for Matrix-----------------------------------------

#include "matrix.h"

T absf(T x) { if (x<0.0) return (-x); else return x; }
I absf(I x) { if (x<0  ) return (-x); else return x; }

I minf(I x,I y) { return x<y ? x : y; }
T minf(T x,T y) { return x<y ? x : y; }

I maxf(I x,I y) { return x>y ? x : y; }
T maxf(T x,T y) { return x>y ? x : y; }

I minf(I x,I y,I z) { I a=x; if (y<a) a=y; if (z<a) a=z; return a; }
T minf(T x,T y,T z) { T a=x; if (y<a) a=y; if (z<a) a=z; return a; }

I maxf(I x,I y,I z) { I a=x; if (y>a) a=y; if (z>a) a=z; return a; }
T maxf(T x,T y,T z) { T a=x; if (y>a) a=y; if (z>a) a=z; return a; }

I minf(I x,I y,I z,I p) { I a=x; if (y<a) a=y; if (z<a) a=z;
                                               if (p<a) a=p; return a; }

T limitabsf(T x, T limit)
  { T a=absf(limit); if (x>a) return a; if (x<-a) return -a; return x; }

T squaref(T x) { return x*x; }
T pif() { return 3.14159265358979; }
T signumf(T x) { if (x>=0.0) return 1.0; else return -1.0; }

void prompt() { cout << "Ready? "; char str[99]; cin.getline(str,99); }
void separate()  { for (int z=0; z<10; z++) cout << "-------"; cout << endl; }
void separateX() { for (int z=0; z<10; z++) cout << "---X---"; cout << endl; }
void skipline() { char str[99]; cin.getline(str,99); }

bool ask_the_user(const char * msg)
{
  cout << msg << endl;
  char answer; cin >> answer;
  skipline;
  if (answer=='y' || answer=='Y') return true;
  else return false;
}

void Matrix::setup2(I m, I n)
  {
    checkdim(m,n);
    m_ = m;
    n_ = n;
    if (m_==0 || n_==0) return;
    I sz = m_*n_;
    data_ = new T[sz];
    for (int i=0; i<sz; i++) data_[i] = 0.0;

    v_ = new T*[m_];
    T* p = &(data_[0]);
    for (int i=0; i<m_; i++) { v_[i] = p; p += n_; }
  }

Matrix::Matrix(int m, int n, T x) : m_(m), n_(n)
  {
    setup();
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = x;
  }

Matrix::Matrix(int m, int n, int ndim, const T *a) : m_(m), n_(n)
  {
    setup();
    for (int i=0; i<m; i++)
      for (int j=0; j<n; j++)
        v_[i][j] = a[i*ndim + j];
  }

Matrix::Matrix(const Matrix &A) : m_(A.m_), n_(A.n_)
  {
    setup();
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = A.v_[i][j];
  }

Matrix Matrix::operator=(const T x)
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = x;
    return *this;
  }

Matrix Matrix::operator=(const Matrix &A)
  {
    clear();
    setup2(A.m_ , A.n_);
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = A[i][j];
    return *this;
  }

bool Matrix::operator==(const Matrix &B) const
  {
    if (m_ != B.m_ || n_ != B.n_ ) return false;
    if (m_ == 0    || n_ == 0    ) return true;
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      { if (data_[i] != B.data_[i]) return false; }
    return true;
  }

bool Matrix::approximate(const Matrix &B, T tolerance) const
  {
    if (m_ != B.m_ || n_ != B.n_ ) return false;
    if (m_ == 0    || n_ == 0    ) return true;
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      { if (absf(data_[i]-B.data_[i])>tolerance) return false; }
    return true;
  }

bool Matrix::operator!=(const Matrix &B) const
  {
    return !((*this)==B);
  }

Matrix Matrix::operator- ()
  {
    Matrix C(m_,n_);
    int sz=m_*n_;
    for (int i=0; i<sz; i++) C.data_[i] = -data_[i];
    return C;
  }

Matrix Matrix::operator+= (const Matrix &B)
  {
    if (m_!=B.m_ || n_!=B.n_) Matrix::xerror(2,"Matrix+=Matrix");
    int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]+=B.data_[i];
    return *this;
  }

Matrix Matrix::operator-= (const Matrix &B)
  {
    if (m_!=B.m_ || n_!=B.n_) Matrix::xerror(2,"Matrix-=Matrix");
    int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]-=B.data_[i];
    return *this;
  }

Matrix Matrix::operator*= (const Matrix &B)
  {
    if (m_!=B.m_ || n_!=B.n_) Matrix::xerror(2,"Matrix*=Matrix");
    int sz=m_*n_; for (int i=0; i<sz; i++) data_[i]*=B.data_[i];
    return *this;
  }

Matrix Matrix::operator+ (const Matrix &B) const
  {
    if (m_!=B.m_ || n_!=B.n_) Matrix::xerror(2,"Matrix+Matrix");
    Matrix C(*this);
    return C+=B;
  }

Matrix Matrix::operator- (const Matrix &B) const
  {
    if (m_!=B.m_ || n_!=B.n_) Matrix::xerror(2,"Matrix-Matrix");
    Matrix C(*this);
    return C-=B;
  }

Matrix Matrix::operator* (const Matrix &B) const
  {
    if (n_ != B.m_) Matrix::xerror(2,"Matrix*Matrix");
    Matrix C(m_,B.n_);
    if (m_==0 || n_==0 || B.m_==0 || B.n_==0) return C;
    T sum;
    for (int i=0; i<m_; i++)
      for (int j=0; j<B.n_; j++)
        {
          sum = 0.0;
          for (int k=0; k<n_; k++) sum=sum+v_[i][k]*B[k][j];
          C[i][j]=sum;
        }
    return C;
  }

void Matrix::normalize_them(Matrix &B, Matrix &E, I i, T rownorm)  //2011
  {
    if (rownorm>0.0) //some rows may be zero
      {
        T scale = 1/rownorm;
        for (int j=0; j<n_; j++)
          v_[i][j] = v_[i][j]*scale;
        for (int j=0; j<B.n_; j++)
          B(i,j) = B(i,j)*scale;
        for (int j=0; j<E.n_; j++)
          E(i,j) = E(i,j)*scale;
      }
    //else what to do with B and E if row is zero??
  }

void Matrix::normalize_rows(Matrix &B, Matrix &E)
  {
    if (m_ != B.m_  || m_ != E.m_) Matrix::xerror(2,"normalize_rows(...)");
    for (I i=0; i<m_; i++)
      {
        Matrix R=this->get_row(i);
        normalize_them(B,E,i,R.norm());
      }
  }

void Matrix::normalize_rows_max1(Matrix &B, Matrix &E)
  {
    if (m_ != B.m_  || m_ != E.m_) Matrix::xerror(2,"normalize_rows_max1(...)");
    for (I i=0; i<m_; i++)
      {
        Matrix R=this->get_row(i);
        normalize_them(B,E,i,R.maxabs());
      }
  }

void Matrix::mabs()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++) data_[i]=absf(data_[i]);
  }

void Matrix::msqrt()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++) data_[i]=sqrt(absf(data_[i]));
  }

void Matrix::msquare()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++) data_[i]=data_[i]*data_[i];
  }

void Matrix::mlog10()
  {
    int sz=m_*n_;
    T tiny = log10((*this).maxabs()) - 30.0;
    for (int i=0; i<sz; i++)
      if (data_[i]!=0.0) data_[i]=log10(absf(data_[i]));
      else               data_[i] = tiny;
  }

void Matrix::mpow10()
  {
    int sz=m_*n_;
    T scale=log(10.0);
    for (int i=0; i<sz; i++) data_[i]=exp(scale*data_[i]);
  }

void Matrix::at_least(T x)
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++) if (data_[i]<x) data_[i]=x;
  }

void Matrix::keep_digits(I n)
  {
    I p=1;
    for (I i=1; i<n; i++) p*=10;
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      {
        if (data_[i]==0.0) continue;
        T x=absf(data_[i]);
        T q=1;
        while (x>10*p) { x/=10; q*=10; }
        while (x<p)    { x*=10; q/=10; }
        I val=x+0.5;
        x = val;
        x *= q;
        if (data_[i]>0.0) data_[i] = x; else data_[i] = -x;
      }
  }

void Matrix::trunc()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      {
        if (data_[i]<0.0) data_[i] = -int(absf(data_[i]));
        else              data_[i] =  int(     data_[i] );
      }
  }

void Matrix::round()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      {
        if (data_[i]<0.0) data_[i] = -int(absf(data_[i])+0.5);
        else              data_[i] =  int(     data_[i] +0.5);
      }
  }

void Matrix::ceil()
  {
    int sz=m_*n_;
    int k;
    T d;
    for (int i=0; i<sz; i++)
      {
        k = int(data_[i]);
        d = T(k);
        data_[i] = d<data_[i] ? d+1 : d;
      }
  }

void Matrix::signum()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      {
        if (data_[i]< 0.0) data_[i] = -1.0;
        else               data_[i] =  1.0;
      }
  }

void Matrix::trinity()
  {
    int sz=m_*n_;
    for (int i=0; i<sz; i++)
      {
        if (data_[i]< 0.0) data_[i] = -1.0;
        if (data_[i]> 0.0) data_[i] =  1.0;
      }
  }

void Matrix::to_percentages()
  {
    for (int k=0; k<n_; k++)
      {
        Matrix obs=get_column(k);
        T sum=obs.sumabs();
        if (sum>0.0)obs*=100.0/sum;
        set_column(k,obs);
      }
  }

T Matrix::maxval() const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::max()");
    T t=data_[0];
    for (int i=1; i<sz; i++) { if (data_[i]>t) t = data_[i]; }
    return t;
  }

T Matrix::minval() const
  {
    I sz=m_*n_;
    if (sz<1) return 0.0;
    T t=data_[0];
    for (int i=1; i<sz; i++) { if (data_[i]<t) t = data_[i]; }
    return t;
  }

T Matrix::maxabs() const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::maxabs()");
    T t=absf(data_[0]);
    for (int i=1; i<sz; i++) { if (absf(data_[i])>t) t = absf(data_[i]); }
    return t;
  }

T Matrix::minabs() const
  {
    I sz=m_*n_;
    if (sz<1) return 0.0;
    T t=absf(data_[0]);
    for (int i=1; i<sz; i++) { if (absf(data_[i])<t) t = absf(data_[i]); }
    return t;
  }

T Matrix::minpos() const
  {
    I sz=m_*n_;
    if (sz<1) return 0.0;
    T t=maxabs();
    for (int i=0; i<sz; i++) { if (data_[i]>0.0 && data_[i]<t) t = data_[i]; }
    return t;
  }

void Matrix::ijmaxabs(I &imax, I &jmax) const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::ijmaxabs()");
    T t=absf(data_[0]);
    imax=0; jmax=0;
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        if (absf(v_[i][j])>t) { t=absf(v_[i][j]); imax=i; jmax=j; }
  }

T Matrix::sum() const
  {
    I sz=m_*n_;
    if (sz<1) return 0.0;
    T t=0.0;
    for (int i=0; i<sz; i++) { t=t+data_[i]; }
    return t;
  }

T Matrix::sumabs() const
  {
    I sz=m_*n_;
    if (sz<1) return 0.0;
    T t=0.0;
    for (int i=0; i<sz; i++) { t=t+absf(data_[i]); }
    return t;
  }

I Matrix::num_non_zero() const
  {
    I sz=m_*n_;
    if (sz<1) return 0;
    I k=0;
    for (int i=0; i<sz; i++) { if (data_[i]!=0.0) k++; }
    return k;
  }

I Matrix::num_non_negative() const
  {
    I sz=m_*n_;
    if (sz<1) return 0;
    I k=0;
    for (int i=0; i<sz; i++) { if (data_[i]>=0.0) k++; }
    return k;
  }

T Matrix::norm2_as_vector() const
  {
    I mn=m_*n_;
    T sum=0.0;
    for (int i=0; i<mn; i++) { sum += data_[i]*data_[i]; }
    return sum;
  }

T Matrix::norm2() const
  {
    if (m_!=1 && n_!=1) Matrix::xerror(5,"Matrix::norm2()");
    return norm2_as_vector();
  }

T Matrix::rowdot(I i,I k) const
  {
    T sum=0.;
    for (int j=0; j<n_; j++) sum += v_[i][j]*v_[k][j];
    return sum;
  }

Matrix Matrix::t()
  {
    Matrix B(*this);
    clear();
    setup2(B.n_,B.m_);
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = B[j][i];
    return *this;
  }

T Matrix::dot (Matrix &B) const
  {
    if ( (m_!=1 && n_!=1) || (B.m_!=1 && B.n_!=1) )
      Matrix::xerror(2,"Matrix::dot(Matrix)");
    I amn = m_*n_;
    I bmn = B.m_ * B.n_;
    if (amn!=bmn) Matrix::xerror(2,"Matrix.dot(Matrix)");
    T sum=0.0;
    for (I i=0; i<amn; i++) sum += data_[i]*B.data_[i];
    return sum;
  }

Matrix Matrix::get_row (I i) const
  { Matrix r(1,n_); for (int j=0; j<n_; j++) r[0][j] = data_[i*n_ + j]; return r; }

Matrix Matrix::get_column (I j) const
  { Matrix c(m_,1); for (int i=0; i<m_; i++) c[i][0] = data_[i*n_ + j]; return c; }

void Matrix::set_row (I i, T val)
  {
    if (i<0 || i>=m_) Matrix::xerror(1,"Matrix::set_row(i,val)");
    for (int j=0; j<n_; j++) data_[i*n_ + j] = val;
  }

void Matrix::set_column (I j, T val)
  {
    if (j<0 || j>=n_) Matrix::xerror(1,"Matrix::set_column(j,val)");
    for (int i=0; i<m_; i++) data_[i*n_ + j] = val;
  }

void Matrix::set_row (I i, Matrix A)
  {
    if (n_ != A.n_ || A.m_!=1) Matrix::xerror(1,"Matrix::set_row(i,A)");
    if (i<0 || i>=m_) Matrix::xerror(1,"set_row");
    for (int j=0; j<n_; j++) data_[i*n_ + j] = A[0][j];
  }

void Matrix::set_column (I j, Matrix A)
  {
    if (m_ != A.m_ || A.n_!=1) Matrix::xerror(1,"Matrix::set_column(i,A)");
    if (j<0 || j>=n_) Matrix::xerror(1,"set_column");
    for (int i=0; i<m_; i++) data_[i*n_ + j] = A[i][0];
  }

void Matrix::set_row_zero(I i)
  {
    if (i<0 || i>=m_) Matrix::xerror(1,"Matrix::set_row_zero");
    for (int j=0; j<n_; j++) data_[i*n_ + j] = 0.0;
  }

void Matrix::set_column_zero(I j)
  {
    if (j<0 || j>=n_) Matrix::xerror(1,"Matrix::set_column_zero");
    for (int i=0; i<m_; i++) data_[i*n_ + j] = 0.0;
  }

void Matrix::resize(I m,I n)
  {
    if (m==m_ && n==n_) return;  //nothing to do
    checkdim(m,n);
    Matrix B(*this); //save a copy
    clear();         //discard current contents
    setup2(m,n);     //reconfigure
    if (m_>0 && n_>0)
      {
        //copy whatever can be copied
        I mm=minf(m_,B.m_);
        I nn=minf(n_,B.n_);
        for (int i=0; i<mm; i++)
          for (int j=0; j<nn; j++)
            v_[i][j] = B[i][j];
      }
  }

void Matrix::del_row(I r)
  {
    //shift later rows up one
    for (int i=r; i<m_-1; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = v_[i+1][j];
    resize(m_-1,n_);
  }

void Matrix::del_column(I c)
  {
    //shift later columns left one
    for (int i=0; i<m_; i++)
      for (int j=c; j<n_-1; j++)
        v_[i][j] = v_[i][j+1];
    resize(m_,n_-1);
  }

void Matrix::append_rows(const Matrix &B)
  {
    if (m_ == 0 || n_ == 0) { *this = B; return; }  //append to null matrix
    if (n_ != B.n_) Matrix::xerror(2,"Matrix::append_rows");
    int mm=m_;  //save current row size of *this
    resize(m_ + B.m_ , n_);
    for (int i=0; i<B.m_; i++)
      for (int j=0; j<n_; j++)
        v_[mm+i][j] = B[i][j];
  }

void Matrix::prepend_rows(const Matrix &B)
  {
    if (m_ == 0 || n_ == 0) { *this = B; return; }
    if (n_ != B.n_) Matrix::xerror(2,"Matrix::prepend_rows");
    Matrix C=B;
    C.append_rows(*this);
    *this = C;
  }

void Matrix::append_columns(const Matrix &B)
  {
    if (m_ == 0 || n_ == 0) { *this = B; return; }
    if (m_ != B.m_) Matrix::xerror(2,"Matrix::append_columns");
    int nn=n_;  //save current column size of *this
    resize(m_ , n_ + B.n_);
    for (int i=0; i<m_; i++)
      for (int j=0; j<B.n_; j++)
        v_[i][nn+j] = B[i][j];
  }

void Matrix::prepend_columns(const Matrix &B)
  {
    if (m_ == 0 || n_ == 0) { *this = B; return; }
    if (m_ != B.m_) Matrix::xerror(2,"Matrix::prepend_columns");
    Matrix C=B;
    C.append_columns(*this);
    *this = C;
  }

void Matrix::zeros()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = 0.0;
  }

void Matrix::ones()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = 1.0;
  }

void Matrix::identity()
  {
    Matrix::zeros();
    int mn = minf(m_,n_);
    for (int i=0; i<mn; i++) v_[i][i]=1.0;
  }

void Matrix::iota()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = i+j+1;
  }

void Matrix::iotazero()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = i+j;
  }


void Matrix::random()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = Matrix::myrandom();
  }

void Matrix::gauss()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = Matrix::mygauss();
  }

void Matrix::hilbert()
  {
    for (int i=0; i<m_; i++)
      for (int j=0; j<n_; j++)
        v_[i][j] = 1.0/T(i+j+1);
  }

void Matrix::heat()
  {
    //a suggested problem size is 11 x 51
    if (m_ == 0 || n_ == 0) return;
    T xx,tt;
    for (int i=0; i<m_; i++)
      {
        xx = 1.5*T(i)/T(m_-1);
        for (int j=0; j<n_; j++)
          {
            tt = 1.5*T(j)/T(n_-1);
            v_[i][j] = exp(-(xx-tt)*(xx-tt));
          }
      }
  }

void Matrix::laplace()
  {
    //a suggested problem size is 19 x 21
    if (m_ == 0 || n_ == 0) return;
    T s,t;
    for (int i=0; i<m_; i++)
      {
        s = 0.5 + 5.0*T(i)/T(m_);
        for (int j=0; j<n_; j++)
          {
            t = 5.0*T(j)/T(n_);
            v_[i][j] = exp(-s*t);
          }
      }
  }

void Matrix::cusp()
  {
    if (m_ == 0 || n_ == 0) return;
    I mn=m_*n_;  //normally should be a Row or Vector... otherwise punt
    for (int i=0; i<mn; i++) data_[i] = sin(pif()*double(i)/double(m_-1));
  }

//supports cout << A; rather than A.print()
ostream& operator<<(ostream& os, const Matrix &A)
  {
    //A.print();
    I m=A.dim1();
    I n=A.dim2();
    os << "Matrix is " << m << " rows by " << n << " columns:" << endl;
    for (int i=0; i<m; i++)
      {
        for (int j=0; j<n; j++) os << setw(12) << setprecision(6) << A(i,j) << " ";
        os << endl;
      }
    os << endl;
    return os;
  }

void Matrix::print() const
  {
    cout << *this;
  }

void Matrix::print_by_row() const
  {
    cout << "Matrix is " << m_ << " rows by " << n_ << " columns:" << endl;
    for (int i=0; i<m_; i++)
      {
        for (int j=0; j<n_; j++)
          {
            if (j%5==0)
              {
                if (j==0) cout         << "Row " << setw(4) << i;
                else      cout << endl << "        ";
              }
            cout << setw(12) << setprecision(6) << data_[i*n_+j] << " ";
          }
        cout << endl;
      }
    cout << endl;
  }

void Matrix::print_by_column() const
  {
    cout << "Matrix is " << m_ << " rows by " << n_ << " columns:" << endl;
    for (int j=0; j<n_; j++)
      {
        for (int i=0; i<m_; i++)
          {
            if (i%5==0)
              {
                if (i==0) cout         << "Col " << setw(4) << j;
                else      cout << endl << "        ";
              }
            cout << setw(12) << setprecision(6) << data_[i*n_+j] << " ";
          }
        cout << endl;
      }
    cout << endl;
  }

void Matrix::printA() const
  {
    if (dmin()==0) { cout << "(matrix is empty)" << endl; return; }
    T t;
    T small=0.00001;
    I m=dim1();
    I n=dim2();
    cout << "Matrix is " << m << " rows by " << n << " columns:" << endl;

    int mx=m; if (mx>15) mx=15;
    int nn=n; if (nn>6) nn=6;
    for (int i=0; i<mx; i++)
      {
        cout << setw(2) << i << ":";
        for (int j=0; j<nn; j++)
	     {
	       t=v_[i][j];
	       if (absf(t)<small) t=0.0;
             cout << setw(10) << setprecision(4) <<  t    << " ";
           }
        if (n>nn) cout << "...";
        cout << endl;
      }
    if (mx<m) cout << "          ..." << endl;
    cout << endl;
  }

void Matrix::printAb(const Matrix &b)const   //revise like following routine???
  {
    if (dmin()==0) { cout << "(matrix is empty)" << endl; return; }
    T t;
    T small=0.00001;
    I m=dim1();
    I n=dim2();
    if (dim1() != b.dim1()) Matrix::xerror(2,"Matrix::printAb");

    cout << "Matrix is " << m << " rows by " << n << " columns:" << endl;

    int mx=m; if (mx>15) mx=15;
    int nn=n; if (nn>5) nn=5;
    for (int i=0; i<mx; i++)
      {
        cout << setw(2) << i << ":";
        for (int j=0; j<nn; j++)
	    {
	      t=v_[i][j];
	      if (absf(t)<small) t=0.0;
            cout << setw(10) << setprecision(4) <<  t  << " ";
          }
        if (n>nn) cout << "...";

        T bb = b[i][0];
        if (absf(bb)<small) bb=0.0;
        cout << " = " << setw(10) << setprecision(4) << bb;
        cout << endl;
      }

    if (mx<m) cout << "          ..." << setw(58) << " " << "..." << endl;
    cout << setprecision(0) << endl;
  }

void Matrix::printAbe(const Matrix &b, const Matrix &e)const
  {
    if (dmin()==0) { cout << "(matrix is empty)" << endl; return; }
    T t;
    I m=dim1();
    I n=dim2();
    if (dim1() != b.dim1()) Matrix::xerror(2,"Matrix::printAbe");
    if (dim1() != e.dim1()) Matrix::xerror(2,"Matrix::printAbe");

    cout << "Matrix is " << m << " rows by " << n << " columns:" << endl;

//format is... (i is in first 2 columns)
//0000000111111111122222222223333333333444444444455555555556666666666777777777
//3456789012345678901234567890123456789012345678901234567890123456789012345678
//:-1.234E-15 -2.345E-14 xxxxxxxxxx yyyyyyyyyyy ... = rrrrrrrrrr +- eeeeeeeeee

    int mx=m; if (mx>15) mx=15;
    int nn=n; if (nn>4) nn=4;
    for (int i=0; i<mx; i++)
      {
        cout << setw(2) << i << ":";
        for (int j=0; j<nn; j++)
	    {
	      t=v_[i][j];
            cout << setw(10) << setprecision(4) << t << " ";
          }
        if (n>nn) cout << "... ";

        T bb = b[i][0];
        cout << "= " << setw(10) << setprecision(4) << bb << " ";

        T ee = e[i][0];
        cout << "+- " << setw(10) << setprecision(4) << ee;
        cout << endl;
      }

    if (mx<m)
      {
        for (int j=0; j<nn; j++) cout << "    ...    ";
        if (n>nn) cout << "... ";
        cout << "=    ...     ";
        cout << "+-    ...";
        cout << endl;
      }
    cout << setprecision(0) << endl;
  }

void Matrix::printAxb(const Matrix &x, const Matrix &b, I maxrows) const
  {
    if (dmin()==0) { cout << "(matrix is empty)" << endl; return; }
    T t;
    T small=0.00001;
    I m=dim1();
    I n=dim2();
    if (dim2() != x.dim1()) Matrix::xerror(2,"Matrix::printAxb");
    if (dim1() != b.dim1()) Matrix::xerror(2,"Matrix::printAxb");

    cout << "Matrix is " << m << " rows by " << n << " columns:" << endl;

    int mx=m; if (n>m) mx=n; if (mx>maxrows) mx=maxrows;
    int nn=n; if (nn>4) nn=4;
    for (int i=0; i<mx; i++)
      {
        cout << setw(2) << i << ":";
        if (i<m)
          {
            for (int j=0; j<nn; j++)
	         {
	           t=v_[i][j];
	           if (absf(t)<small) t=0.0;
                cout << setw(10) << setprecision(4) <<  t    << " ";
              }
            if (n>nn) cout << "...";
          }
        else
          {
	       for (int k=0; k<nn; k++) cout << setw(10) << "  "  << " ";
            if (n>nn) cout << "   ";
          }

        T xx = i<n ? x[i][0] : 0.0;
        if (absf(xx)<small) xx=0.0;
        if (i<n)  cout << " x" << setw(10) << setprecision(4) << xx;
        else      cout << "  " << setw(10) << " ";

        T bb = i<m ? b[i][0] : 0.0;
        if (absf(bb)<small) bb=0.0;
        if (i<m) cout << " = " << setw(10) << setprecision(4) << bb;
        cout << endl;
      }

    if (mx<m) cout << "          ...";
    else      cout << "             ";
    if (mx<n) cout << setw(46) << " " << "...      ";
    else      cout << setw(55) << " ";
    if (mx<m) cout << "    ... ";
    cout << endl;

    cout << setprecision(0) << endl;
    //int junk; cout << "OK? "; cin >> junk;
  }

Matrix Matrix::compute_star_magnitudes() const
  {
    Matrix mag(*this);
    if (dmin()==0) return mag;
    mag.mabs();            //absolute values
    T mx = mag.maxabs();   //largest
    if (mx==0.0) mx = 1.0; //handle all-zeroes case
    mag = mag/mx;          //largest is now 1.0
    mag.mlog10();          //logs range from  0.0 to -15 or so
    mag-=1.0;              //logs range from -1.0 to -16 or so
    mag.mabs();            //now all are flipped positive...ranging from 1.0 to 16
    return mag;
  };

void Matrix::print_star_magnitudes() const
  {
    if (dmin()==0) { cout << "(matrix is empty)" << endl; return; }
    I m=dim1();
    I n=dim2();
    cout << "Printing star magnitudes with rows= " << m << "  columns= " << n << endl;
    T scale = this->maxabs();
    if (scale==0.0) scale = 1.0;
    cout << "Magnitude 1 is " << scale << " to >" << scale/10.0 << endl;
    Matrix B = this->compute_star_magnitudes();
    int k;

    cout << " "; for (int j=0; j<n; j++) cout << "-" ; cout << endl;
    for (int i=0; i<m; i++)
      {
        cout << "|";
        for (int j=0; j<n; j++)
          {
            k = int(B[i][j]);
            if (k<=9) cout << setw(1) << k;
            else      cout << " ";
          }
        cout << "|" << endl;
      }
    cout << " "; for (int j=0; j<n; j++) cout << "-" ; cout << endl;
  };

//Free functions for Matrix------------------------------------------

//supports 2.0+A for example...returns element[i][j] = 2.0+A[i][j]
Matrix operator + (T x, const Matrix &A) { Matrix B(A); B+=x; return B; }

//supports 2.0*A for example...returns element[i][j] = 2.0*A[i][j]
Matrix operator * (T x, const Matrix &A) { Matrix B(A); B*=x; return B; }

//supports 2.0-A for example...returns element[i][j] = 2.0-A[i][j]
Matrix operator - (T x, const Matrix &A)
  {
    Matrix B(A.dim1() , A.dim2() , x);
    B-=A;
    return B;
  }

//returns the transpose of the object.
//Note that the object itself is NOT modified.  To modify the object, use A.t()
Matrix transpose (const Matrix& A)
  {
    I mm=A.dim1();
    I nn=A.dim2();
    Matrix B(nn,mm);
    for (int i=0; i<nn; i++)
      for (int j=0; j<mm; j++)
        B[i][j] = A[j][i];
    return B;
  }

T pilof() { return 3.14159265358979 - 2.0*Matrix::roundoff(); }
T pihif() { return 3.14159265358980 + 2.0*Matrix::roundoff(); }

//implementations for Diagonal

void Diagonal::setupd(I m, I n)
  {
    checkdim(m,n);
    m_ = m;
    n_ = n;
    mm_ = m_; if (n_<m_) mm_ = n_;
    if (m_==0 || n_==0) return;

    data_ = new T[mm_];
    for (int i=0; i<mm_; i++) data_[i] = 0.0;
  }

Diagonal::Diagonal(int m, int n, T x)
  {
    setupd(m,n);
    for (int i=0; i<mm_; i++) data_[i] = x;
  }

Diagonal::Diagonal(int m, int n, const T *a)
  {
    setupd(m,n);
    for (int i=0; i<mm_; i++) data_[i] = a[i];
  }

Diagonal::Diagonal(const Diagonal &D)
  {
    I m = D.dim1();
    I n = D.dim2();
    setupd(m,n);
    for (int i=0; i<mm_; i++) data_[i] = D[i];
  }

Diagonal::Diagonal(int m, int n, const Diagonal &D)
  {
    setupd(m,n);
    I mm = minf(mm_ , D.mm_);
    for (int i=0; i<mm; i++) data_[i] = D[i];
  }

Diagonal::Diagonal(const Matrix &A)
  {
    I m=A.dim1();
    I n=A.dim2();
    if (m==1)
      {
        setupd(n,n);
        for (int j=0; j<n; j++) data_[j] = A[0][j];
      }
    else if (n==1)
      {
        setupd(m,m);
        for (int i=0; i<m; i++) data_[i] = A[i][0];
      }
    else
      {
        setupd(m,n);
        for (int i=0; i<mm_; i++) data_[i] = A[i][i];
      }
  };

Diagonal Diagonal::operator=(const T x)
  {
    for (int i=0; i<mm_; i++) data_[i] = x;
    return *this;
  }

Diagonal Diagonal::operator=(const Diagonal &D)
  {
    clear();
    I m = D.dim1();
    I n = D.dim2();
    setupd(m,n);
    for (int i=0; i<mm_; i++) data_[i] = D[i];
    return *this;
  }

void Diagonal::resize(I m,I n)
  {
    Diagonal P=*this;
    setupd(m,n);
    I mm = minf(mm_ , P.mm_);
    for (int i=0; i<mm; i++) data_[i] = P[i];
 }

Diagonal Diagonal::operator=(const Matrix &A)
  {
    Diagonal E(A);
    *this = E;
    return *this;
  }

bool Diagonal::operator==(const Diagonal &D) const
  {
    if (m_ != D.m_ || n_ != D.n_ ) return false;
    for (int i=0; i<mm_; i++)
      { if (data_[i] != D.data_[i]) return false; }
    return true;
  }

bool Diagonal::approximate(const Diagonal &D, T tolerance) const
  {
    if (m_ != D.m_ || n_ != D.n_ ) return false;
    for (int i=0; i<mm_; i++)
      { if (absf(data_[i]-D.data_[i])>tolerance) return false; }
    return true;
  }

Diagonal Diagonal::operator += (const Diagonal &D)
  {
    if (m_!=D.m_ || n_!=D.n_) Matrix::xerror(2,"Diagonal+=Diagonal");
    for (int i=0; i<mm_; i++) data_[i]+=D[i];
    return *this;
  }

Diagonal Diagonal::operator -= (const Diagonal &D)
  {
    if (m_!=D.m_ || n_!=D.n_) Matrix::xerror(2,"Diagonal-=Diagonal");
    for (int i=0; i<mm_; i++) data_[i]-=D[i];
    return *this;
  }

Diagonal Diagonal::operator - () const
  {
    Diagonal D(*this); for (int i=0; i<mm_; i++) D[i] = -D[i];
    return D;
  }

Diagonal Diagonal::operator + (const Diagonal &D) const
  {
    Diagonal E(*this);
    E+=D;
    return E;
  }

Diagonal Diagonal::operator - (const Diagonal &D) const
  {
    Diagonal E(*this);
    E-=D;
    return E;
  }

Diagonal Diagonal::operator * (const Diagonal &D) const
  {
    if (n_ != D.m_) Matrix::xerror(2,"Diagonal*Diagonal");
    Diagonal E(m_ , D.n_);
    if (m_==0 || n_==0 || D.m_==0 || D.n_==0) return E;

    I m=minf(mm_,D.mm_);  //smallest of all 4 dimensions! 2011
    for (int i=0; i<m; i++) E[i] = data_[i]*D[i];
    return E;
  }

void Diagonal::mabs()
  { for (int i=0; i<mm_; i++) data_[i]=absf(data_[i]); }

void Diagonal::msqrt()
  { for (int i=0; i<mm_; i++) data_[i]=sqrt(absf(data_[i])); }

void Diagonal::msquare()
  { for (int i=0; i<mm_; i++) data_[i]=data_[i]*data_[i]; }

void Diagonal::mlog10()
  {
    T tiny = log10((*this).maxabs()) - 30.0;
    for (int i=0; i<mm_; i++)
      if (data_[i]!=0.0) data_[i]=log10(absf(data_[i]));
      else               data_[i]=tiny;
  }

void Diagonal::mpow10()
  {
    T scale=log(10.0);
    for (int i=0; i<mm_; i++) data_[i]=exp(scale*data_[i]);
  }

void Diagonal::at_least(T x)
  { for (int i=0; i<mm_; i++) if (data_[i]<x) data_[i]=x; }

T Diagonal::maxabs() const
  {
    if (mm_<1) return 0.0;
    T t=absf(data_[0]);
    for (int i=1; i<mm_; i++) { if (absf(data_[i])>t) t = absf(data_[i]); }
    return t;
  }

T Diagonal::minabs() const
  {
    if (mm_<1) return 0.0;
    T t=absf(data_[0]);
    for (int i=1; i<mm_; i++) { if (absf(data_[i])<t) t = absf(data_[i]); }
    return t;
  }

I Diagonal::imaxabs() const
  {
    if (mm_<1) Matrix::xerror(3,"Diagonal::imaxabs");
    T t=absf(data_[0]); I k=0;
    for (int i=1; i<mm_; i++) { if (absf(data_[i])>t) { t = absf(data_[i]); k=i; } }
    return k;
  }

I Diagonal::iminabs() const
  {
    if (mm_<1) Matrix::xerror(3,"Diagonal::iminabs");
    T t=absf(data_[0]); I k=0;
    for (int i=1; i<mm_; i++) { if (absf(data_[i])<t) { t = absf(data_[i]); k=i; } }
    return k;
  }

I Diagonal::ilastnz() const
  {
    if (mm_<1) Matrix::xerror(3,"Diagonal::iminabs");
    I k=-1;
    for (int i=0; i<mm_; i++) { if (data_[i]!=0.0) k=i; }
    return k;
  }

T Diagonal::trace() const
   {
    T t=0.0;
    for (int i=0; i<mm_; i++) t += data_[i];
    return t;
  }

T Diagonal::epsilon() const { return maxabs()*8.0*Matrix::roundoff(); }

I Diagonal::plateau() const
  {
    T eps = 10.0*epsilon();
    T seps = sqrt(eps);

    I p=0;
    for (int i=mm_-1; i>0; i--)
      {
        if (data_[i-1]>eps && data_[i-1]>(1.0+seps)*data_[i]) break;
        p++;
      }
    if (p>=mm_-1) p=0;  //for a constant set of singular values.
    return p;
  }

void Diagonal::zeros()
  { for (int i=0; i<mm_; i++) data_[i]=0.0; }

void Diagonal::ones()
  { for (int i=0; i<mm_; i++) data_[i]=1.0; }

void Diagonal::identity()
  { for (int i=0; i<mm_; i++) data_[i]=1.0; }

void Diagonal::iota()
  { for (int i=0; i<mm_; i++) data_[i]=i+1; }

void Diagonal::random()
  { for (int i=0; i<mm_; i++) data_[i]=Matrix::myrandom(); }

void Diagonal::gauss()
  { for (int i=0; i<mm_; i++) data_[i]=Matrix::mygauss(); }

ostream& operator<<(ostream& os, const Diagonal &D)
  {
    I m=D.dim1();
    I n=D.dim2();
    I mm=D.dmin();
    os << "Matrix is Diagonal of size " << m << " rows by "
                                        << n << " columns:" << endl;
    for (int i=0; i<mm; i++)
      {
        for (int j=0; j<i; j++) os << " ";
        os << D[i] << " " << endl;
      }
    for (int i=mm; i<m; i++)
      {
        for (int j=0; j<i; j++) os << " ";
        os << "..." << endl;
      }
    os << endl;
  }

void Diagonal::print() const
  {
    cout << *this;
  }

//Free functions for Diagonal-------------------------------

//supports 2.0+D for example...returns element[i][j] = 2.0+D[i][j]
Diagonal operator + (T x, const Diagonal &D) { Diagonal B(D); B+=x; return B; }

//supports 2.0*D for example...returns element[i][j] = 2.0*D[i][j]
Diagonal operator * (T x, const Diagonal &D) { Diagonal B(D); B*=x; return B; }

//supports 2.0-D for example...returns element[i][j] = 2.0-D[i][j]
Diagonal operator - (T x, const Diagonal &D)
  {
    Diagonal B(D.dim1() , D.dim2() , x);
    B-=D;
    return B;
  }

//returns a Diagonal which is the transpose of the argument.
//The argument is not changed.
Diagonal transpose (const Diagonal &D)
  {
    Diagonal B(D);
    B.t();
    return B;
  }

//returns the pseudo-inverse of a Diagonal matrix D.
//Values below or near roundoff times the largest (magnitude) diagonal
//element are considered to be zero
Diagonal pseudoinverse(const Diagonal &D)
  {
    I mm=D.dmin();
    if (mm<1) Matrix::xerror(3,"pseudoinverse(diagonal)");
    Diagonal S=transpose(D);
    T eps = D.epsilon();
    for (int i=0; i<mm; i++)
      if (S[i] > eps) S[i] = 1.0/S[i];
      else            S[i] = 0.0;
    return S;
  }

//returns the smoothed (regularized) pseudo-inverse of a Diagonal matrix D.
//Values below or near roundoff times the largest (magnitude) diagonal
//element are considered to be zero
Diagonal smoothinverse(const Diagonal &S, T lambda)
  {
    I m=S.dmin();
    if (m<1) Matrix::xerror(3,"smoothinverse(diagonal)");

    if (lambda==0.0) return pseudoinverse(S);
    T lambda2 = lambda*lambda;
    Diagonal P=transpose(S);
    for (int i=0; i<m; i++) P[i] = S[i]/(S[i]*S[i] + lambda2);

    T eps = S.epsilon();
    T large = 1.0/eps;
    for (int i=0; i<m; i++) if (P[i]>large) P[i] = 0.0;

    return P;
  }

//Tikhonov regularize the Matrix A with given lambda.
//Seldom necessary. Use the above when possible.
Diagonal regularize(const Diagonal &S, T lambda)
  {
    I m=S.dim1();
    I n=S.dim2();
    if (m==0 || n==0) return Diagonal(m,n);
    if (lambda<=0.0) return S;

    T lambda2=lambda*lambda;
    T eps = S.epsilon();
    Diagonal P=S;
    for (int i=0; i<P.dmin(); i++)
      {
        if (S[i]>eps) P[i] = (S[i]*S[i] + lambda2)/S[i];
        else          P[i] = 0.0;
      }
    return P;
  }

//returns the condition number of a Diagonal matrix D.
T condition_number(const Diagonal &D)
  {
    I mm=D.dmin();
    if (mm<1) Matrix::xerror(3,"condition_number(diagonal)");
    T a=absf(D[0]);
    T big = a;
    T small = a;
    for (int i=1; i<mm; i++)
      {
        a=absf(D[i]);
        if (a>big) big=a;
        if (a<small) small=a;
      }
    if (big==0.0) return 1.0/Matrix::roundoff();
    if (small==0.0) return 1.0/D.epsilon();
    return big/small;
  }

//returns the condition number of a Diagonal matrix ignoring zero or near-zero values
T condition_number_nonzero(const Diagonal &D)
  {
    I mm=D.dmin();
    if (mm<1) Matrix::xerror(3,"condition_number(diagonal)");
    T eps=D.epsilon();
    T a=absf(D[0]);
    T big = a;
    T small = a;
    for (int i=1; i<mm; i++)
      {
        a=absf(D[i]);
        if (a<=eps) continue;
        if (a>big) big=a;
        if (a<small) small=a;
      }
    if (big==0.0) return 1.0/Matrix::roundoff();
    if (small==0.0) return 1.0/D.epsilon();
    return big/small;
  }

//Matrix * Diagonal
Matrix operator * (const Matrix &A, const Diagonal &D)
  {
    int am = A.dim1();
    int an = A.dim2();

    int mm = D.dmin();
    int bm = D.dim1();
    int bn = D.dim2();
    if (an != bm) Matrix::xerror(2,"Matrix*Diagonal");

    Matrix C(am , bn);
    for (int j=0; j<mm; j++)
      for (int i=0; i<am; i++)
        C[i][j]=A[i][j] * D[j];
    //any extra columns remain at 0.0
    return C;
  }

//Diagonal * Matrix
Matrix operator * (const Diagonal &D , const Matrix &B)
  {
    int am = D.dim1();
    int an = D.dim2();
    int mm = D.dmin();

    int bm = B.dim1();
    int bn = B.dim2();
    if (an != bm) Matrix::xerror(2,"Diagonal*Matrix");

    Matrix C(am , bn);
    for (int i=0; i<mm; i++)
      for (int j=0; j<bn; j++)
        C[i][j] = D[i] * B[i][j];
    //any extra rows remain at 0.0
    return C;
  }

  //create a full Matrix of the same shape as D
  //with diagonal elements taken from D and zeros otherwise
  Matrix full(const Diagonal &D)
    {
      I m=D.dim1();
      I n=D.dim2();
      Matrix A(m,n);
      I mm=D.dmin();
      for (int i=0; i<mm; i++) A[i][i] = D[i];
      return A;
    }



Row::Row(int m, const T* a)
  {
    setup2(1,m);
    for (int i=0; i<m; i++) data_[i] = a[i];
  }

Row::Row(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10)
  {
    int k=Matrix::countargs(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);
    setup2(1,k);
               data_[0] = t1;
    if (k>=2)  data_[1] = t2;
    if (k>=3)  data_[2] = t3;
    if (k>=4)  data_[3] = t4;
    if (k>=5)  data_[4] = t5;
    if (k>=6)  data_[5] = t6;
    if (k>=7)  data_[6] = t7;
    if (k>=8)  data_[7] = t8;
    if (k>=9)  data_[8] = t9;
    if (k>=10) data_[9] = t10;
  }

Row::Row(const Diagonal &D)
  {
    I mm = D.dmin();
    setup2(1,mm);
    for (int i=0; i<mm; i++) data_[i] = D[i];
  }

Row Row::operator=(T x)
  {
    for (int j=0; j<n_; j++) data_[j] = x;
    return *this;
  }

Row Row::operator=(const Matrix &A)
    {
      clear();
      if (A.dim1()>1) Matrix::xerror(2,"Row=Matrix");
      I n=A.dim2();
      setup2(1,n);
      for (int j=0; j<n; j++) data_[j] = A[0][j];
      return *this;
    }



Vector::Vector(int m, const T* a)
  {
    setup2(m,1);
    if (m>0) for (int i=0; i<m; i++) data_[i] = a[i];
  }

Vector::Vector(const Diagonal &D)
  {
    I mm = D.dmin();
    setup2(mm,1);
    for (int i=0;  i<mm; i++) data_[i] = D[i];
  }

Vector::Vector(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10)
{
  int k=Matrix::countargs(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);
  setup2(k,1);
             data_[0] = t1;
  if (k>=2)  data_[1] = t2;
  if (k>=3)  data_[2] = t3;
  if (k>=4)  data_[3] = t4;
  if (k>=5)  data_[4] = t5;
  if (k>=6)  data_[5] = t6;
  if (k>=7)  data_[6] = t7;
  if (k>=8)  data_[7] = t8;
  if (k>=9)  data_[8] = t9;
  if (k>=10) data_[9] = t10;
}

Vector Vector::operator=(T x)
  {
    for (int i=0; i<m_; i++) data_[i] = x;
    return *this;
  }

Vector Vector::operator=(const Matrix &A)
  {
    clear();
    if (A.dim2()>1) Matrix::xerror(2,"Vector=Matrix");
    I m = A.dim1();
    setup2(m,1);
    for (int i=0; i<m; i++) data_[i] = A[i][0];
    return *this;
  }

Vector Vector::operator = (const Diagonal &D)
  {
    clear();
    I m = D.dmin();
    setup2(m,1);
    for (int i=0; i<m; i++) data_[i] = D[i];
    return *this;
  }

I Vector::imax() const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::imaxabs");
    T t=data_[0]; I k=0;
    for (int i=1; i<sz; i++) { if (data_[i]>t) { t = data_[i]; k=i; } }
    return k;
  }

I Vector::imin() const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::iminabs");
    T t=data_[0]; I k=0;
    for (int i=1; i<sz; i++) { if (data_[i]<t) { t = data_[i]; k=i; } }
    return k;
  }

I Vector::imaxabs() const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::imaxabs");
    T t=absf(data_[0]); I k=0;
    for (int i=1; i<sz; i++) { if (absf(data_[i])>t) { t = absf(data_[i]); k=i; } }
    return k;
  }

I Vector::iminabs() const
  {
    I sz=m_*n_;
    if (sz<1) Matrix::xerror(3,"Matrix::iminabs");
    T t=absf(data_[0]); I k=0;
    for (int i=1; i<sz; i++) { if (absf(data_[i])<t) { t = absf(data_[i]); k=i; } }
    return k;
  }

Vector Vector::range(I i, I j)
  {
    if (i<0 || j<0 || i>=dim1() || j>=dim1() || j<i)
      Matrix::xerror(4,"Vector::range(,)");
    Vector x(j-i+1);
    for (int k=i; k<=j; k++) x[k-i] = data_[k];
    return x;
  }

void Vector::push_front(T value)
  {
    I m = m_+1;
    Vector B(*this);
    clear();
    setup2(m,1);
    for (int i=1; i<m; i++) data_[i] = B[i-1];
    data_[0] = value;
  }

void Vector::pop_front()
  {
    I m = m_-1;
    Vector B(*this);
    clear();
    setup2(m,1);
    for (int i=0; i<m; i++) data_[i] = B[i+1];
  }

void Vector::push_end(T value)
  {
    I m = m_+1;
    Vector B(*this);
    clear();
    setup2(m,1);
    for (int i=0; i<m-1; i++) data_[i] = B[i];
    data_[m-1] = value;
  }

void Vector::pop_end()
  {
    I m = m_-1;
    Vector B(*this);
    clear();
    setup2(m,1);
    for (int i=0; i<m; i++) data_[i] = B[i];
  }

void Vector::sort(T *v, T *w, bool carry, I *p, I *q, I m, I n)
  {
    int i,j,k;
    T temp;
    I itemp;

    //note that if m==n or the arguments are misordered nothing happens.
    if (n < m+10)
      {
        //do a bubble sort for short sequences
        for (i=m+1; i<=n; i++)
          for (j=i; j>m; j--)
            {
              if (v[j]>v[j-1]) break;
                            temp=v[j]; v[j]=v[j-1]; v[j-1]=temp;
              if (carry) { itemp=p[j]; p[j]=p[j-1]; p[j-1]=itemp; }
            }
        return;
      }

    //for long sequences split them into two and sort each...
    I mid = (m+n)/2;
    sort(v,w,carry,p,q,m,mid);
    sort(v,w,carry,p,q,mid+1,n);

    //then merge the two parts...
    i=m;
    j=mid+1;
    k=m-1;
    while (i<=mid || j<=n)
      {
        k++;
        if      (i>mid)      { w[k]=v[j]; if (carry) q[k]=p[j]; j++; }
        else if (j>n)        { w[k]=v[i]; if (carry) q[k]=p[i]; i++; }
        else if (v[i]<=v[j]) { w[k]=v[i]; if (carry) q[k]=p[i]; i++; }
        else                 { w[k]=v[j]; if (carry) q[k]=p[j]; j++; }
      }

    //and copy the result back into original arrays
    for (i=m; i<=n; i++) v[i] = w[i];
    if (carry) for (i=m; i<=n; i++) p[i] = q[i];
  }

void Vector::sort()
  {
    I m=(*this).dim1();
    Vector W(m);
    int *p; p=new int;
    int *q; q=new int;
    sort(data_,W.data_,false,p,q,0,m-1);
  }

void Vector::sort(int *p)
  {
    I m=(*this).dim1();
    Vector W(m);
    int *q; q = new int[m];
    sort(data_,W.data_,true,p,q,0,m-1);
  }

T Vector::median() const
  {
    Vector B=*this;
    B.sort();
    I m=B.dim1();
    if (m<1) return 0.0;
    I h=m/2;
    if (2*h==m)
      return 0.5*(B[h-1] + B[h]);
    else
      return B[h];
  }

Vector Vector::moving_average(I w)
  {
    //compute moving average accurately, but sloe
    if (w<=0 || w>m_) Matrix::xerror(7,"Vector::moving_average()");
    I ln=m_-w+1;
    Vector avg(ln);
    T sum;
    for (int i=0; i<ln; i++)
    {
      sum=0.0;
      for (int j=i; j<i+w; j++) sum += data_[j];
      avg[i] = sum;
    }
    return avg;
  }


Vector Vector::moving_average_fast(I w)
  {
    //compute fast moving average which will have cumulative round-off error
    //cost is about 1 addition per element of the input vector
    if (w<=0 || w>m_) Matrix::xerror(7,"Vector::moving_average()");
    I ln=m_-w+1;
    Vector avg = Vector(ln);

    //do first sum
    T sum=0.0;
    for (int j=0; j<w; j++) sum += data_[j];
    avg[0] = sum;

    //do subsequent sum adjustments
    for (int i=1; i<ln; i++)
    {
      sum = sum - data_[i-1] + data_[i+w-1];
      avg[i] = sum;
    }
    return avg;
  }

//end class Vector

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

Matrix householder(I m, const Vector &x)
  {
    I k=x.dim();
    if (m<k+1) Matrix::xerror(11,"Matrix::householder");
    T mu=x.norm();
    Vector v=x;
    if (mu!=0.0)
      {
        T beta = x[0] + signumf(x[0])*mu;
        for (int i=1; i<k; i++) v[i]/=beta;
      }
    v[0] = 1.0;

    //now build the whole matrix
    Matrix h = Matrix::identity(k,k) - (2.0/v.norm2())* v*transpose(v);
    I d=m-k;
    Matrix H = Matrix::identity(m,m);
    for (I i=0; i<k; i++)
      for (I j=0; j<k; j++)
        H(i+d,j+d) = h(i,j);
    return H;
  }

//******************************************************************
//Simple "printer plot" routines for demos.
//******************************************************************

//plot yy versus xx; xx and yy must be columns or rows
void plot(const Matrix &xx, const Matrix &yy)
  {
    if (xx.dim1()*xx.dim2() < 2)
      { cout << "Can't plot one value -- skipping plot" << endl; return; }
    if (xx.dim1()==0 || yy.dim1()==0) return;
    if (xx.dim1()!=1 && xx.dim2()!=1) Matrix::xerror(4,"plot(,)");
    if (yy.dim1()!=1 && yy.dim2()!=1) Matrix::xerror(4,"plot(,)");
    cout << "Plotting " << xx.dim1()*xx.dim2() << " points." << endl;
    Diagonal x(xx);
    Diagonal y(yy);
    if (x.dim1()!=y.dim1()) Matrix::xerror(2,"plot(,)");
    int n=x.dim1();
    if (n<1) return;

    int           width=  n;
    if (n<30)     width=2*n-1;
    if (width>60) width= 60;
    int height=21; if (n<height) height=n;
    char p[61];
    int i,j,k;

    T top=y[0];
    T bot=y[0];
    for (i=0; i<n; i++) if (y[i]>top) top = y[i];
    for (i=0; i<n; i++) if (y[i]<bot) bot = y[i];
    if (bot>0.0 && bot<1.0 && top-bot>.1) bot=0.0; //cool
    if ((top-bot)<1.0 && absf(top)>0.1) top=bot+1.0;
    if (top==bot) { top=bot+1.0; };
    T yinc = (top-bot)/(height-1);
    T ytop = top + 0.5*yinc;
    T ybot = ytop - yinc;

    T xleft=x[0];
    T xrite=x[0];
    for (i=0; i<n; i++) if (x[i]<xleft) xleft = x[i];
    for (i=0; i<n; i++) if (x[i]>xrite) xrite = x[i];
    T xlo=xleft;
    T xhi=xrite;
    if (xrite<=xleft) xrite=xleft+1.0;
    T xinc = (xrite-xleft)/T(width-1);
    xleft = xleft-0.5*xinc;

    for (k=0; k<height; k++) //lines
      {
        for (j=0; j<width; j++) p[j]=' ';

        //axes
        if (xleft<=0.0 && xrite>=0.0)
          { j =int((0.0-xleft)/xinc + 0.001); p[j] = '|'; }
        if (0.0<ytop && 0.0>=ybot)
          {
            for (int a=0; a<width; a++) p[a] = '-';
            if (xleft<=0.0 && xrite>=0.0)
              { j =int((0.0-xleft)/xinc + 0.001); p[j] = '+'; }
          }

        //data points
        for (i=0; i<n; i++)
          {
            if (y[i]>=ytop) continue; //belongs above
            if (y[i]< ybot) continue; //belongs below
            j =int((x[i]-xleft)/xinc + 0.001); p[j] = '*';
          }

        T axis = 0.5*(ytop+ybot);
        if (absf(axis) < (yinc/100.0) ) axis = 0.;
        cout << setw(14) << axis << " ";
        for (j=0; j<width; j++) cout << p[j];  cout << endl;
        ytop = ybot;
        ybot = ytop - yinc;
      }

    for (i=1; i<=15;   i++) cout << ' '; cout << '^';
    for (i=2; i<width; i++) cout << ' '; cout << '^' << endl;

    cout << setw(16) << xlo;
    for (k=0; k<width-12; k++) cout << " ";
    cout << setw(12) << xhi << endl;
    prompt();
  }

//plot yy versus the indices from 0 to the size of yy
void plot(const Matrix yy)
  {
    I m = maxf(yy.dim1(),yy.dim2());
    Vector xx(m); xx.iota();
    plot(xx,yy);
  }

//plot a one-dimensional array of integers
void plot(I* yy, I n)
  {
    Vector y(n);
    for (int i=0; i<n; i++) y[i]=yy[i];
    plot(y);
  }

//dmatrix.h-----------------------------------------------------------
//***********************************************************************
//Decomposition routines adapted from Template Numerical Toolkit.
//***********************************************************************
//Following matrix decomposition routines for SVD, QR, and Eigenvalue
//are adapted from http://math.nist.gov/tnt/index.html.
//These are public domain routines.
//My thanks to the authors for this nice public domain package.

T hypot2(T a, T b)
{
  if (absf(a)<absf(b))
    {
      T c = a/b;
      return absf(b) * sqrt(1.0 + c*c);
    }
  else
    {
      if (a==0.0) return 0.0;
      T c = b/a;
      return absf(a) * sqrt(1.0 + c*c);
    }
}

//the SVD decomposition----------------------------------------------
static void svd (const Matrix& Arg, Matrix& U, Diagonal& s, Matrix& V)
   {
     //Singular Value Decomposition.
     //For an m-by-n matrix A with m >= n, the singular value decomposition is
     //an m-by-n orthogonal matrix U, an n-by-n diagonal matrix S, and
     //an n-by-n orthogonal matrix V so that A = U*S*V'.

     //The singular values, sigma[k] = S[k][k], are ordered so that
     //sigma[0] >= sigma[1] >= ... >= sigma[n-1].

     //The singular value decompostion always exists, so the constructor will
     //never fail.  The matrix condition number and the effective numerical
     //rank can be computed from this decomposition.
     //(Adapted from JAMA, a Java Matrix Library, developed by jointly
     //by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).

     //Must have m>=n
     //Sizes must be Arg(m,n)  U(m,n)   s(m+1)   V(n,n)

      int m = Arg.dim1();
      int n = Arg.dim2();
      if (m<n) Matrix::xerror(4,"svd()");
      if (m<n) return;

      Matrix A;
      A=Arg;
      U = Matrix(m,n,0.0);
      V = Matrix(n,n,0.0);
      s = Diagonal(n);

      T *e; e=new T[n];
      T *work; work = new T[m];
      int nu = minf(m,n);

	//---end of interface changes

      int wantu = 1;  					// boolean
      int wantv = 1;  					// boolean
	int i=0, j=0, k=0;

      // Reduce A to bidiagonal form, storing the diagonal elements
      // in s and the super-diagonal elements in e.
      int nct = minf(m-1,n);
      int nrt = maxf(0,minf(n-2,m));
      for (k = 0; k < maxf(nct,nrt); k++) {
         if (k < nct) {
            // Compute the transformation for the k-th column and
            // place the k-th diagonal in s[k].
            // Compute 2-norm of k-th column without under/overflow.
            s[k] = 0.0;
            for (i = k; i < m; i++) {
               s[k] = hypot2(s[k],A[i][k]);
            }
            if (s[k] != 0.0) {
               if (A[k][k] < 0.0) {
                  s[k] = -s[k];
               }
               for (i = k; i < m; i++) {
                  A[i][k] /= s[k];
               }
               A[k][k] += 1.0;
            }
            s[k] = -s[k];
         }
         for (j = k+1; j < n; j++) {
            if ((k < nct) && (s[k] != 0.0))  {

            // Apply the transformation.

               double t = 0;
               for (i = k; i < m; i++) {
                  t += A[i][k]*A[i][j];
               }
               t = -t/A[k][k];
               for (i = k; i < m; i++) {
                  A[i][j] += t*A[i][k];
               }
            }

            // Place the k-th row of A into e for the
            // subsequent calculation of the row transformation.

            e[j] = A[k][j];
         }
         if (wantu && (k < nct)) {

            // Place the transformation in U for subsequent back
            // multiplication.

            for (i = k; i < m; i++) {
               U[i][k] = A[i][k];
            }
         }
         if (k < nrt) {

            // Compute the k-th row transformation and place the
            // k-th super-diagonal in e[k].
            // Compute 2-norm without under/overflow.
            e[k] = 0;
            for (i = k+1; i < n; i++) {
               e[k] = hypot2(e[k],e[i]);
            }
            if (e[k] != 0.0) {
               if (e[k+1] < 0.0) {
                  e[k] = -e[k];
               }
               for (i = k+1; i < n; i++) {
                  e[i] /= e[k];
               }
               e[k+1] += 1.0;
            }
            e[k] = -e[k];
            if ((k+1 < m) && (e[k] != 0.0)) {

            // Apply the transformation.

               for (i = k+1; i < m; i++) {
                  work[i] = 0.0;
               }
               for (j = k+1; j < n; j++) {
                  for (i = k+1; i < m; i++) {
                     work[i] += e[j]*A[i][j];
                  }
               }
               for (j = k+1; j < n; j++) {
                  double t = -e[j]/e[k+1];
                  for (i = k+1; i < m; i++) {
                     A[i][j] += t*work[i];
                  }
               }
            }
            if (wantv) {

            // Place the transformation in V for subsequent
            // back multiplication.

               for (i = k+1; i < n; i++) {
                  V[i][k] = e[i];
               }
            }
         }
      }

      // Set up the final bidiagonal matrix or order p.

      int p = minf(n,m+1);
      if (nct < n) {
         s[nct] = A[nct][nct];
      }
      if (m < p) {
         s[p-1] = 0.0;
      }
      if (nrt+1 < p) {
         e[nrt] = A[nrt][p-1];
      }
      e[p-1] = 0.0;

      // If required, generate U.

      if (wantu) {
         for (j = nct; j < nu; j++) {
            for (i = 0; i < m; i++) {
               U[i][j] = 0.0;
            }
            U[j][j] = 1.0;
         }
         for (k = nct-1; k >= 0; k--) {
            if (s[k] != 0.0) {
               for (j = k+1; j < nu; j++) {
                  double t = 0;
                  for (i = k; i < m; i++) {
                     t += U[i][k]*U[i][j];
                  }
                  t = -t/U[k][k];
                  for (i = k; i < m; i++) {
                     U[i][j] += t*U[i][k];
                  }
               }
               for (i = k; i < m; i++ ) {
                  U[i][k] = -U[i][k];
               }
               U[k][k] = 1.0 + U[k][k];
               for (i = 0; i < k-1; i++) {
                  U[i][k] = 0.0;
               }
            } else {
               for (i = 0; i < m; i++) {
                  U[i][k] = 0.0;
               }
               U[k][k] = 1.0;
            }
         }
      }

      // If required, generate V.

      if (wantv) {
         for (k = n-1; k >= 0; k--) {
            if ((k < nrt) && (e[k] != 0.0)) {
               for (j = k+1; j < nu; j++) {
                  double t = 0;
                  for (i = k+1; i < n; i++) {
                     t += V[i][k]*V[i][j];
                  }
                  t = -t/V[k+1][k];
                  for (i = k+1; i < n; i++) {
                     V[i][j] += t*V[i][k];
                  }
               }
            }
            for (i = 0; i < n; i++) {
               V[i][k] = 0.0;
            }
            V[k][k] = 1.0;
         }
      }

      // Main iteration loop for the singular values.

      int pp = p-1;
      int iter = 0;
      double eps = pow(2.0,-52.0);
      while (p > 0) {
         int k=0;
		 int kase=0;

         // Here is where a test for too many iterations would go.

         // This section of the program inspects for
         // negligible elements in the s and e arrays.  On
         // completion the variables kase and k are set as follows.

         // kase = 1     if s(p) and e[k-1] are negligible and k<p
         // kase = 2     if s(k) is negligible and k<p
         // kase = 3     if e[k-1] is negligible, k<p, and
         //              s(k), ..., s(p) are not negligible (qr step).
         // kase = 4     if e(p-1) is negligible (convergence).

         for (k = p-2; k >= -1; k--) {
            if (k == -1) {
               break;
            }
            if (absf(e[k]) <= eps*(absf(s[k]) + absf(s[k+1]))) {
               e[k] = 0.0;
               break;
            }
         }
         if (k == p-2) {
            kase = 4;
         } else {
            int ks;
            for (ks = p-1; ks >= k; ks--) {
               if (ks == k) {
                  break;
               }
               double t = (ks != p ? absf(e[ks]) : 0.) +
                          (ks != k+1 ? absf(e[ks-1]) : 0.);
               if (absf(s[ks]) <= eps*t)  {
                  s[ks] = 0.0;
                  break;
               }
            }
            if (ks == k) {
               kase = 3;
            } else if (ks == p-1) {
               kase = 1;
            } else {
               kase = 2;
               k = ks;
            }
         }
         k++;
         // Perform the task indicated by kase.

         switch (kase) {

            // Deflate negligible s(p).

            case 1: {
               double f = e[p-2];
               e[p-2] = 0.0;
               for (j = p-2; j >= k; j--) {
                  double t = hypot2(s[j],f);
                  double cs = s[j]/t;
                  double sn = f/t;
                  s[j] = t;
                  if (j != k) {
                     f = -sn*e[j-1];
                     e[j-1] = cs*e[j-1];
                  }
                  if (wantv) {
                     for (i = 0; i < n; i++) {
                        t = cs*V[i][j] + sn*V[i][p-1];
                        V[i][p-1] = -sn*V[i][j] + cs*V[i][p-1];
                        V[i][j] = t;
                     }
                  }
               }
            }
            break;

            // Split at negligible s(k).

            case 2: {
               double f = e[k-1];
               e[k-1] = 0.0;
               for (j = k; j < p; j++) {
                  double t = hypot2(s[j],f);
                  double cs = s[j]/t;
                  double sn = f/t;
                  s[j] = t;
                  f = -sn*e[j];
                  e[j] = cs*e[j];
                  if (wantu) {
                     for (i = 0; i < m; i++) {
                        t = cs*U[i][j] + sn*U[i][k-1];
                        U[i][k-1] = -sn*U[i][j] + cs*U[i][k-1];
                        U[i][j] = t;
                     }
                  }
               }
            }
            break;

            // Perform one qr step.

            case 3: {
               // Calculate the shift.

               double scale = maxf(maxf(maxf(maxf(
                       absf(s[p-1]),absf(s[p-2])),absf(e[p-2])),
                       absf(s[k])),absf(e[k]));
               double sp = s[p-1]/scale;
               double spm1 = s[p-2]/scale;
               double epm1 = e[p-2]/scale;
               double sk = s[k]/scale;
               double ek = e[k]/scale;
               double b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1)/2.0;
               double c = (sp*epm1)*(sp*epm1);
               double shift = 0.0;
               if ((b != 0.0) || (c != 0.0)) {
                  shift = sqrt(b*b + c);
                  if (b < 0.0) {
                     shift = -shift;
                  }
                  shift = c/(b + shift);
               }
               double f = (sk + sp)*(sk - sp) + shift;
               double g = sk*ek;
               // Chase zeros.

               for (j = k; j < p-1; j++) {
                  double t = hypot2(f,g);
                  double cs = f/t;
                  double sn = g/t;
                  if (j != k) {
                     e[j-1] = t;
                  }
                  f = cs*s[j] + sn*e[j];
                  e[j] = cs*e[j] - sn*s[j];
                  g = sn*s[j+1];
                  s[j+1] = cs*s[j+1];
                  if (wantv) {
                     for (i = 0; i < n; i++) {
                        t = cs*V[i][j] + sn*V[i][j+1];
                        V[i][j+1] = -sn*V[i][j] + cs*V[i][j+1];
                        V[i][j] = t;
                     }
                  }
                  t = hypot2(f,g);
                  cs = f/t;
                  sn = g/t;
                  s[j] = t;
                  f = cs*e[j] + sn*s[j+1];
                  s[j+1] = -sn*e[j] + cs*s[j+1];
                  g = sn*e[j+1];
                  e[j+1] = cs*e[j+1];
                  if (wantu && (j < m-1)) {
                     for (i = 0; i < m; i++) {
                        t = cs*U[i][j] + sn*U[i][j+1];
                        U[i][j+1] = -sn*U[i][j] + cs*U[i][j+1];
                        U[i][j] = t;
                     }
                  }
               }
               e[p-2] = f;
               iter = iter + 1;
            }
            break;

            // Convergence.

            case 4: {
               // Make the singular values positive.

               if (s[k] <= 0.0) {
                  s[k] = (s[k] < 0.0 ? -s[k] : 0.0);
                  if (wantv) {
                     for (i = 0; i <= pp; i++) {
                        V[i][k] = -V[i][k];
                     }
                  }
               }

               // Order the singular values.

               while (k < pp) {
                  if (s[k] >= s[k+1]) {
                     break;
                  }
                  double t = s[k];
                  s[k] = s[k+1];
                  s[k+1] = t;
                  if (wantv && (k < n-1)) {
                     for (i = 0; i < n; i++) {
                        t = V[i][k+1]; V[i][k+1] = V[i][k]; V[i][k] = t;
                     }
                  }
                  if (wantu && (k < m-1)) {
                     for (i = 0; i < m; i++) {
                        t = U[i][k+1]; U[i][k+1] = U[i][k]; U[i][k] = t;
                     }
                  }
                  k++;
               }
               iter = 0;
               p--;
            }
            break;
         }
      };
   }



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

void tred2(Matrix &V, Vector &d, Vector &e)
  {
    // Symmetric Householder reduction to tridiagonal form.
    //  This is derived from the Algol procedures tred2 by
    //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
    //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
    //  Fortran subroutine in EISPACK.
      int n = V.dim1();
      for (int j = 0; j < n; j++) {
         d[j] = V[n-1][j];
      }

      // Householder reduction to tridiagonal form.

      for (int i = n-1; i > 0; i--) {

         // Scale to avoid under/overflow.

         double scale = 0.0;
         double h = 0.0;
         for (int k = 0; k < i; k++) {
            scale = scale + absf(d[k]);
         }
         if (scale == 0.0) {
            e[i] = d[i-1];
            for (int j = 0; j < i; j++) {
               d[j] = V[i-1][j];
               V[i][j] = 0.0;
               V[j][i] = 0.0;
            }
         } else {

            // Generate Householder vector.

            for (int k = 0; k < i; k++) {
               d[k] /= scale;
               h += d[k] * d[k];
            }
            double f = d[i-1];
            double g = sqrt(h);
            if (f > 0) {
               g = -g;
            }
            e[i] = scale * g;
            h = h - f * g;
            d[i-1] = f - g;
            for (int j = 0; j < i; j++) {
               e[j] = 0.0;
            }

            // Apply similarity transformation to remaining columns.

            for (int j = 0; j < i; j++) {
               f = d[j];
               V[j][i] = f;
               g = e[j] + V[j][j] * f;
               for (int k = j+1; k <= i-1; k++) {
                  g += V[k][j] * d[k];
                  e[k] += V[k][j] * f;
               }
               e[j] = g;
            }
            f = 0.0;
            for (int j = 0; j < i; j++) {
               e[j] /= h;
               f += e[j] * d[j];
            }
            double hh = f / (h + h);
            for (int j = 0; j < i; j++) {
               e[j] -= hh * d[j];
            }
            for (int j = 0; j < i; j++) {
               f = d[j];
               g = e[j];
               for (int k = j; k <= i-1; k++) {
                  V[k][j] -= (f * e[k] + g * d[k]);
               }
               d[j] = V[i-1][j];
               V[i][j] = 0.0;
            }
         }
         d[i] = h;
      }

      // Accumulate transformations.

      for (int i = 0; i < n-1; i++) {
         V[n-1][i] = V[i][i];
         V[i][i] = 1.0;
         double h = d[i+1];
         if (h != 0.0) {
            for (int k = 0; k <= i; k++) {
               d[k] = V[k][i+1] / h;
            }
            for (int j = 0; j <= i; j++) {
               double g = 0.0;
               for (int k = 0; k <= i; k++) {
                  g += V[k][i+1] * V[k][j];
               }
               for (int k = 0; k <= i; k++) {
                  V[k][j] -= g * d[k];
               }
            }
         }
         for (int k = 0; k <= i; k++) {
            V[k][i+1] = 0.0;
         }
      }
      for (int j = 0; j < n; j++) {
         d[j] = V[n-1][j];
         V[n-1][j] = 0.0;
      }
      V[n-1][n-1] = 1.0;
      e[0] = 0.0;
   }

// Symmetric tridiagonal QL algorithm.

void tql2 (Matrix &V, Vector &d, Vector &e)
  {
   //  This is derived from the Algol procedures tql2, by
   //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
   //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
   //  Fortran subroutine in EISPACK.
      int n = V.dim1();
      for (int i = 1; i < n; i++) {
         e[i-1] = e[i];
      }
      e[n-1] = 0.0;

      double f = 0.0;
      double tst1 = 0.0;
      double eps = pow(2.0,-52.0);
      for (int l = 0; l < n; l++) {

         // Find small subdiagonal element

         tst1 = maxf(tst1,absf(d[l]) + absf(e[l]));
         int m = l;

        // Original while-loop from Java code
         while (m < n) {
            if (absf(e[m]) <= eps*tst1) {
               break;
            }
            m++;
         }


         // If m == l, d[l] is an eigenvalue,
         // otherwise, iterate.

         if (m > l) {
            int iter = 0;
            do {
               iter = iter + 1;  // (Could check iteration count here.)

               // Compute implicit shift

               double g = d[l];
               double p = (d[l+1] - g) / (2.0 * e[l]);
               double r = hypot2(p,1.0);
               if (p < 0) {
                  r = -r;
               }
               d[l] = e[l] / (p + r);
               d[l+1] = e[l] * (p + r);
               double dl1 = d[l+1];
               double h = g - d[l];
               for (int i = l+2; i < n; i++) {
                  d[i] -= h;
               }
               f = f + h;

               // Implicit QL transformation.

               p = d[m];
               double c = 1.0;
               double c2 = c;
               double c3 = c;
               double el1 = e[l+1];
               double s = 0.0;
               double s2 = 0.0;
               for (int i = m-1; i >= l; i--) {
                  c3 = c2;
                  c2 = c;
                  s2 = s;
                  g = c * e[i];
                  h = c * p;
                  r = hypot2(p,e[i]);
                  e[i+1] = s * r;
                  s = e[i] / r;
                  c = p / r;
                  p = c * d[i] - s * g;
                  d[i+1] = h + s * (c * g + s * d[i]);

                  // Accumulate transformation.

                  for (int k = 0; k < n; k++) {
                     h = V[k][i+1];
                     V[k][i+1] = s * V[k][i] + c * h;
                     V[k][i] = c * V[k][i] - s * h;
                  }
               }
               p = -s * s2 * c3 * el1 * e[l] / dl1;
               e[l] = s * p;
               d[l] = c * p;

               // Check for convergence.

            } while (absf(e[l]) > eps*tst1);
         }
         d[l] = d[l] + f;
         e[l] = 0.0;
      }

      // Sort eigenvalues and corresponding vectors.

      for (int i = 0; i < n-1; i++) {
         int k = i;
         double p = d[i];
         for (int j = i+1; j < n; j++) {
            if (d[j] < p) {
               k = j;
               p = d[j];
            }
         }
         if (k != i) {
            d[k] = d[i];
            d[i] = p;
            for (int j = 0; j < n; j++) {
               p = V[j][i];
               V[j][i] = V[j][k];
               V[j][k] = p;
            }
         }
      }
   }

void print_svd_quality(const Matrix &A, const Matrix &U, const Diagonal &s, const Matrix &V,
         ostream &mout)
  {
    I mn;
    Matrix B=U*s*transpose(V);
    Matrix UTU; UTU=transpose(U)*U;
    Matrix VTV; VTV=transpose(V)*V;
    mn=UTU.dim1(); Matrix IU=Matrix::identity(mn,mn);
    mn=VTV.dim1(); Matrix IV=Matrix::identity(mn,mn);

    mout << "SVD quality: max absolute value of A - U*S*trans(V) is: " << (A-B).maxabs();
    mout << endl;
    mout << "SVD quality: max orthonomality error in U is: " << (UTU-IU).maxabs();
    mout << endl;
    mout << "SVD quality: max orthonomality error in V is: " << (VTV-IV).maxabs();
    mout << endl;
    mout << endl;
  }
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

void sym_eig(const Matrix &A, Matrix &V, Vector &d)
  {
    if (A.dim1() != A.dim2()) Matrix::xerror(2,"Eigenvalue");
    V=A;
    int n = A.dim2();
    d=Vector(n);
    Vector e(n);

    //copy upper right triangle to lower left
    for (int j = 0; j < n; j++)
      for (int i = 0; i < j; i++)
         V[j][i] = V[i][j];

    tred2(V,d,e);
    tql2(V,d,e);
  }

//returns a "compact" SVD of A
void mysvd(const Matrix &A, Matrix &U, Diagonal &S, Matrix &V)
  {
    I m=A.dim1();
    I n=A.dim2();
    if (m*n<1) { U.clear(); S.clear(); V.clear(); return; }

    if (m<n)
      {
        Matrix AT=transpose(A);
        svd(AT,V,S,U);
      }
    else
      {
        svd(A,U,S,V);
      }
  }

//returns a "compact" SVD of A with reduced rank
void mysvdrank(const Matrix &A, I rank, Matrix &U, Diagonal &S, Matrix &V)
  {
    I m=A.dim1();
    I n=A.dim2();
    if (m*n<1) { U.clear(); S.clear(); V.clear(); return; }

    if (m<n)
      {
        Matrix AT=transpose(A);
        svd(AT,V,S,U);
        if (rank<m)
          { V.resize(V.dim1(),rank); S.resize(rank,rank); U.resize(U.dim1(),rank); }
      }
    else
      {
        svd(A,U,S,V);
        if (rank<n)
          { U.resize(U.dim1(),rank); S.resize(rank,rank); V.resize(V.dim1(),rank); }
      }
  }

//Matrix norm == s[0] ... use carefully due to expense
T matrix_norm(const Matrix &A)
  {
    Matrix U,V;
    Diagonal S;
    mysvd(A,U,S,V);
    return S[0];
  }

//return condition number = largest singular value / smallest non-zero singular value
T condition_number(const Matrix &A)
  {
    Matrix U,V; Diagonal S;
    mysvd(A,U,S,V);
    return condition_number(S);
  }

//returns V * S+ * transpose(U)
Matrix pseudoinverse(const Matrix &A)
  {
    Matrix U,V;
    Diagonal S;
    mysvd(A,U,S,V);
    Matrix x = V * pseudoinverse(S) * transpose(U);
    return x;
  }

//general pseudoinverse solver... solves A*x = B.
//Note that B can have multiple columns.
Matrix solve(const Matrix &A, const Matrix &B)
  {
    if (A.dim1() != B.dim1()) Matrix::xerror(2,"solve(A,B)");
    return pseudoinverse(A) * B;
  }

//Allows user to say either x=solve(A,B) or x=B/A
//Note that B can have multiple columns.
Matrix operator / (const Matrix &B, const Matrix &A) { return solve(A,B); }

//power of a Matrix
Matrix power(const Matrix &A, I p)
  {
    I m=A.dim1();
    I n=A.dim2();
    if (m!=n) Matrix::xerror(4,"power");
    if (p==0) { Matrix C(m,m); C.identity(); return C; }

    //if exponent is negative compute the inverse
    Matrix B(A);
    I q=p;
    if (q<0) { B=pseudoinverse(A); q = -q; }

    //use binary decomposition: for example x^13 = x^1 * x^4 * x^8
    Matrix C;
    I started=0;
    while (q>0)
      {
        I r=q - 2*int(q/2);
        if (r>0)
          {
            if (started==0) { C=B; started=1; }
            else C = C*B;
          }
        q = q/2;
        if (q>0) B=B*B;  //B**2, B**4, B**8, etc
      }
    return C;
  }

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
Matrix get_linear_dependencies (const Matrix &A)
  {
    I m=A.dim1();
    I n=A.dim2();
    if (m < 2 || n < 2) { Matrix B; return B; } //return 0 x 0 matrix

    Matrix U,V; Diagonal S;
    mysvd(A,U,S,V);

    T frac = minf(0.1/double(m),0.0001);
    T neglect = S[0] * sqrt(Matrix::roundoff());

    Matrix dep(m,0);
    I ns=minf(m,n);

    for (int j=ns-1; j>=0; j--)  //walk from smallest singular value to largest
      {
        if (S[j] > neglect) break;
        Vector u=U.get_column(j);
        Vector d(m);  //working vector for new dependency
        d = -1.0;
        I r;
        for (r=0; r<m; r++)
          {
            I i=u.imaxabs();
            d[r] = i; //another element of the dependency
            u[i] = 0.0;
            if (u.norm() < frac) break;
          }
        if (r>0) dep.append_columns(d);
      }
    return dep;
  }

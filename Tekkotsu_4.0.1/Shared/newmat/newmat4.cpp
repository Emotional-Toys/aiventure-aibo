//$$ newmat4.cpp       Constructors, ReSize, basic utilities

// Copyright (C) 1991,2,3,4,8,9: R B Davies

#include "include.h"

#include "newmat.h"
#include "newmatrc.h"

#ifdef use_namespace
namespace NEWMAT {
#endif



#ifdef DO_REPORT
#define REPORT { static ExeCounter ExeCount(__LINE__,4); ++ExeCount; }
#else
#define REPORT {}
#endif


#define DO_SEARCH                   // search for LHS of = in RHS

// ************************* general utilities *************************/

static int tristore(int n)                    // elements in triangular matrix
{ return (n*(n+1))/2; }


// **************************** constructors ***************************/

GeneralMatrix::GeneralMatrix()
{ store=0; storage=0; nrows_value=0; ncols_value=0; tag=-1; }

GeneralMatrix::GeneralMatrix(ArrayLengthSpecifier s)
{
   REPORT
   storage=s.Value(); tag=-1;
   if (storage)
   {
      store = new Real [storage]; MatrixErrorNoSpace(store);
      MONITOR_REAL_NEW("Make (GenMatrix)",storage,store)
   }
   else store = 0;
}

Matrix::Matrix(int m, int n) : GeneralMatrix(m*n)
{ REPORT nrows_value=m; ncols_value=n; }

SquareMatrix::SquareMatrix(ArrayLengthSpecifier n)
   : Matrix(n.Value(),n.Value())
{ REPORT }

SymmetricMatrix::SymmetricMatrix(ArrayLengthSpecifier n)
   : GeneralMatrix(tristore(n.Value()))
{ REPORT nrows_value=n.Value(); ncols_value=n.Value(); }

UpperTriangularMatrix::UpperTriangularMatrix(ArrayLengthSpecifier n)
   : GeneralMatrix(tristore(n.Value()))
{ REPORT nrows_value=n.Value(); ncols_value=n.Value(); }

LowerTriangularMatrix::LowerTriangularMatrix(ArrayLengthSpecifier n)
   : GeneralMatrix(tristore(n.Value()))
{ REPORT nrows_value=n.Value(); ncols_value=n.Value(); }

DiagonalMatrix::DiagonalMatrix(ArrayLengthSpecifier m) : GeneralMatrix(m)
{ REPORT nrows_value=m.Value(); ncols_value=m.Value(); }

Matrix::Matrix(const BaseMatrix& M)
{
   REPORT // CheckConversion(M);
   // MatrixConversionCheck mcc;
   GeneralMatrix* gmx=((BaseMatrix&)M).Evaluate(MatrixType::Rt);
   GetMatrix(gmx);
}

SquareMatrix::SquareMatrix(const BaseMatrix& M) : Matrix(M)
{
   REPORT
   if (ncols_value != nrows_value)
   {
      Tracer tr("SquareMatrix");
      Throw(NotSquareException(*this));
   }
}


SquareMatrix::SquareMatrix(const Matrix& gm)
{
   REPORT
   if (gm.ncols_value != gm.nrows_value)
   {
      Tracer tr("SquareMatrix(Matrix)");
      Throw(NotSquareException(gm));
   }
   GetMatrix(&gm);
}


RowVector::RowVector(const BaseMatrix& M) : Matrix(M)
{
   REPORT
   if (nrows_value!=1)
   {
      Tracer tr("RowVector");
      Throw(VectorException(*this));
   }
}

ColumnVector::ColumnVector(const BaseMatrix& M) : Matrix(M)
{
   REPORT
   if (ncols_value!=1)
   {
      Tracer tr("ColumnVector");
      Throw(VectorException(*this));
   }
}

SymmetricMatrix::SymmetricMatrix(const BaseMatrix& M)
{
   REPORT  // CheckConversion(M);
   // MatrixConversionCheck mcc;
   GeneralMatrix* gmx=((BaseMatrix&)M).Evaluate(MatrixType::Sm);
   GetMatrix(gmx);
}

UpperTriangularMatrix::UpperTriangularMatrix(const BaseMatrix& M)
{
   REPORT // CheckConversion(M);
   // MatrixConversionCheck mcc;
   GeneralMatrix* gmx=((BaseMatrix&)M).Evaluate(MatrixType::UT);
   GetMatrix(gmx);
}

LowerTriangularMatrix::LowerTriangularMatrix(const BaseMatrix& M)
{
   REPORT // CheckConversion(M);
   // MatrixConversionCheck mcc;
   GeneralMatrix* gmx=((BaseMatrix&)M).Evaluate(MatrixType::LT);
   GetMatrix(gmx);
}

DiagonalMatrix::DiagonalMatrix(const BaseMatrix& M)
{
   REPORT //CheckConversion(M);
   // MatrixConversionCheck mcc;
   GeneralMatrix* gmx=((BaseMatrix&)M).Evaluate(MatrixType::Dg);
   GetMatrix(gmx);
}

IdentityMatrix::IdentityMatrix(const BaseMatrix& M)
{
   REPORT //CheckConversion(M);
   // MatrixConversionCheck mcc;
   GeneralMatrix* gmx=((BaseMatrix&)M).Evaluate(MatrixType::Id);
   GetMatrix(gmx);
}

GeneralMatrix::~GeneralMatrix()
{
   if (store)
   {
      MONITOR_REAL_DELETE("Free (GenMatrix)",storage,store)
      delete [] store;
   }
}

CroutMatrix::CroutMatrix(const BaseMatrix& m)
{
   REPORT
   Tracer tr("CroutMatrix");
   indx = 0;                     // in case of exception at next line
   GeneralMatrix* gm = ((BaseMatrix&)m).Evaluate(MatrixType::Rt);
   GetMatrix(gm);
   if (nrows_value!=ncols_value) { CleanUp(); Throw(NotSquareException(*gm)); }
   d=true; sing=false;
   indx=new int [nrows_value]; MatrixErrorNoSpace(indx);
   MONITOR_INT_NEW("Index (CroutMat)",nrows_value,indx)
   ludcmp();
}

CroutMatrix::~CroutMatrix()
{
   MONITOR_INT_DELETE("Index (CroutMat)",nrows_value,indx)
   delete [] indx;
}

//ReturnMatrix::ReturnMatrix(GeneralMatrix& gmx)
//{
//   REPORT
//   gm = gmx.Image(); gm->ReleaseAndDelete();
//}


GeneralMatrix::operator ReturnMatrix() const
{
   REPORT
   GeneralMatrix* gm = Image(); gm->ReleaseAndDelete();
   return ReturnMatrix(gm);
}



ReturnMatrix GeneralMatrix::ForReturn() const
{
   REPORT
   GeneralMatrix* gm = Image(); gm->ReleaseAndDelete();
   return ReturnMatrix(gm);
}

// ************ Constructors for use with NR in C++ interface ***********

#ifdef SETUP_C_SUBSCRIPTS

Matrix::Matrix(Real a, int m, int n) : GeneralMatrix(m * n)
   { REPORT nrows_value=m; ncols_value=n; operator=(a); }
   
Matrix::Matrix(const Real* a, int m, int n) : GeneralMatrix(m * n)
   { REPORT nrows_value=m; ncols_value=n; *this << a; }

#endif



// ************************** ReSize matrices ***************************/

void GeneralMatrix::ReSize(int nr, int nc, int s)
{
   REPORT
   if (store)
   {
      MONITOR_REAL_DELETE("Free (ReDimensi)",storage,store)
      delete [] store;
   }
   storage=s; nrows_value=nr; ncols_value=nc; tag=-1;
   if (s)
   {
      store = new Real [storage]; MatrixErrorNoSpace(store);
      MONITOR_REAL_NEW("Make (ReDimensi)",storage,store)
   }
   else store = 0;
}

void Matrix::ReSize(int nr, int nc)
{ REPORT GeneralMatrix::ReSize(nr,nc,nr*nc); }

void SquareMatrix::ReSize(int n)
{ REPORT GeneralMatrix::ReSize(n,n,n*n); }

void SquareMatrix::ReSize(int nr, int nc)
{
   REPORT
   Tracer tr("SquareMatrix::ReSize");
   if (nc != nr) Throw(NotSquareException(*this));
   GeneralMatrix::ReSize(nr,nc,nr*nc);
}

void SymmetricMatrix::ReSize(int nr)
{ REPORT GeneralMatrix::ReSize(nr,nr,tristore(nr)); }

void UpperTriangularMatrix::ReSize(int nr)
{ REPORT GeneralMatrix::ReSize(nr,nr,tristore(nr)); }

void LowerTriangularMatrix::ReSize(int nr)
{ REPORT GeneralMatrix::ReSize(nr,nr,tristore(nr)); }

void DiagonalMatrix::ReSize(int nr)
{ REPORT GeneralMatrix::ReSize(nr,nr,nr); }

void RowVector::ReSize(int nc)
{ REPORT GeneralMatrix::ReSize(1,nc,nc); }

void ColumnVector::ReSize(int nr)
{ REPORT GeneralMatrix::ReSize(nr,1,nr); }

void RowVector::ReSize(int nr, int nc)
{
   Tracer tr("RowVector::ReSize");
   if (nr != 1) Throw(VectorException(*this));
   REPORT GeneralMatrix::ReSize(1,nc,nc);
}

void ColumnVector::ReSize(int nr, int nc)
{
   Tracer tr("ColumnVector::ReSize");
   if (nc != 1) Throw(VectorException(*this));
   REPORT GeneralMatrix::ReSize(nr,1,nr);
}

void IdentityMatrix::ReSize(int nr)
{ REPORT GeneralMatrix::ReSize(nr,nr,1); *store = 1; }


void Matrix::ReSize(const GeneralMatrix& A)
{ REPORT  ReSize(A.Nrows(), A.Ncols()); }

void SquareMatrix::ReSize(const GeneralMatrix& A)
{
   REPORT
   int n = A.Nrows();
   if (n != A.Ncols())
   {
      Tracer tr("SquareMatrix::ReSize(GM)");
      Throw(NotSquareException(*this));
   }
   ReSize(n);
}

void nricMatrix::ReSize(const GeneralMatrix& A)
{ REPORT  ReSize(A.Nrows(), A.Ncols()); }

void ColumnVector::ReSize(const GeneralMatrix& A)
{ REPORT  ReSize(A.Nrows(), A.Ncols()); }

void RowVector::ReSize(const GeneralMatrix& A)
{ REPORT  ReSize(A.Nrows(), A.Ncols()); }

void SymmetricMatrix::ReSize(const GeneralMatrix& A)
{
   REPORT
   int n = A.Nrows();
   if (n != A.Ncols())
   {
      Tracer tr("SymmetricMatrix::ReSize(GM)");
      Throw(NotSquareException(*this));
   }
   ReSize(n);
}

void DiagonalMatrix::ReSize(const GeneralMatrix& A)
{
   REPORT
   int n = A.Nrows();
   if (n != A.Ncols())
   {
      Tracer tr("DiagonalMatrix::ReSize(GM)");
      Throw(NotSquareException(*this));
   }
   ReSize(n);
}

void UpperTriangularMatrix::ReSize(const GeneralMatrix& A)
{
   REPORT
   int n = A.Nrows();
   if (n != A.Ncols())
   {
      Tracer tr("UpperTriangularMatrix::ReSize(GM)");
      Throw(NotSquareException(*this));
   }
   ReSize(n);
}

void LowerTriangularMatrix::ReSize(const GeneralMatrix& A)
{
   REPORT
   int n = A.Nrows();
   if (n != A.Ncols())
   {
      Tracer tr("LowerTriangularMatrix::ReSize(GM)");
      Throw(NotSquareException(*this));
   }
   ReSize(n);
}

void IdentityMatrix::ReSize(const GeneralMatrix& A)
{
   REPORT
   int n = A.Nrows();
   if (n != A.Ncols())
   {
      Tracer tr("IdentityMatrix::ReSize(GM)");
      Throw(NotSquareException(*this));
   }
   ReSize(n);
}

void GeneralMatrix::ReSize(const GeneralMatrix&)
{
   REPORT
   Tracer tr("GeneralMatrix::ReSize(GM)");
   Throw(NotDefinedException("ReSize", "this type of matrix"));
}

void GeneralMatrix::ReSizeForAdd(const GeneralMatrix& A, const GeneralMatrix&)
{ REPORT ReSize(A); }

void GeneralMatrix::ReSizeForSP(const GeneralMatrix& A, const GeneralMatrix&)
{ REPORT ReSize(A); }


// ************************* SameStorageType ******************************/

// SameStorageType checks A and B have same storage type including bandwidth
// It does not check same dimensions since we assume this is already done

bool GeneralMatrix::SameStorageType(const GeneralMatrix& A) const
{
   REPORT
   return Type() == A.Type();
}


// ******************* manipulate types, storage **************************/

int GeneralMatrix::search(const BaseMatrix* s) const
{ REPORT return (s==this) ? 1 : 0; }

int GenericMatrix::search(const BaseMatrix* s) const
{ REPORT return gm->search(s); }

int MultipliedMatrix::search(const BaseMatrix* s) const
{ REPORT return bm1->search(s) + bm2->search(s); }

int ShiftedMatrix::search(const BaseMatrix* s) const
{ REPORT return bm->search(s); }

int NegatedMatrix::search(const BaseMatrix* s) const
{ REPORT return bm->search(s); }

int ReturnMatrix::search(const BaseMatrix* s) const
{ REPORT return (s==gm) ? 1 : 0; }

MatrixType Matrix::Type() const { return MatrixType::Rt; }
MatrixType SquareMatrix::Type() const { return MatrixType::Sq; }
MatrixType SymmetricMatrix::Type() const { return MatrixType::Sm; }
MatrixType UpperTriangularMatrix::Type() const { return MatrixType::UT; }
MatrixType LowerTriangularMatrix::Type() const { return MatrixType::LT; }
MatrixType DiagonalMatrix::Type() const { return MatrixType::Dg; }
MatrixType RowVector::Type() const { return MatrixType::RV; }
MatrixType ColumnVector::Type() const { return MatrixType::CV; }
MatrixType CroutMatrix::Type() const { return MatrixType::Ct; }
MatrixType BandMatrix::Type() const { return MatrixType::BM; }
MatrixType UpperBandMatrix::Type() const { return MatrixType::UB; }
MatrixType LowerBandMatrix::Type() const { return MatrixType::LB; }
MatrixType SymmetricBandMatrix::Type() const { return MatrixType::SB; }

MatrixType IdentityMatrix::Type() const { return MatrixType::Id; }



MatrixBandWidth BaseMatrix::BandWidth() const { REPORT return -1; }
MatrixBandWidth DiagonalMatrix::BandWidth() const { REPORT return 0; }
MatrixBandWidth IdentityMatrix::BandWidth() const { REPORT return 0; }

MatrixBandWidth UpperTriangularMatrix::BandWidth() const
   { REPORT return MatrixBandWidth(0,-1); }

MatrixBandWidth LowerTriangularMatrix::BandWidth() const
   { REPORT return MatrixBandWidth(-1,0); }

MatrixBandWidth BandMatrix::BandWidth() const
   { REPORT return MatrixBandWidth(lower,upper); }

MatrixBandWidth GenericMatrix::BandWidth()const
   { REPORT return gm->BandWidth(); }

MatrixBandWidth AddedMatrix::BandWidth() const
   { REPORT return gm1->BandWidth() + gm2->BandWidth(); }

MatrixBandWidth SPMatrix::BandWidth() const
   { REPORT return gm1->BandWidth().minimum(gm2->BandWidth()); }

MatrixBandWidth KPMatrix::BandWidth() const
{
   int lower, upper;
   MatrixBandWidth bw1 = gm1->BandWidth(), bw2 = gm2->BandWidth();
   if (bw1.Lower() < 0)
   {
      if (bw2.Lower() < 0) { REPORT lower = -1; }
      else { REPORT lower = bw2.Lower() + (gm1->Nrows() - 1) * gm2->Nrows(); }
   }
   else
   {
      if (bw2.Lower() < 0)
         { REPORT lower = (1 + bw1.Lower()) * gm2->Nrows() - 1; }
      else { REPORT lower = bw2.Lower() + bw1.Lower() * gm2->Nrows(); }
   }
   if (bw1.Upper() < 0)
   {
      if (bw2.Upper() < 0) { REPORT upper = -1; }
      else { REPORT upper = bw2.Upper() + (gm1->Nrows() - 1) * gm2->Nrows(); }
   }
   else
   {
      if (bw2.Upper() < 0)
         { REPORT upper = (1 + bw1.Upper()) * gm2->Nrows() - 1; }
      else { REPORT upper = bw2.Upper() + bw1.Upper() * gm2->Nrows(); }
   }
   return MatrixBandWidth(lower, upper);
}

MatrixBandWidth MultipliedMatrix::BandWidth() const
{ REPORT return gm1->BandWidth() * gm2->BandWidth(); }

MatrixBandWidth ConcatenatedMatrix::BandWidth() const { REPORT return -1; }

MatrixBandWidth SolvedMatrix::BandWidth() const
{
   if (+gm1->Type() & MatrixType::Diagonal)
      { REPORT return gm2->BandWidth(); }
   else { REPORT return -1; }
}

MatrixBandWidth ScaledMatrix::BandWidth() const
   { REPORT return gm->BandWidth(); }

MatrixBandWidth NegatedMatrix::BandWidth() const
   { REPORT return gm->BandWidth(); }

MatrixBandWidth TransposedMatrix::BandWidth() const
   { REPORT return gm->BandWidth().t(); }

MatrixBandWidth InvertedMatrix::BandWidth() const
{
   if (+gm->Type() & MatrixType::Diagonal)
      { REPORT return MatrixBandWidth(0,0); }
   else { REPORT return -1; }
}

MatrixBandWidth RowedMatrix::BandWidth() const { REPORT return -1; }
MatrixBandWidth ColedMatrix::BandWidth() const { REPORT return -1; }
MatrixBandWidth DiagedMatrix::BandWidth() const { REPORT return 0; }
MatrixBandWidth MatedMatrix::BandWidth() const { REPORT return -1; }
MatrixBandWidth ReturnMatrix::BandWidth() const
   { REPORT return gm->BandWidth(); }

MatrixBandWidth GetSubMatrix::BandWidth() const
{

   if (row_skip==col_skip && row_number==col_number)
      { REPORT return gm->BandWidth(); }
   else { REPORT return MatrixBandWidth(-1); }
}

// ********************** the memory managment tools **********************/

//  Rules regarding tDelete, reuse, GetStore, BorrowStore
//    All matrices processed during expression evaluation must be subject
//    to exactly one of reuse(), tDelete(), GetStore() or BorrowStore().
//    If reuse returns true the matrix must be reused.
//    GetMatrix(gm) always calls gm->GetStore()
//    gm->Evaluate obeys rules
//    bm->Evaluate obeys rules for matrices in bm structure

void GeneralMatrix::tDelete()
{
   if (tag<0)
   {
      if (tag<-1) { REPORT store = 0; delete this; return; }  // borrowed
      else { REPORT return; }   // not a temporary matrix - leave alone
   }
   if (tag==1)
   {
      if (store)
      {
         REPORT  MONITOR_REAL_DELETE("Free   (tDelete)",storage,store)
         delete [] store;
      }
      MiniCleanUp(); return;                           // CleanUp
   }
   if (tag==0) { REPORT delete this; return; }

   REPORT tag--; return;
}

static void BlockCopy(int n, Real* from, Real* to)
{
   REPORT
   int i = (n >> 3);
   while (i--)
   {
      *to++ = *from++; *to++ = *from++; *to++ = *from++; *to++ = *from++;
      *to++ = *from++; *to++ = *from++; *to++ = *from++; *to++ = *from++;
   }
   i = n & 7; while (i--) *to++ = *from++;
}

bool GeneralMatrix::reuse()
{
   if (tag < -1)                 // borrowed storage
   {
      if (storage)
      {
         REPORT
         Real* s = new Real [storage]; MatrixErrorNoSpace(s);
         MONITOR_REAL_NEW("Make     (reuse)",storage,s)
         BlockCopy(storage, store, s); store = s;
      }
      else { REPORT MiniCleanUp(); }                // CleanUp
      tag = 0; return true;
   }
   if (tag < 0 ) { REPORT return false; }
   if (tag <= 1 )  { REPORT return true; }
   REPORT tag--; return false;
}

Real* GeneralMatrix::GetStore()
{
   if (tag<0 || tag>1)
   {
      Real* s;
      if (storage)
      {
         s = new Real [storage]; MatrixErrorNoSpace(s);
         MONITOR_REAL_NEW("Make  (GetStore)",storage,s)
         BlockCopy(storage, store, s);
      }
      else s = 0;
      if (tag > 1) { REPORT tag--; }
      else if (tag < -1) { REPORT store = 0; delete this; } // borrowed store
      else { REPORT }
      return s;
   }
   Real* s = store;                             // CleanUp - done later
   if (tag==0) { REPORT store = 0; delete this; }
   else { REPORT  MiniCleanUp(); }
   return s;
}

void GeneralMatrix::GetMatrix(const GeneralMatrix* gmx)
{
   REPORT  tag=-1; nrows_value=gmx->Nrows(); ncols_value=gmx->Ncols();
   storage=gmx->storage; SetParameters(gmx);
   store=((GeneralMatrix*)gmx)->GetStore();
}

GeneralMatrix* GeneralMatrix::BorrowStore(GeneralMatrix* gmx, MatrixType mt)
// Copy storage of *this to storage of *gmx. Then convert to type mt.
// If mt == 0 just let *gmx point to storage of *this if tag==-1.
{
   if (!mt)
   {
      if (tag == -1) { REPORT gmx->tag = -2; gmx->store = store; }
      else { REPORT gmx->tag = 0; gmx->store = GetStore(); }
   }
   else if (Compare(gmx->Type(),mt))
   { REPORT  gmx->tag = 0; gmx->store = GetStore(); }
   else
   {
      REPORT gmx->tag = -2; gmx->store = store;
      gmx = gmx->Evaluate(mt); gmx->tag = 0; tDelete();
   }

   return gmx;
}

void GeneralMatrix::Eq(const BaseMatrix& X, MatrixType mt)
// Count number of references to this in X.
// If zero delete storage in this;
// otherwise tag this to show when storage can be deleted
// evaluate X and copy to this
{
#ifdef DO_SEARCH
   int counter=X.search(this);
   if (counter==0)
   {
      REPORT
      if (store)
      {
         MONITOR_REAL_DELETE("Free (operator=)",storage,store)
         REPORT delete [] store; storage = 0; store = 0;
      }
   }
   else { REPORT Release(counter); }
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate(mt);
   if (gmx!=this) { REPORT GetMatrix(gmx); }
   else { REPORT }
   Protect();
#else
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate(mt);
   if (gmx!=this)
   {
      REPORT
      if (store)
      {
         MONITOR_REAL_DELETE("Free (operator=)",storage,store)
         REPORT delete [] store; storage = 0; store = 0;
      }
      GetMatrix(gmx);
   }
   else { REPORT }
   Protect();
#endif
}

// version with no conversion
void GeneralMatrix::Eq(const GeneralMatrix& X)
{
   GeneralMatrix* gmx = (GeneralMatrix*)&X;
   if (gmx!=this)
   {
      REPORT
      if (store)
      {
         MONITOR_REAL_DELETE("Free (operator=)",storage,store)
         REPORT delete [] store; storage = 0; store = 0;
      }
      GetMatrix(gmx);
   }
   else { REPORT }
   Protect();
}

// version to work with operator<<
void GeneralMatrix::Eq(const BaseMatrix& X, MatrixType mt, bool ldok)
{
   REPORT
   if (ldok) mt.SetDataLossOK();
   Eq(X, mt);
}

void GeneralMatrix::Eq2(const BaseMatrix& X, MatrixType mt)
// a cut down version of Eq for use with += etc.
// we know BaseMatrix points to two GeneralMatrix objects,
// the first being this (may be the same).
// we know tag has been set correctly in each.
{
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate(mt);
   if (gmx!=this) { REPORT GetMatrix(gmx); }  // simplify GetMatrix ?
   else { REPORT }
   Protect();
}

void GeneralMatrix::Inject(const GeneralMatrix& X)
// copy stored values of X; otherwise leave els of *this unchanged
{
   REPORT
   Tracer tr("Inject");
   if (nrows_value != X.nrows_value || ncols_value != X.ncols_value)
      Throw(IncompatibleDimensionsException());
   MatrixRow mr((GeneralMatrix*)&X, LoadOnEntry);
   MatrixRow mrx(this, LoadOnEntry+StoreOnExit+DirectPart);
   int i=nrows_value;
   while (i--) { mrx.Inject(mr); mrx.Next(); mr.Next(); }
}

// ************* checking for data loss during conversion *******************/

bool Compare(const MatrixType& source, MatrixType& destination)
{
   if (!destination) { destination=source; return true; }
   if (destination==source) return true;
   if (!destination.DataLossOK && !(destination>=source))
      Throw(ProgramException("Illegal Conversion", source, destination));
   return false;
}

// ************* Make a copy of a matrix on the heap *********************/

GeneralMatrix* Matrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new Matrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* SquareMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new SquareMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* SymmetricMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new SymmetricMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* UpperTriangularMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new UpperTriangularMatrix(*this);
   MatrixErrorNoSpace(gm); return gm;
}

GeneralMatrix* LowerTriangularMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new LowerTriangularMatrix(*this);
   MatrixErrorNoSpace(gm); return gm;
}

GeneralMatrix* DiagonalMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new DiagonalMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* RowVector::Image() const
{
   REPORT
   GeneralMatrix* gm = new RowVector(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* ColumnVector::Image() const
{
   REPORT
   GeneralMatrix* gm = new ColumnVector(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* BandMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new BandMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* UpperBandMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new UpperBandMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* LowerBandMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new LowerBandMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* SymmetricBandMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new SymmetricBandMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* nricMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new nricMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* IdentityMatrix::Image() const
{
   REPORT
   GeneralMatrix* gm = new IdentityMatrix(*this); MatrixErrorNoSpace(gm);
   return gm;
}

GeneralMatrix* GeneralMatrix::Image() const
{
   bool dummy = true;
   if (dummy)                                   // get rid of warning message
      Throw(InternalException("Cannot apply Image to this matrix type"));
   return 0;
}


// *********************** nricMatrix routines *****************************/

void nricMatrix::MakeRowPointer()
{
   REPORT
   if (nrows_value > 0)
   {
      row_pointer = new Real* [nrows_value]; MatrixErrorNoSpace(row_pointer);
      Real* s = Store() - 1; int i = nrows_value; Real** rp = row_pointer;
      if (i) for (;;) { *rp++ = s; if (!(--i)) break; s+=ncols_value; }
   }
   else row_pointer = 0;
}

void nricMatrix::DeleteRowPointer()
   { REPORT if (nrows_value) delete [] row_pointer; }

void GeneralMatrix::CheckStore() const
{
   REPORT
   if (!store)
      Throw(ProgramException("NRIC accessing matrix with unset dimensions"));
}


// *************************** CleanUp routines *****************************/

void GeneralMatrix::CleanUp()
{
   // set matrix dimensions to zero, delete storage
   REPORT
   if (store && storage)
   {
      MONITOR_REAL_DELETE("Free (CleanUp)    ",storage,store)
      REPORT delete [] store;
   }
   store=0; storage=0; nrows_value=0; ncols_value=0; tag = -1;
}

void nricMatrix::CleanUp()
   { REPORT DeleteRowPointer(); GeneralMatrix::CleanUp(); }

void nricMatrix::MiniCleanUp()
   { REPORT DeleteRowPointer(); GeneralMatrix::MiniCleanUp(); }

void RowVector::CleanUp()
   { REPORT GeneralMatrix::CleanUp(); nrows_value=1; }

void ColumnVector::CleanUp()
   { REPORT GeneralMatrix::CleanUp(); ncols_value=1; }

void CroutMatrix::CleanUp()
{
   REPORT
   if (nrows_value) delete [] indx;
   GeneralMatrix::CleanUp();
}

void CroutMatrix::MiniCleanUp()
{
   REPORT
   if (nrows_value) delete [] indx;
   GeneralMatrix::MiniCleanUp();
}

void BandLUMatrix::CleanUp()
{
   REPORT
   if (nrows_value) delete [] indx;
   if (storage2) delete [] store2;
   GeneralMatrix::CleanUp();
}

void BandLUMatrix::MiniCleanUp()
{
   REPORT
   if (nrows_value) delete [] indx;
   if (storage2) delete [] store2;
   GeneralMatrix::MiniCleanUp();
}

// ************************ simple integer array class ***********************

// construct a new array of length xn. Check that xn is non-negative and
// that space is available

SimpleIntArray::SimpleIntArray(int xn) : n(xn)
{
   if (n < 0) Throw(Logic_error("invalid array length"));
   else if (n == 0) { REPORT  a = 0; }
   else { REPORT  a = new int [n]; if (!a) Throw(Bad_alloc()); }
}

// destroy an array - return its space to memory

SimpleIntArray::~SimpleIntArray() { REPORT  if (a) delete [] a; }

// access an element of an array; return a "reference" so elements
// can be modified.
// check index is within range
// in this array class the index runs from 0 to n-1

int& SimpleIntArray::operator[](int i)
{
   REPORT
   if (i < 0 || i >= n) Throw(Logic_error("array index out of range"));
   return a[i];
}

// same thing again but for arrays declared constant so we can't
// modify its elements

int SimpleIntArray::operator[](int i) const
{
   REPORT
   if (i < 0 || i >= n) Throw(Logic_error("array index out of range"));
   return a[i];
}

// set all the elements equal to a given value

void SimpleIntArray::operator=(int ai)
   { REPORT  for (int i = 0; i < n; i++) a[i] = ai; }

// set the elements equal to those of another array.
// now allow length to be changed

void SimpleIntArray::operator=(const SimpleIntArray& b)
{
   REPORT
   if (b.n != n) ReSize(b.n);
   for (int i = 0; i < n; i++) a[i] = b.a[i];
}

// construct a new array equal to an existing array
// check that space is available

SimpleIntArray::SimpleIntArray(const SimpleIntArray& b) : Janitor(), n(b.n)
{
   if (n == 0) { REPORT  a = 0; }
   else
   {
      REPORT
      a = new int [n]; if (!a) Throw(Bad_alloc());
      for (int i = 0; i < n; i++) a[i] = b.a[i];
   }
}

// change the size of an array; optionally copy data from old array to
// new array

void SimpleIntArray::ReSize(int n1, bool keep)
{
   if (n1 == n) { REPORT  return; }
   else if (n1 == 0) { REPORT  n = 0; delete [] a; a = 0; }
   else if (n == 0)
      { REPORT  a = new int [n1]; if (!a) Throw(Bad_alloc()); n = n1; }
   else
   {
      int* a1 = a;
      if (keep)
      {
         REPORT
         a = new int [n1]; if (!a) Throw(Bad_alloc());
         if (n > n1) n = n1;
         for (int i = 0; i < n; i++) a[i] = a1[i];
         n = n1; delete [] a1;
      }
      else
      {
         REPORT  n = n1; delete [] a1;
         a = new int [n]; if (!a) Throw(Bad_alloc());
      }
   }
}

//************************** swap values ********************************

// swap values

void GeneralMatrix::swap(GeneralMatrix& gm)
{
   REPORT
   int t;
   t = tag; tag = gm.tag; gm.tag = t;
   t = nrows_value; nrows_value = gm.nrows_value; gm.nrows_value = t;
   t = ncols_value; ncols_value = gm.ncols_value; gm.ncols_value = t;
   t = storage; storage = gm.storage; gm.storage = t;
   Real* s = store; store = gm.store; gm.store = s;
}
   
void nricMatrix::swap(nricMatrix& gm)
{
   REPORT
   GeneralMatrix::swap((GeneralMatrix&)gm);
   Real** rp = row_pointer; row_pointer = gm.row_pointer; gm.row_pointer = rp;
}

void CroutMatrix::swap(CroutMatrix& gm)
{
   REPORT
   GeneralMatrix::swap((GeneralMatrix&)gm);
   int* i = indx; indx = gm.indx; gm.indx = i;
   bool b;
   b = d; d = gm.d; gm.d = b;
   b = sing; sing = gm.sing; gm.sing = b;
}

void BandMatrix::swap(BandMatrix& gm)
{
   REPORT
   GeneralMatrix::swap((GeneralMatrix&)gm);
   int i;
   i = lower; lower = gm.lower; gm.lower = i;
   i = upper; upper = gm.upper; gm.upper = i;
}

void SymmetricBandMatrix::swap(SymmetricBandMatrix& gm)
{
   REPORT
   GeneralMatrix::swap((GeneralMatrix&)gm);
   int i;
   i = lower; lower = gm.lower; gm.lower = i;
}

void BandLUMatrix::swap(BandLUMatrix& gm)
{
   REPORT
   GeneralMatrix::swap((GeneralMatrix&)gm);
   int* i = indx; indx = gm.indx; gm.indx = i;
   bool b;
   b = d; d = gm.d; gm.d = b;
   b = sing; sing = gm.sing; gm.sing = b;
   int m;
   m = storage2; storage2 = gm.storage2; gm.storage2 = m;
   m = m1; m1 = gm.m1; gm.m1 = m;
   m = m2; m2 = gm.m2; gm.m2 = m;
   Real* s = store2; store2 = gm.store2; gm.store2 = s;
}

void GenericMatrix::swap(GenericMatrix& x)
{
   REPORT
   GeneralMatrix* tgm = gm; gm = x.gm; x.gm = tgm;
}

// ********************** C subscript classes ****************************

RealStarStar::RealStarStar(Matrix& A)
{
   REPORT
   Tracer tr("RealStarStar");
   int n = A.ncols();
   int m = A.nrows();
   a = new Real*[m];
   MatrixErrorNoSpace(a);
   Real* d = A.data();
   for (int i = 0; i < m; ++i) a[i] = d + i * n;
} 

ConstRealStarStar::ConstRealStarStar(const Matrix& A)
{
   REPORT
   Tracer tr("ConstRealStarStar");
   int n = A.ncols();
   int m = A.nrows();
   a = new const Real*[m];
   MatrixErrorNoSpace(a);
   const Real* d = A.data();
   for (int i = 0; i < m; ++i) a[i] = d + i * n;
} 



#ifdef use_namespace
}
#endif


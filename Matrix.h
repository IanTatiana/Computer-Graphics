#define TYPE double

#define M_OK			0
#define M_ERR_PTRISNULL	1
#define M_ERR_COLSROWS	2
#define M_ERR_ALLOC		3
#define M_ERR			4

#define MatHandler void *

MatHandler CreateMat(int cols, int rows, TYPE *nums);
void DestroyMat(MatHandler mat);

int AddMat(MatHandler mat1, MatHandler mat2, MatHandler *ans);
int SubMat(MatHandler mat1, MatHandler mat2, MatHandler *ans);
int NumberMulMat(MatHandler mat, double k, MatHandler *ans);

int ColCount(MatHandler mat);
int RowCount(MatHandler mat);

int MatrixMulMatrix(MatHandler mat1, MatHandler mat2, MatHandler *ans);
int DetMat(MatHandler mat, double *ans);
int TransposeMat(MatHandler mat, MatHandler *ans);
int InvertMat(MatHandler mat, MatHandler *ans);
// MulVect

void ShowMatrix(MatHandler mat);
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "Matrix.h"

int main(){
	/*double mas[] = { 3, 4, 2 };
	double mas2[] = { 5, -3, 1 };
	VecHandler t = CreateVec(3, mas);
	VecHandler t2 = CreateVec(3, mas2);
	ShowVec(t); ShowVec(t2);

	VecHandler s = NULL;
	int resp;
	//SetElemVec(t, 2, 1);
	int sum = NumberMulMat(t, 2, &s);
	ShowVec(s);
	Normalize(s);
	ShowVec(s);*/
	double mmas[3][2] = { 
		{ 1.0, 2.0 }, 
		{ 7.0, 6.0 },
		{ 4.0, 5.0 } };
	
	double mmas2[3][3] = {
		{ 2.0, 5.0, 8.0 },
		{ 3.0, 6.0, 7.0 },
		{ 1.0, 2.0, 5.0 },
	};
	MatHandler mat = CreateMat(2, 3, &mmas[0][0]);
	//ShowMatrix(mat);
	MatHandler mat2 = CreateMat(3, 3, &mmas2[0][0]);
	ShowMatrix(mat2);
	MatHandler t = NULL;
	InvertMat(mat2, &t);
	ShowMatrix(t);
	MatrixMulMatrix(mat2, t, &mat);
	ShowMatrix(mat);
	//printf("%lf", res)
	//DestroyMat(mat2);
	/*NumberMulMat(mat, 2, &t);
	ShowMatrix(mat);
	ShowMatrix(t);
	*/
	//printf("Size: %d\n", SizeVec(s));
	//printf("sum = (%d, %d);\n", GetElemVec(s, 1), GetElemVec(s, 2));

	system("pause");
}
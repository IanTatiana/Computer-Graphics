#include <stdio.h>
#include <assert.h>
#include "Matrix.h"

#define idx(i, cols, j) ((i) ? (i*cols + j) : j)

typedef struct {
	TYPE* data;
	int cols;
	int rows;
} matrix_t;

void ShowMatrix(MatHandler mat){
	matrix_t *m = (matrix_t *)mat;
	if (!m){
		printf("Matrix is NULL\n\n");
		return;
	}

	printf("Matrix:\nCols: %d;  Rows: %d;\n", m->cols, m->rows);
	for (int i = 0; i < m->rows; ++i){
		for (int j = 0; j < m->cols; ++j){
			printf("[%d]%lf\t", idx(j, m->cols, i), m->data[idx(j, m->cols, i)]);
		}
		printf("\n");
	}
	printf("\n");
}


MatHandler CreateMat(int cols, int rows, TYPE *nums){
	matrix_t *m = (matrix_t *)calloc(1, sizeof(matrix_t));
	if (m){
		assert(cols > 0 && rows > 0);
		m->cols = cols; m->rows = rows;
		m->data = (TYPE *)calloc(cols*rows, sizeof(TYPE));
		if (m->data){
			if (nums){
				for (int i = 0; i < cols; ++i){
					for (int j = 0; j < rows; ++j){
						m->data[idx(i, cols, j)] = *(nums + j*cols + i);
					}
				}
			}
			return (MatHandler *)m;
		}
	}
	return NULL;
}


void DestroyMat(MatHandler mat){
	matrix_t *m = (matrix_t *)mat;
	free(m->data);
	free(m);
}


int AddMat(MatHandler mat1, MatHandler mat2, MatHandler *ans){
	matrix_t *m1, *m2, *a;
	m1 = (matrix_t *)mat1; m2 = (matrix_t *)mat2;

	if (!m1 || !m2) return M_ERR_PTRISNULL;
	if (m1->cols != m2->cols || m1->rows != m2->rows) return M_ERR_COLSROWS;

	a = (matrix_t *)(*ans) = (matrix_t *)CreateMat(m1->cols, m1->rows, NULL);
	if (!a) return M_ERR_ALLOC;

	for (int i = 0; i < a->cols; ++i){
		for (int j = 0; j < a->rows; ++j){
			int t = idx(i, a->cols, j);
			a->data[t] = m1->data[t] + m2->data[t];
		}
	}
	return M_OK;
}


int SubMat(MatHandler mat1, MatHandler mat2, MatHandler *ans){
	matrix_t m = *(matrix_t *)mat2;
	if (!mat2) return M_ERR_PTRISNULL;
	for (int i = 0; i < m.cols; ++i){
		for (int j = 0; j < m.rows; ++j){
			int t = idx(i, m.cols, j);
			m.data[t] *= -1;
		}
	}
	return AddMat(mat1, &m, ans);
}


int NumberMulMat(MatHandler mat, double k, MatHandler *ans){
	matrix_t *m1, *a;
	m1 = (matrix_t *)mat;
	if (!m1) return M_ERR_PTRISNULL;

	a = (matrix_t *)(*ans) = (matrix_t *)CreateMat(m1->cols, m1->rows, NULL);
	if (!a) return M_ERR_ALLOC;

	for (int i = 0; i < a->cols; ++i){
		for (int j = 0; j < a->rows; ++j){
			int t = idx(i, a->cols, j);
			a->data[t] = m1->data[t] * k;
		}
	}
	return M_OK;
}


int ColCount(MatHandler mat){
	return ((matrix_t *)mat) ? ((matrix_t *)mat)->cols : 0;
}


int RowCount(MatHandler mat){
	return ((matrix_t *)mat) ? ((matrix_t *)mat)->rows : 0;
}


int MatrixMulMatrix(MatHandler mat1, MatHandler mat2, MatHandler *ans){
	matrix_t *m1, *m2, *a;
	m1 = (matrix_t *)mat1; m2 = (matrix_t *)mat2;
	if (m1->cols != m2->rows) return M_ERR_COLSROWS;

	a = (matrix_t *)(*ans) = CreateMat(m2->cols, m1->rows, NULL);
	if (!a) return M_ERR_ALLOC;
	
	for (int i = 0; i < a->cols; ++i){
		for (int j = 0; j < a->rows; ++j){
			for (int k = 0; k < m1->cols; ++k){
				a->data[idx(i, a->cols, j)] += 
					m1->data[idx(k, m1->cols, j)] * m2->data[idx(i, m2->cols, k)];
			}
		}
	}
	return M_OK;
}


int DetMat(MatHandler mat, double *ans){
	matrix_t *m = (matrix_t *)mat;
	if (m->cols != m->rows) return M_ERR_COLSROWS;
	if (m->cols == 1){
		*ans = m->data[0];
		return M_OK;
	}
	if (m->cols == 2){
		*ans = m->data[0] * m->data[3] - m->data[1] * m->data[2];
		return M_OK;
	}
	matrix_t *t = (matrix_t *)CreateMat(m->cols - 1, m->rows - 1, NULL);

	/*	Разложение по первой строке	*/
	int id = 0;
	for (int i = 0; i < m->cols; ++i){	/*	Бежит по всем определителям, дополнительным минорам */
		id = 0;
		for (int j = 0; j < m->cols; ++j){	/*	Бежит по всем столбцам	*/
			if (j == i) continue;
			for (int k = 1; k < m->rows; ++k){			
				t->data[id] = m->data[idx(j, m->cols, k)];
				++id;
			}
		}
		//ShowMatrix(t); printf("\n");
		double res = 0;
		int r = DetMat(t, &res);
		if (r == M_OK){
			if (i % 2)
				*ans -= m->data[idx(i, m->cols, 0)] * res;
			else
				*ans += m->data[idx(i, m->cols, 0)] * res;
		}
		else return r;
	}
	DestroyMat(t);
	return M_OK;
}


int TransposeMat(MatHandler mat, MatHandler *ans){
	matrix_t *m, *a;
	m = (matrix_t *)mat;
	a = (matrix_t *)(*ans) = CreateMat(m->rows, m->cols, NULL);
	if (!a) return M_ERR_ALLOC;
	for (int i = 0; i < m->cols; ++i){
		for (int j = 0; j < m->rows; ++j){
			a->data[idx(j, m->rows, i)] = m->data[idx(i, m->cols, j)];
		}
	}
	return M_OK;
}


int CreateMinorMat(MatHandler mat, int icol, int jrow, MatHandler *ans){ /*	Создает минор только n-1-ого порядка*/
	matrix_t *a, *m = (matrix_t *)mat;
	assert(m->cols > 1 && m->rows > 1);
	a = (matrix_t *)(*ans) = (matrix_t *)CreateMat(m->cols - 1, m->rows - 1, NULL);
	if (!a) return M_ERR_ALLOC;
	a->cols = m->cols - 1;
	a->rows = m->rows - 1;
	int id = 0;
	for (int i = 0; i < m->cols; ++i){
		if (i == icol) continue;
		for (int j = 0; j < m->rows; ++j){
			if (j == jrow) continue;
			int r = idx(i, m->cols, j);
			a->data[id] = m->data[r];
			++id;
		}
	}
	return M_OK;
}


int InvertMat(MatHandler mat, MatHandler *ans){
	matrix_t *m, *alg;
	double det = 0.0;
	int k = DetMat(mat, &det);
	if (k != M_OK || !det) return M_ERR;
	/*	Найдем матрицу алгебраических дополнений	*/
	m = (matrix_t *)mat;
	if (!m) return M_ERR_PTRISNULL;
	alg = (matrix_t *)CreateMat(m->cols, m->rows, NULL);
	if (!alg) return M_ERR_ALLOC;
	for (int i = 0; i < m->cols; ++i){
		for (int j = 0; j < m->rows; ++j){
			MatHandler t = NULL;
			if (CreateMinorMat(mat, i, j, &t) != M_OK) return M_ERR;
			double res = 0.0;
			if (DetMat(t, &res) != M_OK) return M_ERR;
			if ((i + j) % 2)
				alg->data[idx(i, m->cols, j)] = -res;
			else
				alg->data[idx(i, m->cols, j)] = res;
		}
	}
	matrix_t *talg = NULL;
	if (TransposeMat((MatHandler)alg, &talg) != M_OK) return M_ERR;
	if (NumberMulMat((MatHandler)talg, 1/det, ans) != M_OK) return M_ERR;
	return M_OK;
}
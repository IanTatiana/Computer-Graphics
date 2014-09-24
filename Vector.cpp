#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <errno.h>
#include <assert.h>
#include "Vector.h"

typedef struct {
	TYPE* data;
	int size;
} vector_t;

void ShowVec(VecHandler vec){	/*	ShowVec() created for debug	*/
	vector_t *v = (vector_t *)vec;
	if (!v){
		printf("Vector is NULL\n\n");
		return;
	}
	printf("Size: %d;\nLength: %lf;\n(", v->size, LenghtVec(vec));
	for (int i = 0; i < v->size; ++i)
		printf(" %lf ", v->data[i]);
	printf(")\n\n");
}


VecHandler CreateVec(int size, TYPE coords[]){
	vector_t *v = (vector_t *)calloc(1, sizeof(vector_t));
	if (v) {
		v->data = (TYPE *)calloc(size, sizeof(TYPE));
		if (v->data) {
			for (int i = 0; i < size; ++i)
				v->data[i] = coords[i];
			v->size = size;
			return (VecHandler *)v;
		}
	}
	return NULL;
}


int DestroyVec(VecHandler vec){
	if (vec) free(vec); vec = NULL;
	return V_OK;
}


int AddVec(VecHandler vec1, VecHandler vec2, VecHandler *ans){
	vector_t *v1, *v2, *a;
	v1 = (vector_t *)vec1; v2 = (vector_t *)vec2;
	if (!v1 || !v2) return V_ERR_PTRISNULL;
	if (v1->size != v2->size) return V_ERR_SIZECOND;

	a = (vector_t *)(*ans) = (vector_t *)calloc(1, sizeof(vector_t));
	a->size = v1->size;
	a->data = (TYPE *)calloc(v1->size, sizeof(TYPE));
	if (!a->data) return V_ERR_ALLOC;

	for (int i = 0; i < v1->size; ++i)
		a->data[i] = v1->data[i] + v2->data[i];
	return V_OK;
}


int SubVec(VecHandler vec1, VecHandler vec2, VecHandler *ans){
	vector_t t = *(vector_t *)vec2;
	if (!(vector_t *)vec1) return V_ERR_PTRISNULL;
	for (int i = 0; i < t.size; ++i)
		t.data[i] *= -1;
	return AddVec(vec1, &t, ans);
}


int NumberMulVec(VecHandler vec, int k, VecHandler *ans){
	vector_t *a, *v = (vector_t *)vec;
	if (!v) return V_ERR_PTRISNULL;
	a = (vector_t *)(*ans) = (vector_t *)calloc(1, sizeof(vector_t));
	if (!a) return V_ERR_ALLOC;
	
	a->size = v->size;
	a->data = (TYPE *)calloc(a->size, sizeof(TYPE));
	if (!a->data) return V_ERR_ALLOC;
	
	for (int i = 0; i < v->size; ++i)
		a->data[i] = k * v->data[i];
	return V_OK;
}


double LenghtVec(VecHandler vec){
	vector_t *t = (vector_t *)vec;
	if (!t) return 0.0;
	double l = 0;
	for (int i = 0; i < t->size; ++i)
		l += t->data[i] * t->data[i];
	return sqrt(l);
}


int SizeVec(VecHandler vec){
	return ((vector_t *)vec) ? ((vector_t *)vec)->size : 0;
}


int ScalarMulVec(VecHandler vec1, VecHandler vec2, int *ans){
	vector_t *v1, *v2, *a;
	v1 = (vector_t *)vec1; v2 = (vector_t *)vec2;
	if (!v1 || !v2) return V_ERR_PTRISNULL;
	if (v1->size != v2->size) return V_ERR_SIZECOND;
	
	int t = 0;
	for (int i = 0; i < v1->size; ++i)
		t += v1->data[i] * v2->data[i];
	*ans = t;
	return V_OK;
}


int VectorMulVec(VecHandler vec1, VecHandler vec2, VecHandler *ans){
	vector_t *v1, *v2, *a;
	v1 = (vector_t *)vec1; v2 = (vector_t *)vec2;
	if (!v1 || !v2) return V_ERR_PTRISNULL;
	if (v1->size != 3 || v2->size != 3) return V_ERR_SIZECOND;

	a = (vector_t *)(*ans) = (vector_t *)calloc(1, sizeof(vector_t));
	if (!a) return V_ERR_ALLOC;
	a->size = 3;
	a->data = (TYPE *)calloc(a->size, sizeof(TYPE));
	if (!a->data) return V_ERR_ALLOC;
	
	a->data[0] = v1->data[1] * v2->data[2] - v1->data[2] * v2->data[1];
	a->data[1] = v1->data[2] * v2->data[0] - v1->data[0] * v2->data[2];
	a->data[2] = v1->data[0] * v2->data[1] - v1->data[1] * v2->data[0];
	return V_OK;
}


int Normalize(VecHandler vec){
	vector_t *t = (vector_t *)vec;
	if (!t) return V_ERR_PTRISNULL;	
	double l = LenghtVec(vec);
	assert(l > 0);

	for (int i = 0; i < t->size; ++i)
		t->data[i] /= l;
	return V_OK;
}


int SetElemVec(VecHandler vec, int pos, int val){
	vector_t *v = (vector_t *)vec;
	if (!v) return V_ERR_PTRISNULL;
	assert(v->size >= pos && pos > 0);
	v->data[pos - 1] = val;
	return V_OK;
}


int GetElemVec(VecHandler vec, int pos){
	assert(((vector_t *)vec)->size >= pos && pos > 0);
	return ((vector_t *)vec)->data[pos - 1];
}
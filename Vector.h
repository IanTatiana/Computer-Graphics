#define TYPE double

#define V_OK			0
#define V_ERR_PTRISNULL	1
#define V_ERR_SIZECOND	2
#define V_ERR_ALLOC		3
#define V_ERR			4

#define VecHandler void *

VecHandler CreateVec(int size, TYPE coords[]);
int DestroyVec(VecHandler vec);

int AddVec(VecHandler vec1, VecHandler vec2, VecHandler *ans);
int SubVec(VecHandler vec1, VecHandler vec2, VecHandler *ans);
int NumberMulVec(VecHandler vec, int k, VecHandler *ans);

double LenghtVec(VecHandler vec);
int SizeVec(VecHandler vec);

int ScalarMulVec(VecHandler vec1, VecHandler vec2, int *ans);
int VectorMulVec(VecHandler vec1, VecHandler vec2, VecHandler *ans);
int Normalize(VecHandler vec);

int SetElemVec(VecHandler vec, int pos, int val);
int GetElemVec(VecHandler vec, int pos);

void ShowVec(VecHandler vec);

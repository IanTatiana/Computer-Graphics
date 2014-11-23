#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

#include "VMinc\Vector.h"
#include "VMinc\Matrix.h"
#include "VMinc\VectorMatrix.h"

int screen_width = 500, screen_height = 600;
GLuint program;
GLuint vbo_triangle_vertices, vbo_triangle_colors, ibo_triangle_elements, vao_id;
GLint attr_coord3d, attr_v_color, uniform_m_3x3_inv_transp, uniform_mvp, uniform_m;

GLuint textureID;

GLuint CreateShader(const char *fname, GLenum shtype){
	/*  Открываем файл и считываем текст шейдера  */
	FILE *fin = fopen(fname, "rt");
	if (!fin) return 0;

	fseek(fin, 0, SEEK_END);
	int cnt = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	char *src = (char *)calloc(cnt, sizeof(char));
	fread(src, sizeof(char), cnt, fin);
	
	/*  Компилируем шейдер  */
	GLint compile_ok = GL_FALSE;
	GLuint res = glCreateShader(shtype);

 	glShaderSource(res, 1, &src, NULL);
	glCompileShader(res);
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok){
		fprintf(stderr, "Error in creating shader\n");
		return 0;
	}
	return res;
}

char LinkShaders(GLuint vs, GLuint fs){
	GLint link_ok = GL_FALSE;
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		return 0;
	}
	glUseProgram(program);
	return 1;
}

GLfloat triangle_vertices[] = {
	0.0, 0.2, 0.0,
	-0.2, -0.2, 0.2,
	0.2, -0.2, 0.2,
	-0.2, -0.2, -0.2,
	0.2, -0.2, -0.2
};

GLfloat triangle_colors[] = {
	1.0, 1.0, 0.0,
	1.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 1.0
};

GLushort triangle_elements[] = {
	/*  Боковые грани  */
	0, 1, 2,
	0, 1, 3,
	0, 4, 2,
	0, 4, 3, 
	/*  Основание  */
	1, 2, 3,
	2, 3, 4
};

char MyInit(void){
	GLuint vs, fs;
	if (!(vs = CreateShader("vshader.txt", GL_VERTEX_SHADER))) return 0;
	if (!(fs = CreateShader("fshader.txt", GL_FRAGMENT_SHADER))) return 0;
	if (!LinkShaders(vs, fs)) return 0;

	// Создание VBO - Vertex Buffer Object

	glGenBuffers(1, &vbo_triangle_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_triangle_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo_triangle_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_triangle_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_elements), triangle_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, )
	//  Создание VAO - Vertex Array Object
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	//  Получение указателя на переменные
	const char *attr_name = "coord3d";
	attr_coord3d = glGetAttribLocation(program, attr_name);
	if (attr_coord3d == -1){
		fprintf(stderr, "Couldn't bind attribute %s\n", attr_name);
		return 0;
	}
	const char *attr_v_color = "v_color";
	attr_v_color = glGetAttribLocation(program, attr_name);
	if (attr_coord3d == -1){
		fprintf(stderr, "Couldn't bind attribute %s\n", attr_name);
		return 0;
	}
	const char *uniform_name;/* = "m_3x3_inv_transp";
	uniform_m_3x3_inv_transp = glGetUniformLocation(program, uniform_name);
	if (uniform_m_3x3_inv_transp == -1){
		fprintf(stderr, "Couldn't bind uniform %s\n", uniform_name);
		return 0;
	}
	uniform_name = "m";
	uniform_m = glGetUniformLocation(program, uniform_name);
	if (uniform_m == -1){
		fprintf(stderr, "Couldn't bind uniform %s\n", uniform_name);
		return 0;
	}*/
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if (uniform_mvp == -1){
		fprintf(stderr, "Couldn't bind uniform %s\n", uniform_name);
		return 0;
	}
	return 1;
}

void Display(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glEnableVertexAttribArray(attr_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_vertices);
	glVertexAttribPointer(attr_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(attr_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle_colors);
	glVertexAttribPointer(attr_v_color, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int size;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_triangle_elements);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(attr_coord3d);
	glDisableVertexAttribArray(attr_v_color);

	/*  Display the result  */
	glutSwapBuffers();
}

GLfloat mat4_e[] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

typedef struct {
	GLfloat X, Y, Z;
} point3d_t;
point3d_t camera, vertexes;
GLfloat angle_diff = 0.01, scale_diff = 0.01, diff = 0.01, scale = 1.0;
GLfloat angle_y = 0.0, angle_x = 0.0;


void KeyboardEvents(unsigned char key, int x, int y){
	switch (key){
		case 'W': case 'w': {
			vertexes.Z -= diff;
			break;
		}
		case 'S': case 's': {
			vertexes.Z += diff;
			break;
		}
		case 'A': case 'a': {
			vertexes.X += diff;
			break;
		}
		case 'D': case 'd': {
			vertexes.X -= diff;
			break;
		}
		case '+': {
			scale += scale_diff;
			break;
		}
		case '-': {
			if (scale - scale_diff >= 0)
				scale -= scale_diff;
			break;
		}
		case '4': {
			angle_y += angle_diff;
			break;
		}
		case '6': {
			angle_y -= angle_diff;
			break;
		}
		case '8': {
			angle_x -= angle_diff;
			break;
		}
		case '2': {
			angle_x += angle_diff;
			break;
		}
	}
}
 
MatHandler CreateRotateMatrix(GLfloat angle, int first_col, int last_col){
	MatHandler mat = CreateMat(4, 4, mat4_e);
	GLfloat cos_a = cos(angle), sin_a = sin(angle);
	if (mat){
		SetElemMat(mat, first_col, first_col, cos_a);
		SetElemMat(mat, last_col, first_col, sin_a);
		SetElemMat(mat, first_col, last_col, -sin_a);
		SetElemMat(mat, last_col, last_col, cos_a);
	}
	return mat;
}

MatHandler CreateScaleMatrix(){
	MatHandler mat = CreateMat(4, 4, mat4_e);
	if (mat){
		for (int i = 0; i < 3; ++i)
			SetElemMat(mat, i, i, scale);
	}
	return mat;
}

MatHandler CreateTranslateMatrix(GLfloat a, GLfloat b, GLfloat c){
	MatHandler mat = CreateMat(4, 4, mat4_e);
	if (mat){
		SetElemMat(mat, 3, 0, a);
		SetElemMat(mat, 3, 1, b);
		SetElemMat(mat, 3, 2, c);
	}
	return mat;
}

void OnIdle(void){
	printf("angle_x: %lf;\nangle_y %lf;\n\n", angle_x, angle_y);
	MatHandler rotate_mat_x = CreateRotateMatrix(angle_x, 1, 2);
	MatHandler rotate_mat_y = CreateRotateMatrix(angle_y, 0, 2);
	MatHandler rotate_mat = NULL;
	if (MatrixMulMatrix(rotate_mat_x, rotate_mat_y, &rotate_mat) != M_OK) return;

	MatHandler scale_mat = CreateScaleMatrix();
	MatHandler translate_mat = CreateTranslateMatrix(vertexes.X, vertexes.Y, vertexes.Z);
	if (!rotate_mat || !scale_mat || !translate_mat){
		printf("Error in creating matrixes of transformations\n");
		return;
	}

	MatHandler ModelMatrix = NULL; MatHandler t = NULL;
	if (MatrixMulMatrix(rotate_mat, scale_mat, &t) != M_OK) return;
	if (MatrixMulMatrix(translate_mat, t, &ModelMatrix) != M_OK) return;
	DestroyMat(t); t = NULL;

	MatHandler ViewMatrix = CreateTranslateMatrix(-camera.X, -camera.Y, -camera.Z);
	if (!ViewMatrix) return;

	MatHandler ProjectionMatrix = CreateMat(4, 4, mat4_e);
	if (!ProjectionMatrix) return;

	float ar = 1.0 * screen_width / screen_height, near_z = 0.1, far_z = 100.0, angle = 45.0;
	float d = 1 / tan(angle/2.0);
	SetElemMat(ProjectionMatrix, 0, 0, d / ar);
	SetElemMat(ProjectionMatrix, 1, 1, d);
	SetElemMat(ProjectionMatrix, 2, 2, -(near_z + far_z) / (near_z - far_z));
	SetElemMat(ProjectionMatrix, 3, 2, 2.0f * near_z * far_z / (near_z - far_z));
	SetElemMat(ProjectionMatrix, 2, 3, 1.0);
	
	MatHandler MVP = NULL;
	if (MatrixMulMatrix(ViewMatrix, ModelMatrix, &t) != M_OK) return;
	if (MatrixMulMatrix(ProjectionMatrix, t, &MVP) != M_OK) return;
	DestroyMat(t); t = NULL;
//	glUniform1f(uniform_fade, 0.0);/*
	
	//glUniformMatrix4fv(uniform_m_3x3_inv_transp, 1, GL_FALSE, *(GLfloat **)ModelMatrix);
	//glUniformMatrix4fv(uniform_m, 1, GL_FALSE, *(GLfloat **)ModelMatrix);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, *(GLfloat **)MVP);
	glutPostRedisplay();
	
	DestroyMat(rotate_mat);
	DestroyMat(scale_mat);
	DestroyMat(translate_mat);
	DestroyMat(ModelMatrix);
	DestroyMat(ViewMatrix);
	DestroyMat(ProjectionMatrix);
	DestroyMat(MVP);
}

void OnReshape(int width, int height){
	screen_height = height;
	screen_width = width;
	glViewport(0, 0, width, height);
}

int main(int argc, char **argv){
	camera.X = camera.Y = camera.Z = 0.0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen_width, screen_height);
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("The First Program - Triangle");
	glewExperimental = 1;
	
	GLenum gl_status = glewInit();
	if (gl_status != GLEW_OK){
		fprintf(stderr, "Error gl_status");
		return 0;
	}
	if (MyInit()){
		glutDisplayFunc(Display);
		glEnable(GL_DEPTH_TEST);
		glutKeyboardFunc(KeyboardEvents);
		glutIdleFunc(OnIdle);
		glutReshapeFunc(OnReshape);
		glutMainLoop();
	}
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle_vertices);
	glDeleteBuffers(1, &vbo_triangle_colors);
	system("pause");
	return EXIT_SUCCESS;
}

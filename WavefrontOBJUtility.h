#pragma once
#include <vector>

#if defined(__APPLE__) && defined(__MACH__)

#  include <GLUT/glut.h>

#else
#  include <GL/glut.h>
#endif

//this class exists to share some important typedefs and the OpenGL includes with many classes including WavefrontOBJ, Geometry and more
class WavefrontOBJUtility
{
public:
	//Wavefront OBJ features
	const struct OBJ_COLOR {
		GLfloat red, green, blue;
		OBJ_COLOR() : red(1.0), green(1.0), blue(1.0) {}
	} OBJ_COLOR;

	typedef struct vertex {
		double x, y, z;
	} vertex;
	typedef struct face_triangle {
		unsigned long v1, v2, v3;
	} face_triangle;
	typedef struct face_quad {
		unsigned long v1, v2, v3, v4;
	} face_quad;

	std::vector<vertex> vertices;
	std::vector<face_quad> faces_quads;
	std::vector<face_triangle> faces_triangles;
	bool is_quad;
};


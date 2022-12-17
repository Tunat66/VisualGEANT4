#pragma once
//code from source: https://wiki.wxwidgets.org/WxGLCanvas

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "wx/wx.h"
#include "wx/sizer.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif


class GLGeometryViewer : public wxGLCanvas
{
	wxGLContext* m_context;

public:
	GLGeometryViewer(wxWindow* parent);
	virtual ~GLGeometryViewer();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();


	void render(wxPaintEvent& evt);


	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	//obj renderer methods
	void init();
	
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


	//the elements of a wavefront OBJ file, extracted through a parser
	std::vector<vertex> vertices;
	std::vector<face_quad> faces_quads;
	std::vector<face_triangle> faces_triangles;
	bool is_quad;

	//using the attributes just above are:
	void calculate_normal(face_triangle f, GLdouble* normal);
	void draw_obj();

	//and finally, or command macro:
	DECLARE_EVENT_TABLE()
};



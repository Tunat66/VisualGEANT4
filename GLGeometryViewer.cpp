//minimal cube code from source: https://wiki.wxwidgets.org/WxGLCanvas
#include "GLGeometryViewer.h"
BEGIN_EVENT_TABLE(GLGeometryViewer, wxGLCanvas)
    EVT_MOTION(GLGeometryViewer::mouseMoved)
    EVT_LEFT_DOWN(GLGeometryViewer::mouseDown)
    EVT_RIGHT_UP(GLGeometryViewer::mouseReleased)
    EVT_RIGHT_DOWN(GLGeometryViewer::rightClick)
    EVT_LEAVE_WINDOW(GLGeometryViewer::mouseLeftWindow)
    EVT_SIZE(GLGeometryViewer::resized)
    EVT_KEY_DOWN(GLGeometryViewer::keyPressed)
    EVT_KEY_UP(GLGeometryViewer::keyReleased)
    EVT_MOUSEWHEEL(GLGeometryViewer::mouseWheelMoved)
    EVT_PAINT(GLGeometryViewer::render)
END_EVENT_TABLE()


//bool is_quad; DUPLICATE (violates one-def rule)
bool render_mode = true; // true = solid body, false = wireframe

const float ZOOM_SPEED = 0.1f;
const float ROTATE_SPEED = 0.1f;
float       DISTANCE = 4.0f;

void GLGeometryViewer::calculate_normal(face_triangle f, GLdouble* normal) {
    // x
    normal[0] = (vertices[f.v2 - 1].y - vertices[f.v1 - 1].y) * (vertices[f.v3 - 1].z - vertices[f.v1 - 1].z)
        - (vertices[f.v3 - 1].y - vertices[f.v1 - 1].y) * (vertices[f.v2 - 1].z - vertices[f.v1 - 1].z);
    // y
    normal[1] = (vertices[f.v2 - 1].z - vertices[f.v1 - 1].z) * (vertices[f.v3 - 1].x - vertices[f.v1 - 1].x)
        - (vertices[f.v2 - 1].x - vertices[f.v1 - 1].x) * (vertices[f.v3 - 1].z - vertices[f.v1 - 1].z);
    // z
    normal[2] = (vertices[f.v2 - 1].x - vertices[f.v1 - 1].x) * (vertices[f.v3 - 1].y - vertices[f.v1 - 1].y)
        - (vertices[f.v3 - 1].x - vertices[f.v1 - 1].x) * (vertices[f.v2 - 1].y - vertices[f.v1 - 1].y);
}

struct camera {
    GLfloat x, y, z, phi, theta;
    camera() : x(-4.0f), y(2.0f), z(0.0f), phi(0), theta(0) {}
} camera;

// some useful events to use

/*TRASH
template <typename doubleType> 
doubleType GLGeometryViewer::ArcSinMod(doubleType value)
{
    //value is in radians
    doubleType rotation_value = value / (2 * M_PI);
    int complete_rotation_value = (int) rotation_value;
    doubleType rotation_modulo = rotaton_value - (doubleType)complete_rotation_value;
    //case less than pi
}*/

//to load a snapshot of view parameters to SystemVariables
void GLGeometryViewer::UpdateSystemVariablesViewpointSnapshots()
{
    GLSystemManager.latitude_current = latitude_current;
    GLSystemManager.longitude_current = longitude_current;
    GLSystemManager.zoom = zoom;
}

void GLGeometryViewer::mouseMoved(wxMouseEvent& event) 
{
    GLfloat longinit = longitude_current;
    GLfloat latinit = latitude_current;
    if (event.Dragging() && isRightMouseButtonDown)
    {
        PositionChange = event.GetLogicalPosition(wxClientDC(this)) - initialPos;
        //GLfloat longtmp = 180 / M_PI * asin((-shiftRate * deltay + sin(M_PI / 180 * longinit)));
        //GLfloat lattmp = 180 / M_PI * asin((-shiftRate * deltay + sin(M_PI / 180 * latinit)));
        deltax = (GLfloat)PositionChange.x;
        deltay = (GLfloat)PositionChange.y;
        GLfloat longtmp = shiftRate * deltax + longinit;
        GLfloat lattmp = -shiftRate * deltay + latinit;
        render_again(longtmp, lattmp, zoom);
    }
    UpdateSystemVariablesViewpointSnapshots();
}

void GLGeometryViewer::mouseDown(wxMouseEvent& event) {

    //used while testing
    //longitude_current -= 10.0f;
    //render_again(longitude_current, latitude_current, zoom);

}
void GLGeometryViewer::mouseWheelMoved(wxMouseEvent& event) 
{
    zoom = zoom*exp((GLfloat) 0.0001*event.GetWheelRotation());
    render_again(longitude_current, latitude_current, zoom);
    UpdateSystemVariablesViewpointSnapshots();
}

void GLGeometryViewer::mouseReleased(wxMouseEvent& event) 
{ // this method fixes final view when mouse is released
    isRightMouseButtonDown = false;
    InitialPositionOriginalReset = true;
    latitude_current = -shiftRate * deltay + latitude_current;
    longitude_current = shiftRate * deltax + longitude_current;
    PositionChange = wxPoint(0, 0);
    deltax = 0.0f;
    deltay = 0.0f;
}
void GLGeometryViewer::rightClick(wxMouseEvent& event) {

    if (InitialPositionOriginalReset)
    {
        initialPos = event.GetLogicalPosition(wxClientDC(this));
        InitialPositionOriginalReset = false;
    }
    //RMB control, now defunct:
    //initialPos2 = event.GetLogicalPosition(wxClientDC(this));
    //longitude_current += 10.0f;
    //render_again(longitude_current, latitude_current, zoom);

    isRightMouseButtonDown = true;

}
void GLGeometryViewer::mouseLeftWindow(wxMouseEvent& event) {}

//WASD control, now defunct

void GLGeometryViewer::keyPressed(wxKeyEvent& event) {}
    /*
    enum {
        wKey=87,
        aKey=65,
        sKey=83,
        dKey=68
    };
    //std::string stringer = std::to_string(event.GetKeyCode());
    //wxString debug(stringer);
    //wxLogMessage(debug);
    switch (event.GetKeyCode())
    {
    case aKey:
        longitude_current += 10.0f;
        render_again(longitude_current, latitude_current, zoom);
        break;
    case dKey:
        longitude_current -= 10.0f;
        render_again(longitude_current, latitude_current, zoom);
        break;
    case wKey:
        latitude_current -= 10.0f;
        render_again(longitude_current, latitude_current, zoom);
        break;
    case sKey:
        latitude_current += 10.0f;
        render_again(longitude_current, latitude_current, zoom);
        break;
    }

}*/
void GLGeometryViewer::keyReleased(wxKeyEvent& event) {}

// Vertices and faces of a simple cube to demonstrate 3D render
// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
GLfloat v[8][3];
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
    {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
    {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };



GLGeometryViewer::GLGeometryViewer(wxWindow* parent, GLfloat latitude_current_in, GLfloat longitude_current_in, GLfloat zoom_in) :
    wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), WavefrontOBJUtility()//, //WavefrontObj(File) //changed: args to NULL
{
    m_context = new wxGLContext(this);
    // prepare a simple cube to demonstrate 3D render
    // source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

    // To avoid flashing on MSWindows
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    //set the file source
    File = GLSystemManager.CurrentProjectDir + "\\setup.obj"; //note that for any project, the obj file is called "setup.obj"
    DisplayedObj = new WavefrontObj(File);

    latitude_current = latitude_current_in;
    longitude_current = longitude_current_in;
    zoom = zoom_in;


    //initialize, see below for the method
    init();
}

//this method below is almost IDENTICAL to the constructor, used when a new project is opened (this is becaause
//in wxwidgets, if this viewer is re-instantiated, it gets small on the screen (IDK why??!!))
void GLGeometryViewer::refresh_view() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the screen and the depth buffer
    glLoadIdentity(); // Reset The Projection Matrix
    m_context = new wxGLContext(this);

    // To avoid flashing on MSWindows
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    //set the file source
    File = GLSystemManager.CurrentProjectDir + "\\setup.obj"; //note that for any project, the obj file is called "setup.obj"
    DisplayedObj = new WavefrontObj(File);

    //initialize, see below for the method
    init();
}

void GLGeometryViewer::refresh_view_dbg()
{
    m_context = new wxGLContext(this);

    // To avoid flashing on MSWindows
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    //set the file source
    File = GLSystemManager.CurrentProjectDir + "\\dummy.obj"; //note that for any project, the obj file is called "setup.obj"
    DisplayedObj = new WavefrontObj(File);

    //initialize, see below for the method
    init();
}


GLGeometryViewer::~GLGeometryViewer()
{
    delete m_context;
}


void GLGeometryViewer::resized(wxSizeEvent& evt)
{
    //	wxGLCanvas::OnSize(evt);

    Refresh();
}

//OBJ VIEWER METHODS
void GLGeometryViewer::init() {


}

void GLGeometryViewer::load_obj() {

    if (GLSystemManager.Project_isOpen) {
        wxString str(File);
        wxLogMessage(str);
        DisplayedObj->open_obj(File);

        //load wavefrontobj data to this viewer class:
        vertices = DisplayedObj->vertices;
        faces_quads = DisplayedObj->faces_quads;
        faces_triangles = DisplayedObj->faces_triangles;
        is_quad = DisplayedObj->is_quad;
        wxLogMessage("Object successfully in viewer memory!");
    }
    render_again(longitude_current, latitude_current, zoom);
}

void GLGeometryViewer::draw_obj() {
    
    if (GLSystemManager.Project_isOpen) {

        if (render_mode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (/*is_quad*/true) {
            for (face_quad f : faces_quads) {
                // Calculate normal by calculating two tri normals & averaging
                face_triangle f_tri1 = { f.v1, f.v2, f.v3 };
                face_triangle f_tri2 = { f.v2, f.v3, f.v4 };
                GLdouble normal_tri1[3];
                GLdouble normal_tri2[3];
                calculate_normal(f_tri1, normal_tri1);
                calculate_normal(f_tri2, normal_tri2);
                GLdouble normal[3] = {
                        (normal_tri1[0] + normal_tri2[0]) / 2,
                        (normal_tri1[1] + normal_tri2[1]) / 2,
                        (normal_tri1[2] + normal_tri2[2]) / 2
                };

                glBegin(GL_QUADS);
                glColor3f(OBJ_COLOR.red, OBJ_COLOR.green, OBJ_COLOR.blue);
                glNormal3dv(normal);
                glVertex3d(vertices[f.v1 - 1].x, vertices[f.v1 - 1].y, vertices[f.v1 - 1].z);
                glVertex3d(vertices[f.v2 - 1].x, vertices[f.v2 - 1].y, vertices[f.v2 - 1].z);
                glVertex3d(vertices[f.v3 - 1].x, vertices[f.v3 - 1].y, vertices[f.v3 - 1].z);
                glVertex3d(vertices[f.v4 - 1].x, vertices[f.v4 - 1].y, vertices[f.v4 - 1].z);
                glEnd();
            }
        }
        if(true) {
            for (face_triangle f : faces_triangles) {
                GLdouble normal[3];
                calculate_normal(f, normal);
                glBegin(GL_TRIANGLES);
                glColor3f(OBJ_COLOR.red, OBJ_COLOR.green, OBJ_COLOR.blue);
                glNormal3dv(normal);
                glVertex3d(vertices[f.v1 - 1].x, vertices[f.v1 - 1].y, vertices[f.v1 - 1].z);
                glVertex3d(vertices[f.v2 - 1].x, vertices[f.v2 - 1].y, vertices[f.v2 - 1].z);
                glVertex3d(vertices[f.v3 - 1].x, vertices[f.v3 - 1].y, vertices[f.v3 - 1].z);
                glEnd();
            }
        }
        glFlush();
    }
    else { //show the dummy cube
        glColor4f(1, 0, 0, 1);
        for (int i = 0; i < 6; i++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex3fv(&v[faces[i][0]][0]);
            glVertex3fv(&v[faces[i][1]][0]);
            glVertex3fv(&v[faces[i][2]][0]);
            glVertex3fv(&v[faces[i][3]][0]);
            glVertex3fv(&v[faces[i][0]][0]);
            glEnd();
        }
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (h == 0) {
        gluPerspective(80, (float)w, 1.0, 5000.0);
    }
    else {
        gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/** Inits the OpenGL viewport for drawing in 3D. */
void GLGeometryViewer::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_COLOR_MATERIAL);

    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio_w_h = (float)(bottomrigth_x - topleft_x) / (float)(bottomrigth_y - topleft_y);
    gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_FLAT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT1);
    GLfloat lightAmbient1[4] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat lightPos1[4] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat lightDiffuse1[4] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat lightSpec1[4] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightLinAtten = 0.0f;
    GLfloat lightQuadAtten = 1.0f;
    //glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat*)&lightPos1);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat*)&lightAmbient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat*)&lightDiffuse1);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, (GLfloat*)&lightSpec1);
    glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, &lightLinAtten);
    glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, &lightQuadAtten);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

}

/** Inits the OpenGL viewport for drawing in 2D. */
void GLGeometryViewer::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int GLGeometryViewer::getWidth()
{
    return GetSize().x;
}

int GLGeometryViewer::getHeight()
{
    return GetSize().y;
}


//the MAIN method here
void GLGeometryViewer::render(wxPaintEvent& evt)
{
    if (!IsShown()) return; //may require modification
    if (IsNotFirst) return;
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*TRIALS BEGIN
    // ------------- draw some 2D ----------------
    prepare2DViewport(0, 0, getWidth() / 2, getHeight());
    glLoadIdentity();

    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(getWidth(), 0, 0);
    glVertex3f(getWidth(), getHeight(), 0);
    glVertex3f(0, getHeight(), 0);
    glEnd();

    // red square
    glColor4f(1, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(getWidth() / 8, getHeight() / 3, 0);
    glVertex3f(getWidth() * 3 / 8, getHeight() / 3, 0);
    glVertex3f(getWidth() * 3 / 8, getHeight() * 2 / 3, 0);
    glVertex3f(getWidth() / 8, getHeight() * 2 / 3, 0);
    glEnd();
    */
    // ------------- draw some 3D ----------------
    prepare3DViewport(0, 0, getWidth(), getHeight());
    glLoadIdentity();

    glColor4f(0, 0, 1, 1);
    glTranslatef(0, 0, -15);
    //glRotatef(longitude_current, 0.0f, 1.0f, 0.0f);
    //glRotatef(latitude_current, 1.0f/*(GLfloat)sin(M_PI / 180.0 * (double)latitude)*/, 0.0f, /*(GLfloat)-cos(M_PI / 180.0 * (double)latitude)*/0.0f);
    //glScalef(zoom, zoom, zoom);

    /*glColor4f(1, 0, 0, 1);
    for (int i = 0; i < 6; i++)
    {
        glBegin(GL_LINE_STRIP);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glEnd();
    }*/
    draw_obj();

    //glFlush();
    SwapBuffers();
    IsNotFirst = true;
}

void GLGeometryViewer::render_again(GLfloat longitude, GLfloat latitude, GLfloat zoom)
{
    if (!IsShown()) return; //may require modification
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*TRIALS BEGIN
    // ------------- draw some 2D ----------------
    prepare2DViewport(0, 0, getWidth() / 2, getHeight());
    glLoadIdentity();

    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(getWidth(), 0, 0);
    glVertex3f(getWidth(), getHeight(), 0);
    glVertex3f(0, getHeight(), 0);
    glEnd();

    // red square
    glColor4f(1, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(getWidth() / 8, getHeight() / 3, 0);
    glVertex3f(getWidth() * 3 / 8, getHeight() / 3, 0);
    glVertex3f(getWidth() * 3 / 8, getHeight() * 2 / 3, 0);
    glVertex3f(getWidth() / 8, getHeight() * 2 / 3, 0);
    glEnd();
    */
    // ------------- draw some 3D ----------------
    prepare3DViewport(0, 0, getWidth(), getHeight());
    glLoadIdentity();

    glColor4f(0, 0, 1, 1);
    glTranslatef(0, 0, -15);
    glRotatef(longitude, 0.0f, 1.0f, 0.0f);
    ///glRotatef(latitude, (GLfloat)sin(M_PI / 180.0 * (double)longitude), 0.0f, (GLfloat)-cos(M_PI / 180.0 * (double)longitude));//this one rotates your head
    glRotatef(latitude, -(GLfloat)cos(M_PI / 180.0 * (double)longitude) , 0.0f, -(GLfloat)sin(M_PI / 180.0 * (double)longitude));
    glScalef(zoom, zoom, zoom);
    draw_obj();

    //glFlush();
    SwapBuffers();
}


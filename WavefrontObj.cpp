#include "WavefrontObj.h"
//for debug
#include "wx/wx.h"

WavefrontObj::WavefrontObj(std::string File)
{
	FileName = File;
    //classes which use the WavefrontObj Class for utility
}

WavefrontObj::~WavefrontObj()
{
}



void WavefrontObj::calculate_normal(face_triangle f, GLdouble* normal) {
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

void WavefrontObj::open_obj(std::string filename) {


    std::cout << "Opening file: " << filename << std::endl;

    std::ifstream file_stream;
    file_stream.open(filename, std::ifstream::in);

    
    if ((file_stream.rdstate() & std::ifstream::failbit) != 0) {
        std::cerr << "Error opening " << filename << std::endl;
        exit(1);
    }

    while (file_stream.good()) {
        char file_line[100];
        file_stream.getline(file_line, 100);
        obj_parse(file_line);
    }

    file_stream.close();

    is_quad = (faces_quads.size() > faces_triangles.size());

    wxLogMessage("Object loaded succesfully");
}

void WavefrontObj::obj_parse(char* file_line) {

    if (file_line[0] == '\0') {
        // End of file
        return;
    }

    char* tokenized;
    tokenized = strtok(file_line, " ");
    char first_char = tokenized[0];

    if (first_char == '#') {
        // Comment line, ignore
        return;
    }
    else if (first_char == 'v') {
        // Vertex

        double x = strtod(strtok(NULL, " "), NULL);
        double y = strtod(strtok(NULL, " "), NULL);
        double z = strtod(strtok(NULL, " "), NULL);

        vertex v = { x , y , z }; // it used to have * 20 values for whatever reason, removed them
        vertices.push_back(v);
    }
    else if (first_char == 'f') {
        // Face

        unsigned long v1 = strtoul(strtok(NULL, " "), NULL, 0);
        unsigned long v2 = strtoul(strtok(NULL, " "), NULL, 0);
        unsigned long v3 = strtoul(strtok(NULL, " "), NULL, 0);

        unsigned long v4;
        char* v4_str = strtok(NULL, " ");
        if (v4_str != NULL) {
            // Face is a quad
            v4 = strtoul(v4_str, NULL, 0);

            face_quad f = { v1, v2, v3, v4 };
            faces_quads.push_back(f);
        }
        else {
            // Face is a triangle
            face_triangle f = { v1, v2, v3 };
            faces_triangles.push_back(f);
        }
    }
}

/*void WavefrontObj::ParseFile()
{
	std::ifstream ObjFile;
	ObjFile.open(FileName);

	std::string Value;

	bool flag = true;
	while (flag)
	{
		ObjFile >> Value;
		if (Value != "v")
			flag = false;
		else 
		{
			Vertex* NewVertex = new Vertex();
			for (int i = 0; i < 3; i++)
			{
				std::string Number;
				ObjFile >> Number;
				double temp = std::stod(Number);
				NewVertex->vertexvalue[i] = temp;
			}
			Vertices->push_back(*NewVertex); //don't forget to dereferance
			delete NewVertex;
		}
	}

	ObjFile.close();
}*/

void WavefrontObj::TranslateBy(std::vector<double> direction)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertex current = vertices.at(i);
        current.x += direction.at(0);
        current.y += direction.at(1);
        current.z += direction.at(2);
    }
}

double WavefrontObj::SeperationAxial(vertex point1, vertex point2, int coordinate)
{
    double res;
    switch (coordinate)
    {
    case 1:
        res = point1.x - point2.x;
        break;
    case 2:
        res = point1.y - point2.y;
        break;
    case 3:
        res = point1.z - point2.z;
        break;
    }
    return res;
}

void WavefrontObj::Scale1D(vertex center, int coordinate, double ScaleFactor)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertex& current = vertices.at(i); //create reference for easy use
        current.x = center.x + ScaleFactor * SeperationAxial(current, center, coordinate);
        
    }
}


void WavefrontObj::Scale(vertex center, double ScaleFactor)
{
    Scale1D(center, x, ScaleFactor);
    Scale1D(center, y, ScaleFactor);
    Scale1D(center, z, ScaleFactor);
}

std::vector<double> WavefrontObj::FindLengthWidthHeight()
{
    //find the max and min coordinates of vertices
    vertex current = vertices.at(0);
    double xMax = current.x;
    double xMin = current.x;
    double yMax = current.y;
    double yMin = current.y;
    double zMax = current.z;
    double zMin = current.z;

    for (int i = 1; i < vertices.size(); i++)
    {
        current = vertices.at(i);
        if (current.x < xMin)
            xMin = current.x;
        if (current.y < yMin)
            yMin = current.y;
        if (current.z < zMin)
            zMin = current.z;
        if (current.x > xMax)
            xMax = current.x;
        if (current.y > yMax)
            yMax = current.y;
        if (current.z > zMax)
            zMax = current.z;
    }
    std::vector<double> result{ xMax - xMin, yMax - yMin, zMax - zMin };
    return result;
}

std::vector<double> WavefrontObj::GetBoundingBoxCenter()
{
    //find the max and min coordinates of vertices
    vertex current = vertices.at(0);
    double xMax = current.x;
    double xMin = current.x;
    double yMax = current.y;
    double yMin = current.y;
    double zMax = current.z;
    double zMin = current.z;

    for (int i = 1; i < vertices.size(); i++)
    {
        current = vertices.at(i);
        if (current.x < xMin)
            xMin = current.x;
        if (current.y < yMin)
            yMin = current.y;
        if (current.z < zMin)
            zMin = current.z;
        if (current.x > xMax)
            xMax = current.x;
        if (current.y > yMax)
            yMax = current.y;
        if (current.z > zMax)
            zMax = current.z;
    }
    std::vector<double> result{ (xMax + xMin)/2, (yMax + yMin)/2, (zMax + zMin)/2 };
    return result;
}
  
//a method specifically for boxes:
void WavefrontObj::BoxResize(std::vector<double> NewDimensions)
{
    std::vector OldDimensions = FindLengthWidthHeight();
    std::vector BoxCenter = GetBoundingBoxCenter();
    
    //parse vector to vertex
    vertex Center;
    Center.x = BoxCenter.at(0);
    Center.y = BoxCenter.at(1);
    Center.z = BoxCenter.at(2);

    double ScaleFactor_x = NewDimensions.at(0) / OldDimensions.at(0);
    double ScaleFactor_y = NewDimensions.at(1) / OldDimensions.at(1);
    double ScaleFactor_z = NewDimensions.at(2) / OldDimensions.at(2);
    
    Scale1D(Center, x, ScaleFactor_x);
    Scale1D(Center, y, ScaleFactor_y);
    Scale1D(Center, z, ScaleFactor_z);

}

void WavefrontObj::PrintToFile(std::string FileNew)
{
    //Warning: this shall be a file whose name is different than that containing essential
    //comments, as it will overwrite its contents
	std::ofstream OutputFile;
	OutputFile.open(FileNew, std::ofstream::out | std::ofstream::trunc); //options for overwriting
    
    //print vertices
    for (int i = 0; i < vertices.size(); i++)
    {
        vertex current = vertices.at(i);
        OutputFile << 'v ';
        OutputFile << current.x;
        OutputFile << ' ';
        OutputFile << current.y;
        OutputFile << ' ';
        OutputFile << current.z;
        OutputFile << '\n';
    }

    OutputFile << '\n';

    //print rectangular faces
    for (int i = 0; i < faces_quads.size(); i++)
    {
        face_quad current = faces_quads.at(i);
        OutputFile << 'f ';
        OutputFile << current.v1;
        OutputFile << ' ';
        OutputFile << current.v2;
        OutputFile << ' ';
        OutputFile << current.v3;
        OutputFile << ' ';
        OutputFile << current.v4;
        OutputFile << '\n';
    }

    OutputFile << '\n';

    //print triangular faces
    for (int i = 0; i < faces_triangles.size(); i++)
    {
        face_triangle current = faces_triangles.at(i);
        OutputFile << 'f ';
        OutputFile << current.v1;
        OutputFile << ' ';
        OutputFile << current.v2;
        OutputFile << ' ';
        OutputFile << current.v3;
        OutputFile << '\n';
    }

}
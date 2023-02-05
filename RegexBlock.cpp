#include <fstream>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main()
{
    /*ifstream f("drawByParticleID.mac"); //taking file as inputstream
    string str;
    if(f) {
      ostringstream ss;
      ss << f.rdbuf(); // reading data
      str = ss.str();
    }
    cout<<str;
    
    f.close();

    ifstream f2("vis.mac");
    string newstr2 = " ";
    if(f2) {
      ostringstream ss1;
      ss1 << f2.rdbuf(); // reading data
      newstr2 = ss1.str();
    }

    newstr2 = std::regex_replace(newstr2, std::regex("#TRAJMOD[^*]+#TRAJMOD"), str);

    f2.close();

    ofstream ofs;
    ofs.open("vis.mac", std::ofstream::out | std::ofstream::trunc);
    ofs << newstr2;
    ofs.close();*/

    system("D:/VisualGEANT4/VisualGEANT4/VisualGEANT4.exe");
    //config vis D:/VisualGEANT4/Example new <macroname>.mac

    return 0;
}
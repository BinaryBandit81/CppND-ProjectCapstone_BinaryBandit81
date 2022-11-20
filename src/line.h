#ifndef LINE_H
#define LINE_H

#include <iostream>

class Line {
public:
    Line();
    Line(std::string name, int colorR, int colorB, int colorG) : _name(name), _colorR(colorR), _colorB(colorB), _colorG(colorG){}

    std::string getName(){return _name;}
    void getColor(int &red, int &blue, int &green){red = _colorR; blue = _colorB; green = _colorG;}

private:
    std::string _name;
    int _colorR, _colorB, _colorG;
};
#endif
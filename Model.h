#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <GL/freeglut.h>

class Model
{
private:
    //Attribute
    std::vector<float> position_texture_normal;
    int sz;
    //Function
    std::vector<std::string> splitString(std::string str, char delimiter);
    float getFloat(std::string input);
    int getInt(std::string input);
public:
    Model(std::string objFilePath);
    ~Model();
    void Draw();
};

#endif

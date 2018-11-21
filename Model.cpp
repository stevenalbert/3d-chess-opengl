#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "Model.h"

Model::Model(std::string objFilePath)
{
    std::vector<float> position_vector;
    std::vector<float> normal_vector;
    std::vector<float> texture_coord;

    std::fstream obj;
	std::string input;
	std::vector<std::string> split;
	obj.open(objFilePath);
	if(obj.is_open())
	{
		while(getline(obj, input))
		{
			split = splitString(input, ' ');
			int sz = split.size();
			if(sz==0)continue;
			if(split[0]=="v")
            {
                position_vector.push_back(getFloat(split[1]));
                position_vector.push_back(getFloat(split[2]));
                position_vector.push_back(getFloat(split[3]));
            }
			else if(split[0]=="vt")
            {
                texture_coord.push_back(getFloat(split[1]));
                texture_coord.push_back(getFloat(split[2]));
            }
			else if(split[0]=="vn")
            {
                normal_vector.push_back(getFloat(split[1]));
                normal_vector.push_back(getFloat(split[2]));
                normal_vector.push_back(getFloat(split[3]));
            }
            else if(split[0]=="f")
            {
                std::vector<std::string> inSplit;
                for(int i=1; i<=3; i++)
                {
                    inSplit = splitString(split[i], '/');
                    int index = 3 * (getInt(inSplit[0])-1);
                    position_texture_normal.push_back(position_vector[index]);
                    position_texture_normal.push_back(position_vector[index+1]);
                    position_texture_normal.push_back(position_vector[index+2]);
                    index = 2 * (getInt(inSplit[1])-1);
                    position_texture_normal.push_back(texture_coord[index]);
                    position_texture_normal.push_back(texture_coord[index+1]);
                    index = 3 * (getInt(inSplit[2])-1);
                    position_texture_normal.push_back(normal_vector[index]);
                    position_texture_normal.push_back(normal_vector[index+1]);
                    position_texture_normal.push_back(normal_vector[index+2]);
                }
            }
		}
	}
    sz = position_texture_normal.size() / 8;
}

void Model::Draw()
{
    int index;
    glPushMatrix();
        glBegin(GL_TRIANGLES);
            for(int i=0; i<sz; i++)
            {
                index = 8 * i;
                glNormal3f(position_texture_normal[index+5], position_texture_normal[index+6], position_texture_normal[index+7]);
                glTexCoord2f(position_texture_normal[index+3], position_texture_normal[index+4]);
                glVertex3f(position_texture_normal[index], position_texture_normal[index+1], position_texture_normal[index+2]);
            }
        glEnd();
    glPopMatrix();
}

Model::~Model()
{
    position_texture_normal.clear();
}

std::vector<std::string> Model::splitString(std::string str, char delimiter)
{
	std::vector<std::string> split;
	int sz = str.length(), lastIdx = -1, i;

	for(i=0; i<sz; i++)
	{
		if(str[i]==delimiter)
		{
			split.push_back(str.substr(lastIdx+1, i-lastIdx-1));
			lastIdx = i;
		}
	}
	if(i==sz && lastIdx!=i-1) split.push_back(str.substr(lastIdx+1));

	return split;
}

float Model::getFloat(std::string input)
{
    std::istringstream ssinput(input);
	float check;

	ssinput >> check;
	return check;
}

int Model::getInt(std::string input)
{
    std::istringstream ssinput(input);
	int check;

	ssinput >> check;
	return check;
}

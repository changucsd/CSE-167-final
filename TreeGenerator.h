#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include<GLFW/glfw3.h>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<algorithm>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"Transform.h"
#include"Geometry.h"



#define X_AXIS_VEC (glm::vec3(1, 0, 0))
#define Y_AXIS_VEC (glm::vec3(0, 1, 0))
#define Z_AXIS_VEC (glm::vec3(0, 0, 1))

#define TRUNKHEIGHT (2.0f)
#define TRUNKWIDTH (0.5f)
//define POTENTIAL(x,level,maxlevel) (((float)(x)) * ((((float)(level)) + 1.0f) / (((float)(maxlevel)) + 1.0f)))
#define POTENTIAL(x,level,maxlevel) (((float)(x)) * ((((float)(level)) + 1.0f) / (((float)(maxlevel)) + 1.0f)) * ((((float)(level)) + 1.0f) / (((float)(maxlevel)) + 1.0f)))
//define POTENTIAL(x,level,maxlevel) (((float)(x)) * pow((1.0f / 2.0f), ((maxlevel) - (level))))
#define RANDOMIZE(min,max) ((((float)((rand() % (max)) + 1)) / 100.0f) + (((float)(min)) / 100.0f))

class TreeGenerator
{
public:
	TreeGenerator();
	~TreeGenerator();
	Transform* generateTree(int levels);

private:
	std::string barkTextureFile;
	std::string leafTextureFile;
	Geometry branch;
	Geometry leaf;
    Transform world;
	Transform* createTree(int level, int maxlevel);
};

#endif

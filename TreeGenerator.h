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

#define TRUNKHEIGHT (0.25f)
#define TRUNKWIDTH (0.5f)
#define Zturn (40.0f)
#define zturn (-40.0f)
#define Yturn (40.0f)
#define yturn (-50.0f)
//define POTENTIAL(x,level,maxlevel) (((float)(x)) * ((((float)(level)) + 1.0f) / (((float)(maxlevel)) + 1.0f)))
#define POTENTIALW(x,level,maxlevel) (((float)(x)) * ((((float)(level)) + 2.0f) / (((float)(maxlevel)) + 2.0f)) * ((((float)(level)) + 1.0f) / (((float)(maxlevel)) + 1.0f)))
#define POTENTIAL(x,level,maxlevel) (((float)(x)) * pow((9.0f / 10.0f), ((maxlevel) - (level))))
//define POTENTIALW(x, level, maxlevel) (((float)(x)) * pow((1.0f / 1.1f), (level)))
//define RANDOMIZE(min,max) ((((float)((rand() % (max)) + 1)) / 100.0f) + (((float)(min)) / 100.0f))
#define PLUSMINUS(x) (((2.0f * ((float)(x))) * (((float)((rand() % 100) + 1.0f)) / 100.0f)) - ((float)(x)))

class TreeGenerator
{
public:
	TreeGenerator();
	~TreeGenerator();
	Transform* generateTree(int levels, int which);

private:
	std::string barkTextureFile;
	std::string leafTextureFile;
	Geometry branch;
	Geometry leaf;
    Transform world;
	Transform* createTree(std::string rule, int& index, int level, int maxlevel);
};

#endif

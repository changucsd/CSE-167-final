#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <list>
#include "Node.h"
#include<glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Node
{
public:
	Transform();
	Transform(glm::mat4 M);
	~Transform();
	void draw(glm::mat4 C, GLuint shaderProgram);
	void update();
	Transform* addChild(Node* node);
	Transform* removeChild(Node* node);
	Transform* translate(float x, float y, float z);
    Transform* translate(glm::vec3 trans);
	Transform* scale(float amount);
	Transform* scale(float x, float y, float z);
	Transform* scaleAbs(float x, float y, float z);
	Transform* rotate(glm::vec3 rotAxis, float deg);
	Transform* resetTranslate();
	Transform* resetScale();
	Transform* setAnimation(glm::mat4 (*animate)(glm::mat4 M));

private:
	glm::mat4 M; //Transformation Matrix
	glm::mat4 totalRot;
	glm::mat4 totalTrans;
	glm::mat4 totalScale;
	std::list<Node*> children;
	glm::mat4 (*animate)(glm::mat4 M);

};

#endif

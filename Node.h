#ifndef NODE_H
#define NODE_H

#include<glm/glm.hpp>
#include <GLFW/glfw3.h>



class Node
{
public:
	Node();
	~Node();
	virtual void draw(glm::mat4 C, GLuint shaderProgram) = 0;
	virtual void update() = 0;

	bool heap;

protected:

private:

};

#endif

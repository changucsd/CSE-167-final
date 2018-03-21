#include "Transform.h"

Transform::Transform()
{
	M = glm::mat4(1.0f);
	totalRot = glm::mat4(1.0f);
	totalTrans = glm::mat4(1.0f);
	totalScale = glm::mat4(1.0f);
}

Transform::Transform(glm::mat4 M)
{
	this->M = M;
	totalRot = glm::mat4(1.0f);
	totalTrans = glm::mat4(1.0f);
	totalScale = glm::mat4(1.0f);
}

Transform::~Transform()
{
	std::list<Node*>::iterator it = children.begin();
	std::list<Node*>::iterator end = children.end();
	for (; it != end; ++it)
	{
		if((*it)->heap)
		{
			delete (*it);
		}
	}
}

void Transform::draw(glm::mat4 C, GLuint shaderProgram)
{
	glm::mat4 totalTransformation = C * totalTrans * totalRot * totalScale * M;
	std::list<Node*>::iterator it = children.begin();
	std::list<Node*>::iterator end = children.end();
	for (; it != end; ++it)
	{
		(*it)->draw(totalTransformation, shaderProgram);
	}
}

void Transform::update()
{
	if (animate)
	{
		M = animate(M);
	}
	std::list<Node*>::iterator it = children.begin();
	std::list<Node*>::iterator end = children.end();
	for (; it != end; ++it)
	{
		(*it)->update();
	}
}

Transform* Transform::addChild(Node* node)
{
    if(node != NULL)
    {
        children.push_back(node);
    }
	return this;
}

Transform* Transform::removeChild(Node * node)
{
	children.remove(node);
	return this;
}

Transform* Transform::translate(float x, float y, float z)
{
	totalTrans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * totalTrans;
	return this;
}

Transform* Transform::translate(glm::vec3 trans)
{
    totalTrans = glm::translate(glm::mat4(1.0f), trans) * totalTrans;
    return this;
}

Transform* Transform::scale(float amount)
{
	totalScale = glm::scale(glm::mat4(1.0f), glm::vec3(pow(1.2f, amount))) * totalScale;
	return this;
}

Transform* Transform::scale(float x, float y, float z)
{
	totalScale = glm::scale(glm::mat4(1.0f), glm::vec3(pow(1.2f, x), pow(1.2f, y), pow(1.2f, z))) * totalScale;
	return this;
}

Transform* Transform::scaleAbs(float x, float y, float z)
{
	totalScale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z)) * totalScale;
	return this;
}

Transform* Transform::rotate(glm::vec3 rotAxis, float deg)
{
	totalRot = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), rotAxis) * totalRot;
	return this;
}

Transform* Transform::resetTranslate()
{
	totalTrans = glm::mat4(1.0f);
	return this;
}

Transform* Transform::resetScale()
{
	totalScale = glm::mat4(1.0f);
	return this;
}

Transform* Transform::setAnimation(glm::mat4 (*animate)(glm::mat4 M))
{
	this->animate = animate;
	return this;
}

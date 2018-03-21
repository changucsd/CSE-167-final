#include "Geometry.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Geometry::Geometry()
{
	initialized = false;
	transFirst = false;
	hasTexture = false;
}

Geometry::~Geometry()
{
	if (initialized)
	{
		// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
		// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &NBO);
		if (hasTexture)
		{
			glDeleteBuffers(1, &textureID);
		}
	}
}

void Geometry::init(std::string filepath)
{
	initialized = true;
	toWorld = glm::mat4(1.0f);
    
    if(filepath.length())
    {
        parse(filepath.c_str());
        centerAndResize();
    }

	angle = 0;
	direction = 0;
	scalePointAmount = 1;
	x = 0;
	y = 0;
	z = 0;
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &NBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
	totalRot = glm::mat4(1.0f);
	rotOffset = glm::mat4(1.0f);
	transFirst = false;
}

void Geometry::update()
{
}

void Geometry::centerForTerrain()
{
        int size = vertices.size();
        glm::vec3 center;
        float xMin = FLT_MAX;
        float xMax = FLT_MIN;
        float yMax = FLT_MIN;
        float zMin = FLT_MAX;
        float zMax = FLT_MIN;
        float scaleDownAmount = 0;
        glm::vec4 intermVal;
        for (int i = 0; i < size; ++i)
            {
                    xMin = (vertices[i].x < xMin) ? vertices[i].x : xMin;
                    xMax = (vertices[i].x > xMax) ? vertices[i].x : xMax;
                    yMax = (vertices[i].y > yMax) ? vertices[i].y : yMax;
                    zMin = (vertices[i].z < zMin) ? vertices[i].z : zMin;
                    zMax = (vertices[i].z > zMax) ? vertices[i].z : zMax;
            }
        center = glm::vec3((xMax + xMin) / 2.0f, 0.0f, (zMax + zMin) / 2.0f);
        for (int i = 0; i < vertices.size(); ++i)
            {
                    vertices[i] = (glm::translate(glm::mat4(1.0f), (center * -1.0f)) * glm::vec4(vertices[i], 1));
                }
}

void Geometry::draw(glm::mat4 C, GLuint shaderProgram)
{

    glm::vec3 setColor = color;
    glUniform3fv(glGetUniformLocation(shaderProgram, "myColor"), 1, glm::value_ptr(setColor));

    
    glUniform1f(glGetUniformLocation(shaderProgram, "terrain"),terr);
	// Now send these values to the shader program

	//shaderProgram->setMat4("model", C * toWorld);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(C * toWorld));
	// Now draw. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
     
}

std::vector<glm::vec3>& Geometry::getVertices()
{
        return vertices;
    
}

std::vector<glm::vec3>& Geometry::getNormals()
{
        return normals;
    }

std::vector<unsigned int>& Geometry::getIndices()
{
        return indices;
}
        
void Geometry::parse(const char * filepath)
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	errno = 0;
	FILE* inFile = fopen(filepath, "r");
	char vertexType[3] = { 0 };
	float x = 0;
	float y = 0;
	float z = 0;
	float red = 0;
	float green = 0;
	float blue = 0;
	float vertex;
	float texture;
	float normal;
	float notUsed = 0;
	std::vector<unsigned int> temp_indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_texCoord;

	if (inFile == NULL)
	{
		std::cerr << "Error loading file:" << filepath << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	else
	{
		while (fscanf(inFile, "%s", vertexType) != EOF)
		{
			if (strncmp(vertexType, "vn", 2) == 0)
			{
				fscanf(inFile, "%f %f %f\n", &x, &y, &z);
				normals.push_back(glm::vec3(x, y, z));
			}
			else if (strncmp(vertexType, "vt", 2) == 0)
			{
				fscanf(inFile, "%f %f\n", &x, &y);
				texCoord.push_back(glm::vec2(x, y));
			}
			else if (strncmp(vertexType, "v", 1) == 0)
			{
				fscanf(inFile, "%f %f %f %f %f %f\n", &x, &y, &z, &red, &green, &blue);
				vertices.push_back(glm::vec3(x, y, z));
			}
			else if (strncmp(vertexType, "f", 1) == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						fscanf(inFile, "/");
						if (j == 0)
						{
							if(fscanf(inFile, "%f", &vertex))
							{
								indices.push_back(vertex - 1);
							}
						}
						else if (j == 1)
						{
							if(fscanf(inFile, "%f", &texture))
							{
								if (strcmp(filepath, "c:\\Users\\Wilson\\Documents\\School\\CSE167\\PA3\\CSE167StarterCode-master\\robot-parts\\head.obj") == 0)
								{
									std::cout << texture << std::endl;
								}
								tindices.push_back(texture - 1);
							}
						}
						else if (j == 2)
						{
							if(fscanf(inFile, "%f", &normal))
							{
								nindices.push_back(normal - 1);
							}
						}
					}
				}
				fscanf(inFile, "\n");
			}
			else
			{
				fscanf(inFile, "%*[^\n]\n");
			}
			//vertexType[1] = 0;
		}

	}
}

void Geometry::centerAndResize()
{
	int size = vertices.size();
	glm::vec3 center;
	float xMin = FLT_MAX;
	float xMax = FLT_MIN;
	float yMin = FLT_MAX;
	float yMax = FLT_MIN;
	float zMin = FLT_MAX;
	float zMax = FLT_MIN;
	float scaleDownAmount = 0;
	glm::vec4 intermVal;
	for (int i = 0; i < size; ++i)
	{
		xMin = (vertices[i].x < xMin) ? vertices[i].x : xMin;
		xMax = (vertices[i].x > xMax) ? vertices[i].x : xMax;
		yMin = (vertices[i].y < yMin) ? vertices[i].y : yMin;
		yMax = (vertices[i].y > yMax) ? vertices[i].y : yMax;
		zMin = (vertices[i].z < zMin) ? vertices[i].z : zMin;
		zMax = (vertices[i].z > zMax) ? vertices[i].z : zMax;
	}
	center = glm::vec3((xMax + xMin) / 2.0f, (yMax + yMin) / 2.0f, (zMax + zMin) / 2.0f);
	scaleDownAmount = 2 / std::max(std::max(xMax - xMin, yMax - yMin), zMax - zMin);
	for (int i = 0; i < vertices.size(); ++i)
	{
		intermVal = (glm::translate(glm::mat4(1.0f), (center * -1.0f)) * glm::vec4(vertices[i], 1));
		vertices[i] = glm::vec3(glm::scale(glm::mat4(1.0f), glm::vec3(scaleDownAmount)) * intermVal);
	}
}

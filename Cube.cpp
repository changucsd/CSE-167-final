#include "Cube.h"
#include "Window.h"
#include "stb_image.h"

Cube::Cube(const char* filepath)
{
	toWorld = glm::mat4(1.0f);

    parse(filepath);
    
    //cubeTexture = loadTexture("");
    
    //printf("Size is %lu \n", vertices.size());
    //printf("Size is %lu \n", normals.size());
    //printf("Size is %lu \n", indices.size());
  
    
    //printf("%f, %f, %f\n",dimensionX,dimensionY,dimensionZ);
    
    dimensionX = maxX + minX;
    dimensionY = maxY + minY;
    dimensionZ = maxZ + minZ;
    
    distanceX = maxX - minX;
    distanceY = maxY - minY;
    distanceZ = maxZ - minZ;
   
    float zoom;
    
    if(distanceX >= distanceY && distanceX >= dimensionZ)
    {
        zoom = distanceX;
        
    }
    else if(distanceY >= distanceX && distanceY >= distanceZ)
    {
        
        zoom = distanceY;
    }
    else
    {
        zoom = distanceZ;
    }
    
    printf("Zoom is %f\n", zoom);
    
    
    for(int i = 0;  i < vertices.size(); i ++)
    {
        vertices[i].x = (vertices[i].x -(dimensionX/2.0f))*8.0F/zoom;
        vertices[i].y = (vertices[i].y -(dimensionY/2.0f))*8.0F/zoom;
        vertices[i].z = (vertices[i].z -(dimensionZ/2.0f))*8.0F/zoom;
        
    }
    
    
    position = glm::vec3(0.0f,0.0f,0.0f);
    scale = glm::vec3(1.0f,1.0f,1.0f);
    //rotation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
    rotation = glm::rotate(glm::mat4(1.0f), 0.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    
    dirPostion = glm::vec3(1.0f,0.0f, 2.0f); /// intial value for dirlight
    
    
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), vertices.data(), GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*3*sizeof(float), normals.data(), GL_STATIC_DRAW);
    // Enable the usage of layout location 1 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(float), indices.data(), GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

Cube::~Cube()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Cube::draw(GLuint shaderProgram)
{ 
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc  = glGetUniformLocation(shaderProgram,  "view");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &Window::V[0][0]);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &toWorld[0][0]);
    
    GLint lightPosLoc    = glGetUniformLocation(shaderProgram, "lightPos");
    GLint lightColorLoc  = glGetUniformLocation(shaderProgram, "lightColor");
    GLint viewPosLoc     = glGetUniformLocation(shaderProgram, "viewPos");
    
    glUniform3f(lightPosLoc,10.0f, 0.0f, 20.0f);
    glUniform3f(lightColorLoc,1.0f, 1.0f, 1.0f);
    glUniform3f(viewPosLoc, 0.0f, 0.0f, 20.0f);  // copies from window's camera
    
    
    GLint matAmbientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
    GLint matDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
    GLint matShineLoc = glGetUniformLocation(shaderProgram, "material.shininess");
    
    glUniform3f(matAmbientLoc, ambient.x,ambient.y,ambient.z);
    glUniform3f(matDiffuseLoc, diffuse.x,diffuse.y,diffuse.z);
    glUniform3f(matSpecularLoc, specular.x,specular.y,specular.z);
    glUniform1f(matShineLoc, shininess);
    

    
    //glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), dirPostion.x+ 1.0f + sin(glfwGetTime()) * 2.0f,dirPostion.y,dirPostion.z+ sin(glfwGetTime() / 2.0f) * 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), dirPostion.x,dirPostion.y,dirPostion.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
    

    
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
    
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    */
    
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Cube::parse(const char* filepath)
{
    
    //TODO parse the OBJ file
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    
    unsigned int a,k,c,d,e,f;
    float x,y,z;
    float r,g,b,normal;
    char c1,c2;
    
    FILE *file = fopen(filepath,"rb");
    
    
    do{
        c1 = fgetc(file);
        
        if(c1 != EOF ) {
            
            c2 = fgetc(file);
            
            if(c1 == 'v' && c2 == ' ')
            {
                fscanf(file,"%f %f %f", &x, &y, &z);
                //printf("Vertex is %f %f %f \n", x, y, z);
                
                vertices.push_back(glm::vec3(x,y,z));
                
                if(x > maxX)
                {
                    maxX = x;
                }
                else if(x < minX)
                {
                    minX = x;
                }
                
                if(y > maxY)
                {
                    maxY = y;
                }
                else if(y < minY)
                {
                    minY = y;
                }
                
                if(z > maxZ)
                {
                    maxZ = z;
                }
                
                else if(z < minZ)
                {
                    minZ = z;
                }
                
                while (c1 != '\n')   // read through this line
                {
                    c1 = fgetc(file);
                }
            }
            
            else if(c1 == 'v' && c2 == 'n')
            {
                fscanf(file,"%f %f %f",&r,&g,&b);
                //printf(" Prev Value is %f %f %f \n", r, g, b);
                
                normal = r*r + g*g + b*b;
                normal = sqrt(normal);
                
                r = r/normal;
                
                g = g/normal;
                
                b = b/normal;
                
                normals.push_back(glm::vec3(r,g,b));
                //printf(" After Value is %f %f %f \n", r, g, b);
                while (c1 != '\n')   // read through this line
                {
                    c1 = fgetc(file);
                }
            }
            
            else if (c1 == 'f' && c2 == ' ')  // face line
            {
                //fscanf(file,"%f//%f %f//%f %f//%f",&x,&y,&z rgb);
                //printf("I am here\n");
                fscanf(file,"%d//%d %d//%d %d//%d",&a,&k,&c,&d,&e,&f);
                //printf(" The F value is %d %d %d %d %d %d\n", a,k,c,d,e,f);
                indices.push_back(a-1);
                indices.push_back(c-1);
                indices.push_back(e-1);
                //indices.push_back(d);
                //indices.push_back(e);
                //indices.push_back(f);
                while (c1 != '\n')   // read through this line
                {
                    c1 = fgetc(file);
                }
            }
            
            else
            {
                while (c1 != EOF && c1 != '\n')   // read through this line if it is comment line
                {
                    c1 = fgetc(file);
                }
            }
            
        }
        
        else{
            
            break;
        }
        
    } while(1);
    
    fclose(file);
    
}

void Cube::update()
{
	spin(1.0f);
}

void Cube::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}


void Cube::setAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}
void Cube::setDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}
void Cube::setSpecular(glm::vec3 specular)
{
    this->specular = specular;
}
void Cube::setShiness(float shininess)
{
    this->shininess = shininess;
}

unsigned int Cube::loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}



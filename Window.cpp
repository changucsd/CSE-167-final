#include "Window.h"
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint shaderProgram;
GLint shaderPlane;
GLint simpleDepthShader;
GLint debugDepthQuad;

GLint Skyshader;

GLuint planeVAO,planeVBO, depthMapFBO, depthMap,skyboxVAO,skyboxVBO;
unsigned int botTexture, grassTexture,  cubemapTexture,woodTexture;

GLuint quadVAO = 0;
GLuint quadVBO = 0;

GLuint cubeVAO = 0;
GLuint cubeVBO = 0;

GLuint objVAO = 0;
GLuint objVBO = 0;

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

float lastX,lastY;
bool firstMouse;
bool leftPress, rightPress;

float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float fov   =  45.0f;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

// for shadow mapping effect
#define VERTEX_PLANE_PATH "shaderplane.vert"
#define FRAGMENT_PLANE_PATH "shaderplane.frag"

#define VERTEX_SIMPLE_PATH "shadersimple.vert"
#define FRAGMENT_SIMPLE_PATH "shadersimple.frag"

#define VERTEX_DEBUG_PATH "debugDepthQuad.vert"
#define FRAGMENT_DEBUG_PATH "debugDepthQuad.frag"

// for skybox
#define VERTEX_Sky_PATH "Skyshader.vert"
#define FRAGMENT_Sky_PATH "Skyshader.frag"



// Default camera parameters
glm::vec3 cam_pos(0.0f, 3.0f, 7.0f);		// e  | Position of camera
glm::vec3 cam_front(0.0f, -3.0f, -7.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 lightPos(3.0f, 4.0f, -1.0f);

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

std::vector<std::string> faces
{
    "siege_ft.tga",
    "siege_bk.tga",
    "siege_up.tga",
    "siege_dn.tga",
    "siege_rt.tga",
    "siege_lf.tga"

};

GLfloat skyboxVertices[] = {
    // positions
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    
    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    
    1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    
    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    
    -1000.0f,  1000.0f, -1000.0f,
    1000.0f,  1000.0f, -1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    
    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    1000.0f, -1000.0f,  1000.0f
};

void Window::initialize_objects()
{
    
	cube = new Cube("sphere.obj");

    cube->setAmbient(glm::vec3(0.0f, 0.0f, 0.8f));
    cube->setDiffuse(glm::vec3(1.0f, 0.5f, 0.8f));
    cube->setSpecular(glm::vec3(0.9f, 0.9f, 0.9f));
    cube->setShiness(32.0f);
    
    printf("finish!!!!!!\n");

    
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    shaderPlane = LoadShaders(VERTEX_PLANE_PATH, FRAGMENT_PLANE_PATH);
    simpleDepthShader = LoadShaders(VERTEX_SIMPLE_PATH, FRAGMENT_SIMPLE_PATH);
    debugDepthQuad = LoadShaders(VERTEX_DEBUG_PATH, FRAGMENT_DEBUG_PATH);
    Skyshader = LoadShaders(VERTEX_Sky_PATH, FRAGMENT_Sky_PATH);
    
   
    glEnable(GL_DEPTH_TEST);
    
    
    // --------------------
    glUseProgram(shaderPlane);
    glUniform1i(glGetUniformLocation(shaderPlane,  "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderPlane,  "shadowMap"), 1);
    //glUniform1i(glGetUniformLocation(shaderPlane,  "grassTexture"), 2);
    
    GLfloat planeVertices[] = {
        // positions            // normals         // texcoords
        25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
        
        25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
        25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
    };
    
    woodTexture = loadTexture("wood.png");
    botTexture = loadTexture("siege_dn.tga");
    grassTexture = loadTexture("grasstexture.jpg");
    cubemapTexture = loadCubemap(faces);
    
    // plane VAO
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    
    // shadow mapping
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // skybox
   
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glUseProgram(debugDepthQuad);
    glUniform1i(glGetUniformLocation(debugDepthQuad,  "depthMap"), 0);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderPlane);
    glDeleteProgram(simpleDepthShader);
    glDeleteProgram(debugDepthQuad);
    glDeleteProgram(Skyshader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
	glViewport(0, 0, width, height);

	if (height > 0)
	{
        /*
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
		V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
        */
        P = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1500.0f);
        V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
    P = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1500.0f);
    V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    /*
	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	cube->draw(shaderProgram);
    */
    
    
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0)); // light position!
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    
    glUseProgram(simpleDepthShader);
    GLint lightSpaceMatrixLocation = glGetUniformLocation(simpleDepthShader,  "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, botTexture);
        renderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
 

    // 2. render scene as normal using the generated depth/shadow map
    // --------------------------------------------------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderPlane);

    glUniformMatrix4fv(glGetUniformLocation(shaderPlane, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderPlane, "view"), 1, GL_FALSE, &Window::V[0][0]);
    //glUniform1i(glGetUniformLocation(shaderPlane,  "diffuseTexture"), 0);
    //glUniform1i(glGetUniformLocation(shaderPlane,  "shadowMap"), 1);
    
    // set light uniforms
    glUniform3fv(glGetUniformLocation(shaderPlane, "viewPos"), 1, &cam_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderPlane, "lightPos"), 1, &lightPos[0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderPlane, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    renderScene(shaderPlane);
    
    // reset viewport
    //printf("Hi hi hi %u %u \n", SCR_WIDTH, SCR_HEIGHT);
    
    //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // render Depth map to quad for visual debugging
    // ---------------------------------------------
    glUseProgram(debugDepthQuad);
    glUniform1f(glGetUniformLocation(debugDepthQuad, "near_plane"),near_plane);
    glUniform1f(glGetUniformLocation(debugDepthQuad, "far_plane"),far_plane);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    //renderQuad();
    
    
    // render skybox
    
    glDepthFunc(GL_LEQUAL);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(Skyshader);
    
    GLint projectLoc = glGetUniformLocation(Skyshader, "projection");
    GLint viewLoc  = glGetUniformLocation(Skyshader,  "view");
    GLint skybox = glGetUniformLocation(Skyshader,  "skybox");
    
    glUniformMatrix4fv(projectLoc, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &Window::V[0][0]);
    glUniform1i(skybox, 0);
    
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
    
    
	// Swap buffers
	glfwSwapBuffers(window);
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}
unsigned int Window::loadCubemap(std::vector<std::string> faces)
{
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

GLint Window::loadTexture(char const * path)
{
    GLuint textureID;
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
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    printf("Done loading textures\n");
    return textureID;
}

// renders the 3D scene
// --------------------
void Window::renderScene(GLuint shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, botTexture);
    // floor
    GLint modelLoc = glGetUniformLocation(shader, "model");
    glm::mat4 model = glm::mat4(1);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    /*
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    renderCube();
     */
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    // cubes
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    renderCube();
    
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    renderCube();
    
    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    renderCube();
    
    
}

// -------------------------------------------------
GLuint VBO2;

void Window::renderCube()
{
   
    
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        GLfloat vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    
    
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------

void Window::renderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

/*
void Window::renderObj(Cube*)
{
    
}
 */


glm::vec3 Window::trackBallmapping(float x,float y)
{
    //printf("width and height is %d %d\n", width, height);
    glm::vec3 vector;
    float d;
    vector.x = (2.0f*x - width)/width;
    vector.y = (height - 2.0f*y)/height;
    vector.z = 0.0f;
    
    d = glm::length(vector);
    if(d > 1.0f)
    {
        d = 1.0f;
    }
    
    vector.z = sqrt(1.001 - d*d);
    vector = glm::normalize(vector);
    
    return vector;
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //printf("Mouse is called");
    if(leftPress == true)
    {
        
        glfwGetCursorPos(window, &xpos, &ypos);
        
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        
        //float xoffset = xpos - lastX;
        //float yoffset = lastY - ypos;
        
        
        //glm::vec3 lastPoint = trackBallmapping(lastX, lastY);
        //glm::vec3 newPoint = trackBallmapping((float)xpos, (float)ypos);
        
        // if(lastPoint != newPoint)
        // {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        
        
        float sensitivity = 0.2f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        yaw += xoffset;
        pitch += yoffset;
        
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cam_front = glm::normalize(front);
        //}
        
        lastX = xpos;
        lastY = ypos;
    }
    
    else if (rightPress == true)
    {
        glfwGetCursorPos(window, &xpos, &ypos);
        
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        glm::vec3 lastPoint = trackBallmapping(lastX, lastY);
        glm::vec3 newPoint = trackBallmapping((float)xpos, (float)ypos);
        
        if(lastPoint != newPoint)
        {
            glm::vec3 axis = glm::cross(lastPoint,newPoint);
            float c = glm::dot(lastPoint,newPoint);
            float d = glm::acos(c);
            
            glm::vec4 newcampos = glm::rotate(glm::mat4(1.0f), d*2.0f, axis) * glm::vec4(cam_pos,1.0f);
            
            //std::cout << newcampos.x << std::endl;
            
            cam_pos.x = newcampos.x;
            cam_pos.y = newcampos.y;
            cam_pos.z = newcampos.z;
            
            
            cam_front.x = -cam_pos.x;
            cam_front.y = -cam_pos.y;
            cam_front.z = -cam_pos.z;
            
            
            lastX = xpos;
            lastY = ypos;
        }
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //printf("hahaha\n");
        leftPress = true;
        firstMouse = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        //printf("lololo\n");
        leftPress = false;
        
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rightPress = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rightPress = false;
        firstMouse = true;
    }
    
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    //if (fov >= 1.0f && fov <= 45.0f)
    if (fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 90.0f)
        fov = 90.0f;
    
}

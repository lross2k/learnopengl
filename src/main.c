#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#pragma warning(disable:4996)
#include <cglm/cglm.h>
#pragma warning(enable:4996)

#include "shader.h"
#include "stb_image.h"

void rzm_glm_vec3_make(float x, float y, float z, vec3 dest)
{
    float vec3_src[3] = {x, y, z};
    glm_vec3_make(vec3_src, dest);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, vec3 cameraPos, vec3 cameraFront, vec3 cameraUp);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // NEEDED FOR MAC OSX
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return(-1);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return(-1);
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    // ShaderProgram
    struct shader ourShader;
    shader_init(&ourShader, "./resources/shader.vs", "./resources/shader.fs");

    float vertices[] = {
         -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,
          0.5f, -0.5f, -0.5f, 1.0f,  0.0f,
          0.5f,  0.5f, -0.5f, 1.0f,  1.0f,
          0.5f,  0.5f, -0.5f, 1.0f,  1.0f,
	 -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,

         -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,
          0.5f, -0.5f,  0.5f, 1.0f,  0.0f,
          0.5f,  0.5f,  0.5f, 1.0f,  1.0f,
          0.5f,  0.5f,  0.5f, 1.0f,  1.0f,
         -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,

         -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f, 1.0f,  1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

          0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
          0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
          0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
          0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
          0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

         -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
	  0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  
	  0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load and create a texture
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("./resources/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
        return 1;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //stbi_set_flip_vertically_on_load(true);
    data = stbi_load("./resources/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
        return 1;
    }
    stbi_image_free(data);

    ourShader.Use(&ourShader);
    //glUniform1i(glGetUniformLocation(ourShader.id, "texture1"), 0); // set it manually
    ourShader.setInt(&ourShader, "texture1", 0);
    ourShader.setInt(&ourShader, "texture2", 1); // or with shader class

    vec4 vec;
    vec3 srcVec3;
    vec3 vec3Dest;
    vec3 vec3Dest1;
    vec3 vec3Dest2;
    vec4 dest;
    float angle;
    mat4 model;
    mat4 view;
    mat4 projection;
    // Camera variables
    vec3 cameraTarget;
    vec3 cameraDirection;
    vec3 up;
    vec3 cameraRight;
    float camX;
    float camZ;
    const float radius = 10.0f;

    vec3 cameraPos;
    vec3 cameraFront;
    vec3 cameraUp;
    vec3 lookAtAddition;

    float cubePositionsBase[] = {
        0.0f, 0.0f, 0.0f,
        2.0f, 5.0f, -15.0f,
        -1.5f, -2.2f, -2.5f,
        -3.8f, -2.0f, -12.3f,
        2.4f, -0.4f, -3.5f,
        -1.7f, 3.0f, -7.5f,
        1.3f, -2.0f, -2.5f,
        1.5f, 2.0f, -2.5f,
        1.5f, 0.2f, -1.5f,
        -1.3f, 1.0f, -1.5f
    };

    vec3 *cubePositions = (vec3*)malloc(sizeof(vec3) * 10);

    int i;
    for (i = 0; i < 10; i = i + 1)
    {
	rzm_glm_vec3_make(
	    cubePositionsBase[3*i], 
	    cubePositionsBase[3*i + 1], 
	    cubePositionsBase[3*i + 2], 
	    cubePositions[i]
	);
    };

    // Camera preparations
    rzm_glm_vec3_make(0.0f, 0.0f, 3.0f, cameraPos);
    rzm_glm_vec3_make(0.0f, 0.0f, -1.0f, cameraFront);
    rzm_glm_vec3_make(0.0f, 1.0f, 0.0f, cameraUp);
    //glm_lookat(vec3Dest, vec3Dest1, vec3Dest2, view);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
    	glm_vec3_add(cameraPos, cameraFront, lookAtAddition);
        glm_lookat(cameraPos, lookAtAddition, cameraUp, view);

        // input
        processInput(window, cameraPos, cameraFront, cameraUp);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.Use(&ourShader);
        //ourShader.setFloat(&ourShader, "mixStrenght", mix_strenght);

        // model matrix for translating and scaling plane
        glm_mat4_identity(model);
        rzm_glm_vec3_make(0.5f, 1.0f, 0.0f, vec3Dest);
        // convert angle in degrees to radians
        angle = -50.0f;
        glm_make_rad(&angle);
        // finally call rotate
        glm_rotate(model, (float)glfwGetTime() * angle, vec3Dest);

        // And the projection matrix
        glm_mat4_identity(projection);
        angle = 45.0f;
        glm_make_rad(&angle);
        glm_perspective(angle, 800.0f / 600.0f, 0.1f, 100.0f, projection);

        // send matrices to the shader
        // model matrix
        unsigned int modelLoc = glGetUniformLocation(ourShader.id, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
        // view matrix
        unsigned int viewLoc = glGetUniformLocation(ourShader.id, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
        // projection matrix
        unsigned int projectionLoc = glGetUniformLocation(ourShader.id, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
	for (unsigned int i = 0; i < 10; i++)
	{
            glm_mat4_identity(model);
	    glm_translate(model, cubePositions[i]);
	    angle = 20.0f * i;
            rzm_glm_vec3_make(1.0f, 0.3f, 0.5f, vec3Dest);
            glm_make_rad(&angle);
	    glm_rotate(model, angle, vec3Dest);
	    ourShader.setMat4(&ourShader, "model", (float*)model);

	    glDrawArrays(GL_TRIANGLES, 0, 36);
	}

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, vec3 cameraPos, vec3 cameraFront, vec3 cameraUp)
{
    vec3 vec3Temp;
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WIREFRAME MODE
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // FILL MODE

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
	glm_vec3_scale(cameraFront, cameraSpeed, vec3Temp);
	glm_vec3_add(cameraPos, vec3Temp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
	glm_vec3_scale(cameraFront, cameraSpeed, vec3Temp);
	glm_vec3_sub(cameraPos, vec3Temp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
	glm_vec3_cross(cameraFront, cameraUp, vec3Temp);
        glm_vec3_normalize(vec3Temp);
	glm_vec3_scale(vec3Temp, cameraSpeed, vec3Temp);
	glm_vec3_sub(cameraPos, vec3Temp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
	glm_vec3_cross(cameraFront, cameraUp, vec3Temp);
        glm_vec3_normalize(vec3Temp);
	glm_vec3_scale(vec3Temp, cameraSpeed, vec3Temp);
	glm_vec3_add(cameraPos, vec3Temp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {

	glm_vec3_scale(cameraUp, cameraSpeed, vec3Temp);
	glm_vec3_sub(cameraPos, vec3Temp, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {

	glm_vec3_scale(cameraUp, cameraSpeed, vec3Temp);
	glm_vec3_add(cameraPos, vec3Temp, cameraPos);
    }
}

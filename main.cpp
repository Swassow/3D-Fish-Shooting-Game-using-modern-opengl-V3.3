//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
float shoot_angle = 0.0f;
//initial values

float bullet_z = 0.0f;
float bull_inc = 1.0f;
float gun_x=0.0f;
float gun_y=0.0f;
float balloon_x=1.5f;
float balloon_y = 1.2f;

bool fruit = true;
float fruit_y = 0.0f;
float inc = 0.01f;

float bb_x = 0.0f;
float bb_y=0.0f;

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 10.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  1.50f,  -5.0f),
    glm::vec3(1.5f,  1.5f,  10.0f),
    glm::vec3(-1.5f,  1.5f,  -5.0f),
    glm::vec3(-1.5f,  1.5f,  10.0f),

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
int score = 3;
bool touched = false;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float speed = 0.003;

float fish_y = -9.9f;
float fish_z = -30.f;
int time_cube = 6;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();
    Sphere point = Sphere();
    point.setRadius(0.001f);
    float inc2 = 0.1f;

    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    string diffuseMapPath = "grass.jpg";
    string specularMapPath = "grass.jpg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube grass = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath2 = "game_over.png";
    string specularMapPath2 = "game_over.png";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath10 = "water1.png";
    string specularMapPath10 = "water1.png";
    unsigned int diffMap10 = loadTexture(diffuseMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap10 = loadTexture(specularMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water1 = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath10 = "water2.png";
    specularMapPath10 = "water2.png";
    diffMap10 = loadTexture(diffuseMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap10 = loadTexture(specularMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water2 = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath10 = "water3.png";
    specularMapPath10 = "water3.png";
    diffMap10 = loadTexture(diffuseMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap10 = loadTexture(specularMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water3 = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath10 = "water4.png";
    specularMapPath10 = "water4.png";
    diffMap10 = loadTexture(diffuseMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap10 = loadTexture(specularMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water4 = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(-20.0f,30.0f);
    float randomValue = dis(gen);
    float rate = -1.4f;
    float fish_x = rate * (randomValue - 20.0);

    int count = 1;
    int num = 0;
    float angle = 90.0f;
    float extra = 0.0f;
    float draw = true;
    float time = 0.0f;
    int ind = 1;
    float fishy_inc = 0.01f;
    bool legeche = false;
    int last_time = 0;

    float scale_rope = 0.0;
    //bb_x = gun_x + 0.5f;
    //bb_y = gun_y - 5.1f;

    Cylinder gun = Cylinder();
    Sphere fish = Sphere();
    float pause_time = 0.0f;
    score = 0;

   

    //camera.Position = glm::vec3(0.0, 0.0, 50.0);
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);



        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //func(cubeVAO, lightingShader, model);


        
        

        


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.5f, -5.1f, -30.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.2f, 2.2f, 0.2f));
        glm::mat4 modelMatrixForFish = model * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelMatrixForFish, 1.0f, 0.0f, 0.0f);*/


        //time cube
        int tt = currentFrame;
        if (tt!= last_time && tt % 5 == 0)
        {
            time_cube--;
            last_time = tt;
        }
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0, 10.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.0f, 3.0f, 1.0f));
        glm::mat4 modelForTime = model * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForTime, 1.0, 1.0, 1.0);

        float xx = 16.0;
        for (int i = 0; i < time_cube; i++)
        {
            if (time_cube == 1)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 10.5f, 9.9f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
                glm::mat4 modelForTime = model * translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForTime, 1.0, 0.0, 0.0);
            }
            else if (time_cube <= 3)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 10.5f, 9.9f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
                glm::mat4 modelForTime = model * translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForTime, 1.0, 1.0, 0.0);
            }
            else
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 10.5f, 9.9f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
                glm::mat4 modelForTime = model * translateMatrix * scaleMatrix;
                drawCube(cubeVAO, lightingShader, modelForTime, 0.0, 1.0, 0.0);
            }

            

            xx -= 3.05;
        }





        

        if (legeche)
        {
            //cout << "legeche " <<fish_z<< endl;
            scale_rope -= 0.5f;

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bb_x, bb_y, -62.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 3.5 + scale_rope));
            glm::mat4 modelForRope = model * translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRope, 1.0, 1.0, 1.0);
            fish_z -= 0.5f;
        }
        else
        {
            fish_x += inc;
            fish_y += fishy_inc;
        }

        if (fish_z <= -62.0)
        {
         
            legeche = false;
            fish_z = dis(gen);
            randomValue = dis(gen);
            fish_x = randomValue;
            fish_y = -14.9f;
            score++;
            fishy_inc = abs(fishy_inc) + 0.01 * score;
            inc += 0.01 * score;
            

        }
        
        
        if (fish_y >= -3.0f)
            fishy_inc *= -1.0f;

        if (fish_y <= -15.0)
        {
            fishy_inc *= -1.0f;
            fish_z = dis(gen);
            fish_x = rate * (fish_z - 20.0);
            fish_y = -14.9;

        }
            

        float trnx = 0.0f;
        float scy = 0.0f;
        for (int i = 0; i < 200; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(fish_x-10.5f-trnx, fish_y, fish_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.6f+scy, 1.0f));
            trnx += 0.01f;
            scy += 0.1f;
            glm::mat4 modelForFish = model * translateMatrix * scaleMatrix;
            fish.drawSphere(lightingShader, modelForFish);
        }
        float center_fish_x = fish_x - 10.5f - trnx;
        float region_y = -5.1f;

        

        for (int i = 0; i < 200; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(fish_x -10.5f - trnx, fish_y, fish_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.6f + scy, 1.0f));
            trnx += 0.01f;
            scy -= 0.09f;
            glm::mat4 modelForFish = model * translateMatrix * scaleMatrix;
            fish.drawSphere(lightingShader, modelForFish);
        }

        for (int i = 0; i < 100; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(fish_x - 10.5f - trnx, fish_y, fish_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.6f + scy, 1.0f));
            trnx += 0.01f;
            scy += 0.1f;
            glm::mat4 modelForFish = model * translateMatrix * scaleMatrix;
            fish.drawSphere(lightingShader, modelForFish);
        }
            


        

        //stored fish

        float stored_fy = -7.0;
        for (int sc = 0; sc < score; sc++)
        {
            float scy = 0.0f;
            trnx = 0.0f;
            for (int i = 0; i < 100; i++)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0 - trnx, stored_fy, -62.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.6f + scy, 1.0f));
                trnx += 0.01f;
                scy += 0.1f;
                glm::mat4 modelForFish = model * translateMatrix * scaleMatrix;
                fish.drawSphere(lightingShader, modelForFish);
            }
            for (int i = 0; i <100; i++)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0 - trnx, stored_fy, -62.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.6f + scy, 1.0f));
                trnx += 0.01f;
                scy -= 0.09f;
                glm::mat4 modelForFish = model * translateMatrix * scaleMatrix;
                fish.drawSphere(lightingShader, modelForFish);
            }

            for (int i = 0; i < 50; i++)
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0 - trnx, stored_fy, -62.0));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.6f + scy, 1.0f));
                trnx += 0.01f;
                scy += 0.1f;
                glm::mat4 modelForFish = model * translateMatrix * scaleMatrix;
                fish.drawSphere(lightingShader, modelForFish);
            }

            stored_fy += 1.5f;
        }



        //gun
        translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x+0.5f , gun_y-5.1f, -60.0f-2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 3.6f , 1.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 modelForGun = model * translateMatrix * rotateXMatrix * scaleMatrix;
        gun.drawCylinder(lightingShader, modelForGun, 1.0, 0.0, 0.0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.1f, gun_y - 5.8f, -61.5f - 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 2.0f));
        glm::mat4 modelForGun2 = model * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForGun2, 0.8, 0.1, 0.1);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.1f, gun_y - 5.8f, -61.5f - 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 2.0f));
        modelForGun2 = model * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForGun2, 0.8, 0.1, 0.1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x - 0.1f, gun_y - 7.0f, -62.5f - 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 1.0f));
        glm::mat4 modelForGun3 = model * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForGun3, 0.8, 0.1, 0.1);

        if (shoot)
        {
            bullet_z += 1.0f;
            scale_rope += 1.0f;
        }
            

        if (bullet_z >= 100.0f)
        {
            shoot = false;
            bullet_z = 0.0f;
            scale_rope = 0.0f;
        }


        //bullet
        if (shoot)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(bb_x, bb_y, -60.0f + bullet_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 modelForBullet = model * translateMatrix * scaleMatrix;
            fish.drawSphere(lightingShader, modelForBullet);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(bb_x, bb_y, -62.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 3.5 + scale_rope));
            glm::mat4 modelForRope = model * translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, modelForRope, 1.0, 1.0, 1.0);

        }

        //kill or not

        if (shoot)
        {
            float fx = center_fish_x, fy = fish_y, fz = fish_z;
            float bx = bb_x, by = bb_y, bz = -60.0f + bullet_z;

            float distance = sqrt((fx - bx) * (fx - bx) + (fy - by) * (fy - by) + (fz - bz) * (fz - bz));
            if (distance <= 3.0)
            {

                shoot = false;
                bullet_z = 0.0f;
                legeche = true;
                if (time_cube < 6)
                    time_cube++;
            }

        }





        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);



        //grass
        if (currentFrame - time >= 0.2)
        {
            if (ind == 4)
                ind = 1;
            else ind++;
            time = currentFrame;
        }

        if (ind == 1)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-70.5f, -10.1f, -65.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(200.0f, 0.2f, 100.0f));
            glm::mat4 modelMatrixForContainer3 = model * translateMatrix * scaleMatrix;
            water1.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        }
        else if (ind == 2)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-70.5f, -10.1f, -65.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(200.0f, 0.2f, 100.0f));
            glm::mat4 modelMatrixForContainer3 = model * translateMatrix * scaleMatrix;
            water2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        }
        else if (ind == 3)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-70.5f, -10.1f, -65.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(200.0f, 0.2f, 100.0f));
            glm::mat4 modelMatrixForContainer3 = model * translateMatrix * scaleMatrix;
            water3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        }
        else if (ind == 4)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-70.5f, -10.1f, -65.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(200.0f, 0.2f, 100.0f));
            glm::mat4 modelMatrixForContainer3 = model * translateMatrix * scaleMatrix;
            water4.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        }


        //grass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-70.5f, -10.0f, -150.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(200.0f, 0.2f, 100.0f));
        glm::mat4 modelMatrixForContainer3 = model * translateMatrix * scaleMatrix;
        grass.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);


        
        if (time_cube <= 0)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.0, -10.0f, -50.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(50.0f, 25.0f, 25.0f));
            glm::mat4 modelForGameOver = model * translateMatrix * scaleMatrix;
            cube2.drawCubeWithTexture(lightingShaderWithTexture, modelForGameOver);
        }



        //wall
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.5f, -10.1f, -20.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(40.2f, 20.2f, 0.2f));
        //glm::mat4 modelMatrixForWall = model * translateMatrix * scaleMatrix;
        //drawCube(cubeVAO, lightingShader, modelMatrixForWall, 1.0f, 1.0f, 1.0f);
        

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.1;
    float width = 5;
    float length = 10;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, 0.3);


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;


    //wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.0f, 2.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 5.0f, 0.1f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.8, 0.8, 0.5);

    float an = 90.0f + shoot_angle;
    if (shoot)
    {
        shoot_angle += inc;
        an = 90.0f + shoot_angle;
        printf("%f\n", an);
        if (an == 90.0f)
        {
            shoot = false;
            shoot_angle = 0.0f;
            inc = 1.0f;
        }

    }
     
    

    if (an >= 180.0)
        inc = - 1.0f;

    //gun
    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x, gun_y+0.05f, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.05f, 0.5f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(an), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0,1.0,0.5);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x-0.03f, gun_y+0.05, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.15f, 0.05f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(an-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translateMatrix * rotateXMatrix  * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 0.5);


    //shoot
    




}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //gun_y += 0.003f;

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //gun_y -= 0.003f;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
       // gun_x += 0.003f;
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
       // gun_x -= 0.003f;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (!shoot)
        {
            //gun_x+0.5f , gun_y-5.1f
            shoot = true;
            bb_x = gun_x + 0.5f;
            bb_y = gun_y - 5.1f;
        }
            
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        gun_y += 0.3f;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        gun_y -= 0.3f;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        gun_x += 0.3f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        gun_x -= 0.3f;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (time_cube <= 0) {
            time_cube = 6;
            score = 0;
        }
    }










    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(ROLL_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(ROLL_R, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_U, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_D, deltaTime);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
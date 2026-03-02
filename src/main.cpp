#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "TileSet.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include <imgui_impl_opengl3.h>
#include "Wfc.h"
// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 45.0f;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    glfwSetCursorPosCallback(window, mouse_callback);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW ERROR" << std::endl;
    std::cout << glGetString(GL_VERSION) << "\n";

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    {

        float positions[] = {
            -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // trás
             0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // frente
             0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // esquerda
            -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

             0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // direita
             0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // baixo
             0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

            -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // topo
             0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,       // trás
            4, 5, 6, 6, 7, 4,       // frente
            8, 9,10,10,11, 8,       // esquerda
            12,13,14,14,15,12,       // direita
            16,17,18,18,19,16,       // baixo
            20,21,22,22,23,20
        };

        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glEnable(GL_BLEND));

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);
      
        IndexBuffer ib(indices,sizeof(indices)/ sizeof(unsigned int));

        glm::mat4 proj = glm::perspective(glm::radians(75.0f), 960.0f / 540.0f, 0.1f, 100.0f);
        //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0));

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)); // Note that we convert the angle to radians first
        direction.z = sin(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));

        Shader shader("res/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.9f, 1.f);

        TileSet tiles;
        tiles.LoadFromFolder("res/textures/");
        for (const auto& text : tiles.textures)
        {
            //std::cout << text->m_Name << " " << text->m_RendererID << "\n";

        }


        Texture texture("res/textures/012_02_02_02_02_02.png");
        texture.Bind();
        //shader.SetUniform1i("u_Texture", 0);


        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        glm::vec3 translationA(-1.0f, 0, 0);
        glm::vec3 translationB(1, 0, 0);

        //float r = 0.0f;
        //float increment = 0.05f;
        
        WFC WFC(20,20,20,tiles, 0);
        Grid3D grid = WFC.getGrid();
        
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);

            renderer.Clear();
            GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // 🔹 limpar depth buffer
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            shader.Bind();

            {
                for (int i = 0; i < grid.width(); i++)
                {

                    for (int j = 0; j < grid.height(); j++)
                    {
                        for (int k = 0; k < grid.depth(); k++)
                        {
                            Wave* wave = grid.tryAt(i, j, k);

                            int tileID = wave->tileID;

                            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(
                                i * 1.f,
                                j * 1.f,
                                k * 1.f
                            ));
                            glm::mat4 mvp = proj * view * model;
                            shader.SetUniformMat4f("u_MVP", mvp);
                            tiles.textures[tileID]->Bind(0); // bind da textura correta
                            shader.SetUniform1i("u_Texture", 0);


                            renderer.Draw(va, ib, shader);
                        }
                    }
                }
            }


            {

                //glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
                //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

                //glm::mat4 mvp = proj * view; // *model;
                //shader.Bind();
                //shader.SetUniformMat4f("u_MVP", mvp);
                //shader.SetUniform4f("u_Color", r, 0.3f, 0.9f, 1.f);
                //renderer.Draw(va, ib, shader);

            }
            //{
            //    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            //    glm::mat4 mvp = proj * view * model;
            //    shader.Bind();
            //    shader.SetUniformMat4f("u_MVP", mvp);
            //    //shader.SetUniform4f("u_Color", r, 0.3f, 0.9f, 1.f);

            //    renderer.Draw(va, ib, shader);
            //}


     

            //if (r > 1.0f)
            //    increment = -0.05f;
            //else if (r < 0.0f)
            //    increment = 0.05f;

            //r += increment;


            //{
            //    static float f = 0.0f;
            //    static int counter = 0;

            //    ImGui::Begin("Debug");

            //    ImGuiIO& io = ImGui::GetIO();
            //    ImGui::SliderFloat3("Translation A", &translationA.x, -5.0f, 5.0f);
            //    ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);

            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            //    ImGui::End();

            //}
            /* Swap front and back buffers */
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    
       const float cameraSpeed = 5.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    //if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        //cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwDestroyWindow(window);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
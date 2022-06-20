#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core \n"
    "layout (location = 0) in vec3 aPos; \n"
    "void main() \n"
    "{ \n"
    "   gl_Position = vec4(aPos, 1.0); \n"
    "} \n\0";
const char *fragmentShader1Source = "#version 330 core \n"
    "out vec4 FragColor; \n"
    "void main() \n"
    "{ \n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core \n"
    "out vec4 FragColor; \n"
    "void main() \n"
    "{ \n"
    " FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f); \n"
    "} \n\0";


int main()
{
    // glfw 初始化配置
    // --
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // 创建窗口
    // --
    GLFWwindow* window  = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL -- learn", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // 初始化GLAD，glfwGetProcAddress加载系统相关的OpenGL函数指针地址
    // --
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Faile to initialize GLAD" << std::endl;
        return -1;
    }
    
    
    // 编译着色器
    // --
    // 顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);
    // 链接着色器到第一个着色程序
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    
    // 链接第二个程序
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // 顶点数据和属性
    // --
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); //可以同时生成多个VAO
    glGenBuffers(2, VBOs);
    // 设置第一个三角形
    // --
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 设置第二个三角形
    // --
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    // 解除绑定 VAO，以便其他 VAO调用不会意外修改此VAO，但这种情况很少发生。
    // 无论如何，修改其他VAO都需要调用glBindVertexArray，因此通常不会在没有直接必要时解除绑定VAO
    glBindVertexArray(0);
    // 画线
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // 绘制
    // --
    while (!glfwWindowShouldClose(window))
    {
        // 监听鼠标键盘输入
        // --
        processInput(window);
        
        // render
        // --
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgramOrange);
        // 使用VAO[0]画第一个三角形
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // 使用VAO[1]画第二个
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // 交换颜色缓冲区
        glfwSwapBuffers(window);
        // 检测输入事件
        glfwPollEvents();
    }
    
    // 释放内存
    // 释放内存属于非必须操作
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);
    
    // 终止运行
    // 释放GLFW资源
    glfwTerminate();
    return 0;
}

// 处理输入事件
// -
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// 调整窗口回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 myColor;\n"
    "void main() \n"
    "{ \n"
    "   gl_Position = vec4(aPos, 1.0); \n"
    "   myColor = aColor;\n"
    "} \n\0";

const char *fragmentShader1Source = "#version 330 core \n"
    "out vec4 FragColor; \n"
    "in vec3 myColor;\n"
    "void main() \n"
    "{ \n"
    "   FragColor = vec4(myColor, 1.0); \n"
    "}\n\0";

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
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检查编译结果
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShader);
    // 检查编译结果
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 链接着色器到第一个着色程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查结果
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 顶点数据和属性
    // --
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
        -0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); //可以同时生成多个VAO
    glGenBuffers(1, &VBO);
    // 设置第一个三角形
    // --
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // 位置信息
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色信息
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);


    // 解除绑定 VAO，以便其他 VAO调用不会意外修改此VAO，但这种情况很少发生。
    // 无论如何，修改其他VAO都需要调用glBindVertexArray，因此通常不会在没有直接必要时解除绑定VAO
    glBindVertexArray(VAO);
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
        
        // 使用glUniform前一定要激活程序
        glUseProgram(shaderProgram);
        
        // 更新uniform值
        // double time = glfwGetTime();
        //float greenValue = static_cast<float>(sin(time) / 2.0 + 0.5);
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor");
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        // 画一个三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // 交换颜色缓冲区
        glfwSwapBuffers(window);
        // 检测输入事件
        glfwPollEvents();
    }
    
    // 释放内存
    // 释放内存属于非必须操作
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
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

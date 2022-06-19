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
const char *fragmentShaderSource = "#version 330 core \n"
    "out vec4 FragColor; \n"
    "void main() \n"
    "{ \n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
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
    // 是否编译报错
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPLIE_FAILED \n" << infoLog << std::endl;
    }
    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 编译结果
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT::COMPILATION_FAILD \n" << infoLog << std::endl;
    }
    // 链接着色器到着色程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 链接结果
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED \n" << infoLog << std::endl;
    }
    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 顶点数据和属性
    // --
    float vertices[] = {
        0.5f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
       -0.5f, -0.5f,  0.0f,
       -0.5f,  0.5f,  0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定顶点数组对象，然后绑定顶点缓冲区，设置顶点属性
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // 解绑VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // 当 VAO 处于活动状态时，不要解除绑定 EBO，因为绑定元素缓冲区对象存储在 VAO 中;需要保持 EBO 绑定
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // 解除绑定 VAO，以便其他 VAO调用不会意外修改此VAO，但这种情况很少发生。
    // 无论如何，修改其他VAO都需要调用glBindVertexArray，因此通常不会在没有直接必要时解除绑定VAO
    glBindVertexArray(0);
    
    
    // 画线
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
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
        
        // 第一个三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);// 非必须，不需要每次都解绑
        
        // 交换颜色缓冲区
        glfwSwapBuffers(window);
        // 检测输入事件
        glfwPollEvents();
    }
    
    // 释放内存
    // 释放内存属于非必须操作
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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

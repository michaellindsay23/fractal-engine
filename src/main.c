#include "../include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>

const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec2 pos;\n"
"uniform float N;\n"
"uniform float R;\n"
"\n"
"const float PI = 3.14159265359;"
"\n"
"out vec2 outPos;\n"
"\n"
"float rand(vec2 co) {\n"
"   return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);\n"
"}\n"
"\n"
"void main() {\n"
"   int target = int(rand(pos + gl_VertexID) * float(N));\n"
"\n"
"   float angle = (float(target) * 2.0 * PI) / float(N);\n"
"   vec2 vertexPos = vec2(cos(angle), sin(angle)) * 0.8;\n"
"\n"
"   outPos = pos + ((vertexPos - pos) * R);\n"
"   gl_Position = vec4(outPos, 0.0, 1.0);\n"
"   gl_PointSize = 1.0;\n"
" }\n\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
//"in vec3 color;\n"
"void main() {\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  glfwInit();
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "fractal engine", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    return -1;
  }

  //Shaders
  int success;
  char infoLog[512];

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    printf("%s\n", infoLog);
  };

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
  };

  // Shader object
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  const char* varyings[] = { "outPos" };
  glTransformFeedbackVaryings(shaderProgram, 1, varyings, GL_INTERLEAVED_ATTRIBS);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // setup
  glViewport(0, 0, 800, 800);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // Vertex objects
  const int NUM_POINTS = 100000;
  float points[NUM_POINTS * 2];
  for (int i = 0; i < NUM_POINTS * 2; i++) {
    points[i] = 0.0f;
  }

  float PI = 3.14159265359;

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);


  // Uniforms
  float N_loc = glGetUniformLocation(shaderProgram, "N");
  float R_loc = glGetUniformLocation(shaderProgram, "R");

  float N_sides = 1.0f;

  // main loop
  int readIdx = 0;
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO);

    float R_factor = 1 / (1 + (sin(PI/N_sides)/(2 * sin((PI/N_sides)*(floor((float)N_sides/4))))));

    glUniform1f(N_loc, N_sides);
    glUniform1f(R_loc, R_factor);

    glUseProgram(shaderProgram);

    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, NUM_POINTS);
    glEndTransformFeedback();

    glfwSwapBuffers(window);
    glfwPollEvents();

    N_sides += 0.1f;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

//回调函数:每当窗口大小被调整的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//glViewport(左下角x，左下角y，宽，高)
}

//处理输入:按下ESC键，关闭窗口
void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core模式

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	//glfwCreateWindow(宽，高，标题，monitor，share)
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//设置回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化GLAD
	//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//渲染循环:一直运行，直到用户关闭窗口
	while (!glfwWindowShouldClose(window)) {
		//输入
		processInput(window);
		
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓冲

		//交换缓冲区和轮询IO事件
		glfwSwapBuffers(window); //交换颜色缓冲：在显示器（即缓冲区）中显示
		glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
	}

	//释放/删除之前的分配的所有资源
	glfwTerminate();

	return 0;
} 


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <iostream>
using namespace std;

float mixValue = 0.2f;

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.001f;
		if (mixValue > 1.0f) mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.001f;
		if (mixValue < 0.0f) mixValue = 0.0f;
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


	//定义并编译着色器项目
	Shader myShader("shader.vs", "shader.fs");

	//定义顶点数据
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	unsigned int indices[] = {  // 注意索引从0开始!
		0, 1, 3,   // 第一个三角形
		1, 2, 3    // 第二个三角形
	};
	float texCoords[] = {
		0.0f, 0.0f, // 左下角
		1.0f, 0.0f, // 右下角
		0.5f, 1.0f  // 上中
	};

	//生成缓冲对象
	unsigned int VBO; //顶点缓冲对象(Vertex Buffer Object)：在显存中存顶点数据
	unsigned int VAO; //顶点数组对象(Vertex Array Object)：记录顶点属性如何从 VBO 读取，不存数据本身
	unsigned int EBO; //索引缓冲对象(Element Buffer Object)：在显存中存索引数据
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);//生成一个缓冲ID
	glGenBuffers(1, &EBO);//生成一个索引缓冲ID
	//参数：生成几个对象，生成的ID写到哪个数组里

	//1.绑定VAO
	glBindVertexArray(VAO); //使后面的顶点属性配置和缓冲绑定都储存在这个VAO中

	//2.复制 顶点数组 和 索引数据 到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //绑定缓冲对象
	//把VBO绑定到GL_ARRAY_BUFFER目标上，GL_ARRAY_BUFFER这个缓冲区就是用来存储顶点数据的
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //把顶点数据复制到缓冲中
	//显卡如何管理给定的数据：GL_STATIC_DRAW:数据不会或几乎不会改变、
	// GL_DYNAMIC_DRAW:数据会被改变很多次、 GL_STREAM_DRAW:数据每次绘制时都会改变
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //绑定索引缓冲对象
	//把EBO绑定到GL_ELEMENT_ARRAY_BUFFER目标上，GL_ELEMENT_ARRAY_BUFFER这个缓冲区就是用来存储索引数据的
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //把索引数据复制到缓冲中
	//EBO是绑在VAO上的，VBO通过glVertexAttribPointer函数设置顶点属性指针，VAO记录VBO的绑定状态

	//3.设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//参数说明：属性位置、每个属性的组件数量、数据类型、是否标准化、步长、偏移量
	glEnableVertexAttribArray(0); //启用顶点属性指针
	glEnableVertexAttribArray(1); //启用颜色属性指针
	glEnableVertexAttribArray(2); //启用纹理坐标属性指针
	//VAO会记录VBO的绑定状态和顶点属性指针的配置，所以只要绑定了VAO，就不需要再绑定VBO和设置顶点属性指针了
	glBindVertexArray(0);//解绑VAO：防止后续操作意外修改了VAO的配置

	//创建纹理
	unsigned int texture1,texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//纹理过滤
	// 加载并生成纹理
	int width, height, nrChannels;//颜色通道个数
	stbi_set_flip_vertically_on_load(true);//翻转y轴：openGL要求y轴0.0在图片底部，但是图片的0.0通常在顶部
	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//生成纹理，参数：target，mipmap层级，纹理存储成格式，宽，高，0，纹理是何种格式，数据类型，图像数据
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);//释放内存

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//纹理过滤
	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//生成纹理，参数：target，mipmap层级，纹理存储成格式，宽，高，0，纹理是何种格式，数据类型，图像数据
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);

	myShader.use(); //uniform变量之前激活着色器程序
	//告诉着色器去哪个texture unit 取纹理
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);//手动设置
	myShader.setInt("texture2", 1); //或着色器类设置
	myShader.setFloat("mixValue", mixValue);


	//渲染循环:一直运行，直到用户关闭窗口
	while (!glfwWindowShouldClose(window)) {
		//输入
		processInput(window);
		
		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓冲

		//在对应的纹理单元（即sampler）绑定纹理
		glActiveTexture(GL_TEXTURE0); //激活纹理单元0
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		myShader.setFloat("mixValue", mixValue);

		//变换
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));//平移
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//旋转
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));//放缩
		
		//4.调用着色器程序对象
		myShader.use(); //使用着色器程序对象
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		//变量：uniform, 矩阵数量，是否转置，矩阵数据

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //绘制两个三角形
		////参数：图元类型、索引数量、索引类型、索引偏移量

		trans = glm::mat2(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));//平移		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //绘制两个三角形
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //绘制两个三角形


		//交换缓冲区和轮询IO事件
		glfwSwapBuffers(window); //交换颜色缓冲：在显示器（即缓冲区）中显示
		glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
	}

	//释放/删除之前的分配的所有资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(myShader.ID);

	glfwTerminate();

	return 0;
} 


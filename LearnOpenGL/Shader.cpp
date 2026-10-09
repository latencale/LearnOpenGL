#include "Shader.h"

 // 构造器读取并构建着色器
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //std::ifstream是 文件输入流
    //有四种状态位：goodbit / eofbit / failbit / badbit
    //        一切正常 / 到达文件末尾 / 操作失败 / 严重错误
       
    // 保证ifstream对象可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    //failbit或badbit时，抛出异常

	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读写字符串：向文件流一样使用<< 和 >>

		//读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//vShaderFile.rdbuf() 拿到文件的缓冲区
		//vShaderStream << vShaderFile.rdbuf() 把整个内容读进 stringstream

		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();

		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	//取出C风格字符串的指针


	// 2. 编译着色器
   	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER); //创建顶点着色器对象
	glShaderSource(vertex, 1, &vShaderCode, NULL); //把着色器源码附加到着色器对象上
	//参数：着色器对象，字符串数量，指向字符串指针的指针，长度
	glCompileShader(vertex); //编译顶点着色器
	//打印编译错误
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); //检查编译是否成功
	//参数：着色器对象，要查询什么pname，输出参数params
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog); //获取编译错误信息
		//着色器，日志缓冲区大小，写入日志的长度，接收日志的字符数组
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//打印编译错误
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// 打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

    // 使用/激活程序
void Shader::use() {
    glUseProgram(ID);
}

// uniform工具函数
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string & name, glm::mat4 mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

#include "GLTestWidget.hh"

#include <asterales/brassica.hh>
namespace b = asterales::brassica;
using mat4_t = b::mat4_t<GLfloat>;
using quat_t = b::quat_t<GLfloat>;

static constexpr GLfloat equilateral_triangle [] = {
	0, 1,
	-0.866025, -0.5,
	0.866025, -0.5
};

static constexpr char const * test_v_src = R"(
	
#version 450 core
layout(location = 0) in vec2 pos;
layout(location = 0) uniform mat4 mvp;
void main() {
	gl_Position = mvp * vec4(pos, 0, 1);
};
	
)";

static constexpr char const * test_f_src = R"(
	
#version 450 core
layout(location = 1) uniform vec3 in_color;
out vec4 frag_color;
void main() {
	frag_color = vec4(in_color, 1);
};

)";

struct GLTestWidget::Trunk {
	std::chrono::high_resolution_clock::time_point start;
	std::array<float, 3> clearColor {1.0, 0.5, 0.0};
	std::array<float, 3> triColor {0.0, 0.5, 1.0};
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint program = 0;
	mat4_t VP;
};

GLTestWidget::GLTestWidget(QWidget * parent) : QOpenGLWidget(parent), QOpenGLFunctions_4_5_Core(), trunk { new Trunk } {
	QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
	fmt.setMajorVersion(4);
	fmt.setMinorVersion(5);
	fmt.setSwapInterval(1);
	this->setFormat(fmt);
	
	this->setMinimumSize(600, 400);
	
	static constexpr double FPS = 60;
	
	frameMaster = new QTimer {this};
	frameMaster->callOnTimeout([this](){this->update();});
	frameMaster->setInterval( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1/FPS)) );
	frameMaster->start();
	
	trunk->start = std::chrono::high_resolution_clock::now();
}

GLTestWidget::~GLTestWidget() {
	makeCurrent();
	if (trunk->VAO) glDeleteVertexArrays(1, &trunk->VAO);
	if (trunk->VBO) glDeleteBuffers(1, &trunk->VBO);
}

void GLTestWidget::initializeGL() {
	initializeOpenGLFunctions();
	
	glCreateVertexArrays(1, &trunk->VAO);
	glCreateBuffers(1, &trunk->VBO);
	
	glNamedBufferData(trunk->VBO, sizeof(equilateral_triangle), equilateral_triangle, GL_STATIC_DRAW);
	
	glVertexArrayAttribBinding(trunk->VAO, 0, 0);
	glVertexArrayVertexBuffer(trunk->VAO, 0, trunk->VBO, 0, 2 * sizeof(GLfloat));
	glEnableVertexArrayAttrib(trunk->VAO, 0);
	glVertexArrayAttribFormat(trunk->VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
	
	GLuint shad_v = glCreateShader(GL_VERTEX_SHADER);
	GLuint shad_f = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(shad_v, 1, &test_v_src, nullptr);
	glShaderSource(shad_f, 1, &test_f_src, nullptr);
	
	glCompileShader(shad_v);
	glCompileShader(shad_f);
	
	trunk->program = glCreateProgram();
	glAttachShader(trunk->program, shad_v);
	glAttachShader(trunk->program, shad_f);
	
	glLinkProgram(trunk->program);
	
	glDeleteShader(shad_v);
	glDeleteShader(shad_f);
}

void GLTestWidget::resizeGL(int w, int h) {
	glScissor(0, 0, w, h);
	glViewport(0, 0, w, h);
	
	float wf = w, hf = h;
	trunk->VP = mat4_t::ortho(hf/2, -hf/2, -wf/2, wf/2, -1, 1);
}

void GLTestWidget::paintGL() {
	glClearColor(trunk->clearColor[0], trunk->clearColor[1], trunk->clearColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	auto dur = std::chrono::high_resolution_clock::now() - trunk->start;
	
	quat_t rot {{0, 0, 1}, std::chrono::duration_cast<std::chrono::duration<float>>(dur).count()};
	mat4_t m = mat4_t::scale(120) * mat4_t(rot);
	
	glUseProgram(trunk->program);
	glUniform3f(1, trunk->triColor[0], trunk->triColor[1], trunk->triColor[2]);
	glUniformMatrix4fv(0, 1, GL_FALSE, m * trunk->VP);
	
	glBindVertexArray(trunk->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

#include <random>

void GLTestWidget::randomizeColor() {
	static std::mt19937_64 rng { std::default_random_engine {} () };
	static std::uniform_real_distribution<float> dist {0.0, 1.0};
	trunk->clearColor = { dist(rng), dist(rng), dist(rng) };
	trunk->triColor = { dist(rng), dist(rng), dist(rng) };
	update();
}

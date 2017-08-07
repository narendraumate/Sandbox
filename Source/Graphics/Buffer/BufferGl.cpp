//
//  BufferGl.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "BufferGl.h"

#include "Image.h"

#include "Platform.h"

#include "glad/glad.h"

#define RENDER_TEXTURE 1 // 0 and 1 both work. 1 is to be used for multi viewport blit.

VertexBufferGl::VertexBufferGl(const std::vector<P2T2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_buffer);
	initialize();
}

VertexBufferGl::VertexBufferGl(const std::vector<P4N4T4B4T2P2>& vertices)
: VertexBuffer(vertices)
, m_vertexArray(0)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	glGenVertexArrays(1, &m_vertexArray);
	glGenBuffers(1, &m_buffer);
	initialize();
}

VertexBufferGl::~VertexBufferGl() {
	deinitialize();
	glDeleteBuffers(1, &m_buffer);
	glDeleteVertexArrays(1, &m_vertexArray);
}

void VertexBufferGl::bind() {
	glBindVertexArray(m_vertexArray);
	////glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
}

void VertexBufferGl::unbind() {
	////glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void VertexBufferGl::initialize() {
	if (m_vertexFormat == Format::FormatP2T2) {
		m_count = m_vertexCount * sizeof(P2T2) / sizeof(float);
	} else if (m_vertexFormat == Format::FormatP4N4T4B4T2P2) {
		m_count = m_vertexCount * sizeof(P4N4T4B4T2P2) / sizeof(float);
	}
	m_size = m_count * sizeof(float);
	//
	glBindVertexArray(m_vertexArray);
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
	//
	float* m_vertexDataIn = m_vertices;
	float* m_vertexDataOut = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
	std::memcpy(m_vertexDataOut, m_vertexDataIn, m_size);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	//
	if (m_vertexFormat == Format::FormatP2T2) {
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(P2T2), BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(P2T2), BUFFER_OFFSET(sizeof(float) * 2));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	} else if (m_vertexFormat == Format::FormatP4N4T4B4T2P2) {
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(P4N4T4B4T2P2), BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(P4N4T4B4T2P2), BUFFER_OFFSET(sizeof(float) * 4));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(P4N4T4B4T2P2), BUFFER_OFFSET(sizeof(float) * 8));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(P4N4T4B4T2P2), BUFFER_OFFSET(sizeof(float) * 12));
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(P4N4T4B4T2P2), BUFFER_OFFSET(sizeof(float) * 16));
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(P4N4T4B4T2P2), BUFFER_OFFSET(sizeof(float) * 18));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	glBindVertexArray(0);
}

void VertexBufferGl::deinitialize() {
	m_count = 0;
	m_size = 0;
	//
	glBindVertexArray(m_vertexArray);
	//
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
	//
	if (m_vertexFormat == Format::FormatP2T2) {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	} else if (m_vertexFormat == Format::FormatP4N4T4B4T2P2) {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	glBindVertexArray(0);
}

IndexBufferGl::IndexBufferGl(const Type& type, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& counts)
: IndexBuffer(type, indices, counts)
, m_buffer(0)
, m_count(0)
, m_size(0) {
	glGenBuffers(1, &m_buffer);
	initialize();
}

IndexBufferGl::~IndexBufferGl() {
	deinitialize();
	glDeleteBuffers(1, &m_buffer);
}

void IndexBufferGl::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
}

void IndexBufferGl::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferGl::initialize() {
	m_count = m_indices.size();
	m_size = m_count * sizeof(unsigned int);
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
	//
	unsigned int* m_indexDataIn = &m_indices[0];
	unsigned int* m_indexDataOut = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE));
	std::memcpy(m_indexDataOut, m_indexDataIn, m_size);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//
}

void IndexBufferGl::deinitialize() {
	m_count = 0;
	m_size = 0;
	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//
}

VisualEffectGl::VisualEffectGl(const Effect& effect)
: VisualEffect(effect) {
	std::string vertexShaderPath = getShaderDirectory() + "/" + effect.vertexShader + "Gl.vert";
	std::string fragmentShaderPath = getShaderDirectory() + "/" + effect.fragmentShader + "Gl.frag";
	if (fileExists(vertexShaderPath) && fileExists(fragmentShaderPath)) {
		auto loadShaderLambda = [](const std::string& shaderFilePath, const GLenum& shaderType) {
			if (fileExists(shaderFilePath)) {
				std::string shaderSString;
				fileReadIntoString(shaderFilePath, shaderSString);
				const char* shaderCString = shaderSString.c_str();
				GLuint shader = glCreateShader(shaderType);
				glShaderSource(shader, 1, const_cast<const GLchar**>(&shaderCString), nullptr);
				glCompileShader(shader);
				GLint status = GL_FALSE;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE) {
					GLint logLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
					GLchar* logString = new GLchar[logLength + 1];
					glGetProgramInfoLog(shader, logLength, &logLength, logString);
					fprintf(stderr, "Compile Error: %s\n%s", shaderFilePath.c_str(), logString);
					delete[] logString;
					return static_cast<GLuint>(0);
				} else {
					return shader;
				}
			} else {
				fprintf(stderr, "File Missing: %s\n", shaderFilePath.c_str());
				return static_cast<GLuint>(0);
			}
		};
		GLuint vertexShaderIndex = loadShaderLambda(vertexShaderPath, GL_VERTEX_SHADER);
		GLuint fragmentShaderIndex = loadShaderLambda(fragmentShaderPath, GL_FRAGMENT_SHADER);
		m_program = glCreateProgram();
		glAttachShader(m_program, vertexShaderIndex);
		glAttachShader(m_program, fragmentShaderIndex);
		glLinkProgram(m_program);
		GLint status = GL_FALSE;
		glGetProgramiv(m_program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint logLength = 0;
			glGetShaderiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* logString = new GLchar[logLength + 1];
			glGetProgramInfoLog(m_program, logLength, &logLength, logString);
			std::cout << "Link Error: " << logString;
			delete[] logString;
		}
		glDetachShader(m_program, vertexShaderIndex);
		glDeleteShader(vertexShaderIndex);
		glDetachShader(m_program, fragmentShaderIndex);
		glDeleteShader(fragmentShaderIndex);
	} else {
		if (!fileExists(vertexShaderPath)) {
			std::cout << "File Missing: " + vertexShaderPath + "\n";
		}
		if (!fileExists(fragmentShaderPath)) {
			std::cout << "File Missing: " + fragmentShaderPath + "\n";
		}
	}
}

VisualEffectGl::~VisualEffectGl() {
	glDeleteProgram(m_program);
}

void VisualEffectGl::set1iv(const char* name, unsigned int count, const int* v0) {
	glUniform1iv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set1fv(const char* name, unsigned int count, const float* v0) {
	glUniform1fv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set2iv(const char* name, unsigned int count, const int* v0) {
	glUniform2iv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set2fv(const char* name, unsigned int count, const float* v0) {
	glUniform2fv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set3iv(const char* name, unsigned int count, const int* v0) {
	glUniform3iv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set3fv(const char* name, unsigned int count, const float* v0) {
	glUniform3fv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set4iv(const char* name, unsigned int count, const int* v0) {
	glUniform4iv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::set4fv(const char* name, unsigned int count, const float* v0) {
	glUniform4fv(glGetUniformLocation(m_program, name), count, v0);
}

void VisualEffectGl::setMatrix3fv(const char* name, unsigned int count, bool transpose, const float* v0) {
	glUniformMatrix3fv(glGetUniformLocation(m_program, name), count, transpose, v0);
}

void VisualEffectGl::setMatrix4fv(const char* name, unsigned int count, bool transpose, const float* v0) {
	glUniformMatrix4fv(glGetUniformLocation(m_program, name), count, transpose, v0);
}

void VisualEffectGl::set1iv(int location, unsigned int count, const int* v0) {
	glUniform1iv(location, count, v0);
}

void VisualEffectGl::set1fv(int location, unsigned int count, const float* v0) {
	glUniform1fv(location, count, v0);
}

void VisualEffectGl::set2iv(int location, unsigned int count, const int* v0) {
	glUniform2iv(location, count, v0);
}

void VisualEffectGl::set2fv(int location, unsigned int count, const float* v0) {
	glUniform2fv(location, count, v0);
}

void VisualEffectGl::set3iv(int location, unsigned int count, const int* v0) {
	glUniform3iv(location, count, v0);
}

void VisualEffectGl::set3fv(int location, unsigned int count, const float* v0) {
	glUniform3fv(location, count, v0);
}

void VisualEffectGl::set4iv(int location, unsigned int count, const int* v0) {
	glUniform4iv(location, count, v0);
}

void VisualEffectGl::set4fv(int location, unsigned int count, const float* v0) {
	glUniform4fv(location, count, v0);
}

void VisualEffectGl::setMatrix3fv(int location, unsigned int count, bool transpose, const float* v0) {
	glUniformMatrix3fv(location, count, transpose, v0);
}

void VisualEffectGl::setMatrix4fv(int location, unsigned int count, bool transpose, const float* v0) {
	glUniformMatrix4fv(location, count, transpose, v0);
}

void VisualEffectGl::setSampler(const char* name, int location, const unsigned int& texture) {
	static bool bound = false;
	if (!bound) {
		GLint uniformLocation = glGetUniformLocation(m_program, name);
		if (uniformLocation != -1) {
			bound = true;
			glUniform1i(uniformLocation, location);
			glActiveTexture(GL_TEXTURE0 + location);
			glBindTexture(GL_TEXTURE_2D, texture);
		} else {
			std::cout << "VisualEffectGl::setSampler" << std::endl;
		}
	} else {
		bound = false;
		glActiveTexture(GL_TEXTURE0 + location);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

FramebufferGl::FramebufferGl(const int& width, const int& height, const Framebuffer::Descriptor& descriptor)
: Framebuffer(width, height, descriptor) {
	// hack disable stencil for now
	m_descriptor.stencil.clear();

	m_framebuffer = new unsigned int(1);
	m_colorTexture = new unsigned int(m_descriptor.color.size());
	m_depthTexture = new unsigned int(m_descriptor.depth.size());
	m_stencilTexture = new unsigned int(m_descriptor.stencil.size());

	// frame buffer
	glGenFramebuffers(1, (unsigned int*)m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *(unsigned int*)m_framebuffer);

	// color
	for (int i = 0; i < m_descriptor.color.size(); ++i) {
#if RENDER_TEXTURE
		// render texture
		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		if (m_descriptor.color[i] == Framebuffer::Format::Color) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		} else if (m_descriptor.color[i] == Framebuffer::Format::Depth) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		// color render texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texId, 0);
		((unsigned int*)m_colorTexture)[i] = texId;
#else
		// render buffer
		GLuint rboId;
		glGenRenderbuffers(1, &rboId);
		glBindRenderbuffer(GL_RENDERBUFFER, rboId);
		if (m_descriptor.color[i] == Framebuffer::Format::Color) {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		} else if (m_descriptor.color[i] == Framebuffer::Format::Depth) {
			glRenderbufferStorage(GL_RENDERBUFFER, GL_R32F, width, height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// color render buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rboId);
		((unsigned int*)m_colorTexture)[i] = rboId;
#endif
	}

	// depth
	if (m_descriptor.depth.size()) {
#if RENDER_TEXTURE
		// render texture
		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		// color render texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
		((unsigned int*)m_depthTexture)[0] = texId;
#else
		// render buffer
		GLuint rboId;
		glGenRenderbuffers(1, &rboId);
		glBindRenderbuffer(GL_RENDERBUFFER, rboId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// depth render buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
		((unsigned int*)m_depthTexture)[0] = rboId;
#endif
	}

	// stencil
	if (m_descriptor.stencil.size()) {
#if RENDER_TEXTURE
		// render texture
		GLuint texId;
		glGenTextures(1, &texId); //??
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		// stencil render texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
		((unsigned int*)m_stencilTexture)[0] = texId;
#else
		// render buffer
		GLuint rboId;
		glGenRenderbuffers(1, &rboId);
		glBindRenderbuffer(GL_RENDERBUFFER, rboId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// stencil render buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);
		((unsigned int*)m_stencilTexture)[0] = rboId;
#endif
	}

#if 1
	// This can be removed but leaving here for debugging multiple attachments which hasn't been tested yet
	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
			break;

		case GL_FRAMEBUFFER_COMPLETE:
			break;

		default:
			break;
	}
#endif // 0
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FramebufferGl::~FramebufferGl() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, (unsigned int*)m_framebuffer);

	if (m_descriptor.color.size()) {
#if RENDER_TEXTURE
		glDeleteTextures(m_descriptor.color.size(), (unsigned int*)m_colorTexture);
#else
		glDeleteRenderbuffers(m_descriptor.color.size(), (unsigned int*)m_colorTexture);
#endif
	}

	if (m_descriptor.stencil.size() == 0) {
#if RENDER_TEXTURE
		glDeleteTextures(m_descriptor.depth.size(), (unsigned int*)m_depthTexture);
#else
		glDeleteRenderbuffers(m_descriptor.depth.size(), (unsigned int*)m_depthTexture);
#endif
	} else {
#if RENDER_TEXTURE
		glDeleteTextures(m_descriptor.stencil.size(), (unsigned int*)m_stencilTexture);
#else
		glDeleteRenderbuffers(m_descriptor.stencil.size(), (unsigned int*)m_stencilTexture);
#endif
	}

	m_descriptor.color.clear();
	m_descriptor.depth.clear();
	m_descriptor.stencil.clear();

	delete[] (unsigned int*)m_framebuffer;
	delete[] (unsigned int*)m_colorTexture;
	delete[] (unsigned int*)m_depthTexture;
	delete[] (unsigned int*)m_stencilTexture;
}

void FramebufferGl::bind(const Target& target) {
	if (target == Target::Draw) {
		GLenum result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (0 != result) {
			switch (result) {
				case GL_FRAMEBUFFER_COMPLETE: {
					// Special condition below 3 lines.
					GLsizei n = 2;
					const GLenum bufs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
					glDrawBuffers(n, bufs);
					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *(unsigned int*)m_framebuffer);
				} break;
			}
		}
	} else if (target == Target::Read) {
		GLenum result = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
		if (0 != result) {
			switch (result) {
				case GL_FRAMEBUFFER_COMPLETE: {
					glBindFramebuffer(GL_READ_FRAMEBUFFER, *(unsigned int*)m_framebuffer);
				} break;
			}
		}
	} else {
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (0 != result) {
			switch (result) {
				case GL_FRAMEBUFFER_COMPLETE: {
					// Special condition below 3 lines.
					GLsizei n = 2;
					const GLenum bufs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
					glDrawBuffers(n, bufs);
					glBindFramebuffer(GL_FRAMEBUFFER, *(unsigned int*)m_framebuffer);
				} break;
			}
		}
	}
}

void FramebufferGl::unbind(const Target& target) {
	if (target == Target::Draw) {
		GLenum result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (0 != result) {
			switch (result) {
				case GL_FRAMEBUFFER_COMPLETE: {
					// Special condition below 3 lines.
					GLsizei n = 1;
					const GLenum bufs[1] = { GL_NONE };
					glDrawBuffers(n, bufs);
					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
				} break;
			}
		}
	} else if (target == Target::Read) {
		GLenum result = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
		if (0 != result) {
			switch (result) {
				case GL_FRAMEBUFFER_COMPLETE: {
					glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
				} break;
			}
		}
	} else {
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (0 != result) {
			switch (result) {
				case GL_FRAMEBUFFER_COMPLETE: {
					// Special condition below 3 lines.
					GLsizei n = 1;
					const GLenum bufs[1] = { GL_NONE };
					glDrawBuffers(n, bufs);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				} break;
			}
		}
	}
}

std::unordered_map<std::string, Texture2DGl::Container> Texture2DGl::s_map;

Texture2DGl::Texture2DGl(const std::string& type, const std::string& filepath)
: Texture2D() {
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	int sizeX = 0;
	int sizeY = 0;
	unsigned char* pixels = nullptr;

#if DEBUG_TEXTURE
	sizeX = 512;
	sizeY = 512;
	int channels = 4;
	int dimensions = sizeX * sizeY;
	pixels = new unsigned char[channels * dimensions];

	if ("ambient" == type) {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4 + 0] = 0xFF;
			pixels[i * 4 + 1] = 0x00;
			pixels[i * 4 + 2] = 0x00;
			pixels[i * 4 + 3] = 0xFF;
		}
	} else if ("diffuse" == type) {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4 + 0] = 0x00;
			pixels[i * 4 + 1] = 0xFF;
			pixels[i * 4 + 2] = 0x00;
			pixels[i * 4 + 3] = 0xFF;
		}
	} else if ("alpha" == type) {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4 + 0] = 0x00;
			pixels[i * 4 + 1] = 0x00;
			pixels[i * 4 + 2] = 0xFF;
			pixels[i * 4 + 3] = 0xFF;
		}
	} else {
		for (int i = 0; i < dimensions; ++i) {
			pixels[i * 4 + 0] = 0xFF;
			pixels[i * 4 + 1] = 0xFF;
			pixels[i * 4 + 2] = 0xFF;
			pixels[i * 4 + 3] = 0xFF;
		}
	}
#else
	load(filepath, sizeX, sizeY, pixels);
#endif

#if 0
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#else
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, sizeX, sizeY);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sizeX, sizeY, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif

#if DEBUG_TEXTURE
	delete[] pixels;
#else
	unload(pixels);
#endif

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2DGl::~Texture2DGl() {
	glDeleteTextures(1, &m_texture);
}

void Texture2DGl::bind() {
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture2DGl::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2DGl* Texture2DGl::allocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 0) {
		s_map[key].count = 1;
		s_map[key].pointer = new Texture2DGl(type, filepath);
		return s_map[key].pointer;
	} else {
		s_map[key].count += 1;
		return s_map[key].pointer;
	}
}

void Texture2DGl::deallocate(const std::string& type, const std::string& filepath) {
	std::string key = type + ":" + filepath;
	if (s_map[key].count == 1) {
		s_map[key].count = 0;
		delete s_map[key].pointer;
		s_map[key].pointer = nullptr;
	} else {
		s_map[key].count -= 1;
	}
}

VisualMaterialGl::VisualMaterialGl(const Material& material)
: VisualMaterial(material) {
	if (!material.ambient_texname.empty()) {
		ambient_texture = Texture2DGl::allocate(std::string("ambient"), material.ambient_texname);
	}
	if (!material.diffuse_texname.empty()) {
		diffuse_texture = Texture2DGl::allocate(std::string("diffuse"), material.diffuse_texname);
	}
	if (!material.specular_texname.empty()) {
		specular_texture = Texture2DGl::allocate(std::string("specular"), material.specular_texname);
	}
	if (!material.specular_highlight_texname.empty()) {
		specular_highlight_texture = Texture2DGl::allocate(std::string("specular_highlight"), material.specular_highlight_texname);
	}
	if (!material.bump_texname.empty()) {
		bump_texture = Texture2DGl::allocate(std::string("bump"), material.bump_texname);
	}
	if (!material.displacement_texname.empty()) {
		displacement_texture = Texture2DGl::allocate(std::string("displacement"), material.displacement_texname);
	}
	if (!material.alpha_texname.empty()) {
		alpha_texture = Texture2DGl::allocate(std::string("alpha"), material.alpha_texname);
	}
}

VisualMaterialGl::~VisualMaterialGl() {
	if (ambient_texture) {
		Texture2DGl::deallocate(std::string("ambient"), ambient_texname);
	}
	if (diffuse_texture) {
		Texture2DGl::deallocate(std::string("diffuse"), diffuse_texname);
	}
	if (specular_texture) {
		Texture2DGl::deallocate(std::string("specular"), specular_texname);
	}
	if (specular_highlight_texture) {
		Texture2DGl::deallocate(std::string("specular_highlight"), specular_highlight_texname);
	}
	if (bump_texture) {
		Texture2DGl::deallocate(std::string("bump"), bump_texname);
	}
	if (displacement_texture) {
		Texture2DGl::deallocate(std::string("displacement"), displacement_texname);
	}
	if (alpha_texture) {
		Texture2DGl::deallocate(std::string("alpha"), alpha_texname);
	}
}

void VisualMaterialGl::enable() {
	GLuint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&program);
	if (ambient_texture) {
		GLint location = glGetUniformLocation(program, "ambientTextureSampler");
		if (location != -1) {
			glUniform1i(location, 0);
			glActiveTexture(GL_TEXTURE0);
			ambient_texture->bind();
		}
	}
	if (diffuse_texture) {
		GLint location = glGetUniformLocation(program, "diffuseTextureSampler");
		if (location != -1) {
			glUniform1i(location, 1);
			glActiveTexture(GL_TEXTURE1);
			diffuse_texture->bind();
		}
	}
	if (specular_texture) {
		GLint location = glGetUniformLocation(program, "specularTextureSampler");
		if (location != -1) {
			glUniform1i(location, 2);
			glActiveTexture(GL_TEXTURE2);
			specular_texture->bind();
		}
	}
	if (specular_highlight_texture) {
		GLint location = glGetUniformLocation(program, "specularHighlightTextureSampler");
		if (location != -1) {
			glUniform1i(location, 3);
			glActiveTexture(GL_TEXTURE3);
			specular_highlight_texture->bind();
		}
	}
	if (bump_texture) {
		GLint location = glGetUniformLocation(program, "bumpTextureSampler");
		if (location != -1) {
			glUniform1i(location, 4);
			glActiveTexture(GL_TEXTURE4);
			bump_texture->bind();
		}
	}
	if (displacement_texture) {
		GLint location = glGetUniformLocation(program, "displacementTextureSampler");
		if (location != -1) {
			glUniform1i(location, 5);
			glActiveTexture(GL_TEXTURE5);
			displacement_texture->bind();
		}
	}
	if (alpha_texture) {
		GLint location = glGetUniformLocation(program, "alphaTextureSampler");
		if (location != -1) {
			glUniform1i(location, 6);
			glActiveTexture(GL_TEXTURE6);
			alpha_texture->bind();
		}
	}
}

void VisualMaterialGl::disable() {
	if (ambient_texture) {
		glActiveTexture(GL_TEXTURE0);
		ambient_texture->unbind();
	}
	if (diffuse_texture) {
		glActiveTexture(GL_TEXTURE1);
		diffuse_texture->unbind();
	}
	if (specular_texture) {
		glActiveTexture(GL_TEXTURE2);
		specular_texture->unbind();
	}
	if (specular_highlight_texture) {
		glActiveTexture(GL_TEXTURE3);
		specular_highlight_texture->unbind();
	}
	if (bump_texture) {
		glActiveTexture(GL_TEXTURE4);
		bump_texture->unbind();
	}
	if (displacement_texture) {
		glActiveTexture(GL_TEXTURE5);
		displacement_texture->unbind();
	}
	if (alpha_texture) {
		glActiveTexture(GL_TEXTURE6);
		alpha_texture->unbind();
	}
}

void VisualMaterialGl::initialize() {
}

void VisualMaterialGl::deinitialize() {
}


#ifndef SHADER_H__
#define SHADER_H__

#include <GL/glew.h>

struct Shader {

	int _filter;
	char *_buf;
	const char *_vsSource[1];
	const char *_fsSource[1];

	Shader();
	~Shader();

	void init(const char *path);
};

#endif

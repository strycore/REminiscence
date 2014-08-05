
#ifdef USE_GL

#include "file.h"
#include "shader.h"


Shader::Shader()
	: _buf(0) {
	_filter = GL_LINEAR;
	_vsSource[0] = 0;
	_fsSource[0] = 0;
}

Shader::~Shader() {
	free(_buf);
}

static char *readFile(const char *path) {
	File f;
	if (!f.open(path, "r")) {
		warning("Unable to open '%s'", path);
		return 0;
	}
	const uint32_t len = f.size();
	debug(DBG_INFO, "Shader '%s' size is %d bytes", path, len);
	char *buf = (char *)malloc(len + 1);
	if (buf) {
		const uint32_t count = f.read(buf, len);
		if (count != len) {
			warning("Failed to read %d bytes (expected %d)", len, count);
			free(buf);
			return 0;
		}
		buf[count] = 0;
	}
	return buf;
}

static const char *defaultVertexShader = "void main() { gl_Position = ftransform(); gl_TexCoord[0] = gl_MultiTexCoord0; }";

void Shader::init(const char *path) {
	_buf = readFile(path);

	// extract the vertex and fragment shaders from xml
	// 
	// https://gitorious.org/bsnes/pages/XmlShaderFormat

	_vsSource[0] = defaultVertexShader;
	_fsSource[0] = 0;
	char *vs = 0;
	char *fs = 0;

	char *tok = strstr(_buf, "<vertex");
	if (tok) {
		char *end = strstr(tok, "<![CDATA[");
		if (end) {
			vs = end + 9;
		}
	}
	tok = strstr(_buf, "<fragment");
	if (tok) {
		char *end = strstr(tok, "<![CDATA[");
		if (end) {
			fs = end + 9;
			if (strstr(tok, "filter=\"nearest")) {
				_filter = GL_NEAREST;
			} else if (strstr(tok, "filter=\"linear")) {
				_filter = GL_LINEAR;
			}
		}
	}
	if (vs) {
		tok = strstr(vs, "]]>");
		if (tok) {
			*tok = 0;
		}
		_vsSource[0] = vs;
	}
	if (fs) {
		tok = strstr(fs, "]]>");
		if (tok) {
			*tok = 0;
		}
		_fsSource[0] = fs;
	} else {
		warning("No fragment shader defined");
	}

	free(_buf);
	_buf = 0;
}

#endif

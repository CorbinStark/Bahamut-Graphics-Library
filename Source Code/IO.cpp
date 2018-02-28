#include "IO.h"
namespace IO {
	GLchar* IO::readFile(const GLchar* filename) {
		FILE* file = fopen(filename, "rt");
		fseek(file, 0, SEEK_END);
		unsigned long length = ftell(file);
		GLchar* data = new GLchar[length + 1];
		memset(data, 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, length, file);
		fclose(file);

		return data;
	}
}
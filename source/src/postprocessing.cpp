#include "postprocessing.h"
#include "platform.h"
#include <fstream>
#include <string>

bool readfilecontentasstring(const std::string &path, std::string &contentstringbuilder) {
	std::ifstream ifs(path.c_str());
	if (ifs.is_open()) {
		std::string stringbuf{};
		while (std::getline(ifs, stringbuf)) {
			contentstringbuilder += stringbuf;
			contentstringbuilder += '\n';
		}

		ifs.close();
		return false;
	}

	return true;
}

pipelineprogram::pipelineprogram(const std::vector<pipelineprogram::shader> &pipelineprogramshaderlist) {
	std::vector<uint32_t> compiledshaderlist{};
	this->id = glCreateProgram();

	for (const pipelineprogram::shader& shaders : pipelineprogramshaderlist) {
		std::string shadersrc {shaders.srcorpath};
		if (!shaders.issrc && readfilecontentasstring(shadersrc, shadersrc)) {
			std::cout << "Failed to read shader content from file '" << shaders.srcorpath << "\'\n";
			break;
		}

		uint32_t shader = glCreateShader(shaders.stage);
		const char* pshadersrc = shadersrc.c_str();

		glShaderSource(shader, 1, &pshadersrc, nullptr);
		glCompileShader(shader);

		int32_t signal {};
		glGetShaderiv(shader, GL_COMPILE_STATUS, &signal);

		if (signal == 0) {
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &signal);

			GLchar *pinfologmsg = new GLchar[signal];
			glGetShaderInfoLog(shader, signal, nullptr, pinfologmsg);
			std::cout << "Failed to compile shader: \n" << *pinfologmsg << '\n';
			delete[] pinfologmsg;

			break;
		}

		compiledshaderlist.push_back(shader);
	}

	for (uint32_t& shaders : compiledshaderlist) {
		glAttachShader(this->id, shaders);
		glDeleteShader(shaders);
	}

	glLinkProgram(this->id);

	int32_t signal {};
	glGetProgramiv(this->id, GL_LINK_STATUS, &signal);

	if (signal == 0) {
		glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &signal);

		std::string infologmsg{};
		infologmsg.resize(signal);

		GLchar* pinfologmsg = new GLchar[signal];
		glGetProgramInfoLog(this->id, signal, nullptr, pinfologmsg);
		std::cout << "Failed to link program: \n" << *pinfologmsg << '\n';
		delete[] pinfologmsg;
	}
	else {
		std::cout << "A program was linked successfully!\n";
	}
}
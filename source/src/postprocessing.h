#ifndef _POST_PROCESSING
#define _POST_PROCESSING

#include <iostream>
#include <vector>

bool readfilecontentasstring(const std::string& path, std::string& contentstringbuilder);

struct pipelineprogram {
private:
	uint32_t id = 0;
public:
	struct shader {
		uint32_t stage {};
		std::string srcorpath {};
		bool issrc {};
	};
public:
	explicit pipelineprogram(const std::vector<pipelineprogram::shader> &pipelineprorgamshaderlist);
	~pipelineprogram();
};

#endif // _POST_PROCESSING
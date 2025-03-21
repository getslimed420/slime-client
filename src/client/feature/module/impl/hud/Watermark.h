#pragma once
#include "../../TextModule.h"

class Watermark : public TextModule {
public:
	Watermark();

	std::wstringstream text(bool isDefault, bool inEditor) override;
};

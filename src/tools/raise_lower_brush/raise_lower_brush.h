#pragma once

#include "../brush.h"

namespace Core::Tools {

	class RaiseLowerBrush : public Brush {
	public:

		RaiseLowerBrush() = default;

		bool isLowering = false; // Hold Shift to lower terrain
	};
}
//
// BetterCarryingEachOther 2016.04.29
// null objects (header-only with inline)
//

#pragma once

#include "object.h"
#include "view.h"

class NullLimiter : public Object {
public:
	inline NullLimiter(float y) {
		locate(y);
	}

	inline void update() {
		if (View::getY() > pos.y + View::getZfar()) {
			expire();
			return;
		}
		Object::update();
	}
};

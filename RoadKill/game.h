//
// BetterCarryingEachOther 2016.03.11
// game manage module (header-only with inline)
//

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <vmath.h>
#include "player.h"
#include "enemy.h"
#include "deco.h"
#include "view.h"
#include "generator.h"
#include "shader.h"
#include "resource.h"
#include "null.h"
#include "control.h"
#include "key.h"
#include "debug.h"
#include "framebuffer.h"

/*
// Game class is being deprecated. Its functionalities are moving to RoadKillApp.

class Game {
private:
	// ... (members like pool, player, grid, etc. are now in RoadKillApp or constants there)
	// ... (fb, fbs are now m_gbuffer_fbo, m_single_fbo in RoadKillApp)

public:
	// init(), setup(), clear() logic moved to RoadKillApp::startup() and RoadKillApp::shutdown()

	// reshape() logic will be part of RoadKillApp::onResize or similar sb7 method.
	// For now, commenting out, will be handled if a specific subtask targets event loop/drawing.
	// inline static void reshape(int w, int h) {
	// 	View::setAspect((float)w / h);
	// 	glViewport(0, 0, w, h);
	// }

	// draw() logic will be moved to RoadKillApp::render()
	// inline static void draw() { ... }

	// update() logic will be moved to RoadKillApp::render() or a separate update method called by render()
	// inline static void update() { ... }

	// getGrid(), getWidthLimit() are replaced by RoadKillApp::APP_GRID_SIZE, RoadKillApp::APP_WIDTH_LIMIT
	// getFrameInterval() - frame interval is usually managed by the application loop driver (e.g. sb7 itself or glutTimerFunc if used)
};
*/

// It's useful to keep View related static methods if they are still used globally,
// or they might also be refactored into RoadKillApp members if they hold state per instance.
// For now, assuming View class and its methods are still needed as they are.
// If Game::reshape was the only user of View::setAspect, that might need review.

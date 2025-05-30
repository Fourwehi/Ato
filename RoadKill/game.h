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

class Game {
private:
	inline static Object* pool = nullptr;
	inline static Player* player = nullptr;
	inline static Deco* hillL = nullptr;
	inline static Deco* hillR = nullptr;
	inline static float grid = 64.0f;
	inline static float widthLimit = grid * 6.0f;
	inline static int frameInterval = 16;
	inline static Framebuffer fb = Framebuffer((int)View::getWidth(), (int)View::getHeight());
	inline static FramebufferSingle fbs = FramebufferSingle((int)View::getWidth(), (int)View::getHeight());

public:
	inline static void init() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		srand(time(NULL));
		setup();

		fb.generate();
		fbs.generate();
	}

	inline static void setup() {
		MetaGenerator* mg = new MetaGenerator();

		for (float y = -grid * 4.0f; y < 0.0f; y += grid)
			mg->placeTrees(y);

		pool = new Object();
		hillL = new Deco(
			vmath::vec3(0.0, 0.0, 0.0),
			&Resource::hill,
			&Resource::Tex::grass,
			&Resource::Norm::grass);
		hillR = new Deco(
			vmath::vec3(0.0, 0.0, 0.0),
			vmath::vec3(0.0, 0.0, 180.0),
			&Resource::hill,
			&Resource::Tex::grass,
			&Resource::Norm::grass);
		pool->push(mg);
		pool->push(hillL);
		pool->push(hillR);

		NullLimiter* nl = new NullLimiter(0.0);

		nl->push(new Deco(
			vmath::vec3(0.0, 0.0, 0.0),
			vmath::vec3(0.0, 0.0, 90.0),
			&Resource::hill,
			&Resource::Tex::grass,
			&Resource::Norm::grass));
		nl->push(new Deco(
			vmath::vec3(0.0, grid * 3.0f, 0.0),
			&Resource::grass9,
			&Resource::Tex::grass,
			&Resource::Norm::grass));
		nl->push(new Deco(
			vmath::vec3(128.0, 0.0, 4.0),
			vmath::vec3(0.0, 0.0, 90.0),
			&Resource::chicken,
			&Resource::Tex::chicken,
			&Resource::Norm::chicken));

		pool->push(nl);

		player = new Player(pool);
	}

	inline static void clear() {
		delete player;
		pool->expire();
		delete pool;
	}

	inline static void reshape(int w, int h) {
		View::setAspect((float)w / h);
		glViewport(0, 0, w, h);
	}

	inline static void draw() {
		errorecho("Flush");
		glDisable(GL_BLEND);

		PhysicalShader& ps = Shader::getPhysicalShader();
		ps.setAmbient(Control::getAmbient());
		ps.setSpecular(1.0);
		ps.setUVOffset(vmath::vec2(0.0, 0.0));
		Shader::lightApply();

		fb.bind();
		glClearDepth(1.0);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(2.0);
		if (Shader::isWire()) {
			Shader::useHide();
			View::draw();
			pool->draw();
			player->draw();
		}
		Shader::usePhysicalShader();
		View::draw();
		pool->draw();
		player->draw();

		fb.unbind();

		if (Control::isSSAO()) {
			fbs.bind();
			glClearDepth(1.0);
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			fb.bindPosition();
			glActiveTexture(GL_TEXTURE1);
			fb.bindNormal();
			glActiveTexture(GL_TEXTURE2);
			Resource::Tex::noise.nbind();
			Shader::useSSAO();
			Model::drawFramePassCanonical();

			fbs.unbind();
		}

		glClearDepth(1.0);
		vmath::vec4 fog = Control::getFog();
		glClearColor(fog[0], fog[1], fog[2], fog[3]); // Access components using array style for vmath
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		fb.bindDiffuse();
		Shader::useFramePass();
		Model::drawFramePassCanonical();

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		if (Control::isSSAO()) {
			glActiveTexture(GL_TEXTURE0);
			fbs.bindDiffuse();
			Shader::useBlur();
			Model::drawFramePassCanonical();
		}

		glActiveTexture(GL_TEXTURE0);
		fb.bindPosition();
		Shader::useFog();
		Shader::fogSetColor(Control::getFog());
		Model::drawFramePassCanonical();

		errorecho("Draw");
		glutSwapBuffers();
	}

	inline static void update() {
		if (Key::keyCheckPressed('1'))
			Shader::switchPhysicalShader();

		Control::update();
		Shader::lightClear();

		if (Control::isDay()) {
			Shader::lightPush(vmath::vec4(-1.0, -1.0, 1.0, 0.0), vmath::vec4(1.2, 1.0, 0.8, 1.0));
			Shader::lightPush(vmath::vec4(1.0, 1.0, 1.0, 0.0), vmath::vec4(1.2, 1.0, 0.8, 0.5));
		}

		Object::countReset();

		if (player->isExpired()) {
			View::init();
			clear();
			setup();
		}
		hillL->locate(vmath::vec3(0.0, player->getPos().y, 0.0));
		hillL->shift(vmath::vec2(0.0, -player->getPos().y / 320.0));
		hillR->locate(vmath::vec3(0.0, player->getPos().y, 0.0));
		hillR->shift(vmath::vec2(0.0, player->getPos().y / 320.0));
		pool->update();
		player->update();
		View::update();

		std::cout << Object::countGet() << " Objects in scope" << std::endl;
	}

	inline static float getGrid() { return grid; }
	inline static float getWidthLimit() { return widthLimit; }
	inline static int getFrameInterval() { return frameInterval; }
};

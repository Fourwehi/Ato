//
// BetterCarryingEachOther 2016.04.01
// resource management
//

#include "resource.h"
// #include "vec.h" // Removed
#include "game.h"
#include "lodepng.h"

Texture Resource::Tex::test;
Texture Resource::Tex::white;
Texture Resource::Tex::carWhite;
Texture Resource::Tex::carGray;
Texture Resource::Tex::carBlue;
Texture Resource::Tex::goraniWhole;
Texture Resource::Tex::wheel;
Texture Resource::Tex::logstab;
Texture Resource::Tex::tree;
Texture Resource::Tex::tunnel;
Texture Resource::Tex::road;
Texture Resource::Tex::grass;
Texture Resource::Tex::water;
Texture Resource::Tex::streetlight;
Texture Resource::Tex::noise;
Texture Resource::Tex::chicken;

Texture Resource::Norm::flat;
Texture Resource::Norm::grass;
Texture Resource::Norm::logstab;
Texture Resource::Norm::tree;
Texture Resource::Norm::road;
Texture Resource::Norm::water;
Texture Resource::Norm::tunnel;
Texture Resource::Norm::chicken;
Texture Resource::Norm::car;

// Model Resource::temp; // This was not initialized or used anywhere, consider removing if truly unused.
// For now, keeping it commented out as it's not part of the explicit changes.

Model Resource::grass[3];
Model Resource::grass9;
Model Resource::water;
Model Resource::logstab;
Model Resource::flower;
Model Resource::hill;
Model Resource::streetlight;

Model Resource::player;

Model Resource::car;
Model Resource::wheel;
Model Resource::tree;
Model Resource::tunnel;
Model Resource::drain;
Model Resource::road[4];

Model Resource::goraniTorso;
Model Resource::goraniHead;
Model Resource::goraniNeck;
Model Resource::goraniLegFrontTop;
Model Resource::goraniLegFrontBottom;
Model Resource::goraniLegBackTop;
Model Resource::goraniLegBackBottom;

Model Resource::chicken;

void Resource::Tex::init() {
	test.load("RoadKill/test.png", GL_TEXTURE0);
	white.load("RoadKill/white.png", GL_TEXTURE0);
	carWhite.load("RoadKill/car.png", GL_TEXTURE0);
	carGray.load("RoadKill/car_gray.png", GL_TEXTURE0);
	carBlue.load("RoadKill/car_blue.png", GL_TEXTURE0);
	goraniWhole.load("RoadKill/gorani_whole.png", GL_TEXTURE0);
	wheel.load("RoadKill/wheel.png", GL_TEXTURE0);
	logstab.load("RoadKill/log.png", GL_TEXTURE0);
	tree.load("RoadKill/tree.png", GL_TEXTURE0);
	tunnel.load("RoadKill/tunnel.png", GL_TEXTURE0);
	road.load("RoadKill/road.png", GL_TEXTURE0);
	grass.load("RoadKill/grass.png", GL_TEXTURE0);
	water.load("RoadKill/water.png", GL_TEXTURE0);
	streetlight.load("RoadKill/streetlight.png", GL_TEXTURE0);
	noise.load("RoadKill/noise.png", GL_TEXTURE0);
	chicken.load("RoadKill/chicken.png", GL_TEXTURE0);
}

void Resource::Norm::init() {
	flat.load("RoadKill/normal_flat.png", GL_TEXTURE1);
	grass.load("RoadKill/normal_grass.png", GL_TEXTURE1);
	logstab.load("RoadKill/normal_log.png", GL_TEXTURE1);
	tree.load("RoadKill/normal_tree.png", GL_TEXTURE1);
	road.load("RoadKill/normal_road.png", GL_TEXTURE1);
	water.load("RoadKill/normal_water.png", GL_TEXTURE1);
	tunnel.load("RoadKill/normal_tunnel.png", GL_TEXTURE1);
	chicken.load("RoadKill/normal_chicken.png", GL_TEXTURE1);
	car.load("RoadKill/normal_car.png", GL_TEXTURE1);
}

void Resource::init() {
	Tex::init();
	Norm::init();

	// const float wlimit = Game::getWidthLimit(); // These seem unused
	// const float xlimit = Game::getGrid() * 3.5;
	// const float grid = Game::getGrid();
	// const float thickness = 3.0;

	grass[0].load("RoadKill/grass1.obj");
	grass[1].load("RoadKill/grass2.obj");
	grass[2].load("RoadKill/grass3.obj");
	grass9.load("RoadKill/grass9.obj");
	water.load("RoadKill/water.obj");
	logstab.load("RoadKill/log.obj");
	flower.load("RoadKill/flower.obj");
	hill.load("RoadKill/hill.obj");
	streetlight.load("RoadKill/streetlight.obj");

	player.load("RoadKill/player.obj");
	car.load("RoadKill/car.obj");
	wheel.load("RoadKill/wheel.obj");
	tree.load("RoadKill/tree.obj");
	tunnel.load("RoadKill/tunnel.obj");
	drain.load("RoadKill/drain.obj");

	road[0].load("RoadKill/road1.obj");
	road[1].load("RoadKill/road2.obj");
	road[2].load("RoadKill/road3.obj");
	road[3].load("RoadKill/road4.obj");

	// Assuming Model::load uses Model::loadOBJ which takes only filepath.
	// The second argument (group name) will be ignored if that's the case.
	goraniTorso.load("RoadKill/gorani.obj", "torso");
	goraniHead.load("RoadKill/gorani.obj", "head");
	goraniNeck.load("RoadKill/gorani.obj", "neck");
	goraniLegFrontTop.load("RoadKill/gorani.obj", "leg_front_top");
	goraniLegFrontBottom.load("RoadKill/gorani.obj", "leg_front_bottom");
	goraniLegBackTop.load("RoadKill/gorani.obj", "leg_back_top");
	goraniLegBackBottom.load("RoadKill/gorani.obj", "leg_back_bottom");

	chicken.load("RoadKill/chicken.obj");
}
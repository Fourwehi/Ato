#include "RoadKillApp.h"
#include <vmath.h>
#include <iostream> // For std::cout
#include <sb7program.h>
#include "shader.h" // For Shader class static methods
#include "Resource.h" // For Resource::init()
#include "Player.h"
#include "Object.h"
#include "Deco.h"
#include "generator.h" // For MetaGenerator
#include "null.h"      // For NullLimiter
#include <time.h>      // For srand(time(NULL))
// Ensure vmath is included, usually via RoadKillApp.h or shader.h
// #include <vmath.h> // Already in RoadKillApp.h

RoadKillApp::RoadKillApp() : m_pool(nullptr), m_player(nullptr), m_previousTime(0.0), m_hillL_ptr(nullptr), m_hillR_ptr(nullptr) {
    // Initialize key states
    for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
        m_keyStatePend[i] = false;
        m_keyStateAccept[i] = false;
    }
}

void RoadKillApp::startup()
{
    m_previousTime = info.app_time; // Or glfwGetTime() if not using sb7 info
    // srand(time(NULL)); // srand is already called in the previous version of startup, ensure it's not called twice or move if necessary.
    // It's better to have srand in the constructor or once at the very beginning if possible.
    // For now, assuming it's fine here if it was the only place. Let's ensure it's called once.
    // The previous diff showed srand(time(NULL)); was already added to startup.

    // OpenGL states
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    std::cout << "RoadKillApp startup() - Initializing Shaders and Resources" << std::endl;
    m_program_gouraud = sb7::program::load_from_files({"RoadKill/gouraud_v.glsl", "RoadKill/gouraud_f.glsl"}, true);
    m_program_flat = sb7::program::load_from_files({"RoadKill/flat_v.glsl", "RoadKill/flat_f.glsl", "RoadKill/flat_g.glsl"}, true);
    m_program_hide = sb7::program::load_from_files({"RoadKill/hide_v.glsl", "RoadKill/hide_f.glsl"}, true);
    m_program_framepass = sb7::program::load_from_files({"RoadKill/framepass_v.glsl", "RoadKill/framepass_f.glsl"}, true);
    m_program_fog = sb7::program::load_from_files({"RoadKill/fog_v.glsl", "RoadKill/fog_f.glsl"}, true);
    m_program_blur = sb7::program::load_from_files({"RoadKill/blur_v.glsl", "RoadKill/blur_f.glsl"}, true);

    // SSAO shaders are missing, load with error handling
    std::cout << "Attempting to load SSAO shaders: RoadKill/ssao_v.glsl, RoadKill/ssao_f.glsl" << std::endl;
    m_program_ssao = sb7::program::load_from_files({"RoadKill/ssao_v.glsl", "RoadKill/ssao_f.glsl"}, false); // false to not exit on error
    if (m_program_ssao == 0) {
        std::cerr << "Warning: Failed to load SSAO shaders (RoadKill/ssao_v.glsl, RoadKill/ssao_f.glsl). SSAO will not function." << std::endl;
    }

    Shader::initialize_shader_programs(
        m_program_gouraud, m_program_flat, m_program_hide,
        m_program_framepass, m_program_fog, m_program_blur, m_program_ssao
    );
    Shader::init();
    Resource::init(); // Initialize resources after shaders

    std::cout << "RoadKillApp startup() - Initializing Framebuffers" << std::endl;
    m_gbuffer_fbo.setup(info.windowWidth, info.windowHeight);
    m_gbuffer_fbo.generate();
    m_single_fbo.setup(info.windowWidth, info.windowHeight);
    m_single_fbo.generate();

    setup_game_objects(); // Call to new helper method

    std::cout << "RoadKillApp startup() - Complete" << std::endl;
}

// Helper method to setup game objects
void RoadKillApp::setup_game_objects() {
    std::cout << "RoadKillApp setup_game_objects() - Initializing Game Objects" << std::endl;

    // Ensure m_pool is clean if this is called multiple times (e.g. in reset_game)
    // However, reset_game should handle deleting old m_pool first.
    m_pool = new Object();

    MetaGenerator* mg = new MetaGenerator();
    for (float y = -APP_GRID_SIZE * 4.0f; y < 0.0f; y += APP_GRID_SIZE) {
        mg->placeTrees(y);
    }
    m_pool->push(mg);

    m_hillL_ptr = new Deco(vmath::vec3(0.0f, 0.0f, 0.0f), &Resource::hill, &Resource::Tex::grass, &Resource::Norm::grass);
    m_pool->push(m_hillL_ptr);
    m_hillR_ptr = new Deco(vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 180.0f), &Resource::hill, &Resource::Tex::grass, &Resource::Norm::grass);
    m_pool->push(m_hillR_ptr);

    NullLimiter* nl = new NullLimiter(0.0f);
    nl->push(new Deco(vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 90.0f), &Resource::hill, &Resource::Tex::grass, &Resource::Norm::grass));
    nl->push(new Deco(vmath::vec3(0.0f, APP_GRID_SIZE * 3.0f, 0.0f), &Resource::grass9, &Resource::Tex::grass, &Resource::Norm::grass));
    nl->push(new Deco(vmath::vec3(128.0f, 0.0f, 4.0f), vmath::vec3(0.0f, 0.0f, 90.0f), &Resource::chicken, &Resource::Tex::chicken, &Resource::Norm::chicken));
    m_pool->push(nl);

    m_player = new Player(m_pool);
    // Player needs to be located, assuming default or constructor handles initial pos
    // Or m_player->locate(vmath::vec3(initial_x, initial_y, initial_z));
}

void RoadKillApp::reset_game() {
    std::cout << "RoadKillApp reset_game() - Resetting game state" << std::endl;

    // View::init(); // Assuming View class is still used and has an init method.
                   // This might need adjustment based on how View is handled in sb7.
                   // For now, commented out if View::init() is not immediately available or adapted.

    // Clean up old objects
    if (m_player) {
        delete m_player;
        m_player = nullptr;
    }
    if (m_pool) {
        m_pool->expire(); // This should delete objects pushed into pool (mg, nl, hillL, hillR)
        delete m_pool;    // Delete the pool container itself
        m_pool = nullptr;
    }
    // Pointers were to objects in the pool, which should now be deleted.
    m_hillL_ptr = nullptr;
    m_hillR_ptr = nullptr;

    // Recreate game objects
    setup_game_objects();
}

// Forward declaration or include for View and Control if not already done via other headers
#include "View.h" // Assuming View class is used for camera/projection related calls
#include "Control.h" // For Control::isDay(), Control::getAmbient(), etc.
// Model.h for Model::drawFramePassCanonical() might be needed if not pulled by other headers
#include "Model.h"


void RoadKillApp::render(double currentTime) {
    // DeltaTime Calculation
    if (m_previousTime == 0.0) { // First frame initialization
        m_previousTime = currentTime;
    }
    // double deltaTime = currentTime - m_previousTime; // Not directly used in original logic
    m_previousTime = currentTime;

    // --- Ported Game::update() logic ---
    if (is_key_first_pressed(GLFW_KEY_1)) { // Replaced Key::keyCheckPressed('1')
        Shader::switchPhysicalShader();
    }

    // Control::update(); // Logic moved here directly
    if (is_key_first_pressed(GLFW_KEY_2)) { // Former Control::update() logic
        Control::day = !Control::day;
    }
    if (is_key_first_pressed(GLFW_KEY_3)) { // Former Control::update() logic
        Control::ssao = !Control::ssao;
    }
    Shader::lightClear();

    if (Control::isDay()) {
        Shader::lightPush(vmath::vec4(-1.0f, -1.0f, 1.0f, 0.0f), vmath::vec4(1.2f, 1.0f, 0.8f, 1.0f));
        Shader::lightPush(vmath::vec4(1.0f, 1.0f, 1.0f, 0.0f), vmath::vec4(1.2f, 1.0f, 0.8f, 0.5f));
    }

    Object::countReset();

    if (m_player && m_player->isExpired()) {
        reset_game();
    }

    // Ensure player and hills exist before using them
    if (m_player) { // Check m_player first as hills depend on it
        if (m_hillL_ptr) {
            m_hillL_ptr->locate(vmath::vec3(0.0f, m_player->getPos()[1], 0.0f));
            m_hillL_ptr->shift(vmath::vec2(0.0f, -m_player->getPos()[1] / 320.0f));
        }
        if (m_hillR_ptr) {
            m_hillR_ptr->locate(vmath::vec3(0.0f, m_player->getPos()[1], 0.0f));
            m_hillR_ptr->shift(vmath::vec2(0.0f, m_player->getPos()[1] / 320.0f));
        }
    }

    if (m_pool) m_pool->update();
    if (m_player) m_player->update(this); // Pass 'this' (RoadKillApp instance)

    View::update(this); // View update (camera movement, etc.) // MODIFIED

    // std::cout << Object::countGet() << " Objects in scope" << std::endl; // Debug output

    // --- Ported Game::draw() logic ---
    // errorecho("Flush"); // Original debug call
    glDisable(GL_BLEND);

    PhysicalShader& ps = Shader::getPhysicalShader();
    ps.setAmbient(Control::getAmbient());
    ps.setSpecular(1.0f);
    ps.setUVOffset(vmath::vec2(0.0f, 0.0f));
    Shader::lightApply();

    m_gbuffer_fbo.bind();
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(2.0f);
    if (Shader::isWire()) {
        Shader::useHide();
        View::draw(); // Setup view and projection matrices
        if (m_pool) m_pool->draw();
        if (m_player) m_player->draw();
    }
    Shader::usePhysicalShader(); // Ensure correct shader is active
    View::draw(); // Setup view and projection matrices
    if (m_pool) m_pool->draw();
    if (m_player) m_player->draw();

    m_gbuffer_fbo.unbind();

    if (Control::isSSAO()) {
        m_single_fbo.bind();
        glClearDepth(1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        m_gbuffer_fbo.bindPosition();
        glActiveTexture(GL_TEXTURE1);
        m_gbuffer_fbo.bindNormal();
        glActiveTexture(GL_TEXTURE2);
        Resource::Tex::noise.nbind(); // Assuming Resource::Tex::noise is correctly initialized
        Shader::useSSAO();
        Model::drawFramePassCanonical(); // Assumed static method

        m_single_fbo.unbind();
    }

    glClearDepth(1.0f);
    vmath::vec4 fogCol = Control::getFog();
    glClearColor(fogCol[0], fogCol[1], fogCol[2], fogCol[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    m_gbuffer_fbo.bindDiffuse();
    Shader::useFramePass();
    Model::drawFramePassCanonical();

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    if (Control::isSSAO()) {
        glActiveTexture(GL_TEXTURE0);
        m_single_fbo.bindDiffuse();
        Shader::useBlur();
        Model::drawFramePassCanonical();
    }

    glActiveTexture(GL_TEXTURE0);
    m_gbuffer_fbo.bindPosition();
    Shader::useFog();
    Shader::fogSetColor(Control::getFog()); // fogSetColor was from Shader class
    Model::drawFramePassCanonical();

    // errorecho("Draw"); // Original debug call
    // glutSwapBuffers(); // sb7 handles buffer swapping

    update_key_states(); // Update key states at the end of the frame
}

void RoadKillApp::onKey(int key, int scancode, int action, int mods) {
    if (key >= 0 && key <= GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            m_keyStatePend[key] = true;
        } else if (action == GLFW_RELEASE) {
            m_keyStatePend[key] = false;
        }
    }
    // Optional: handle GLFW_REPEAT if needed
    // sb7::application::onKey(key, scancode, action, mods); // Call base if you want its behavior
}

void RoadKillApp::update_key_states() {
    for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
        m_keyStateAccept[i] = m_keyStatePend[i];
    }
}

bool RoadKillApp::is_key_first_pressed(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return m_keyStatePend[key] && !m_keyStateAccept[key];
}

bool RoadKillApp::is_key_first_released(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return !m_keyStatePend[key] && m_keyStateAccept[key];
}

bool RoadKillApp::is_key_held(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return m_keyStatePend[key];
}

void RoadKillApp::shutdown()
{
    std::cout << "RoadKillApp shutdown() - Deleting Game Objects and Framebuffers" << std::endl;

    // Note: mg, nl, hillL, hillR are pushed into m_pool.
    // Object class destructor or a specific m_pool->clear() method should handle deleting them.
    // If not, they will leak when m_pool is deleted.
    // For now, only deleting m_player and m_pool directly.
    // 1. Delete scene objects
    if (m_player) {
        delete m_player;
        m_player = nullptr;
    }
    if (m_pool) {
        m_pool->expire(); // Critical for deleting objects in the pool (like mg, nl, hills)
        delete m_pool;    // Then delete the pool container itself
        m_pool = nullptr;
    }

    // 2. Cleanup static resources (like Textures managed by Resource class)
    Resource::cleanup_textures();

    // 3. Cleanup Framebuffers (already implemented)
    m_gbuffer_fbo.destroy();
    m_single_fbo.destroy();

    // 4. Cleanup Shader programs (already implemented from previous steps)
    std::cout << "RoadKillApp shutdown() - Deleting Shader Programs" << std::endl;
    glDeleteProgram(m_program_gouraud);
    glDeleteProgram(m_program_flat);
    glDeleteProgram(m_program_hide);
    glDeleteProgram(m_program_framepass);
    glDeleteProgram(m_program_fog);
    glDeleteProgram(m_program_blur);
    if (m_program_ssao != 0) { // Only delete if successfully loaded
        glDeleteProgram(m_program_ssao);
    }
    m_program_gouraud = 0;
    m_program_flat = 0;
    m_program_hide = 0;
    m_program_framepass = 0;
    m_program_fog = 0;
    m_program_blur = 0;
    m_program_ssao = 0;
}

// Removed duplicated stub render and onKey methods that were here.

DECLARE_MAIN(RoadKillApp);

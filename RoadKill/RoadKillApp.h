#ifndef _ROADKILLAPP_H_
#define _ROADKILLAPP_H_

#include <sb7.h>

// Corrected paths to be direct includes from RoadKill/
#include "Player.h"
#include "object.h" // Corrected case from Object.h to object.h
#include "deco.h" // Corrected case from Deco.h to deco.h
#include "generator.h" // MetaGenerator is in generator.h
#include "null.h"      // NullLimiter is in null.h
#include "Framebuffer.h"
#include "Resource.h"
#include <GLFW/glfw3.h> // For GLFW_KEY_LAST, key codes, and action codes


class RoadKillApp : public sb7::application
{
public:
    RoadKillApp() : m_pool(nullptr), m_player(nullptr), m_previousTime(0.0)
    // m_gbuffer_fbo and m_single_fbo will be default constructed (size 0,0)
    // then setup in startup()
    {}

    void startup() override;
    void shutdown() override;
    void render(double currentTime) override;
    void onKey(int key, int action) override;

private:
    // Constants
    static constexpr float APP_GRID_SIZE = 64.0f;
    static constexpr float APP_WIDTH_LIMIT = APP_GRID_SIZE * 6.0f;

    // Shader programs
    GLuint m_program_gouraud;
    GLuint m_program_flat;
    GLuint m_program_hide;
    GLuint m_program_framepass;
    GLuint m_program_fog;
    GLuint m_program_blur;
    GLuint m_program_ssao;

    // Game Objects
    Object* m_pool;
    Player* m_player;
    // Deco* m_hillL; // Managed by m_pool
    // Deco* m_hillR; // Managed by m_pool

    // Framebuffers
    Framebuffer m_gbuffer_fbo; // Will be default constructed, then setup
    FramebufferSingle m_single_fbo; // Will be default constructed, then setup

    // For time-based updates
    double m_previousTime;

    // Pointers to specific game objects for direct manipulation if needed
    Deco* m_hillL_ptr = nullptr;
    Deco* m_hillR_ptr = nullptr;

    // Private helper methods
    void setup_game_objects();
    void reset_game();

    // Input handling helpers
    bool is_key_first_pressed(int key);
    bool is_key_first_released(int key);
    bool is_key_held(int key);
    void update_key_states();

    // Key state storage
    bool m_keyStatePend[GLFW_KEY_LAST + 1];
    bool m_keyStateAccept[GLFW_KEY_LAST + 1];
};

#endif // _ROADKILLAPP_H_

#include "RoadKillApp.h"
#include <vmath.h>
#include <iostream> // For std::cout
#include <sb7program.h>
#include "shader.h" // For Shader class static methods
#include "Resource.h" // For Resource::init()

void RoadKillApp::startup()
{
    std::cout << "RoadKillApp startup()" << std::endl;
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
}

void RoadKillApp::shutdown()
{
    std::cout << "RoadKillApp shutdown()" << std::endl;
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

void RoadKillApp::render(double currentTime)
{
    // Minimal implementation - can be expanded later
    // std::cout << "RoadKillApp render(" << currentTime << ")" << std::endl; // Commented out to avoid spamming console
}

void RoadKillApp::onKey(int key, int action)
{
    // Minimal implementation - can be expanded later
    std::cout << "RoadKillApp onKey(" << key << ", " << action << ")" << std::endl;
}

DECLARE_MAIN(RoadKillApp);

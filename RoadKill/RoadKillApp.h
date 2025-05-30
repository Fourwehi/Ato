#ifndef _ROADKILLAPP_H_
#define _ROADKILLAPP_H_

#include <sb7.h>

class RoadKillApp : public sb7::application
{
public:
    void startup() override;
    void shutdown() override;
    void render(double currentTime) override;
    void onKey(int key, int action) override;

private:
    GLuint m_program_gouraud;
    GLuint m_program_flat;
    GLuint m_program_hide;
    GLuint m_program_framepass;
    GLuint m_program_fog;
    GLuint m_program_blur;
    GLuint m_program_ssao;
};

#endif // _ROADKILLAPP_H_

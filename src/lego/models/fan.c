#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include "../lego.h"

void lego_fan(const GLfloat *colors[4])
{
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[0]);
        glTranslatef(LEGO_LENGTH / 2, LEGO_WIDTH / 2, LEGO_HEIGHT / 2);
        lego();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[2]);
        glTranslatef(-LEGO_LENGTH, -LEGO_WIDTH, 0);
        lego();
    glPopMatrix();
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[1]);
        glRotatef(90., 0, 0, 1);
        glTranslatef(LEGO_LENGTH / 2, LEGO_WIDTH / 2, LEGO_HEIGHT / 2);
        lego();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[3]);
        glTranslatef(-LEGO_LENGTH, -LEGO_WIDTH, 0);
        lego();
    glPopMatrix();

}

void lego_fan_tessellation(int x, int y, const GLfloat *colors[4])
{
    int i, j;
    glPushMatrix();
        glTranslatef(-(LEGO_LENGTH + LEGO_WIDTH) * x / 2, -(LEGO_LENGTH + LEGO_WIDTH) * y / 2, 0);
        for (i = 0; i < x; i++) {
                glPushMatrix();
            for (j = 0; j < y; j++) {
                lego_fan(colors);
                glTranslatef(0, LEGO_LENGTH + LEGO_WIDTH, 0);
            }
                glPopMatrix();
                glTranslatef(LEGO_LENGTH + LEGO_WIDTH, 0, 0);
        }
    glPopMatrix();
}

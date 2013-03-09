#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include "../lego.h"

#define HORSE_LEG_LENGTH 4

static void horse_head()
{
    int i;
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        lego();
        glTranslatef(LEGO_UNIT * 2, 0, LEGO_HEIGHT);
        lego();
        glTranslatef(-LEGO_UNIT, -LEGO_UNIT / 2.0, LEGO_HEIGHT);
        mini_lego();
        glTranslatef(0, LEGO_UNIT, 0);
        mini_lego();

        //draw mane
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
        for (i = 0; i < 2; i++) {
            glPushMatrix();
                glTranslatef(-LEGO_UNIT, 0, 0);
                mini_lego();
                glTranslatef(0, -LEGO_UNIT, 0);
                mini_lego();
                glTranslatef(0, 0, -LEGO_HEIGHT);
                mini_lego();
                glTranslatef(0, LEGO_UNIT, 0);
                mini_lego();
            glPopMatrix();
            glTranslatef(-LEGO_UNIT, 0, 0);
        }
    glPopMatrix();
}

static void horse_body()
{
    int i, j;
    glPushMatrix();
        //draw body
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        for (i = 0; i < HORSE_LEG_LENGTH - 1; i++) {
            glPushMatrix();
                for (j = 0; j < 3; j++) {
                    lego();
                    glTranslatef(0, LEGO_WIDTH, 0);
                    lego();
                    glTranslatef(-LEGO_LENGTH, -LEGO_WIDTH, 0);
                }
            glPopMatrix();
            glTranslatef(0, 0, LEGO_HEIGHT);
        }

    glPopMatrix();
}

static void horse_tail()
{
    int i, j;
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
        for (i = 0; i < 3; i++) {
            glPushMatrix();
                for (j = 0; j < 2; j++) {
                    if (! (i == 0 && j == 0))
                        mini_lego();
                    glTranslatef(0, LEGO_UNIT, 0);
                }
            glPopMatrix();
            glTranslatef(0, 0, LEGO_HEIGHT);
        }
    glPopMatrix();

}

static void horse_leg()
{
    int i;
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
        lego();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        for (i = 0; i < HORSE_LEG_LENGTH - 1; i++) { 
            glTranslatef(0,0,LEGO_HEIGHT);
            lego();
        }
    glPopMatrix();
}
static void horse_leg_pair() {
    glPushMatrix();
        horse_leg();
        glTranslatef(LEGO_UNIT, LEGO_WIDTH, 0);
        horse_leg();
    glPopMatrix();
}
void horse() {

    glPushMatrix();
        horse_leg_pair();
        glPushMatrix();
        glTranslatef(-LEGO_UNIT,LEGO_UNIT * 0.5,HORSE_LEG_LENGTH * LEGO_HEIGHT);
        horse_tail();
        glPopMatrix();
        glTranslatef(LEGO_LENGTH * 2, 0, 0);
        horse_leg_pair();
        glTranslatef(LEGO_UNIT, 0, HORSE_LEG_LENGTH * LEGO_HEIGHT);
        horse_body();
        glTranslatef(0, LEGO_UNIT, (HORSE_LEG_LENGTH - 1) * LEGO_HEIGHT);
        horse_head();
    glPopMatrix();

}


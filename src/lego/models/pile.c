#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include "../lego.h"

void lego_pile(int max)
{
    
    if (max < 1) return;

    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
        lego();
        if (max == 1) return;
        glRotatef(90., 0, 0, 1);
        glTranslatef(LEGO_WIDTH / 4, LEGO_WIDTH / 4, LEGO_HEIGHT);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
        lego();
        if (max == 2) { glPopMatrix(); return; }
        glTranslatef(0, -LEGO_WIDTH, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
        lego();
        if (max == 3) { glPopMatrix(); return; }
        glTranslatef(0, 0, -2 *LEGO_HEIGHT);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
        lego();
        if (max == 4) { glPopMatrix(); return; }
        glTranslatef(- 2. / 3. * LEGO_LENGTH, - LEGO_WIDTH / 2, LEGO_HEIGHT);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        lego();
        if (max == 5) { glPopMatrix(); return; }
        glRotatef(90., 0, 0, 1);
        glTranslatef(LEGO_WIDTH / 4, LEGO_WIDTH / 4, 0);
        glTranslatef(-LEGO_LENGTH / 3, -LEGO_WIDTH * 1.5, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
        lego();
        glPushMatrix();
            glTranslatef(0, -LEGO_WIDTH, 0);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
            lego();
            glTranslatef(LEGO_LENGTH * 4./3, LEGO_WIDTH, 2 * LEGO_HEIGHT);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
            lego();
        glPopMatrix();
        if (max == 6) { glPopMatrix(); return; }
        glTranslatef(-LEGO_WIDTH / 2, -LEGO_WIDTH / 2, -LEGO_HEIGHT);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
        lego();
        if (max == 7) { glPopMatrix(); return; }
        glTranslatef(LEGO_LENGTH, -LEGO_WIDTH/2, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
        lego();
        if (max == 8) { glPopMatrix(); return; }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,LEGO_LENGTH,LEGO_HEIGHT);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
        lego();
        if (max == 9) { glPopMatrix(); return; }
        glPushMatrix();
            glRotatef(90., 0, 0, 1);
            glTranslatef(LEGO_WIDTH / 4, LEGO_WIDTH / 4, LEGO_HEIGHT);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
            lego();
            if (max == 10) { glPopMatrix(); glPopMatrix(); return; }
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, LEGO_LENGTH, LEGO_HEIGHT);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
            lego();
            glTranslatef(0, 0, LEGO_HEIGHT);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
            lego();
            if (max == 11) { glPopMatrix(); glPopMatrix(); return; }
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0,LEGO_WIDTH,0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
        lego();
        if (max == 12) { glPopMatrix(); return; }
        glPushMatrix();
            glRotatef(90., 0, 0, 1);
            glTranslatef(LEGO_WIDTH / 4, LEGO_WIDTH / 4, 0);
            glTranslatef(LEGO_WIDTH * 2, 0, 0);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
            lego();
            if (max == 13) { glPopMatrix(); glPopMatrix(); return; }
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0, LEGO_LENGTH, LEGO_HEIGHT);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
            lego();
            if (max == 14) { glPopMatrix(); glPopMatrix(); return; }
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,LEGO_WIDTH,0);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
            lego();
            if (max == 15) { glPopMatrix(); glPopMatrix(); return; }
        glPopMatrix();
    glPopMatrix();
}


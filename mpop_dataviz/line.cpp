#include "line.h"

Line::Line()
{

}

Line::~Line() {

}


void Line::draw(const QElapsedTimer& timer) {
    Q_UNUSED(timer);
    glPushMatrix();
    glTranslatef(-0.5, -0.5, 0.0);
    glScalef(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
    glColor4f(255, 127, 0, 255);
    glVertex2f(0.0, 0.0);
    glColor4f(127, 255, 0, 255);
    glVertex2f(1.0, 0.0);
    glColor4f(0, 127, 255, 255);
    glVertex2f(1.0, 1.0);
    glColor4f(255, 0, 127, 255);
    glVertex2f(0.0, 1.0);
    glEnd();
    glPopMatrix();
}

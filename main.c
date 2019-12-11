#include <GL/gl.h>
#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO implement here

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10.0, -10.0, 10.0, -10.0, 10.0);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland");
    init();
    glutDisplayFunc(display);
    glFrontFace(GL_CCW);
    glutMainLoop();
    return 0;
}

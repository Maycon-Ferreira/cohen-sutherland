#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

#define INSIDE 0U
#define LEFT 1U
#define RIGHT 2U
#define BOTTOM 4U
#define TOP 8U

typedef struct {
    double x1;
    double y1;
    double x2;
    double y2;
} line_t;

typedef struct {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
} screen_t;

double x1_coord = 0;
double y1_coord = 0;
double x2_coord = 0;
double y2_coord = 0;

int num_lines = 0;
line_t *lines = NULL;

screen_t screen = {0};

unsigned char area_code(double x, double y) {
    unsigned char code = INSIDE;

    if (x < screen.x_min) { code |= LEFT; }
    if (x > screen.x_max) { code |= RIGHT; }
    if (y < screen.y_min) { code |= BOTTOM; }
    if (y > screen.y_max) { code |= TOP; }

    return code;
}

int cohen_sutherland(line_t *line) {
    unsigned int area_p1 = area_code(line->x1, line->y1);
    unsigned int area_p2 = area_code(line->x2, line->y2);

    if (area_p1 & area_p2) {
        return -1;
    } else {
        if (area_p1 == INSIDE) {
            x1_coord = line->x1;
            y1_coord = line->y1;
        }
        if (area_p2 == INSIDE) {
            x2_coord = line->x2;
            y2_coord = line->y2;
        }

        if (area_p1 & LEFT) {
            y1_coord = line->y1 + (line->y2 - line->y1) * (screen.x_min - line->x1) / (line->x2 - line->x1);
            x1_coord = screen.x_min;
        }
        if (area_p1 & RIGHT) {
            y1_coord = line->y1 + (line->y2 - line->y1) * (screen.x_max - line->x1) / (line->x2 - line->x1);
            x1_coord = screen.x_max;
        }
        if (area_p1 & BOTTOM) {
            x1_coord = line->x1 + (line->x2 - line->x1) * (screen.y_min - line->y1) / (line->y2 - line->y1);
            y1_coord = screen.y_min;
        }
        if (area_p1 & TOP) {
            x1_coord = line->x1 + (line->x2 - line->x1) * (screen.y_max - line->y1) / (line->y2 - line->y1);
            y1_coord = screen.y_max;
        }

        if (area_p2 & LEFT) {
            y2_coord = line->y1 + (line->y2 - line->y1) * (screen.x_min - line->x1) / (line->x2 - line->x1);
            x2_coord = screen.x_min;
        }
        if (area_p2 & RIGHT) {
            y2_coord = line->y1 + (line->y2 - line->y1) * (screen.x_max - line->x1) / (line->x2 - line->x1);
            x2_coord = screen.x_max;
        }
        if (area_p2 & BOTTOM) {
            x2_coord = line->x1 + (line->x2 - line->x1) * (screen.y_min - line->y1) / (line->y2 - line->y1);
            y2_coord = screen.y_min;
        }
        if (area_p2 & TOP) {
            x2_coord = line->x1 + (line->x2 - line->x1) * (screen.y_max - line->y1) / (line->y2 - line->y1);
            y2_coord = screen.y_max;
        }
        return 0;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_STRIP);
    {
        glVertex3d(screen.x_min, screen.y_min, 10.0);
        glVertex3d(screen.x_min, screen.y_max, 10.0);
        glVertex3d(screen.x_max, screen.y_max, 10.0);
        glVertex3d(screen.x_max, screen.y_min, 10.0);
        glVertex3d(screen.x_min, screen.y_min, 10.0);
    }
    glEnd();

    glColor3f(1.0, 0.0, 0.0);

    for (int i = 0; i < num_lines; ++i) {
        glBegin(GL_LINES);
        {
            glVertex3d(lines[i].x1, lines[i].y1, 10.0);
            glVertex3d(lines[i].x2, lines[i].y2, 10.0);
        }
        glEnd();
    }

    glColor3f(0.0, 1.0, 0.0);

    for (int i = 0; i < num_lines; ++i) {
        if (cohen_sutherland(lines + i) == 0) {
            glBegin(GL_LINES);
            {
                glVertex3d(x1_coord, y1_coord, 9.9);
                glVertex3d(x2_coord, y2_coord, 9.9);
            }
            glEnd();
        }
    }

    glutSwapBuffers();
    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

// TODO safe input read
int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Usage: ./cohen-sutherland FILE");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Could not open file");
        return 2;
    }

    fscanf(file, "%lf\n"
                 "%lf\n"
                 "%lf\n"
                 "%lf\n"
                 "%d\n", &screen.x_min, &screen.x_max, &screen.y_min, &screen.y_max, &num_lines);

    lines = calloc(num_lines, sizeof(line_t));

    for (int i = 0; i < num_lines; ++i) {
        fscanf(file, "%lf %lf\n"
                     "%lf %lf\n", &lines[i].x1, &lines[i].y1, &lines[i].x2, &lines[i].y2);
    }

    fclose(file);

    glutInit(&argc, argv);
    glutInitDisplayMode(0);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland");
    init();
    glutDisplayFunc(display);
    glFrontFace(GL_CCW);
    glutMainLoop();
    free(lines);
    return 0;
}

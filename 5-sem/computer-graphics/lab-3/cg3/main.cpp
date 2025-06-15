#include <GL/glut.h>
#include <cmath>

float angle = 0.0f; // Угол для вращения камеры
float radius = 10.0f; // Радиус окружности, по которой движется камера
float height = 1.0f;

void drawCube() {
    glBegin(GL_QUADS);

    // Передняя грань (красная)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Задняя грань (зеленая)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Левый бок (синий)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Правый бок (желтый)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);

    // Верхняя грань (белая)
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);

    // Нижняя грань (черная)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Позиция камеры
    float camX = radius * cos(angle);
    float camY = height;
    float camZ = radius * sin(angle);
    gluLookAt(camX, camY, camZ,  // Позиция камеры
              0.0f, 0.0f, 0.0f,  // Точка, на которую смотрит камера
              0.0f, 1.0f, 0.0f); // Вектор вверх

    drawCube(); // Рисуем куб

    glutSwapBuffers();
}

void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); // Угол обзора, соотношение сторон, ближняя и дальняя плоскости
    glMatrixMode(GL_MODELVIEW);
}

void setup() {
    glEnable(GL_DEPTH_TEST); // Включаем тест глубины
    glClearColor(0.f, 0.f, 0.f, 1.0f); // Устанавливаем цвет фона
    setupProjection(); // Устанавливаем перспективу
}

void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            height -= 0.5f; // Движение вперед
            break;
        case GLUT_KEY_UP:
            height += 0.5f; // Движение назад
            break;
        case GLUT_KEY_LEFT:
            angle -= 0.1f; // Поворот влево
            break;
        case GLUT_KEY_RIGHT:
            angle += 0.1f; // Поворот вправо
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Cube with Camera");

    setup();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard); // Регистрация функции обработки клавиатуры
    glutMainLoop();

    return 0;
}
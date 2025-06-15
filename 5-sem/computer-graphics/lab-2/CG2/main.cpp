#include <SFML/Window.hpp>
#include <GL/glu.h>
#include <vector>
#include <cmath>

struct Object {
    float x, y, z;
    int type; // 0: Cube, 1: Pyramid, 2: Cylinder
};

std::vector<Object> objects;

void drawCube() {
    // Draw faces
    glBegin(GL_QUADS);

    // Front face
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red with transparency
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face
    glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Green with transparency
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Left face
    glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // Blue with transparency
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face
    glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Yellow with transparency
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Top face
    glColor4f(1.0f, 0.0f, 1.0f, 0.5f); // Magenta with transparency
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Bottom face
    glColor4f(0.0f, 1.0f, 1.0f, 0.5f); // Cyan with transparency
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();

    // Draw edges
    glBegin(GL_LINES);

    // Front face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Red edges
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Back face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Red edges
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Left face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Red edges
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Red edges
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Top face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Red edges
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Bottom face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Red edges
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}


void drawPyramid() {
    // Draw the sides (triangles)
    glBegin(GL_TRIANGLES);

    // Front face
    glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Green with transparency
    glVertex3f(0.0f, 0.5f, 0.0f);  // Apex
    glVertex3f(-0.5f, -0.5f, -0.5f); // Bottom left
    glVertex3f(0.5f, -0.5f, -0.5f); // Bottom right

    // Right face
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red with transparency
    glVertex3f(0.0f, 0.5f, 0.0f);  // Apex
    glVertex3f(0.5f, -0.5f, -0.5f); // Bottom right
    glVertex3f(0.5f, -0.5f, 0.5f);  // Bottom front

    // Back face
    glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // Blue with transparency
    glVertex3f(0.0f, 0.5f, 0.0f);  // Apex
    glVertex3f(0.5f, -0.5f, 0.5f);  // Bottom front
    glVertex3f(-0.5f, -0.5f, 0.5f); // Bottom left

    // Left face
    glColor4f(1.0f, 1.0f, 0.0f, 0.5f); // Yellow with transparency
    glVertex3f(0.0f, 0.5f, 0.0f);  // Apex
    glVertex3f(-0.5f, -0.5f, 0.5f); // Bottom left
    glVertex3f(-0.5f, -0.5f, -0.5f); // Bottom back

    glEnd();

    // Draw the base (square)
    glBegin(GL_QUADS);
    glColor4f(0.5f, 0.5f, 0.5f, 0.5f); // Gray with transparency
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    // Draw edges
    glBegin(GL_LINES);

    // Front face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Green edges
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Right face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Green edges
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Back face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Green edges
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Left face edges
    glColor3f(1.0f, 1.0f, 1.0f); // Green edges
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Base edges
    glColor3f(1.0f, 1.0f, 1.0f); // Green edges
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glEnd();
}

void drawCylinder() {
    // Draw the sides (cylinder body)
    glBegin(GL_QUAD_STRIP);
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f); // Blue with transparency
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        float x = 0.5f * cos(theta);
        float z = 0.5f * sin(theta);
        glVertex3f(x, -0.5f, z); // Bottom vertex
        glVertex3f(x, 0.5f, z);  // Top vertex
    }
    glEnd();

    // Draw the top circle
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f); // Blue with transparency
    glVertex3f(0.0f, 0.5f, 0.0f); // Center of the top circle
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        float x = 0.5f * cos(theta);
        float z = 0.5f * sin(theta);
        glVertex3f(x, 0.5f, z); // Top circle vertices
    }
    glEnd();

    // Draw the bottom circle
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f); // Blue with transparency
    glVertex3f(0.0f, -0.5f, 0.0f); // Center of the bottom circle
    for (int i = 0; i <= 360; i += 10) {
        float theta = i * M_PI / 180.0f;
        float x = 0.5f * cos(theta);
        float z = 0.5f * sin(theta);
        glVertex3f(x, -0.5f, z); // Bottom circle vertices
    }
    glEnd();

    // Draw edges
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // Blue edges
    for (int i = 0; i <= 360; i += 1) {
        float theta = i * M_PI / 180.0f;
        float x = 0.5f * cos(theta);
        float z = 0.5f * sin(theta);
        glVertex3f(x, -0.5f, z); // Bottom edge
        glVertex3f(x, 0.5f, z);  // Top edge
    }
    glEnd();
}


void drawLines() {
    glBegin(GL_LINES);
    glColor4f(1.0f, 1.0f, 1.0f, 0.1f); // White lines

    glVertex3f(-5.0f, 0.0f, 0.0f); // Horizontal line
    glVertex3f(5.0f, 0.0f, 0.0f);

    glVertex3f(0.0f, 2.0f, 0.0f);   // Vertical line
    glVertex3f(0.0f, -2.0f, 0.0f);

    glEnd();

    glBegin(GL_POINTS);
    glPointSize(3.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // Красный цвет
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, 0.0f);
    glEnd();
}

void drawObjects() {
    for (const auto &obj: objects) {
        glPushMatrix();
        glEnable(GL_POINT_SMOOTH);
        glTranslatef(obj.x, obj.y, obj.z);
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);

        switch (obj.type) {
            case 0:
                drawCube();
                break;
            case 1:
                drawPyramid();
                break;
            case 2:
                drawCylinder();
                break;
        }

        glPopMatrix();
    }
    drawLines();
}


void setupPerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, 1200.0f / 900.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void moveObject(Object &obj, float dx, float dy, float dz) {
    obj.x += dx;
    obj.y += dy;
    obj.z += dz;
}

int main() {
    sf::Window window(sf::VideoMode(1200, 900), "2-Point Perspective with OpenGL and SFML",
                      sf::Style::Close | sf::Style::Resize);
    window.setActive(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    objects.push_back({0.0f, 0.0f, 0.0f, 0});  // Cube
    objects.push_back({0.0f, 0.0f, 0.0f, 1});  // Pyramid
    objects.push_back({0.0f, 0.0f, 0.0f, 2});  // Cylinder

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) moveObject(objects[0], 0.0f, 0.001f, 0.0f); // Move cube up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) moveObject(objects[0], 0.0f, -0.001f, 0.0f); // Move cube down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveObject(objects[0], -0.001f, 0.0f, 0.0f); // Move cube left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveObject(objects[0], 0.001f, 0.0f, 0.0f); // Move cube right

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) moveObject(objects[1], 0.0f, 0.001f, 0.0f); // Move pyramid up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            moveObject(objects[1], 0.0f, -0.001f, 0.0f); // Move pyramid down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            moveObject(objects[1], -0.001f, 0.0f, 0.0f); // Move pyramid left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            moveObject(objects[1], 0.001f, 0.0f, 0.0f); // Move pyramid right

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            moveObject(objects[2], 0.0f, 0.001f, 0.0f); // Move cylinder up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            moveObject(objects[2], 0.0f, -0.001f, 0.0f); // Move cylinder down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            moveObject(objects[2], -0.001f, 0.0f, 0.0f); // Move cylinder left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            moveObject(objects[2], 0.001f, 0.0f, 0.0f); // Move cylinder right

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setupPerspective();
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, 5.0f,  // Camera position
                  0.0f, 0.0f, 0.0f,  // Look at point
                  0.0f, 1.0f, 0.0f); // Up vector

        drawObjects();

        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include "font.hpp"

float angle = 0.0f;
float scale = 1.0f;
sf::Vector2f position(0.0f, 0.0f);
int windowWith = 800;
int windowHeight = 800;


void drawLine() {
    glBegin(GL_LINES);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(0.5f, 0.0f);
    glEnd();
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    sf::RenderWindow window(sf::VideoMode(windowWith, windowHeight), "OpenGL Line Transformation");

    sf::Font font;
    font.loadFromMemory(&ARIAL_TTF, ARIAL_TTF_len);

    sf::Text positionText;
    positionText.setFont(font);
    positionText.setCharacterSize(20);
    positionText.setFillColor(sf::Color::White);
    positionText.setPosition(10, 10);

    sf::Text angleText;
    angleText.setFont(font);
    angleText.setCharacterSize(20);
    angleText.setFillColor(sf::Color::White);
    angleText.setPosition(10, 40);

    sf::Text guideText;
    guideText.setFont(font);
    guideText.setCharacterSize(20);
    guideText.setFillColor(sf::Color::White);
    guideText.setPosition(10, (float)windowHeight - 120);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            position.y -= 0.001f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            position.y += 0.001f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            position.x -= 0.001f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            position.x += 0.001f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            scale += 0.001f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            scale -= 0.001f;
            if (scale < 0) {
                scale = 0;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            angle += 0.01f;
            if (abs(angle - (360)) < 0.01) {
                angle = 0;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            angle -= 0.01f;
            if (abs(angle - (-360)) < 0.01) {
                angle = 0;
            }
        }

        window.clear();

        glPushMatrix();

        glTranslatef(position.x, position.y, 0.0f);
        glScalef(scale, scale, 1.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        drawLine();

        window.pushGLStates();

        positionText.setString("Position: (" + std::to_string((float)windowWith / 2 + position.x * (float)windowWith / 2) + ", "
                               + std::to_string((float)windowHeight / 2 + position.y * (float)windowHeight / 2) + ")");
        angleText.setString("Angle: " + std::to_string(angle));
        guideText.setString("W - zoom in\nS - zoom out\nA - rotate counterclockwise\nD - rotate clockwise"
                            "\nArrow keys (Up, Down, Left, Right) - move");

        window.draw(positionText);
        window.draw(angleText);
        window.draw(guideText);

        window.popGLStates();

        glPopMatrix();

        window.display();
    }

    return 0;
}

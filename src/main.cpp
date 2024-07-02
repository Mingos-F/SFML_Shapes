#include <iostream>
#include <memory>
#include <fstream>

// Libraries of the gui an graphics
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <misc/cpp/imgui_stdlib.h>

// classes

class MyFont
{
    sf::Font m_Font;
    std::string m_path;
    int m_size, m_r, m_g, m_b;

public:
    MyFont() {}

    const sf::Font &getFont() const { return m_Font; }
    const std::string &getPath() const { return m_path; }
    const int &getSize() const { return m_size; }
    const int &getRed() const { return m_r; }
    const int &getGreen() const { return m_g; };
    const int &getBlue() const { return m_b; }

    void readFromFile(std::ifstream &fin)
    {
        fin >> m_path >> m_size >> m_r >> m_g >> m_b;

        // attempt to load font from file
        if (!m_Font.loadFromFile(m_path))
        {
            std::cerr << "Failed to load the font" << std::endl;
            exit(-1);
        }
    }
};

class Rectangle
{
    sf::RectangleShape m_shape;
    sf::Text m_text;
    std::string m_name;
    float m_x, m_y, m_sx, m_sy, m_width, m_height;
    float m_r, m_g, m_b;
    bool m_isShapeDrawn = true;
    float m_uiColor[3]{0.0f};

public:
    Rectangle()
    {
    }

    sf::RectangleShape &getShape() { return m_shape; }
    sf::Text &getText() { return m_text; }
    std::string &getName() { return m_name; }
    float &getX() { return m_x; }
    float &getY() { return m_y; }
    float &getSpeedX() { return m_sx; }
    float &getSpeedY() { return m_sy; }
    float &getWidth() { return m_width; }
    float &getHeight() { return m_height; }
    float &getRed() { return m_r; }
    float &getGreen() { return m_g; };
    float &getBlue() { return m_b; }
    bool &getIsShapeDrawn() { return m_isShapeDrawn; }
    float *getUiColor() { return m_uiColor; }

    void createText(const MyFont &font)
    {
        m_text.setFont(font.getFont());
        m_text.setCharacterSize(font.getSize());
        m_text.setFillColor(sf::Color(font.getRed(), font.getGreen(), font.getBlue()));

        m_text.setPosition(sf::Vector2f(m_x + (m_width / 2) - (m_text.getGlobalBounds().width / 2), m_y + (m_height / 2) - (m_text.getGlobalBounds().height / 2)));

        m_uiColor[0] = m_r / 255;
        m_uiColor[1] = m_g / 255;
        m_uiColor[2] = m_b / 255;
    }

    void readFromFile(std::ifstream &fin)
    {
        fin >> m_name >> m_x >> m_y >> m_sx >> m_sy >> m_r >> m_g >> m_b >> m_width >> m_height;
        m_text.setString(m_name);
        m_shape.setSize(sf::Vector2f(m_width, m_height));
        m_shape.setPosition(sf::Vector2f(m_x, m_y));
        m_shape.setFillColor(sf::Color(m_r, m_g, m_b));
    }

    void drawAnimation(sf::RenderWindow &window, sf::VideoMode &videoMode)
    {
        sf::Vector2f previousPos = m_shape.getPosition();

        if (previousPos.x < 0 || (previousPos.x + (m_width * m_shape.getScale().x)) > videoMode.width)
        {
            m_sx *= -1.0f;
        }

        if (previousPos.y < 0 || (previousPos.y + (m_height * m_shape.getScale().x)) > videoMode.height)
        {
            m_sy *= -1.0f;
        }

        sf::Vector2f newPosShape(m_shape.getPosition().x + m_sx, m_shape.getPosition().y + m_sy);
        sf::Vector2f newPosText(m_text.getPosition().x + m_sx, m_text.getPosition().y + m_sy);

        m_shape.setPosition(newPosShape);
        m_text.setPosition(newPosText);
        centerText();

        window.draw(m_shape);
        window.draw(m_text);
    }

    void centerText()
    {
        m_text.setPosition(sf::Vector2f(m_shape.getPosition().x + ((m_width / 2) * m_shape.getScale().x) - (m_text.getGlobalBounds().width / 2), m_shape.getPosition().y + ((m_height / 2) * m_shape.getScale().y) - (m_text.getGlobalBounds().height / 2)));
    }
};

class Circle
{
    sf::CircleShape m_shape;
    sf::Text m_text;
    std::string m_name;
    float m_x, m_y, m_sx, m_sy, m_radius;
    float m_r, m_g, m_b;
    bool m_isShapeDrawn = true;
    float m_uiColor[3]{0.0f};

public:
    Circle()
    {
    }

    sf::CircleShape &getShape() { return m_shape; }
    sf::Text &getText() { return m_text; }
    std::string &getName() { return m_name; }
    float &getX() { return m_x; }
    float &getY() { return m_y; }
    float &getSpeedX() { return m_sx; }
    float &getSpeedY() { return m_sy; }
    float &getRadius() { return m_radius; }
    float &getRed() { return m_r; }
    float &getGreen() { return m_g; }
    float &getBlue() { return m_b; }
    bool &getIsShapeDrawn() { return m_isShapeDrawn; }
    float *getUiColor() { return m_uiColor; }

    void createText(const MyFont &font)
    {
        m_text.setFont(font.getFont());
        m_text.setCharacterSize(font.getSize());
        m_text.setFillColor(sf::Color(font.getRed(), font.getGreen(), font.getBlue()));

        m_text.setPosition(sf::Vector2f(m_x + m_radius - (m_text.getGlobalBounds().width / 2), m_y + m_radius - (m_text.getGlobalBounds().height / 2)));
    }

    void readFromFile(std::ifstream &fin)
    {
        fin >> m_name >> m_x >> m_y >> m_sx >> m_sy >> m_r >> m_g >> m_b >> m_radius;
        m_text.setString(m_name);
        m_shape.setRadius(m_radius);
        m_shape.setPosition(sf::Vector2f(m_x, m_y));
        m_shape.setFillColor(sf::Color(m_r, m_g, m_b));

        m_uiColor[0] = m_r / 255;
        m_uiColor[1] = m_g / 255;
        m_uiColor[2] = m_b / 255;
    }

    void drawAnimation(sf::RenderWindow &window, sf::VideoMode &videoMode)
    {
        sf::Vector2f previousPos = m_shape.getPosition();

        if (previousPos.x < 0 || (previousPos.x + (m_radius * 2 * m_shape.getScale().x)) > videoMode.width)
        {
            m_sx *= -1.0f;
        }

        if (previousPos.y < 0 || (previousPos.y + (m_radius * 2 * m_shape.getScale().y)) > videoMode.height)
        {
            m_sy *= -1.0f;
        }

        sf::Vector2f newPosShape(m_shape.getPosition().x + m_sx, m_shape.getPosition().y + m_sy);
        sf::Vector2f newPosText(m_text.getPosition().x + m_sx, m_text.getPosition().y + m_sy);

        m_shape.setPosition(newPosShape);

        m_text.setPosition(newPosText);
        centerText();

        window.draw(m_shape);
        window.draw(m_text);
    }

    void centerText()
    {
        m_text.setPosition(sf::Vector2f(m_shape.getPosition().x + (m_radius * m_shape.getScale().x) - (m_text.getGlobalBounds().width / 2), m_shape.getPosition().y + (m_radius * m_shape.getScale().y) - (m_text.getGlobalBounds().height / 2)));
    }
};

int main(int argc, char const *argv[])
{
    // my font class
    MyFont font;

    // sfml video mode object to create the window with res from file
    sf::VideoMode videoMode;

    // vectors to store the shapes
    std::vector<Rectangle> rectangles;
    std::vector<Circle> circles;

    // helper variable to id what is read from the file;
    std::string id;

    // read configuration from file
    std::ifstream fin("config.txt");

    // check if file is open successfully
    if (!fin)
    {
        std::cerr << "Failed to open the configuration file" << std::endl;
        exit(-1);
    }

    // do the actual reading of the objects
    while (fin >> id)
    {
        if (id == "Window")
        {
            fin >> videoMode.width >> videoMode.height;
        }
        else if (id == "Fonts")
        {
            font.readFromFile(fin);
        }
        else if (id == "Circle")
        {
            Circle circle;
            circle.readFromFile(fin);
            circle.createText(font);
            circles.push_back(circle);
        }
        else
        {
            Rectangle rectangle;
            rectangle.readFromFile(fin);
            rectangle.createText(font);
            rectangles.push_back(rectangle);
        }
    }

    // create the window with the width and height from the file
    sf::RenderWindow window(videoMode, "SFML_Shapes");
    window.setFramerateLimit(60);

    // initialize IMGUI and create a clock used for internal timing
    bool imGuiInit = ImGui::SFML::Init(window);

    if (!imGuiInit)
    {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        exit(-1);
    }

    sf::Clock deltaClock;

    // scale the imgui ui by a given factor, does not affect text size
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    // main loop - continues for each frame the window is open
    while (window.isOpen())
    {
        // check all window events that where triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // this event triggers when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << std::endl;
            }

            // if the event was a close request we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // update imgui for this frame with the time that the last frame took
        ImGui::SFML::Update(window, deltaClock.restart());

        // ImGui::ShowDemoWindow();

        // draw the UI
        ImGui::Begin("Control Panel");

        std::vector<std::string> shapes;

        for (auto &shape : circles)
        {
            shapes.push_back(shape.getText().getString());
        }

        for (auto &shape : rectangles)
        {
            shapes.push_back(shape.getText().getString());
        }

        static int currentIndex = 0;
        // Pass in the preview value visible before opening the combo
        const char *combo_preview_value = shapes[currentIndex].c_str();
        if (ImGui::BeginCombo("Shapes", combo_preview_value))
        {
            for (int i = 0; i < shapes.size(); i++)
            {
                const bool isSelected = (currentIndex == i);
                if (ImGui::Selectable(shapes[i].c_str(), isSelected))
                    currentIndex = i;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (currentIndex > circles.size() - 1)
        {

            int tempIndex = currentIndex - circles.size();

            float uiScale = rectangles[tempIndex].getShape().getScale().x;

            ImGui::Checkbox("draw rectangle?", &rectangles[tempIndex].getIsShapeDrawn());
            ImGui::SliderFloat("Scale", &uiScale, 0.0f, 4.0f);
            ImGui::SliderFloat("X velocity", &rectangles[tempIndex].getSpeedX(), -8.0f, 8.0f);
            ImGui::SliderFloat("Y velocity", &rectangles[tempIndex].getSpeedY(), -8.0f, 8.0f);
            ImGui::ColorEdit3("Color", rectangles[tempIndex].getUiColor());
            ImGui::InputText("##", &rectangles[tempIndex].getName());
            ImGui::SameLine();
            if (ImGui::Button("Change name"))
            {
                rectangles[tempIndex].getText().setString(rectangles[tempIndex].getName());
            }

            ImGui::End();

            // set the circle properties, because the might have been updated with the gui
            rectangles[tempIndex].getShape().setFillColor(sf::Color(rectangles[tempIndex].getUiColor()[0] * 255, rectangles[tempIndex].getUiColor()[1] * 255, rectangles[tempIndex].getUiColor()[2] * 255));
            rectangles[tempIndex].getShape().setScale(sf::Vector2f(uiScale, uiScale));
        }
        else
        {

            // helper variables
            float uiScale = circles[currentIndex].getShape().getScale().x;

            ImGui::Checkbox("draw circle?", &circles[currentIndex].getIsShapeDrawn());
            ImGui::SliderFloat("Scale", &uiScale, 0.0f, 4.0f);
            ImGui::SliderFloat("X velocity", &circles[currentIndex].getSpeedX(), -8.0f, 8.0f);
            ImGui::SliderFloat("Y velocity", &circles[currentIndex].getSpeedY(), -8.0f, 8.0f);
            ImGui::ColorEdit3("Color", circles[currentIndex].getUiColor());
            ImGui::InputText("##", &circles[currentIndex].getName());
            ImGui::SameLine();
            if (ImGui::Button("Change name"))
            {
                circles[currentIndex].getText().setString(circles[currentIndex].getName());
            }

            ImGui::End();

            // set the circle properties, because the might have been updated with the gui
            circles[currentIndex].getShape().setFillColor(sf::Color(circles[currentIndex].getUiColor()[0] * 255, circles[currentIndex].getUiColor()[1] * 255, circles[currentIndex].getUiColor()[2] * 255));
            circles[currentIndex].getShape().setScale(sf::Vector2f(uiScale, uiScale));
        }

        // basic rendering functions
        window.clear();

        for (auto &shape : circles)
        {
            if (shape.getIsShapeDrawn())
            {
                shape.drawAnimation(window, videoMode);
            }
        }

        for (auto &shape : rectangles)
        {
            if (shape.getIsShapeDrawn())
            {
                shape.drawAnimation(window, videoMode);
            }
        }

        ImGui::SFML::Render(window);
        // call the display function (swaps the buffers)
        window.display();
    }

    return 0;
}

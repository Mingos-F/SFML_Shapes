#include <iostream>
#include <memory>
#include <fstream>

// Libraries of the gui an graphics
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

// classes

class MyFont
{
    sf::Font m_Font;
    std::string m_path;
    int m_size, m_r, m_g, m_b;

public:
    MyFont() {}

    const sf::Font &getFont() { return m_Font; }
    std::string &getPath() { return m_path; }
    const int &getSize() { return m_size; }
    const int &getRed() { return m_r; }
    const int &getGreen() { return m_g; };
    const int &getBlue() { return m_b; }

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
    float m_x, m_y, m_sx, m_sy, m_width, m_height;
    int m_r, m_g, m_b;

public:
    Rectangle()
    {
    }

    void readFromFile(std::ifstream &fin)
    {
        // helper var so store the name
        std::string name;

        fin >> name >> m_x >> m_y >> m_sx >> m_sy >> m_r >> m_g >> m_b >> m_width >> m_height;
        m_text.setString(name);
        m_shape.setSize(sf::Vector2f(m_width, m_height));
        m_shape.setPosition(sf::Vector2f(m_x, m_y));
        m_shape.setFillColor(sf::Color(m_r, m_g, m_b));
    }
};

class Circle
{
    sf::CircleShape m_shape;
    sf::Text m_text;
    float m_x, m_y, m_sx, m_sy, m_radius;
    int m_r, m_g, m_b;

public:
    Circle()
    {
    }

    void readFromFile(std::ifstream &fin)
    {
        // helper var so store the name
        std::string name;

        fin >> name >> m_x >> m_y >> m_sx >> m_sy >> m_r >> m_g >> m_b >> m_radius;
        m_text.setString(name);
        m_shape.setRadius(m_radius);
        m_shape.setPosition(sf::Vector2f(m_x, m_y));
        m_shape.setFillColor(sf::Color(m_r, m_g, m_b));
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

    // read from file

    std::ifstream fin("config.txt");

    if (!fin)
    {
        std::cerr << "Failed to open the configuration file" << std::endl;
        exit(-1);
    }

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
            circles.push_back(circle);
        }
        else
        {
            Rectangle rectangle;
            rectangle.readFromFile(fin);
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

    // the imgui color r,g,b wheel requires floats from 0-1 instead of ints from 0-255
    float c[3] = {0.0f, 1.0f, 1.0f};

    // make a shape that will draw to the screen
    float circleRadius = 50;
    int circleSegments = 32;
    float circleSpeedX = 1.0f;
    float circleSpeedY = .5f;
    bool drawCircle = true;
    bool drawText = true;

    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition(10.0f, 10.0f);

    // set up the text object that will be drawn on the screen
    sf::Text text("Sample text", font.getFont(), font.getSize());
    text.setFillColor(sf::Color(font.getRed(), font.getGreen(), font.getBlue()));

    // position the bottom-left corner of the text so that the text aligns on the bottom
    //  text character size is in pixels so move the text up the bottom by its height
    text.setPosition(0, videoMode.height - (float)text.getCharacterSize());

    // set up a character array for the text
    char displayString[255] = "Sample text";

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

        // draw the UI
        ImGui::Begin("window title");
        ImGui::Text("window text");
        ImGui::Checkbox("draw circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("draw text", &drawText);
        ImGui::SliderFloat("radius", &circleRadius, 0.0f, 200.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("color circle", c);
        ImGui::InputText("bottom text", displayString, 255);
        if (ImGui::Button("set text"))
        {
            text.setString(displayString);
        }
        ImGui::SameLine();
        if (ImGui::Button("reset circle"))
        {
            circle.setPosition(0, 0);
        }
        ImGui::End();

        // set the circle properties, because the might have been updated with the gui
        circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        // basic animation
        circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

        // basic rendering functions
        window.clear();

        if (drawCircle)
        {
            // draw the circle
            window.draw(circle);
        }

        if (drawText)
        {
            window.draw(text);
        }

        ImGui::SFML::Render(window);
        // call the display function (swaps the buffers)
        window.display();
    }

    return 0;
}

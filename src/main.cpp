#include <iostream>
#include <memory>
#include <fstream>

// Libraries of the gui an graphics
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main(int argc, char const *argv[])
{
    // create the window with the width and height from the file
    const int wWidth = 1280;
    const int wHeight = 720;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML_Shapes");
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

    // load a font
    sf::Font myFont;

    // attempt to load font from file
    if (!myFont.loadFromFile("fonts/tech.ttf"))
    {
        std::cerr << "Failed to load the font" << std::endl;
        exit(-1);
    }

    // set up the text object that will be drawn on the screen
    sf::Text text("Sample text", myFont, 24);

    // position the bottom-left corner of the text so that the text aligns on the bottom
    //  text character size is in pixels so move the text up the bottom by its height
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

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

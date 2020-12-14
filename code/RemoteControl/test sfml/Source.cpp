#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "FormSFML.h"
#include "Server.h"
#include "Client.h"
#include "Base64.h"

using namespace form;

const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
int string_length = sizeof(alphanum) - 1;

std::string passwordGen();


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 400), "RemoteControl", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(20);
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::Font font;

    if (!font.loadFromFile("font/Roboto.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::RectangleShape shape(sf::Vector2f(800, 400)),
        line(sf::Vector2f(30, 400)),
        infoSpace(sf::Vector2f(340, 300)),
        ipTextBox(sf::Vector2f(340, 50)),
        codeTextBox(sf::Vector2f(340, 50));

    std::string password = passwordGen();
    
    sf::Text IP("Your ip adress: " + ip.toString(), font), 
        Password("Your code: " + password, font), 
        splitLine("----------------------------------------", font);

    IP.setCharacterSize(20);
    IP.setStyle(sf::Text::Regular);
    IP.setFillColor(sf::Color::Black);

    Password.setCharacterSize(20);
    Password.setStyle(sf::Text::Regular);
    Password.setFillColor(sf::Color::Black);

    splitLine.setCharacterSize(20);
    splitLine.setStyle(sf::Text::Regular);
    splitLine.setFillColor(sf::Color::Black);


    shape.setFillColor(sf::Color(206, 206, 206));
    line.setFillColor(sf::Color(61, 101, 139));
    infoSpace.setFillColor(sf::Color::White);
    ipTextBox.setFillColor(sf::Color::White);
    codeTextBox.setFillColor(sf::Color::White);

    infoSpace.setPosition(60.f, 50.f);
    ipTextBox.setPosition(430.f, 50.f);
    codeTextBox.setPosition(430.f, 150.f);
    IP.setPosition(80.f, 70.f);
    Password.setPosition(80.f, 110.f);
    splitLine.setPosition(90.f, 130.f);

    Button button(480, 300, 230, 50, "Connect");
    Label label(600, 380, "d");

    Input inputIP(450, 60, 100, 30, "test");
    Label labelAdress(450, 100, "ip Adress");
    Input inputCode(450, 160, 100, 30, "Test");
    Label labelCode(450, 200, "code");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
           
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                
                    Vector2i mouse = Mouse::getPosition(window);

                    if (button.select(mouse)) {
                        std::cout << inputIP.readText() << std::endl;
                        std::cout << inputCode.readText() << std::endl;
                    }
                    inputIP.select(mouse);			//поле ввода
                    inputCode.select(mouse);	  		//поле ввода
                
            }

        }
        
            if (event.type == sf::Event::TextEntered) {
                if (inputIP.select()) {
                    inputIP.reText(event.text.unicode);
                    sleep(milliseconds(1000 / 10));//Задержка
                }
                if (inputCode.select()) {
                    inputCode.reText(event.text.unicode);
                    sleep(milliseconds(1000 / 10));//Задержка
                }
            }
        
        window.clear();
        window.draw(shape);
        window.draw(line);
        window.draw(infoSpace);
        window.draw(ipTextBox);
        window.draw(codeTextBox);
        window.draw(IP);
        window.draw(Password);
        window.draw(splitLine);
        window.draw(label.displayText());
        window.draw(labelAdress.displayText());
        window.draw(labelCode.displayText());
        window.draw(button.displayButton());
        window.draw(button.displayText());
        window.draw(inputIP.displayText());
        window.draw(inputIP.displayText());
        window.draw(inputCode.displayText());
        window.draw(inputCode.displayText());
        window.display();
       sleep(milliseconds(1000 / 15));//Задержка
    }

    return 0;
}


std::string passwordGen() {
    std::string pass;
    srand(time(0));
    for (int i = 0; i < 9; i++)
        pass += alphanum[rand() % string_length];
    return pass;
}

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

/** \brief A text with a square in the back of every character in the text
    Taking SFML::Text and count how many characters inside that text and initialize
    an array of SFML::RectangleShape and resize it to be the same width as the character
    And it depends on the font to be monospace for it's fixed width for any character
 */
class TextWithBackground
{
public:
    TextWithBackground(RenderWindow &w, Text &text, Font &font)
    {
        this->window = &w;
        this->text = text;
        this->f = font;
        this->text.setFont(this->f);
        this->length = text.getString().getSize();
        this->s = text.getString();
        this->characterSize = text.getCharacterSize();
        this->boxWidth = this->characterSize / 1.7;
        for(unsigned i = 0; i < length; i++)
        {
            box.setSize(Vector2f(this->boxWidth, text.getCharacterSize() + 2));
            this->boxes.push_back(this->box);
        }
    }
    void setPosition(int xpos, int ypos)
    {
        this->text.setPosition(xpos, ypos);
        int x = xpos;
        int y = ypos + 3;
        for(unsigned i = 0; i < this->s.getSize(); i++)
        {
            if(s[i-1] == '\n')
            {
                x = xpos;
                y += this->characterSize + 5;
            }
            this->boxes[i].setPosition(x, y);
            x += this->boxWidth;
        }
    }
    void setBoxesSpacing(double s)
    {
        double x = s;
        for(RectangleShape &b : this->boxes)
        {
            b.setPosition(b.getPosition().x + x, b.getPosition().y);
            x += s;
        }
    }

    void setBackgroundColor(Color s)
    {
        for(RectangleShape &b : this->boxes)
        {
            b.setFillColor(s);
        }
    }
    void setCharacterSize(unsigned int size)
    {
        this->characterSize = size;
        this->text.setCharacterSize(size);
        this->boxWidth = this->characterSize / 1.8;
        for(RectangleShape &b : this->boxes)
        {
            b.setSize(Vector2f(this->boxWidth, text.getCharacterSize() + 2));
        }
    }

    void setColorsArray(std::vector<Color> &colors)
    {
        for(unsigned i = 0; i < colors.size(); i++)
        {
            this->boxes[i].setFillColor(colors[i]);
        }
    }
    void draw()
    {
        for(auto &b : this->boxes)
        {
            this->window->draw(b);
        }
        this->window->draw(this->text);
    }
    void setOutlineThickness(float thickness)
    {
        this->text.setOutlineThickness(thickness);
        this->boxWidth = (this->characterSize + thickness) / 1.8;
        for(RectangleShape &b : this->boxes)
        {
            b.setSize(Vector2f(this->boxWidth, text.getCharacterSize() +thickness+ 2));
        }
    }
    void setOutlineColor(Color &color)
    {
        this->text.setOutlineColor(color);
    }
private:
    RenderWindow *window = nullptr;
    Text text;
    Font f;
    unsigned int length;
    std::vector<RectangleShape> boxes;
    RectangleShape box;
    String s;
    unsigned int characterSize;
    unsigned int boxWidth;

};

/**
    counting the words in the string

    @param s is the string that we will count how many words inside it
*/
unsigned int countWords(std::string &s)
{
    unsigned int c = 1;
    for(char cc : s)
        if(cc == ' ' || cc == '\n')
            c++;
    return c;
}


/**
    adjust the string to make it fits to the screen
    @param s is the string that will be adjusted
    @param screenWidth is the width of the screen
    @param characterSize is the size of the characters
*/
void adjustString(std::string &s, unsigned int screenWidth, unsigned int characterSize)
{
    int maxCharactersPerLine = screenWidth / (characterSize - 9);
    int characters = 0;
    for(unsigned i = 0; i < s.length(); i++)
    {
        characters++;
        if(characters == maxCharactersPerLine)
        {
            for(unsigned j = i; j < s.length(); j++)
            {
                if (s[j] == ' ')
                {
                    s.replace(j, 1, "\n");
                    break;
                }
            }
            characters = 0;
        }
    }
}
int main()
{
    /// Get the desktop resolution
    VideoMode desktop(VideoMode::getDesktopMode());

    /// Initialize the main window
    RenderWindow window(desktop, "Typing speed test");

    /// Set max FPS to be 60 frame
    window.setFramerateLimit(60);

    /**
    Testing paragraph:

    "In writing, the words point and purpose are almost synonymous.\n"
    "Your point is your purpose, and how you decide to make your point clear\n"
    "to your reader is also your purpose. Writers have a point and\n"
    "a purpose for every paragraph that they create."
    */
    std::string correctString = "There are three reasons why Canada is one of the best countries in the world. First, Canada has an excellent health care system. All Canadians have access to medical services at a reasonable price. Second, Canada has a high standard of education. Students are taught by well-trained teachers and are encouraged to continue studying at university. Finally, Canada's cities are clean and efficiently managed. Canadian cities have many parks and lots of space for people to live. As a result, Canada is a desirable place to live.";

    /// Global character size
    unsigned int characterSize = 30;

    /// Adjust the string to make it fit on the window
    adjustString(correctString, desktop.width, characterSize);


    /// Count how many words in the string
    unsigned int words = countWords(correctString);

    /// A typing string
    std::string typingString = "";

    /// Colors for correct and wrong letters
    Color backgroundColor(160, 217, 206);
    Color correctColor(21, 194, 161);
    Color wrongColor(194, 44, 21);

    /// Initializing a color vector with the same size as correctString
    /// And setting the array with the default color first
    std::vector<Color> colors(correctString.length());
    for(Color &c : colors)
    {
        c = backgroundColor;
    }

    /// Main font of the text
    /// MUST BE MONOSPACE
    /// for its fixed size for every character
    Font f;
    f.loadFromFile("fonts/consola.ttf");

    /// The regular SFML text
    Text text;
    text.setString(correctString);
    text.setCharacterSize(characterSize);
    text.setFillColor(Color(255, 255, 255));

    /// Our background text
    TextWithBackground s(window, text, f);
    s.setPosition(10, 0);
    s.setColorsArray(colors);

    /// Regular SFML text for the user
    Text typingText;
    typingText.setFont(f);
    typingText.setString(typingString);
    typingText.setPosition(10, VideoMode::getDesktopMode().height - 400);
    typingText.setCharacterSize(characterSize);
    typingText.setFillColor(Color(255, 255, 255));
    typingText.setOutlineThickness(3);
    typingText.setOutlineColor(Color::Red);

    /// Results text
    /// Some methods are called inside the main loop
    Text resultsText = typingText;
    resultsText.setOutlineColor(Color::Black);


    /// Global clock for the program
    Clock timer;
    Time time;
    long double times = 0;

    /// Let the user type what he wants after the program starts
    bool canType = true;

    /// Main loop
    while(window.isOpen())
    {
        /// Events loop
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            /// If the user pressed a key
            if (event.type == sf::Event::KeyPressed)
            {
                /// if the user allowed to type
                if(canType)
                {
                    /// if the key the user pressed is within A to Z
                    if(event.key.code >= Keyboard::A && event.key.code <= Keyboard::Z)
                    {
                        /// If the user pressing the Shift key
                        if (Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift))
                            /// Add to typingString the capitalized character he pressed
                            typingString += (char)(65+event.key.code);
                        else
                            /// Add to typingString the character he pressed
                            typingString += (char)(97+event.key.code);
                    }
                    /// If that key was space
                    else if(event.key.code == Keyboard::Space)
                    {
                        /// Add a new line character if the current character in
                        /// correctString is a new line or add a space character otherwise
                        if (correctString[typingString.length()] == '\n')
                            typingString += '\n';
                        else if(correctString[typingString.length()] == ' ')
                            typingString += ' ';
                    }
                    /// If that key was a backspace
                    else if(event.key.code == Keyboard::BackSpace)
                    {
                        /// Remove the last character from typingString
                        /// If the length is bigger than 0
                        if(typingString.length() > 0)
                        {
                            colors[typingString.length() - 1] = backgroundColor;
                            typingString.pop_back();
                        }
                    }
                    /// Add a new line if the user pressed Enter
                    else if(event.key.code == Keyboard::Enter)
                    {
                        typingString += '\n';

                    }
                    /// Comma
                    else if(event.key.code == Keyboard::Comma)
                    {
                        typingString += ',';

                    }
                    /// Dot
                    else if(event.key.code == Keyboard::Period)
                    {
                        typingString += '.';
                    }
                    /// Hyphen or the subtract character
                    else if(event.key.code == Keyboard::Hyphen || event.key.code == Keyboard::Subtract)
                    {
                        typingString += '-';
                    }
                    /// The single quote character
                    else if(event.key.code == Keyboard::Quote)
                        typingString += '\'';
                }
            }
        }
        /// If the user never entered anything then restart the timer every frame
        if(typingString == "")
            timer.restart();

        /// Get the mouse position (not used,  you can delete the next two lines if you want)
        Vector2i m = Mouse::getPosition(window);
        Vector2f mouse = window.mapPixelToCoords(m);

        /// Looping for every character the user entered
        /// and color it red if it's wrong and green if it's correct
        for(unsigned i = 0; i < typingString.length(); i++)
        {
            if(typingString[i] == correctString[i])
            {
                colors[i] = correctColor;
            }
            else
            {
                colors[i] = wrongColor;
            }
        }
        /// Set the modifications to the background text
        s.setColorsArray(colors);

        /// set the typing string with the new string
        typingText.setString(typingString);

        /// If the typingString is equal to correctString
        /// Then disable the user from typing and get the time the user
        /// Took to write and calculate how many words he typed per minute
        /// And show a text in the middle of the screen with the results
        if(typingString == correctString && canType)
        {
            time = timer.getElapsedTime();
            timer.restart();
            times = time.asSeconds();
            std::string resultsString = "You are typing " + std::to_string((words / times) * 60.0)+ " word per minute";
            std::cout << resultsString;
            canType = false;
            resultsText.setString(resultsString);
            resultsText.setOrigin(resultsString.length() * 8.5, resultsText.getOrigin().y);
            resultsText.setPosition(desktop.width / 2, desktop.height / 2);
        }
        /// Clear every frame
        window.clear(backgroundColor);

        /// Drawing section
        s.draw();
        window.draw(typingText);

        /// If the user finished typing, draw the results text
        if(canType == false)
            window.draw(resultsText);

        /// Display everything that has been drawn to the screen
        window.display();
    }

    return 0;
}

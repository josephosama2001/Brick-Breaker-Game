#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Menu.h"
#include <cmath>
#include <windows.h>
#include <string>
#include <fstream>
//constants definition
#define linesOfBricks 4 //define the number of rows of bricks
const short int numbricks = 55;     //(windowl/brickl)*linesOfBricks;
const short int windowl = 800, windowh = 600; //window resolution

//score definition
int score = 0, highscore = 0; //scores of the player during the game session

struct rectobject {
    float spawnx, spawny; //spawn x-position of the bar(and bricks),spawn y-position of the bar(and bricks)
    float length, height; //length and height of the rectangleshape (bar,brick)
    float thick; //thickness of the rectangleshape (bar,brick)
    float velx, vely; //velocity in x axis, velocity in y axis
}; //struct for the properties of any rectangleshape

struct circobject {
    float spawnx, spawny; //spawn x-position of the ball, spawn y-position of the ball
    float r; //radius of the ball
    float thick; //thickness of the ball
    float velx, vely; //velocity in x axis, velocity in y axis

};

void assignBrick(rectobject bricks0[]); //it sets the spawn position array of struct of the bricks
void placeBrick(sf::Sprite brickso[], rectobject bricks0[]); //draws the bricks into the game
void updateScore(sf::Text MyScore, int score); //updates the score everytime i call the function
void updateHighscore(sf::Text& Highscore, int highscore); //checks updates the last score everytime i call the function

int main()
{
    //counter of broke bricks
    int broke = 0; //number of bricks broke, used to replace bricks after total breaking

    //assigning rectobject bar0
    rectobject bar0 = { windowl / 2,570,150,30,4,6,0 };

    //assigning rectobject bricks0
    rectobject bricks0[numbricks] = { 0,0,(float)72.0,(float)42.0,0,0,0 };

    //assigning circobject ball0
    circobject ball0 = { bar0.spawnx,bar0.spawny - 60,10,3.5,5,5 };

    //game checks
    bool space = true; //checks if the player pressed space when the ball is still in the game
    bool juststarted = false; //checks if the player pressed space to start the game
    bool touchedbar = false; //checks if the ball touched the bar
    bool start = true; //checks if the menu is displayed
    bool playpressed = false; //plays the game if the player chose "play" from the menu
    bool scorespressed = false; //checks if the player chose "Scores" from the menu

    //fonts definition
    sf::Font font;
    font.loadFromFile("Fonts/arial.ttf");

    //buffers definition
    sf::SoundBuffer bounceffer;
    bounceffer.loadFromFile("matdata/smb_kick.wav");

    sf::SoundBuffer YAGAMED;
    YAGAMED.loadFromFile("matdata/YA GAMED.wav");

    //sounds definition
    sf::Sound Bounce;
    Bounce.setBuffer(bounceffer);
    Bounce.setPitch(0.4); //set the pitch of the bounce sound to 0.4

    sf::Sound WOW;
    WOW.setBuffer(YAGAMED);
    WOW.setPitch(1.3);

    sf::Sound Gameover;
    Gameover.setBuffer(YAGAMED);
    Gameover.setPitch(0.7);

    //music definition
    sf::Music music;
    music.openFromFile("matdata/music.wav");
    music.setVolume(60);//make the volume of the music quieter
    music.setPitch(1.3);

    //image definition
    sf::Image icon;
    icon.loadFromFile("matdata/icon.png");

    //window definition
    sf::RenderWindow window(sf::VideoMode(windowl, windowh), "Brick Breaker", sf::Style::Default);
    window.setIcon(500, 343, icon.getPixelsPtr());
    window.setFramerateLimit(60); //let the game be 60 frames per second

    //textures definition
    sf::Texture galaxy;
    sf::Texture brick;
    sf::Texture bartex;
    sf::Texture menubg;
    galaxy.loadFromFile("matdata/galaxy5.jpg");     //used for the game's background
    galaxy.setSmooth(true);                        //let the background be smoother
    brick.loadFromFile("matdata/brick.png");      //used for the brick's texture
    bartex.loadFromFile("matdata/bartex.png");   //used for the bar texture
    menubg.loadFromFile("matdata/menu.png");    //used to display the menu background

    //bar definition
    sf::RectangleShape bar(sf::Vector2f(bar0.length, bar0.height));
    bar.setOrigin((float)bar0.length / 2, (float)bar0.height / 2);
    bar.setPosition(bar0.spawnx, bar0.spawny);
    bar.setTexture(&bartex);
    /** Old properties before linking it with texture
    *bar.setFillColor(sf::Color(0, 0, 55));
    *bar.setOutlineThickness(bar0.thick);
    *bar.setOutlineColor(sf::Color(255, 255, 255));
    **/

    //brick definition
    sf::Sprite  brickso[numbricks];
    assignBrick(bricks0); //assigning bricks positions in the bricks struct
    placeBrick(brickso, bricks0); //setting the position values inside the brickso sprites position
    for (int i = 0; i < numbricks - 10; i++) {
        brickso[i].setTexture(brick);
        if (bricks0[i].spawny == (84) || bricks0[i].spawny == 0) { //84= 2*bricks0[i].height

            brickso[i].setColor(sf::Color(180, 1, 131));
        }//turns the color of the bricks in the first and the third row to red 
        if (bricks0[i].spawny == 126 || bricks0[i].spawny == 42) { //126= 3*bricks0[i].height

            brickso[i].setColor(sf::Color(28, 100, 208));
        }//turns the color of the bricks in the second and the fourth row to blue
        brickso[i].setOrigin(0, 0);
        std::cout << brickso[i].getPosition().x << " ";
    }

    //ball definition
    sf::CircleShape ball(ball0.r);
    ball.setOrigin(ball0.r, ball0.r);
    ball.setPosition(ball0.spawnx, ball0.spawny);
    ball.setOutlineThickness(ball0.thick);
    ball.setOutlineColor(sf::Color(0, 0, 0));//black color
    ball.setFillColor(sf::Color(255, 255, 255));//white color



    //background definition
    sf::Sprite background;
    background.setTexture(galaxy);
    background.setScale((float)800 / 1280, (float)600 / 720); //scales the background to the resolution of the game

    //menu background
    sf::Sprite MENUBG;
    MENUBG.setTexture(menubg);
    menubg.setSmooth(true);

    //MyScore definition
    sf::Text MyScore;
    MyScore.setFont(font);
    MyScore.setString(std::to_string(score));//casting the variable score to string
    MyScore.setStyle(sf::Text::Bold);
    MyScore.setPosition(65, 25);
    MyScore.setFillColor(sf::Color::White);
    MyScore.setOutlineColor(sf::Color::Black);
    MyScore.setOutlineThickness(3);
    MyScore.setCharacterSize(45);

    //Highscore definition
    sf::Text Highscore;
    Highscore.setFont(font);
    Highscore.setCharacterSize(43);
    Highscore.setPosition(20, 336);
    Highscore.setFillColor(sf::Color::Black);
    Highscore.setString("High Score: \n" + std::to_string(highscore));
    //Filestreams definition
    std::ifstream readfile;
    std::ofstream writefile("matdata/Scores.txt");

    //display menu before game starts
    Menu menu(windowl, windowh);

    //Game loop starts
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {   //menu event
            if (start == true) {  //checker to display menu
                switch (event.type)
                {
                case sf::Event::KeyReleased:
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Up:
                        menu.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        menu.MoveDown();
                        break;

                    case sf::Keyboard::Return:
                        switch (menu.GetPressedItem())
                        {
                        case 0: //if the player chose "Play" button
                            std::cout << "Play button has been pressed" << std::endl;
                            playpressed = true; //starts the game
                            start = false; //menu vanishes
                            scorespressed = false; //scores vanishes
                            music.play(); //starts the music
                            break;
                        case 1: //if the player chose "Scores" button
                            std::cout << "Scores button has been pressed" << std::endl;
                            scorespressed = true; //displays the highscore and the last score
                            playpressed = false; //pauses the game 
                            break;
                        case 2: //if the player chose "Exit" button
                            window.close();
                            break;
                        }

                        break;
                    }

                    break;
                case sf::Event::Closed: //used to close the game by clicking x button
                    window.close();
                    break;

                }
            }

            if (playpressed) { //game starts
                if (event.type == sf::Event::Closed) { //used to close the game by clicking x button
                    window.close();
                }

                if (event.type == sf::Event::KeyPressed) { //checker of all events of 'if a key is pressed'
                    if (bar.getPosition().x > (windowl - (bar0.length / 2)))  //limting bar position boundaries from right
                    {
                        bar.setPosition(sf::Vector2f(windowl - ((bar0.length / 2)), bar0.spawny));
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (bar.getPosition().x < (bar0.length / 2)) //limiting bar position boundaries from left
                    {
                        bar.setPosition(sf::Vector2f(bar0.length / 2, bar0.spawny));
                    }
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        if (space)
                        {
                            ball.setPosition(ball0.spawnx, ball0.spawny - 200);//place the ball higher
                            juststarted = true; //makes the ball fall straight on the bar
                            space = false; //prevents using space unless permitted
                        }
                    }
                }

                //if the player lose
                if (ball.getPosition().y + ball0.r > windowh) {

                    music.stop(); //stops the music
                    Gameover.play(); //play YA GAMED with low pitch
                    if (writefile.is_open()) //saves the score in an external text file
                    {
                        writefile << std::endl << score;
                    }
                    if (score > highscore) highscore = score; //checks and compare the highscore
                    updateHighscore(Highscore, highscore); //updates the highscore and the last score
                    int msgboxID = MessageBoxA(NULL, "GAME OVER\nTo return to menu, press ESC.", "Retry?", MB_OK | MB_ICONSTOP);

                    if (IDOK) { //if the player pressed "OK" button on the message box it resets the game
                        touchedbar = false;
                        music.play();
                        score = 0;
                        updateScore(MyScore, score);
                        broke = 0;
                        assignBrick(bricks0);//set the bricks position to the default position
                        placeBrick(brickso, bricks0);//place those bricks again
                        ball.setPosition(ball0.spawnx, bar0.spawny - 200);
                        ball.move(0, 0);
                        bar.setPosition(bar0.spawnx, bar0.spawny);
                        if (touchedbar == true) {
                            ball.move(ball0.velx, ball0.vely); //return to the bouncy state after touching the bar
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                        ball.setPosition(ball0.spawnx, ball0.spawny - 200);
                        playpressed = false; //pauses the game
                        start = true;
                        score = 0;
                        broke = 0;
                        Bounce.stop();
                    }
                }

                //if the player gets score divisible by 10 plays YA GAMED
                if (score % 10 == 0 && score != 0) {
                    WOW.play(); //plays YA GAMED
                }

                if (ball.getPosition().y == bar0.spawny && ball.getGlobalBounds().intersects(bar.getGlobalBounds()) && ball.getPosition().x == bar.getPosition().x) {
                    ball0.velx = ball0.velx;
                    ball0.vely = -ball0.vely;
                    ball.move(ball0.velx, ball0.vely);
                }


                //changing the ball's color depending on position and the color at the background
                if (ball.getPosition().x + ball0.r > 468)
                    ball.setFillColor(sf::Color(180, 1, 131));//red
                if (ball.getPosition().x + ball0.r < 468 && ball.getPosition().x - ball0.r > 195)
                    ball.setFillColor(sf::Color(255, 255, 255));//white
                if (ball.getPosition().x - ball0.r < 195)
                    ball.setFillColor(sf::Color(28, 100, 208));//blue

                //replace bricks after total breaking within the current game (endless game)
                if (broke == 45 && ball.getPosition().y >= 300) {
                    assignBrick(bricks0);
                    placeBrick(brickso, bricks0);
                    broke = 0;
                }

                //force bounce on the upper side
                if (ball.getPosition().y <= ball0.r) {
                    ball0.vely = -ball0.vely;
                }
            }
        }

        //Logic handling
        //bar movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //moves the bar right
        {
            bar.move(bar0.velx, bar0.vely);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //moves the bar left
        {
            bar.move(-bar0.velx, bar0.vely);
        }

        //ball gravity
        if (juststarted) {

            ball.move(0, ball0.vely);


            if (touchedbar) //returns the bouncy movement after touching the bar
            {
                ball.move(ball0.velx, ball0.vely);
            }
        }

        //setting boundaries for the ball for the  sides of the game window
        if (ball.getPosition().x - ball0.r<0 || ball.getPosition().x + ball0.r > windowl)
        {
            ball0.velx = -ball0.velx;
            ball.move(ball0.velx, ball0.vely);
        }

        //setting boundaries for the ball for the upper side of the game window
        if (ball.getPosition().y <= ball0.r)
        {
            ball0.vely = -ball0.vely;
            ball.move(ball0.velx, ball0.vely);

        }

        //collision of the bar
        if (ball.getGlobalBounds().intersects(bar.getGlobalBounds()))
        {
            ball0.vely = -ball0.vely;
            ball.move(ball0.velx, ball0.vely);
            touchedbar = true;
        }

        //bouncing on bricks after breaking
        for (int i = 0; i < numbricks - 10; i++) {
            if (ball.getGlobalBounds().intersects(brickso[i].getGlobalBounds()))
            {
                ball0.vely = -ball0.vely;
                ball.move(ball0.velx, ball0.vely);
                Bounce.play();
                brickso[i].setPosition(-100, 0); //vanishes bricks
                score++;
                broke++;
                MyScore.setString(std::to_string(score));
            }
        }

        //Rendering part of the game loop
        window.clear();

        //draw things here -by the order of the layers-
        window.draw(background);
        window.draw(ball);
        window.draw(bar);
        for (int i = 0; i < numbricks - 10; i++)
            window.draw(brickso[i]);

        if (start) //menu appears
        {
            window.draw(MENUBG);
            menu.draw(window);
        }
        if (!start) //score appears when menu is invisible
        {
            window.draw(MyScore);
        }

        if (scorespressed) {
            window.draw(Highscore);
        }

        window.display();
    }


    return 0;
}

void assignBrick(rectobject bricks0[])
{
    bricks0[0].spawnx = 0; bricks0[0].spawny = 0; //sets the first position of the bricks
    int i = 0; //starting rows counter
    //j is the number of current brick
    for (int j = 1; j <= (numbricks) && i < linesOfBricks; j++) { //windowl/bricks0[0].length =11
        bricks0[j].spawnx = bricks0[j - 1].spawnx + bricks0[0].length; //increment the position of the current brick by the length of the brick
        bricks0[j].spawny = i * bricks0[0].height;
        if (j == 45) break;
        if (bricks0[j].spawnx == windowl - 8.0) {
            i++;
            bricks0[j].spawnx = 0;
        }
    }
}//assigning bricks positions in the bricks struct

void placeBrick(sf::Sprite brickso[], rectobject bricks0[]) {
    for (int i = 0; i < numbricks - 10; i++) {
        brickso[i].setPosition(bricks0[i].spawnx, bricks0[i].spawny);
    }
}//setting the position values inside the brickso sprites position

void updateScore(sf::Text MyScore, int score) {
    MyScore.setString(std::to_string(score));
}//updates the score everytime i calls the function

void updateHighscore(sf::Text& Highscore, int highscore) {
    Highscore.setString("High Score: \n" + std::to_string(highscore) + "\nScore History:\n" + std::to_string(score));


}//updates the Highscore everytime i call this function


/* "Developed by:
*__________          __                     ________
 \______   \ ____ _/  |_   ____ _______   /  _____/   ____   ____ _______   ____    ____
 |     ___/_/ __ \\   __\_/ __ \\_  __ \ /   \  ___ _/ __ \ /  _ \\_  __ \ / ___\ _/ __ \
 |    |    \  ___/ |  |  \  ___/ |  | \/ \    \_\  \\  ___/(  <_> )|  | \// /_/  >\  ___/
 |____|     \___  >|__|   \___  >|__|     \______  / \___  >\____/ |__|   \___  /  \___  >
                \/            \/                 \/      \/              /_____/       \/

     ____.                               .__       ____ ___
    |    |  ____   ______  ____  ______  |  |__   |    |   \ ___________     _____  _____
    |    | /  _ \ /  ___/_/ __ \ \____ \ |  |  \  |    |   //  ___/\__  \   /     \ \__  \
/\__|    |(  <_> )\___ \ \  ___/ |  |_> >|   Y  \ |    |  / \___ \  / __ \_|  Y Y  \ / __ \_
\________| \____//____  > \___  >|   __/ |___|  / |______/ /____  >(____  /|__|_|  /(____  /
                      \/      \/ |__|         \/                \/      \/       \/      \/

 ___________            .___          _________       ___.    .__
\_   _____/_____     __| _/___.__.  /   _____/  ____ \_ |__  |  |__  ___.__.
 |    __)  \__  \   / __ |<   |  |  \_____  \  /  _ \ | __ \ |  |  \<   |  |
 |     \    / __ \_/ /_/ | \___  |  /        \(  <_> )| \_\ \|   Y  \\___  |
 \___  /   (____  /\____ | / ____| /_______  / \____/ |___  /|___|  // ____|
     \/         \/      \/ \/              \/             \/      \/ \/


__________ .__         .__                        _____   .__ .__               .___
\______   \|__|  ______|  |__    ____  ___.__.   /     \  |__||  |  _____     __| _/
 |    |  _/|  | /  ___/|  |  \  /  _ \<   |  |  /  \ /  \ |  ||  |  \__  \   / __ |
 |    |   \|  | \___ \ |   Y  \(  <_> )\___  | /    Y    \|  ||  |__ / __ \_/ /_/ |
 |______  /|__|/____  >|___|  / \____/ / ____| \____|__  /|__||____/(____  /\____ |
        \/          \/      \/         \/              \/                \/      \/


     ____.                 ___________                                      .__   .__
    |    |_____     ____   \_   _____/_____  _______ _____     ____ _____   |  |  |  |  _____
    |    |\__  \   /    \   |    __)  \__  \ \_  __ \\__  \   / ___\\__  \  |  |  |  |  \__  \
/\__|    | / __ \_|   |  \  |     \    / __ \_|  | \/ / __ \_/ /_/  >/ __ \_|  |__|  |__ / __ \_
\________|(____  /|___|  /  \___  /   (____  /|__|   (____  /\___  /(____  /|____/|____/(____  /
               \/      \/       \/         \/             \//_____/      \/                  \/


     ____.        .__              __________             .___
    |    |  ____  |  |__    ____   \______   \  ____    __| _/_____
    |    | /  _ \ |  |  \  /    \   |       _/_/ __ \  / __ | \__  \
/\__|    |(  <_> )|   Y  \|   |  \  |    |   \\  ___/ / /_/ |  / __ \_
\________| \____/ |___|  /|___|  /  |____|_  / \___  >\____ | (____  /
                               \/          \/      \/     \/       \/
* */
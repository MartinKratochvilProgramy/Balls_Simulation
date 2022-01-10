#include <SFML/Graphics.hpp>
#include <math.h>   
#include <vector>

# define PI 3.14159265358979323846  /* pi */

using namespace std;

const int width = 1280;
const int height = 720;

const int numberOfBalls = 500;

float R, fi, F;
float d, dot1, dot2;
float vInit = 0.1;
float radius = 1;
const float c_rest = 0.25;
float fxi, fyi, fxj, fyj;
float dec = 0.05;
const float G = 0.2;


struct Ball
{
    float x, y, vx, vy, r;
    Ball(float ix, float iy, float ivx, float ivy, float ir)
    {
        x = ix;
        y = iy;
        vx = ivx;
        vy = ivy;
        r = ir;
    }
    
    void move() {
        x += vx;
        y += vy;
    }
    void check_out_of_bounds() {
        if (x - radius / 2 < 0 || x + radius / 2 > width) vx *= -1;
        if (y - radius / 2 < 0 || y + radius / 2 > height) vy *= -1;
    }
};


int main()
{

    vector<Ball> balls;
    sf::RenderWindow window(sf::VideoMode(width, height), "My Program");
    window.setFramerateLimit(60);


    //inicializace poloh a rychlosti
    for (int i = 0; i < numberOfBalls ; i++) {
        float x_init = width * 0.2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (width * 0.8 - width * 0.2)));
        float y_init = height * 0.2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (height * 0.8 - height * 0.2)));
        float vx_init = -vInit + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (vInit - (-vInit))));
        float vy_init = -vInit + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (vInit - (-vInit))));
        if (vx_init == 0) vx_init = 1;
        if (vy_init == 0) vy_init = 1;
        balls.push_back({ x_init, y_init, vx_init, vy_init, radius});
    }



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        //"physics"

        for (int i = 0; i < numberOfBalls; i++) {
            for (int j = i; j < numberOfBalls; j++) {
                R = sqrt(pow(balls[i].x - balls[j].x, 2) + pow(balls[i].y - balls[j].y, 2));
                //calculate gravity
                if (R > radius * 2.5) {
                    fi = atan((balls[i].x - balls[j].x) / (balls[i].y - balls[j].y));

                    F = G / (pow(balls[i].x - balls[j].x, 2) + pow(balls[i].y - balls[j].y, 2));

                    if (balls[i].y - balls[j].y > 0) F *= -1;

                    fxi = F * sin(fi);
                    fyi = F * cos(fi);
                    fxj = -fxi;
                    fyj = -fyi;

                    balls[i].vx += fxi;
                    balls[i].vy += fyi;
                    balls[j].vx += fxj;
                    balls[j].vy += fyj;

                }

                //calc deflextion
                if (R <= radius * 2 && R > radius * 0.1) {

                    d = pow(sqrt(pow(balls[i].x - balls[j].x, 2) + pow(balls[i].y - balls[j].y, 2)), 2);
                    dot1 = (balls[i].x - balls[j].x) * (balls[i].vx - balls[j].vx) + (balls[i].y - balls[j].y) * (balls[i].vy - balls[j].vy);
                    dot2 = (balls[j].x - balls[i].x) * (balls[j].vx - balls[i].vx) + (balls[j].y - balls[i].y) * (balls[j].vy - balls[i].vy);

                    balls[i].vx = balls[i].vx - dot1 / d * (balls[i].x - balls[j].x) * c_rest;
                    balls[i].vy = balls[i].vy - dot1 / d * (balls[i].y - balls[j].y) * c_rest;
                    balls[j].vx = balls[j].vx - dot2 / d * (balls[j].x - balls[i].x) * c_rest;
                    balls[j].vy = balls[j].vy - dot2 / d * (balls[j].y - balls[i].y) * c_rest;


                }
            }
        }


        window.clear();
        //iterate through balls and render
        for (int i = 0; i < numberOfBalls; i++) {


            balls[i].check_out_of_bounds();
            balls[i].move();

            sf::CircleShape circle;
            circle.setPosition(balls[i].x, balls[i].y);
            circle.setRadius(radius);
            circle.setFillColor(sf::Color::Red);

            window.draw(circle);
        }

        window.display();
    }



    return 0;
}
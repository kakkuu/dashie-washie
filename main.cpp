#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
using namespace std;

const float shakiness = 20;
const bool stayinplace = false;

class Object : public sf::Drawable {
    virtual void update(float delta) = 0;
};

class Program {

};

class DashWash : public Object {
    bool washable;
    sf::Sprite dashie, washie;
    sf::Texture dashietexture, washietexture;
    float rotatespeed;

public:
    DashWash(string dashpath, string washpath) {
        sf::Texture &dashie = dashietexture, &washie = washietexture;
        washable = true;
        if (!dashie.loadFromFile(dashpath)) washable = false;
        if (!washie.loadFromFile(washpath)) washable = false;

        if (!washable) {
            cout << "[DashWash::DashWash] Failed loading textures from \"" <<
                dashpath << "\" and \"" << washpath << "\"." << endl;
        }
        dashie.setSmooth(true);
        washie.setSmooth(true);
        this->dashie.setTexture(dashie);
        this->washie.setTexture(washie);

        this->dashie.setPosition(sf::Vector2f(128, 172));
        this->dashie.setOrigin(sf::Vector2f(100,100));
        this->dashie.setScale(sf::Vector2f(0.85, 0.85));

        rotatespeed = 1;
    }

    float getSpeed() const { return rotatespeed; }

    virtual void update(float delta) {
        if (!washable) return;
        dashie.rotate(360*rotatespeed*delta);
    }

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        if (!washable) return;
        target.draw(dashie);
        target.draw(washie);
    }
};

int main(int argc, char **args)
{
    sf::RenderWindow window(sf::VideoMode(258, 342), "Dashie Washie");
    window.setFramerateLimit(60);

    DashWash dw("dashie.png", "washie.png");

    minstd_rand0 rnd(111);
    sf::Vector2i winpos = window.getPosition();

    sf::Clock clock;
    while (window.isOpen())
    {
        // delta time
        float delta = clock.restart().asSeconds();

        // events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        // update everything
        dw.update(delta);

        int shake = shakiness * dw.getSpeed();
        sf::Vector2i refpos;
        refpos = ((stayinplace)?(winpos):(window.getPosition()) );
        window.setPosition(refpos + sf::Vector2i(
                rnd()%max(shake*2,1)-shake,
                rnd()%max(shake*2,1)-shake
            )
        );


        // actual drawing
        window.clear();
        dw.draw(window, sf::RenderStates());
        window.display();
    }

    return 0;
}


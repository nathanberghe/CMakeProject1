#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

using namespace std;
using namespace sf;

float random(float min, float max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}


int main()
{
    RenderWindow window(VideoMode(800, 600), "Ligne de Metro Courbe SFML");

    Vector2f p0(50.0f, 300.0f);
    Vector2f p1(200.0f, 100.0f);
    Vector2f p2(400.0f, 500.0f);
    Vector2f p3(750.0f, 300.0f);

    vector<CircleShape> metroLineStops;
    int numStops = 8;

    for (int i = 0; i <= numStops; ++i) {
        float t = static_cast<float>(i) / numStops;
        Vector2f position = (1 - t) * (1 - t) * (1 - t) * p0 +
            3 * (1 - t) * (1 - t) * t * p1 +
            3 * (1 - t) * t * t * p2 +
            t * t * t * p3;

        CircleShape stop(10.0f);
        stop.setFillColor(Color::Red);
        stop.setPosition(position);

        metroLineStops.push_back(stop);
    }

    RectangleShape train(Vector2f(30.0f, 15.0f));


    train.setFillColor(Color::Green);
    train.setPosition(metroLineStops[0].getPosition().x - 15.0f, metroLineStops[0].getPosition().y - 7.5f);
    float trainSpeed = 0.3f;
    unsigned int currentStop = 0;
    Clock stopClock; // Utilisé pour mesurer le temps d'arrêt à chaque station
    const Time stopTime = seconds(10.0f); // Durée d'arrêt à chaque station

    // Boucle principale
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // Déplacement du train
        if (currentStop < metroLineStops.size() - 1) {
            Vector2f direction = metroLineStops[currentStop + 1].getPosition() - train.getPosition();
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            Vector2f unitDirection = direction / length;

            train.move(unitDirection * trainSpeed);

            if (length < trainSpeed) {
                // Le train a atteint la prochaine station
                currentStop++;
                train.setPosition(metroLineStops[currentStop].getPosition().x - 15.0f,
                    metroLineStops[currentStop].getPosition().y - 7.5f);

                // Réinitialiser le chronomètre d'arrêt
                stopClock.restart();
            }
        }

        window.clear();

        for (int i = 1; i < numStops + 1; ++i) {
            // Dessiner la ligne de métro courbe avec les arrêts
            Vertex line[] = {
                Vertex(metroLineStops[i - 1].getPosition() + Vector2f(5.0f, 5.0f)),
                Vertex(metroLineStops[i].getPosition() + Vector2f(5.0f, 5.0f))
            };
            window.draw(line, 2, Lines);

            // Dessiner une deuxième ligne 10 pixels en dessous de la première ligne
            Vertex lineBelow[] = {
                Vertex(metroLineStops[i - 1].getPosition() + Vector2f(5.0f, 5.0f) + Vector2f(0.0f, 10.0f)),
                Vertex(metroLineStops[i].getPosition() + Vector2f(5.0f, 5.0f) + Vector2f(0.0f, 10.0f))
            };
            window.draw(lineBelow, 2, Lines);
        }

        // Dessiner les arrêts 
        for (const auto& stop : metroLineStops) {
            window.draw(stop);
        }

        // Dessiner le train
        window.draw(train);

        window.display();
    }
    return 0;
}

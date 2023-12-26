#include "TrainStation.h"
#include <iostream>

int Station::prendrePassagers(int nombre) {
    int pris = std::min(passagers, nombre);
    passagers -= pris;
    return pris;
}

void Station::dessiner(sf::RenderWindow& window, const sf::Font& font) {
    sf::CircleShape shape(10);
    shape.setPosition(position.x - 10, position.y - 10);
    shape.setFillColor(sf::Color::Green);
    window.draw(shape);

    // Texte pour le nombre de passagers
    sf::Text passagersText(std::to_string(passagers) + " passagers", font, 12);
    passagersText.setFillColor(sf::Color::White);
    passagersText.setPosition(position.x, position.y + 20);
    window.draw(passagersText);

    // Texte pour le nom de la station
    sf::Text nomText(nom, font, 15);
    nomText.setFillColor(sf::Color::White);
    nomText.setPosition(position.x - 10, position.y - 30);
    window.draw(nomText);
}


void Train::ajouterStation(const Station& station) {
    parcours.push_back(station);
}

void Train::avancer(float deltaTime) {
    if (tempsAttente > 0) {
        tempsAttente -= deltaTime;
        return;
    }

    if (stationActuelle >= parcours.size()) {
        enMouvement = false;
        return;
    }

    position.x += vitesse.x * deltaTime;
    if (position.x >= parcours[stationActuelle].getPosition().x) {
        position.x = parcours[stationActuelle].getPosition().x;
        passagers += parcours[stationActuelle].prendrePassagers(5);
        tempsAttente = 5.0f;
        stationActuelle++;
    }
}

void Train::dessiner(sf::RenderWindow& window, const sf::Font& font) {
    sf::RectangleShape shape(sf::Vector2f(20, 10));
    shape.setPosition(position.x - 10, position.y - 5);
    shape.setFillColor(sf::Color::Red);
    window.draw(shape);

    sf::Text text(std::to_string(passagers) + " passagers", font, 12);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x, position.y - 20);
    window.draw(text);
}

void GestionnaireDeTrain::ajouterTrain(std::unique_ptr<Train> train) {
    trains.push_back(std::move(train));
}

void GestionnaireDeTrain::mettreAJour(float deltaTime) {
    for (auto& train : trains) {
        train->avancer(deltaTime);
    }
}

void GestionnaireDeTrain::dessiner(sf::RenderWindow& window, const sf::Font& font) {
    for (auto& train : trains) {
        train->dessiner(window, font);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Train Simulation");
    GestionnaireDeTrain gestionnaire;
    sf::Font font;

    if (!font.loadFromFile("C:/Program Files/SFML/font/arial.ttf")) {
        std::cerr << "Erreur de chargement de la police 'arial.ttf'" << std::endl;
        return 1;
    }

    Station paris("Paris", { 100, 300 }, 50);
    Station lyon("Lyon", { 250, 300 }, 60);
    Station marseille("Marseille", { 400, 300 }, 40);
    Station lille("Lille", { 550, 300 }, 30);
    Station rennes("Rennes", { 700, 300 }, 20);
    Station toulouse("Toulouse", { 850, 300 }, 25);
    Station bruges("Bruges", { 1000, 300 }, 15);

    auto train1 = std::make_unique<Train>(Coordonnees(50, 300));
    train1->ajouterStation(paris);
    train1->ajouterStation(lyon);
    train1->ajouterStation(marseille);
    train1->ajouterStation(lille);
    train1->ajouterStation(rennes);
    train1->ajouterStation(toulouse);
    train1->ajouterStation(bruges);
    train1->setVitesse(Coordonnees(50, 0));

    gestionnaire.ajouterTrain(std::move(train1));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear();

        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(paris.getPosition().x, paris.getPosition().y)),
            sf::Vertex(sf::Vector2f(lyon.getPosition().x, lyon.getPosition().y)),
            sf::Vertex(sf::Vector2f(marseille.getPosition().x, marseille.getPosition().y)),
            sf::Vertex(sf::Vector2f(lille.getPosition().x, lille.getPosition().y)),
            sf::Vertex(sf::Vector2f(rennes.getPosition().x, rennes.getPosition().y)),
            sf::Vertex(sf::Vector2f(toulouse.getPosition().x, toulouse.getPosition().y)),
            sf::Vertex(sf::Vector2f(bruges.getPosition().x, bruges.getPosition().y))
        };
        window.draw(line, 7, sf::LinesStrip);

        paris.dessiner(window, font);
        lyon.dessiner(window, font);
        marseille.dessiner(window, font);
        lille.dessiner(window, font);
        rennes.dessiner(window, font);
        toulouse.dessiner(window, font);
        bruges.dessiner(window, font);

        gestionnaire.mettreAJour(deltaTime);
        gestionnaire.dessiner(window, font);

        window.display();
    }

    return 0;
}

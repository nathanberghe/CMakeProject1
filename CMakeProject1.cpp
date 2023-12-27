#include "TrainStation.h"
#include <iostream>

int Station::prendrePassagers() {
    int pris = passagers;
    passagers = 0;
    return pris;
}

void Station::reinitialiserEtGenererPassagers() {
    passagers = rand() % 50 + 10;  // Générer un nouveau nombre aléatoire de passagers
}


void Station::dessiner(sf::RenderWindow& window, const sf::Font& font) {
    sf::CircleShape shape(10);
    shape.setPosition(position.x - 10, position.y - 10);
    shape.setFillColor(sf::Color::Green);
    window.draw(shape);

    sf::Text text(nom + " " + std::to_string(passagers), font, 15);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x - 10, position.y - 30);
    window.draw(text);
}

void Train::ajouterStation(const Station& station) {
    parcours.push_back(station);
}

void Train::avancer(float deltaTime) {
    if (tempsAttente > 0) {
        tempsAttente -= deltaTime;
        // Vérifiez si le temps d'attente est terminé et si le train est à une station.
        if (tempsAttente <= 0 && stationActuelle < parcours.size()) {
            // Le train se prépare à partir.
            enMouvement = true;  // Assurez-vous que le train est défini pour se déplacer
        }
        return;
    }

    // Si le train est en mouvement et pas encore arrivé à la prochaine station.
    if (enMouvement && stationActuelle < parcours.size()) {
        position.x += vitesse.x * deltaTime;
        // Vérifiez si le train a atteint la prochaine station.
        if (position.x >= parcours[stationActuelle].getPosition().x) {
            position.x = parcours[stationActuelle].getPosition().x;
            passagers += parcours[stationActuelle].prendrePassagers();
            enMouvement = false;  // Le train s'arrête à la station
            tempsAttente = 5.0f;  // Démarrer le temps d'attente à la station
            stationActuelle++;  // Passer à la station suivante pour le prochain mouvement
        }
    }
}


void Train::dessiner(sf::RenderWindow& window, const sf::Font& font) {
    sf::RectangleShape shape(sf::Vector2f(20, 10));
    shape.setPosition(position.x - 10, position.y - 5);
    shape.setFillColor(sf::Color::Red);
    window.draw(shape);

    sf::Text text("Passagers: " + std::to_string(passagers), font, 12);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x, position.y - 20);
    window.draw(text);
}

void GestionnaireDeTrain::ajouterTrain(std::unique_ptr<Train> train) {
    trains.push_back(std::move(train));
}

void GestionnaireDeTrain::mettreAJour(float deltaTime) {
    static float tempsDepuisDerniereGeneration = 0.0f;
    tempsDepuisDerniereGeneration += deltaTime;

    // Mise à jour des trains
    for (auto& train : trains) {
        train->avancer(deltaTime);
    }

    // Régénération des passagers dans chaque station après un délai
    if (tempsDepuisDerniereGeneration > 10.0f) {
        // Pour chaque train, pour chaque station, réinitialiser et générer des passagers
        tempsDepuisDerniereGeneration = 0.0f;
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

    srand(static_cast<unsigned>(time(nullptr)));

    Station paris("Paris", { 100, 300 });
    Station lyon("Lyon", { 250, 300 });
    Station marseille("Marseille", { 400, 300 });
    Station lille("Lille", { 500, 300 });
    Station rennes("Rennes", { 700,300 });
    Station toulouse("Toulouse", { 800,300 });
    Station bruges("Bruges", { 950,300 });

    auto train1 = std::make_unique<Train>(Coordonnees(100, 300));
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

        window.clear(sf::Color::Black);

        gestionnaire.mettreAJour(deltaTime);

        // Dessiner les lignes entre les stations
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(paris.getPosition().x, paris.getPosition().y)),
            sf::Vertex(sf::Vector2f(lyon.getPosition().x, lyon.getPosition().y)),
            sf::Vertex(sf::Vector2f(marseille.getPosition().x, marseille.getPosition().y)),
            sf::Vertex(sf::Vector2f(bruges.getPosition().x, bruges.getPosition().y)),
            sf::Vertex(sf::Vector2f(rennes.getPosition().x, rennes.getPosition().y)),
            sf::Vertex(sf::Vector2f(toulouse.getPosition().x, toulouse.getPosition().y)),
            sf::Vertex(sf::Vector2f(lille.getPosition().x, lille.getPosition().y))
        };
        window.draw(line, 7, sf::LinesStrip);

        paris.dessiner(window,font);
        lyon.dessiner(window,font);
        marseille.dessiner(window,font);
        lille.dessiner(window,font);
        toulouse.dessiner(window,font);
        rennes.dessiner(window,font);
        bruges.dessiner(window,font);

        gestionnaire.dessiner(window, font);

        window.display();
    }

    return 0;
}

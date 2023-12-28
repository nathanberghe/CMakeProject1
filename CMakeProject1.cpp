﻿#include "TrainStation.h"
#include <iostream>

int Station::prendrePassagers() {
    int pris = passagers;
    std::cout << "Station " << nom << " a " << passagers << " passagers avant le depart du train." << std::endl;
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
    if (estArrive) {
        return; // Si le train est arrivé, il ne bouge plus
    }
    if (tempsAttente > 0) {
        tempsAttente -= deltaTime;
        if (tempsAttente <= 0) {
            enMouvement = true;
            if (stationActuelle < parcours.size()) {
                std::cout << "Le train " << numeroTrain << " quitte la station " << parcours[stationActuelle - 1].getNom() << std::endl;
            }
        }
        return;
    }

    if (enMouvement && stationActuelle < parcours.size()) {
        position.x += vitesse.x * deltaTime;
        if (position.x >= parcours[stationActuelle].getPosition().x) {
            position.x = parcours[stationActuelle].getPosition().x;
            enMouvement = false;
            std::cout << "//////" << std::endl;
            std::cout << "Le train "<< numeroTrain << " est arrive a la station " << parcours[stationActuelle].getNom() << std::endl;

            // À la dernière station, tous les passagers descendent
            if (stationActuelle == parcours.size() - 1) {
                std::cout << "Le train" << numeroTrain << " depose tous les " << passagers << " passagers a la station " << parcours[stationActuelle].getNom() << std::endl;
                passagers = 0;
                estArrive = true; // Marquez le train comme étant arrivé à destination
               
            }
            else { // Sinon, déposer entre 40% et 80% des passagers
                int minPassagersADeposer = passagers * 40 / 100;
                int maxPassagersADeposer = passagers * 80 / 100;
                int passagersADeposer = minPassagersADeposer + rand() % (maxPassagersADeposer - minPassagersADeposer + 1);
                passagers -= passagersADeposer;
                std::cout << "Le train" << numeroTrain << " depose " << passagersADeposer << " passagers a la station " << parcours[stationActuelle].getNom() << std::endl;
                // Prendre les passagers de la station actuelle
                passagers += parcours[stationActuelle].prendrePassagers();
                std::cout << "Le train " << numeroTrain << " prend " << passagers << " passagers" << std::endl;
                parcours[stationActuelle].reinitialiserEtGenererPassagers();
            }
            tempsAttente = 5.0f;  // Attente à la station avant de partir
            if (stationActuelle < parcours.size() - 1) {
                stationActuelle++;  // Passer à la station suivante pour le prochain mouvement
            }
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
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Train Simulation");
    GestionnaireDeTrain gestionnaire;
    GestionnaireDeStations gestionnaireStations;
    sf::Font font;
    sf::Clock trainClock;  // Horloge pour suivre le temps pour l'ajout des trains
    int trainIndex = 0;


    if (!font.loadFromFile("C:/Program Files/SFML/font/arial.ttf")) {
        std::cerr << "Erreur de chargement de la police 'arial.ttf'" << std::endl;
        return 1;
    }

    srand(static_cast<unsigned>(time(nullptr)));

    //SENS 1
    Station paris("Paris", { 100, 300 });
    paris.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(paris);
    Station lyon("Lyon", { 250, 300 });
    lyon.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(lyon);
    Station marseille("Marseille", { 400, 300 });
    marseille.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(marseille);
    Station lille("Lille", { 500, 300 });
    lille.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(lille);
    Station rennes("Rennes", { 700,300 });
    rennes.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(rennes);
    Station toulouse("Toulouse", { 800,300 });
    toulouse.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(toulouse);
    Station bruges("Bruges", { 950,300 });
    bruges.reinitialiserEtGenererPassagers();
    gestionnaireStations.ajouterStation(bruges);

    //SENS 2

    Station paris2("Paris2", {100, 400});
    Station lyon2("Lyon2", { 250, 400 });
    Station marseille2("Marseille2", { 400, 400 });
    Station lille2("Lille2", { 500, 400 });
    Station rennes2("Rennes2", { 700,400 });
    Station toulouse2("Toulouse2", { 800,400 });
    Station bruges2("Bruges2", { 950,400 });

    std::vector<std::unique_ptr<Train>> trainsPrepares;

    for (int i = 0; i < 2; ++i) {
        auto train = std::make_unique<Train>(Coordonnees(100, 300), i + 1);
        train->ajouterStation(paris);
        train->ajouterStation(lyon);
        train->ajouterStation(marseille);
        train->ajouterStation(lille);
        train->ajouterStation(rennes);
        train->ajouterStation(toulouse);
        train->ajouterStation(bruges);
        train->setVitesse(Coordonnees(50, 0));
        trainsPrepares.push_back(std::move(train));
    }

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

        // Gérer l'ajout des trains
        if (trainIndex < trainsPrepares.size() && trainClock.getElapsedTime().asSeconds() >= 20) {
            gestionnaire.ajouterTrain(std::move(trainsPrepares[trainIndex]));
            trainIndex++;
            trainClock.restart(); // Redémarrer le compteur pour le prochain train
        }

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

        sf::Vertex line2[] = {
            sf::Vertex(sf::Vector2f(paris2.getPosition().x, paris2.getPosition().y)),
            sf::Vertex(sf::Vector2f(lyon2.getPosition().x, lyon2.getPosition().y)),
            sf::Vertex(sf::Vector2f(marseille2.getPosition().x, marseille2.getPosition().y)),
            sf::Vertex(sf::Vector2f(bruges2.getPosition().x, bruges2.getPosition().y)),
            sf::Vertex(sf::Vector2f(rennes2.getPosition().x, rennes2.getPosition().y)),
            sf::Vertex(sf::Vector2f(toulouse2.getPosition().x, toulouse2.getPosition().y)),
            sf::Vertex(sf::Vector2f(lille2.getPosition().x, lille2.getPosition().y))
        };
        window.draw(line2, 7, sf::LinesStrip);

        for (auto& station : gestionnaireStations.getStations()) {
            station.dessiner(window, font);
        }

        gestionnaire.mettreAJour(deltaTime);
        gestionnaire.dessiner(window, font);

        window.display();
    }

    return 0;
}

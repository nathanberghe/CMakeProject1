#include "TrainStation.h"
#include <iostream>

Station paris("Paris", { 100, 300 });
Station lyon("Lyon", { 250, 300 });
Station marseille("Marseille", { 400, 300 });
Station lille("Lille", { 500, 300 });
Station rennes("Rennes", { 700,300 });
Station toulouse("Toulouse", { 800,300 });
Station bruges("Bruges", { 950,300 });

Station paris2("Paris2", { 100, 400 });
Station lyon2("Lyon2", { 250, 400 });
Station marseille2("Marseille2", { 400, 400 });
Station lille2("Lille2", { 500, 400 });
Station rennes2("Rennes2", { 700, 400 });
Station toulouse2("Toulouse2", { 800, 400 });
Station bruges2("Bruges2", { 950, 400 });

int Station::prendrePassagers() {
    int pris = passagers;
    std::cout << "Station " << nom << " a " << passagers << " passagers avant le depart du train." << std::endl;
    passagers = 0;
    return pris;
}

void Station::reinitialiserEtGenererPassagers() {
    passagers = rand() % 50 + 10;  // Générer un nouveau nombre aléatoire de passagers
}

void Station::mettreAJour(float deltaTime) {
    tempsDepuisDerniereRegeneration += deltaTime;
    if (tempsDepuisDerniereRegeneration >= intervalleDeRegeneration) {
        reinitialiserEtGenererPassagers();
        tempsDepuisDerniereRegeneration = 0.0f;
    }
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
        return;
    }

    if (tempsAttente > 0) {
        tempsAttente -= deltaTime;
        if (tempsAttente <= 0) {
            enMouvement = true;
            if (stationActuelle < parcours.size()) {
                std::cout << "Le train " << numeroTrain << " quitte la station " << parcours[stationActuelle-1].getNom() << std::endl;
            }
        }
        return;
    }

    if (enMouvement && stationActuelle < parcours.size()) {
        position.x += vitesse.x * deltaTime;
        if (position.x >= parcours[stationActuelle].getPosition().x) {
            position.x = parcours[stationActuelle].getPosition().x;
            enMouvement = false;
            std::cout << "Le train " << numeroTrain << " est arrive a la station " << parcours[stationActuelle].getNom() << std::endl;

            if (stationActuelle == parcours.size() - 1) {
                std::cout << "Le train " << numeroTrain << " depose tous les passagers a la station " << parcours[stationActuelle].getNom() << std::endl;
                passagers = 0;
                estSurParcoursRetour = !estSurParcoursRetour;
                parcours.clear();

                if (estSurParcoursRetour) {
                    position.y = 400; // Position Y pour le parcours retour
                    ajouterStation(bruges2);
                    ajouterStation(toulouse2);
                    ajouterStation(rennes2);
                    ajouterStation(lille2);
                    ajouterStation(marseille2);
                    ajouterStation(lyon2);
                    ajouterStation(paris2);
                    setVitesse(Coordonnees(-50, 0)); // Vitesse pour le parcours retour
                }
                else {
                    position.y = 300; // Position Y pour le parcours aller
                    ajouterStation(paris);
                    ajouterStation(lyon);
                    ajouterStation(marseille);
                    ajouterStation(lille);
                    ajouterStation(rennes);
                    ajouterStation(toulouse);
                    ajouterStation(bruges);
                    setVitesse(Coordonnees(50, 0)); 
                }

                std::cout << "Changement de sens : Le train " << numeroTrain << " repart vers le " << (estSurParcoursRetour ? "parcours retour." : "parcours aller.") << std::endl;
                stationActuelle = 0;
                estArrive = false;
            }

            else {
                int minPassagersADeposer = passagers * 40 / 100;
                int maxPassagersADeposer = passagers * 80 / 100;
                int passagersADeposer = minPassagersADeposer + rand() % (maxPassagersADeposer - minPassagersADeposer + 1);
                passagers -= passagersADeposer;
                parcours[stationActuelle].reinitialiserEtGenererPassagers();

                passagers += parcours[stationActuelle].prendrePassagers();
                std::cout << "Le train " << numeroTrain << " prend " << passagers << " passagers" << std::endl;
                parcours[stationActuelle].reinitialiserEtGenererPassagers();

                tempsAttente = 5.0f;
                stationActuelle++;
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

/* GESTIONNAIRE DE TRAIN*/

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

/*GESTIONNAIRE DE STATION*/

void GestionnaireDeStations::ajouterStation(const Station& station) {
    stations.push_back(station);
}

void GestionnaireDeStations::dessinerStations(sf::RenderWindow& window, const sf::Font& font) {
    for (auto& station : stations) {
        station.dessiner(window, font);
    }
}

void GestionnaireDeStations::mettreAJour(float deltaTime) {
    for (auto& station : stations) {
        station.mettreAJour(deltaTime);
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
    gestionnaireStations.ajouterStation(paris);
    gestionnaireStations.ajouterStation(lyon);
    gestionnaireStations.ajouterStation(marseille);
    gestionnaireStations.ajouterStation(lille);
    gestionnaireStations.ajouterStation(rennes);
    gestionnaireStations.ajouterStation(toulouse);
    gestionnaireStations.ajouterStation(bruges);

    //SENS 2

    gestionnaireStations.ajouterStation(paris2);
    gestionnaireStations.ajouterStation(lyon2);
    gestionnaireStations.ajouterStation(marseille2);
    gestionnaireStations.ajouterStation(lille2);
    gestionnaireStations.ajouterStation(rennes2);
    gestionnaireStations.ajouterStation(toulouse2);
    gestionnaireStations.ajouterStation(bruges2);

    std::vector<std::unique_ptr<Train>> trainsPrepares;

    for (int i = 0; i < 3; ++i) {
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
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    gestionnaire.arreterTousLesTrains();
                    std::cout << "Arret d'urgence active. Tous les trains sont arretes." << std::endl;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::B) {
                    gestionnaire.redemarrerTousLesTrains();
                    std::cout << "Les trains ont redemarre, l'arret d'urgence est fini." << std::endl;
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        gestionnaireStations.mettreAJour(deltaTime);
        gestionnaire.mettreAJour(deltaTime);


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
        gestionnaireStations.dessinerStations(window, font);
        gestionnaire.dessiner(window, font);

        window.display();
    }

    return 0;
}



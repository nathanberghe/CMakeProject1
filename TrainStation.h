#ifndef TRAINSTATION_H
#define TRAINSTATION_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

struct Coordonnees {
    float x, y;
    Coordonnees(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

class Station {
private:
    std::string nom;
    Coordonnees position;
    int passagers;

public:
    Station(std::string nom, Coordonnees pos, int passagersInitiaux = 0)
        : nom(nom), position(pos), passagers(passagersInitiaux) {}

    int prendrePassagers(int nombre);
    void dessiner(sf::RenderWindow& window, const sf::Font& font);
    Coordonnees getPosition() const { return position; }
};

class Train {
private:
    Coordonnees position;
    Coordonnees vitesse;
    std::vector<Station> parcours;
    int stationActuelle;
    bool enMouvement;
    float tempsAttente;
    int passagers;

public:
    Train(Coordonnees pos) : position(pos), stationActuelle(0), enMouvement(true), tempsAttente(0.0f), passagers(0) {}
    void ajouterStation(const Station& station);
    void setVitesse(const Coordonnees& nouvelleVitesse) { vitesse = nouvelleVitesse; }
    void avancer(float deltaTime);
    void dessiner(sf::RenderWindow& window, const sf::Font& font);
};

class GestionnaireDeTrain {
private:
    std::vector<std::unique_ptr<Train>> trains;

public:
    void ajouterTrain(std::unique_ptr<Train> train);
    void mettreAJour(float deltaTime);
    void dessiner(sf::RenderWindow& window, const sf::Font& font);
};

#endif

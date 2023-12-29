#ifndef TRAINSTATION_H
#define TRAINSTATION_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>

struct Coordonnees {
    float x, y;
    Coordonnees(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

class Station {
private:
    std::string nom;
    Coordonnees position;
    int passagers;
    float tempsDepuisDerniereRegeneration = 0.0f;
    float intervalleDeRegeneration = 5.0f;

public:
    Station(std::string nom, Coordonnees pos, int passagersInitiaux = 0)
        : nom(nom), position(pos), passagers(passagersInitiaux) {}
    int prendrePassagers();
    void dessiner(sf::RenderWindow& window, const sf::Font& font);
    Coordonnees getPosition() const { return position; }
    void reinitialiserEtGenererPassagers();
    std::string getNom() const { return nom; }  // Ajout de la méthode getNom
    int getPassagers() const { return passagers; }  // Ajout de la méthode getPassagers
    void mettreAJour(float deltaTime);
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
    int numeroTrain;
    bool estArrive;

public:
    Train(Coordonnees pos, int num) : position(pos), stationActuelle(0), enMouvement(true), tempsAttente(0.0f), passagers(0), numeroTrain(num) {}
    void ajouterStation(const Station& station);
    void setVitesse(const Coordonnees& nouvelleVitesse) { vitesse = nouvelleVitesse; }
    void avancer(float deltaTime);
    void dessiner(sf::RenderWindow& window, const sf::Font& font);
    std::vector<Station>& getParcours() { return parcours; }
    void arreter() {
        enMouvement = false;
        estArrive = true; // Vous pouvez choisir de marquer le train comme arrivé ou gérer cet état séparément.
    }
    void redemarrer() {
        enMouvement = true;
        estArrive = false; // Vous pouvez choisir de marquer le train comme arrivé ou gérer cet état séparément.
    }

};

class GestionnaireDeTrain {
private:
    std::vector<std::unique_ptr<Train>> trains;

public:
    void ajouterTrain(std::unique_ptr<Train> train);
    void mettreAJour(float deltaTime);
    void dessiner(sf::RenderWindow& window, const sf::Font& font);
    void arreterTousLesTrains() {
        for (auto& train : trains) {
            train->arreter();
        }
    }
    void redemarrerTousLesTrains() {
        for (auto& train : trains) {
            train->redemarrer();
        }
    }
};

class GestionnaireDeStations {
private:
    std::vector<Station> stations;

public:
    void ajouterStation(const Station& station);
    void dessinerStations(sf::RenderWindow& window, const sf::Font& font);
    void mettreAJour(float deltaTime);
};


#endif





#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <sstream>

// Constantes
const int TAILLE_CASE = 30;
const int DUREE_MOUTON = 50;
const int DUREE_LOUP = 80;
const int FAIM_MOUTON = 5;
const int FAIM_LOUP = 10;
const int ENERGIE_REPRODUCTION_MOUTON = 50;
const int ENERGIE_REPRODUCTION_LOUP = 50;

// Couleurs
const sf::Color COULEUR_HERBE(34, 139, 34);
const sf::Color COULEUR_SEL(210, 180, 140);
const sf::Color COULEUR_MOUTON(255, 255, 255);
const sf::Color COULEUR_LOUP(139, 0, 0);
const sf::Color COULEUR_TEXTE(255, 255, 255);
const sf::Color COULEUR_FOND(50, 50, 50);

class Entite {
protected:
    int x, y;
    int age;
    char sexe;
    int energie;
    int toursSansManger;
    bool enceinte;
    int toursEnceinte;

public:
    Entite(int x, int y, char sexe) 
        : x(x), y(y), age(0), sexe(sexe), energie(100), toursSansManger(0), 
          enceinte(false), toursEnceinte(0) {}
    
    virtual ~Entite() = default;
    
    virtual void deplacer(int tailleX, int tailleY) = 0;
    virtual void manger(class Univers& univers) = 0;
    virtual bool reproduire(std::vector<std::unique_ptr<Entite>>& nouvellesEntites, int tailleX, int tailleY) = 0;
    virtual bool doitMourir() const = 0;
    virtual char getSymbole() const = 0;
    virtual std::string getType() const = 0;
    virtual sf::Color getCouleur() const = 0;
    virtual int getDureeVie() const = 0;
    virtual int getFaim() const = 0;

    int getX() const { return x; }
    int getY() const { return y; }
    int getAge() const { return age; }
    int getEnergie() const { return energie; }
    char getSexe() const { return sexe; }
    bool estEnceinte() const { return enceinte; }
    
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void vieillir() { age++; }
    void perdreEnergie(int valeur) { energie -= valeur; }
    void incrementerToursSansManger() { toursSansManger++; }
    void resetToursSansManger() { toursSansManger = 0; }
    int getToursSansManger() const { return toursSansManger; }
    void devenirEnceinte() { enceinte = true; toursEnceinte = 0; }
    void avancerGrossesse() { if (enceinte) toursEnceinte++; }
    bool peutDonnerNaissance() const { return enceinte && toursEnceinte >= 3; }
    void accoucher() { enceinte = false; }
};

class Herbe {
private:
    int croissance;
    bool presente;
    int toursDepuisMangee;

public:
    Herbe() : croissance(100), presente(true), toursDepuisMangee(0) {}
    
    bool estPresente() const { return presente; }
    int getCroissance() const { return croissance; }
    
    void manger() { 
        croissance = 0; 
        presente = false;
        toursDepuisMangee = 0;
    }
    
    void pousser() {
        if (!presente) {
            toursDepuisMangee++;
            if (toursDepuisMangee >= 2) {
                croissance += 25;
                if (croissance >= 100) {
                    croissance = 100;
                    presente = true;
                }
            }
        }
    }
    
    sf::Color getCouleur() const {
        if (!presente) return COULEUR_SEL;
        return sf::Color(
            COULEUR_HERBE.r,
            COULEUR_HERBE.g * croissance / 100,
            COULEUR_HERBE.b * croissance / 100
        );
    }
};

class Mouton : public Entite {
private:
    int toursDepuisReproduction;

public:
    Mouton(int x, int y, char sexe) 
        : Entite(x, y, sexe), toursDepuisReproduction(0) {}
    
    void deplacer(int tailleX, int tailleY) override {
        if (enceinte) return; // Ne pas se déplacer si enceinte
        
        // Déplacement plus intelligent vers l'herbe si affamé
        if (getToursSansManger() > 3) {
            // Cherche l'herbe dans les cases adjacentes
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < tailleX && ny >= 0 && ny < tailleY) {
                        // Ici on devrait vérifier s'il y a de l'herbe (nécessite accès à l'univers)
                        x = nx;
                        y = ny;
                        return;
                    }
                }
            }
        }
        
        // Déplacement aléatoire sinon
        x += rand() % 3 - 1;
        y += rand() % 3 - 1;
        x = std::max(0, std::min(x, tailleX - 1));
        y = std::max(0, std::min(y, tailleY - 1));
    }
    
    void manger(class Univers& univers) override;
    
    bool reproduire(std::vector<std::unique_ptr<Entite>>& nouvellesEntites, int tailleX, int tailleY) override {
        if (enceinte) {
            avancerGrossesse();
            if (peutDonnerNaissance()) {
                // Trouver une case adjacente libre
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < tailleX && ny >= 0 && ny < tailleY) {
                            // Vérifier si la case est libre (nécessite accès à l'univers)
                            // Si libre:
                            nouvellesEntites.push_back(std::make_unique<Mouton>(nx, ny, (rand() % 2) ? 'M' : 'F'));
                            energie -= 30;
                            accoucher();
                            return true;
                        }
                    }
                }
                // Si aucune case libre, avorter
                accoucher();
            }
            return false;
        }
        
        if (age > 5 && energie > ENERGIE_REPRODUCTION_MOUTON && toursDepuisReproduction >= 10) {
            devenirEnceinte();
            toursDepuisReproduction = 0;
            return true;
        }
        
        toursDepuisReproduction++;
        return false;
    }
    
    bool doitMourir() const override {
        return age > DUREE_MOUTON || energie <= 0 || getToursSansManger() > FAIM_MOUTON;
    }
    
    char getSymbole() const override { return 'M'; }
    std::string getType() const override { return "Mouton"; }
    sf::Color getCouleur() const override { 
        return enceinte ? sf::Color(200, 200, 255) : COULEUR_MOUTON; 
    }
    int getDureeVie() const override { return DUREE_MOUTON; }
    int getFaim() const override { return FAIM_MOUTON; }
};

class Loup : public Entite {
private:
    int toursDepuisReproduction;

public:
    Loup(int x, int y, char sexe) 
        : Entite(x, y, sexe), toursDepuisReproduction(0) {}
    
    void deplacer(int tailleX, int tailleY) override {
        if (enceinte) return; // Ne pas se déplacer si enceinte
        
        // Déplacement plus intelligent vers les moutons si affamé
        if (getToursSansManger() > 2) {
            // Cherche les moutons dans un rayon plus large
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < tailleX && ny >= 0 && ny < tailleY) {
                        // Ici on devrait vérifier s'il y a un mouton (nécessite accès à l'univers)
                        x = nx;
                        y = ny;
                        return;
                    }
                }
            }
        }
        
        // Déplacement aléatoire sinon
        x += rand() % 3 - 1;
        y += rand() % 3 - 1;
        x = std::max(0, std::min(x, tailleX - 1));
        y = std::max(0, std::min(y, tailleY - 1));
    }
    
    void manger(class Univers& univers) override;
    
    bool reproduire(std::vector<std::unique_ptr<Entite>>& nouvellesEntites, int tailleX, int tailleY) override {
        if (enceinte) {
            avancerGrossesse();
            if (peutDonnerNaissance()) {
                // Trouver une case adjacente libre
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < tailleX && ny >= 0 && ny < tailleY) {
                            // Vérifier si la case est libre (nécessite accès à l'univers)
                            // Si libre:
                            nouvellesEntites.push_back(std::make_unique<Loup>(nx, ny, (rand() % 2) ? 'M' : 'F'));
                            energie -= 50;
                            accoucher();
                            return true;
                        }
                    }
                }
                // Si aucune case libre, avorter
                accoucher();
            }
            return false;
        }
        
        if (age > 10 && energie > ENERGIE_REPRODUCTION_LOUP && toursDepuisReproduction >= 15) {
            devenirEnceinte();
            toursDepuisReproduction = 0;
            return true;
        }
        
        toursDepuisReproduction++;
        return false;
    }
    
    bool doitMourir() const override {
        return age > DUREE_LOUP || energie <= 0 || getToursSansManger() > FAIM_LOUP;
    }
    
    char getSymbole() const override { return 'L'; }
    std::string getType() const override { return "Loup"; }
    sf::Color getCouleur() const override { 
        return enceinte ? sf::Color(255, 200, 200) : COULEUR_LOUP; 
    }
    int getDureeVie() const override { return DUREE_LOUP; }
    int getFaim() const override { return FAIM_LOUP; }
};

class Univers {
private:
    int tailleX, tailleY;
    std::vector<std::vector<Herbe>> herbes;
    std::vector<std::unique_ptr<Entite>> entites;
    int tourActuel;
    bool pause;
    
    // Statistiques
    int naissancesMoutons;
    int naissancesLoups;
    int mortsMoutons;
    int mortsLoups;
    int moutonsManges;
    int herbeMangee;
    int herbeRepousse;

public:
    Univers(int x, int y) 
        : tailleX(x), tailleY(y), tourActuel(0), pause(false),
          naissancesMoutons(0), naissancesLoups(0),
          mortsMoutons(0), mortsLoups(0),
          moutonsManges(0), herbeMangee(0), herbeRepousse(0) {
        herbes.resize(x, std::vector<Herbe>(y));
    }
    
    void ajouterMouton(int x, int y, char sexe) {
        entites.push_back(std::make_unique<Mouton>(x, y, sexe));
    }
    
    void ajouterLoup(int x, int y, char sexe) {
        entites.push_back(std::make_unique<Loup>(x, y, sexe));
    }
    
    void initialiser(int nbMoutons, int nbLoups) {
        // Ajouter de l'herbe partout
        for (int i = 0; i < tailleX; i++) {
            for (int j = 0; j < tailleY; j++) {
                herbes[i][j] = Herbe();
            }
        }
        
        // Ajouter des moutons
        for (int i = 0; i < nbMoutons; i++) {
            ajouterMouton(rand() % tailleX, rand() % tailleY, (rand() % 2) ? 'M' : 'F');
        }
        
        // Ajouter des loups
        for (int i = 0; i < nbLoups; i++) {
            ajouterLoup(rand() % tailleX, rand() % tailleY, (rand() % 2) ? 'M' : 'F');
            
        }
    }
    
    void tour() {
        if (pause) return;
        
        // Réinitialiser les statistiques
        naissancesMoutons = 0;
        naissancesLoups = 0;
        mortsMoutons = 0;
        mortsLoups = 0;
        moutonsManges = 0;
        herbeMangee = 0;
        herbeRepousse = 0;
        
        // Faire pousser l'herbe
        for (int i = 0; i < tailleX; i++) {
            for (int j = 0; j < tailleY; j++) {
                herbes[i][j].pousser();
                if (herbes[i][j].estPresente() && herbes[i][j].getCroissance() == 100) {
                    herbeRepousse++;
                }
            }
        }
        
        // Déplacement et actions des entités
        std::vector<std::unique_ptr<Entite>> nouvellesEntites;
        
        for (auto& entite : entites) {
            entite->deplacer(tailleX, tailleY);
            entite->manger(*this);
            
            if (entite->reproduire(nouvellesEntites, tailleX, tailleY)) {
                if (entite->getType() == "Mouton") {
                    if (entite->estEnceinte()) {
                        // La gestation est en cours
                    } else {
                        naissancesMoutons++;
                    }
                } else {
                    if (entite->estEnceinte()) {
                        // La gestation est en cours
                    } else {
                        naissancesLoups++;
                    }
                }
            }
            
            entite->vieillir();
            entite->incrementerToursSansManger();
        }
        
        // Ajouter les nouvelles entités
        for (auto& nouvelleEntite : nouvellesEntites) {
            entites.push_back(std::move(nouvelleEntite));
        }
        
        // Supprimer les entités mortes
        auto it = std::remove_if(entites.begin(), entites.end(),
            [&](const std::unique_ptr<Entite>& e) {
                if (e->doitMourir()) {
                    if (e->getType() == "Mouton") mortsMoutons++;
                  //  else mortsLoups++;
                  else  {
                    std::cout << "Faim: " << e->getFaim() << std::endl;
                }
                    return true;
                }
                return false;
            });
        entites.erase(it, entites.end());
        
        tourActuel++;
    }
    
    void afficher(sf::RenderWindow& window) {
        // Afficher l'herbe
        for (int i = 0; i < tailleX; i++) {
            for (int j =  0; j < tailleY; j++) {
                sf::RectangleShape caseShape(sf::Vector2f(TAILLE_CASE - 1, TAILLE_CASE - 1));
                caseShape.setPosition(i * TAILLE_CASE, j * TAILLE_CASE);
                caseShape.setFillColor(herbes[i][j].getCouleur());
                window.draw(caseShape);
            }
        }
        
        // Afficher les entités
        for (const auto& entite : entites) {
            sf::CircleShape entiteShape(TAILLE_CASE / 2 - 2);
            entiteShape.setPosition(
                entite->getX() * TAILLE_CASE + 1, 
                entite->getY() * TAILLE_CASE + 1
            );
            entiteShape.setFillColor(entite->getCouleur());
            window.draw(entiteShape);
            
            // Afficher le sexe
            sf::Text sexeText;
            sf::Font font;
            sexeText.setFont(font);
            sexeText.setString(std::string(1, entite->getSexe()));
            sexeText.setCharacterSize(12);
            sexeText.setFillColor(sf::Color::Black);
            sexeText.setPosition(
                entite->getX() * TAILLE_CASE + TAILLE_CASE / 2 - 4, 
                entite->getY() * TAILLE_CASE + TAILLE_CASE / 2 - 8
            );
            window.draw(sexeText);
        }
        
        // Afficher les informations
       // afficherInfos(window);
    }
    
    void afficherInfos(sf::RenderWindow& window) {
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            // Gérer l'erreur de chargement de police
            return;
        }
        
        // Fond semi-transparent pour les infos
        sf::RectangleShape infoBackground(sf::Vector2f(300, 150));
        infoBackground.setPosition(10, 10);
        infoBackground.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(infoBackground);
        
        // Texte des informations
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(14);
        infoText.setFillColor(COULEUR_TEXTE);
        infoText.setPosition(20, 20);
        
        std::stringstream ss;
        ss << "Tour: " << tourActuel << "\n"
           << "Moutons: " << compterMoutons() << " (M: " << compterMoutons('M') 
           << ", F: " << compterMoutons('F') << ")\n"
           << "Loups: " << compterLoups() << " (M: " << compterLoups('M') 
           << ", F: " << compterLoups('F') << ")\n"
           << "Pause: " << (pause ? "OUI" : "NON") << " (P pour basculer)\n"
           << "Espace pour avancer d'un tour\n"
           << "Clic pour ajouter un animal";
        
        infoText.setString(ss.str());
        window.draw(infoText);
        
        // Statistiques du dernier tour
        sf::RectangleShape statsBackground(sf::Vector2f(300, 120));
        statsBackground.setPosition(10, 170);
        statsBackground.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(statsBackground);
        
        sf::Text statsText;
        statsText.setFont(font);
        statsText.setCharacterSize(14);
        statsText.setFillColor(COULEUR_TEXTE);
        statsText.setPosition(20, 180);
        
        std::stringstream stats;
        stats << "Dernier tour:\n"
              << "Naissances: " << naissancesMoutons << " moutons, " << naissancesLoups << " loups\n"
              << "Morts: " << mortsMoutons << " moutons, " << mortsLoups << " loups\n"
              << "Moutons mangés: " << moutonsManges << "\n"
              << "Herbe: " << herbeMangee << " mangée, " << herbeRepousse << " repoussée";
        
        statsText.setString(stats.str());
        window.draw(statsText);
    }
    
    void togglePause() { pause = !pause; }
    bool estEnPause() const { return pause; }
    
    int compterMoutons(char sexe = '\0') const {
        if (sexe == '\0') {
            return std::count_if(entites.begin(), entites.end(), 
                [](const std::unique_ptr<Entite>& e) { return e->getType() == "Mouton"; });
        }
        return std::count_if(entites.begin(), entites.end(), 
            [sexe](const std::unique_ptr<Entite>& e) { 
                return e->getType() == "Mouton" && e->getSexe() == sexe; 
            });
    }
    
    int compterLoups(char sexe = '\0') const {
        if (sexe == '\0') {
            return std::count_if(entites.begin(), entites.end(), 
                [](const std::unique_ptr<Entite>& e) { return e->getType() == "Loup"; });
        }
        return std::count_if(entites.begin(), entites.end(), 
            [sexe](const std::unique_ptr<Entite>& e) { 
                return e->getType() == "Loup" && e->getSexe() == sexe; 
            });
    }
    
    // Méthodes pour les interactions
    bool mangerHerbe(int x, int y) {
        if (x >= 0 && x < tailleX && y >= 0 && y < tailleY && herbes[x][y].estPresente()) {
            herbes[x][y].manger();
            herbeMangee++;
            return true;
        }
        return false;
    }
    
    Entite* getEntiteAt(int x, int y, const std::string& type) {
        for (auto& entite : entites) {
            if (entite->getX() == x && entite->getY() == y && entite->getType() == type) {
                return entite.get();
            }
        }
        return nullptr;
    }
    
    void ajouterAnimal(int x, int y, const std::string& type) {
        if (x >= 0 && x < tailleX && y >= 0 && y < tailleY) {
            if (type == "Mouton") {
                ajouterMouton(x, y, (rand() % 2) ? 'M' : 'F');
            } else if (type == "Loup") {
                ajouterLoup(x, y, (rand() % 2) ? 'M' : 'F');
            }
        }
    }
    
    void sauvegarder(const std::string& fichier) const {
        // Implémentation de la sauvegarde
    }
    
    void charger(const std::string& fichier) {
        // Implémentation du chargement
    }
};

void Mouton::manger(Univers& univers) {
    if (univers.mangerHerbe(x, y)) {
        energie += 20;
        resetToursSansManger();
    }
}

void Loup::manger(Univers& univers) {
    Entite* mouton = univers.getEntiteAt(x, y, "Mouton");
    if (mouton) {
        // Supprimer le mouton (géré par l'univers)
        energie += 40;
        resetToursSansManger();
        // La statistique "moutons mangés" est gérée par l'univers
    }
}

int main() {
    srand(time(0));
    const int LARGEUR = 800;
    const int HAUTEUR = 600;
    const int TAILLE_UNIVERS_X = LARGEUR / TAILLE_CASE;
    const int TAILLE_UNIVERS_Y = HAUTEUR / TAILLE_CASE;
    
    sf::RenderWindow window(sf::VideoMode(LARGEUR, HAUTEUR), "Ecosystème");
    window.setFramerateLimit(60);
    
    Univers univers(TAILLE_UNIVERS_X, TAILLE_UNIVERS_Y);
    univers.initialiser(20, 30);
    
    sf::Clock clock;
    float tempsEcoule = 0;
    const float INTERVALLE_TOUR = 0.5f; // 0.5 seconde entre chaque tour
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    univers.tour();
                }
                if (event.key.code == sf::Keyboard::P) {
                    univers.togglePause();
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x / TAILLE_CASE;
                    int y = event.mouseButton.y / TAILLE_CASE;
                    univers.ajouterAnimal(x, y, "Mouton");
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    int x = event.mouseButton.x / TAILLE_CASE;
                    int y = event.mouseButton.y / TAILLE_CASE;
                    univers.ajouterAnimal(x, y, "Loup");
                }
            }
        }
        
        // Gestion du temps pour les tours automatiques
        if (!univers.estEnPause()) {
            float deltaTime = clock.restart().asSeconds();
            tempsEcoule += deltaTime;
            
            if (tempsEcoule >= INTERVALLE_TOUR) {
                tempsEcoule = 0;
                univers.tour();
            }
        } else {
            clock.restart();
        }
        
        // Affichage
        window.clear(COULEUR_FOND);
        univers.afficher(window);
        window.display();
    }
    
    return 0;
}
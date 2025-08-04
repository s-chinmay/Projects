#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
private:
    std::string name;
    std::string character;
    int age;
    int stamina;
    int strength;
    int wisdom;
    int pridePoints;
    std::string advisor;
    int path;

public:
    Player(std::string n, std::string c, int a, int stam, int str, int wis, int pp);
    void setAdvisor(std::string adv);
    void setPath(int p);
    std::string getName() const;
    std::string getCharacter() const;
    int getAge() const;
    int getStamina() const;
    int getStrength() const;
    int getWisdom() const;
    int getPridePoints() const;
    std::string getAdvisor() const;
    int getPath() const;
    void updateStats(int stam, int str, int wis, int pp);
};

#endif
#include "player.h"

Player::Player(std::string n, std::string c, int a, int stam, int str, int wis, int pp)
    : name(n), character(c), age(a), stamina(stam), strength(str), wisdom(wis), pridePoints(pp), advisor("None"), path(-1) {}

void Player::setAdvisor(std::string adv) {
    advisor = adv;
}

void Player::setPath(int p) {
    path = p;
}

std::string Player::getName() const { return name; }
std::string Player::getCharacter() const { return character; }
int Player::getAge() const { return age; }
int Player::getStamina() const { return stamina; }
int Player::getStrength() const { return strength; }
int Player::getWisdom() const { return wisdom; }
int Player::getPridePoints() const { return pridePoints; }
std::string Player::getAdvisor() const { return advisor; }
int Player::getPath() const { return path; }

void Player::updateStats(int stam, int str, int wis, int pp) {
    stamina = std::max(100, stamina + stam);
    strength = std::max(100, strength + str);
    wisdom = std::max(100, wisdom + wis);
    pridePoints += pp;
}
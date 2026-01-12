#include "envyenemy.h"
#include "character.h"

EnvyEnemy::EnvyEnemy()
    : Enemy("Envy", 110, 22)
{
}

QString EnvyEnemy::getNormalAttackDescription() const
{
    return QString("%1 envies a character's strength, dealing %2 damage!").arg(name).arg(damage);
}

QString EnvyEnemy::getAbilityDescription() const
{
    return QString("%1 copies the strength of the strongest character!").arg(name);
}

void EnvyEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        target->takeDamage(damage);
        int stealAmount = 5;
        health += stealAmount;
        if (health > maxHealth) health = maxHealth;
        emit healthChanged(health);
    }
}

void EnvyEnemy::performAbility(QVector<Character*> team)
{
    Character* strongest = nullptr;
    int maxHealth = 0;

    for (Character* character : team) {
        if (character && character->isAlive() && character->getHealth() > maxHealth) {
            maxHealth = character->getHealth();
            strongest = character;
        }
    }

    if (strongest) {
        damage += maxHealth/10;
    }
}

#include "lustenemy.h"
#include "character.h"

LustEnemy::LustEnemy()
    : Enemy("Lust", 100, 20)
{
}

QString LustEnemy::getNormalAttackDescription() const
{
    return QString("%1 seduces a character, dealing %2 damage and charming them!").arg(name).arg(damage);
}

QString LustEnemy::getAbilityDescription() const
{
    return QString("%1 charms all characters, dealing %2 damage to each!").arg(name).arg(damage/2);
}

void LustEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        target->takeDamage(damage);
    }
}

void LustEnemy::performAbility(QVector<Character*> team)
{
    for (Character* character : team) {
        if (character && character->isAlive()) {
            character->takeDamage(damage / 2);
        }
    }
}

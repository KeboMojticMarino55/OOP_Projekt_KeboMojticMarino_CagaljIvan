#include "greedenemy.h"
#include "character.h"

GreedEnemy::GreedEnemy()
    : Enemy("Greed", 150, 18), stolenGold(0)
{
}

QString GreedEnemy::getNormalAttackDescription() const
{
    return QString("%1 steals gold while attacking for %2 damage!").arg(name).arg(damage);
}

QString GreedEnemy::getAbilityDescription() const
{
    return QString("%1 unleashes a golden storm, dealing %2 damage to all!").arg(name).arg(damage);
}

void GreedEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        target->takeDamage(damage);
        stolenGold += 10;
    }
}

void GreedEnemy::performAbility(QVector<Character*> team)
{
    for (Character* character : team) {
        if (character && character->isAlive()) {
            int bonusDamage = stolenGold / 20;
            character->takeDamage(damage + bonusDamage);
        }
    }
}

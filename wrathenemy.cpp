#include "wrathenemy.h"
#include "character.h"

WrathEnemy::WrathEnemy()
    : Enemy("Wrath", 130, 25), rageStacks(0)
{
}

QString WrathEnemy::getNormalAttackDescription() const
{
    return QString("%1 unleashes fury for %2 damage!").arg(name).arg(damage + rageStacks * 5);
}

QString WrathEnemy::getAbilityDescription() const
{
    return QString("%1 goes into a blood rage, increasing all damage!").arg(name);
}

void WrathEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        int totalDamage = damage + (rageStacks * 5);
        target->takeDamage(totalDamage);
    }
}

void WrathEnemy::performAbility(QVector<Character*> team)
{
    rageStacks += 2;
    if (rageStacks > 10) rageStacks = 10;
}

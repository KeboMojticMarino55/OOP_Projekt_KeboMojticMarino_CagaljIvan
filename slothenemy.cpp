#include "slothenemy.h"
#include "character.h"

SlothEnemy::SlothEnemy()
    : Enemy("Sloth", 200, 10), hasSlept(false)
{
}

QString SlothEnemy::getNormalAttackDescription() const
{
    return QString("%1 lazily attacks for %2 damage.").arg(name).arg(damage);
}

QString SlothEnemy::getAbilityDescription() const
{
    return QString("%1 takes a nap and heals to full health!").arg(name);
}

void SlothEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        target->takeDamage(damage);
    }
}

void SlothEnemy::performAbility(QVector<Character*> team)
{
    if (!hasSlept) {
        health = maxHealth;
        hasSlept = true;
        emit healthChanged(health);
    }
}

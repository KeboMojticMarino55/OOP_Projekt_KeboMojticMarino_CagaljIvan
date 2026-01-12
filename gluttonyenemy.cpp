#include "gluttonyenemy.h"
#include "character.h"

GluttonyEnemy::GluttonyEnemy()
    : Enemy("Gluttony", 260, 30), healOnAttack(5)
{
}

QString GluttonyEnemy::getNormalAttackDescription() const
{
    return QString("%1 devours a character for %2 damage and heals %3 HP!").arg(name).arg(damage).arg(healOnAttack);
}

QString GluttonyEnemy::getAbilityDescription() const
{
    return QString("%1 feasts on all characters, dealing %2 damage to each and healing massively!").arg(name).arg(damage/2);
}

void GluttonyEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        target->takeDamage(damage);
        health += healOnAttack;
        if (health > maxHealth) health = maxHealth;
        emit healthChanged(health);
    }
}

void GluttonyEnemy::performAbility(QVector<Character*> team)
{
    for (Character* character : team) {
        if (character && character->isAlive()) {
            character->takeDamage(damage / 2);
        }
    }
    health += team.size() * healOnAttack * 2;
    if (health > maxHealth) health = maxHealth;
    emit healthChanged(health);
}



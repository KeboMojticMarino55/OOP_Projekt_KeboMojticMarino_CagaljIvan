#include "prideenemy.h"
#include "character.h"
#include <QDebug>

PrideEnemy::PrideEnemy()
    : Enemy("Pride", 180, 30), hasUsedAbilityThisTurn(false)
{
}

QString PrideEnemy::getNormalAttackDescription() const
{
    if (isImmuneToDamage) {
        return QString("%1 proudly strikes for %2 damage while immune!").arg(name).arg(damage * 2);
    }
    return QString("%1 proudly strikes for %2 damage!").arg(name).arg(damage);
}

QString PrideEnemy::getAbilityDescription() const
{
    return QString("%1 declares superiority, becoming immune to all damage this turn!");
}

void PrideEnemy::performNormalAttack(Character* target)
{
    if (target && target->isAlive()) {
        int totalDamage = damage;

        if (isImmuneToDamage) {
            totalDamage *= 2;
            qDebug() << name << "deals double damage while immune:" << totalDamage;
        }

        target->takeDamage(totalDamage);
    }
}

void PrideEnemy::performAbility(QVector<Character*> team)
{
    Q_UNUSED(team);

    if (!hasUsedAbilityThisTurn) {
        setDamageImmunity(true);
        hasUsedAbilityThisTurn = true;

        qDebug() << name << "becomes immune to damage for this turn!";
    } else {
        qDebug() << name << "has already used ability this turn";
    }
}

void PrideEnemy::setDamageImmunity(bool immune)
{
    isImmuneToDamage = immune;
    emit immunityChanged(immune);

    if (immune) {
        qDebug() << name << "is now immune to damage";
    } else {
        qDebug() << name << "is no longer immune to damage";
    }
}

void PrideEnemy::clearImmunity()
{
    isImmuneToDamage = false;
    hasUsedAbilityThisTurn = false;
    emit immunityChanged(false);
    qDebug() << name << "immunity cleared";
}

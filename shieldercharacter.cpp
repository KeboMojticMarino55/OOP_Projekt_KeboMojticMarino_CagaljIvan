#include "shieldercharacter.h"

ShielderCharacter::ShielderCharacter(const QString &name, int maxHealth,
                                     int normalDamage, int shieldAmount, int ultimateShield)
    : Character("Shielder", name, maxHealth),
    normalDamage(normalDamage), shieldAmount(shieldAmount), ultimateShield(ultimateShield)
{
}

QString ShielderCharacter::getAttackDescription(const QString &attackType) const
{
    if (attackType == "Normal") {
        return QString("%1 attacks for %2 damage!").arg(name).arg(normalDamage);
    } else if (attackType == "Ability") {
        return QString("%1 shields an ally for %2 protection!").arg(name).arg(shieldAmount);
    } else if (attackType == "Ultimate") {
        return QString("%1 creates MASSIVE SHIELD for %2 protection to all allies!").arg(name).arg(ultimateShield);
    }
    return "";
}

void ShielderCharacter::applyAbilityEffect(Character* target)
{
    if (target && target->isAlive()) {
        target->heal(shieldAmount);
    }
}

void ShielderCharacter::applyUltimateEffect(QVector<Character*> team)
{
    for (Character* character : team) {
        if (character && character->isAlive()) {
            character->heal(ultimateShield);
        }
    }
}

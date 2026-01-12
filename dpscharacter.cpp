#include "dpscharacter.h"

DPSCharacter::DPSCharacter(const QString &name, int maxHealth,
                           int normalDamage, int abilityDamage, int ultimateDamage)
    : Character("DPS", name, maxHealth),
    normalDamage(normalDamage), abilityDamage(abilityDamage), ultimateDamage(ultimateDamage)
{
}

QString DPSCharacter::getAttackDescription(const QString &attackType) const
{
    if (attackType == "Normal") {
        return QString("%1 attacks for %2 damage!").arg(name).arg(normalDamage);
    } else if (attackType == "Ability") {
        return QString("%1 uses special ability for %2 damage!").arg(name).arg(abilityDamage);
    } else if (attackType == "Ultimate") {
        return QString("%1 unleashes ULTIMATE for %2 damage!").arg(name).arg(ultimateDamage);
    }
    return "";
}

void DPSCharacter::applyAbilityEffect(Character* target)
{
    Q_UNUSED(target);
}

void DPSCharacter::applyUltimateEffect(QVector<Character*> team)
{
    Q_UNUSED(team);
}

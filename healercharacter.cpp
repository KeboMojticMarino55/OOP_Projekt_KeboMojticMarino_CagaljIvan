#include "healercharacter.h"

HealerCharacter::HealerCharacter(const QString &name, int maxHealth,
                                 int normalDamage, int abilityHeal, int ultimateHeal)
    : Character("Healer", name, maxHealth),
    normalDamage(normalDamage), abilityHeal(abilityHeal), ultimateHeal(ultimateHeal)
{
}

QString HealerCharacter::getAttackDescription(const QString &attackType) const
{
    if (attackType == "Normal") {
        return QString("%1 attacks for %2 damage!").arg(name).arg(normalDamage);
    } else if (attackType == "Ability") {
        return QString("%1 heals an ally for %2 health!").arg(name).arg(abilityHeal);
    } else if (attackType == "Ultimate") {
        return QString("%1 unleashes MASS HEAL for %2 health to all allies!").arg(name).arg(ultimateHeal);
    }
    return "";
}

void HealerCharacter::applyAbilityEffect(Character* target)
{
    if (target && target->isAlive()) {
        target->heal(abilityHeal);
    }
}

void HealerCharacter::applyUltimateEffect(QVector<Character*> team)
{
    for (Character* character : team) {
        if (character && character->isAlive()) {
            character->heal(ultimateHeal);
        }
    }
}

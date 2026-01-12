#include "amplifiercharacter.h"
#include <QDebug>

AmplifierCharacter::AmplifierCharacter(const QString &name, int maxHealth,
                                       int normalDamage, int abilityBuff, int ultimateBuff)
    : Character("Amplifier", name, maxHealth),
    normalDamage(normalDamage), abilityBuff(abilityBuff), ultimateBuff(ultimateBuff)
{
}

QString AmplifierCharacter::getAttackDescription(const QString &attackType) const
{
    if (attackType == "Normal") {
        return QString("%1 attacks for %2 damage!").arg(name).arg(normalDamage);
    } else if (attackType == "Ability") {
        return QString("%1 buffs an ally, increasing their next attack by %2%!").arg(name).arg(abilityBuff);
    } else if (attackType == "Ultimate") {
        return QString("%1 unleashes TEAM BUFF, increasing all allies' next attacks by %2%!").arg(name).arg(ultimateBuff);
    }
    return "";
}

void AmplifierCharacter::applyAbilityEffect(Character* target)
{
    if (target == this) {
        qDebug() << "Amplifier" << name << "cannot buff itself!";
        return;
    }

    if (target && target->isAlive()) {
        target->addDamageMultiplier(abilityBuff);
    } else {
        qDebug() << "Amplifier" << name << "tried to buff invalid target";
    }
}

void AmplifierCharacter::applyUltimateEffect(QVector<Character*> team)
{
    int buffedCount = 0;
    for (Character* character : team) {
        if (character && character->isAlive() && character != this) {
            character->addDamageMultiplier(ultimateBuff);
            buffedCount++;
        }
    }

    qDebug() << "Amplifier" << name << "buffed" << buffedCount << "other allies with ultimate";
}



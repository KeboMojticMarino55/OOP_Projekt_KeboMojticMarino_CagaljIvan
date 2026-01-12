#ifndef AMPLIFIERCHARACTER_H
#define AMPLIFIERCHARACTER_H

#include "character.h"
#include <QVector>

class AmplifierCharacter : public Character
{
    Q_OBJECT

public:
    AmplifierCharacter(const QString &name, int maxHealth = 110,
                       int normalDamage = 15, int abilityBuff = 10, int ultimateBuff = 25);

    int getNormalDamage() const override { return normalDamage; }
    int getAbilityDamage() const override { return 0; }
    int getUltimateDamage() const override { return 0; }
    QString getAttackDescription(const QString &attackType) const override;

    void applyAbilityEffect(Character* target) override;
    void applyUltimateEffect(QVector<Character*> team) override;

    int getAbilityBuff() const { return abilityBuff; }
    int getUltimateBuff() const { return ultimateBuff; }

private:
    int normalDamage;
    int abilityBuff;
    int ultimateBuff;
};

#endif

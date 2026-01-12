#ifndef DPSCHARACTER_H
#define DPSCHARACTER_H

#include "character.h"
#include <QVector>

class DPSCharacter : public Character
{
    Q_OBJECT

public:
    DPSCharacter(const QString &name, int maxHealth = 120,
                 int normalDamage = 25, int abilityDamage = 40, int ultimateDamage = 100);

    int getNormalDamage() const override { return normalDamage; }
    int getAbilityDamage() const override { return abilityDamage; }
    int getUltimateDamage() const override { return ultimateDamage; }
    QString getAttackDescription(const QString &attackType) const override;

    void applyAbilityEffect(Character* target) override;
    void applyUltimateEffect(QVector<Character*> team) override;

private:
    int normalDamage;
    int abilityDamage;
    int ultimateDamage;
};

#endif

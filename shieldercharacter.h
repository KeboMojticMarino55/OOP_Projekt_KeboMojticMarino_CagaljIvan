#ifndef SHIELDERCHARACTER_H
#define SHIELDERCHARACTER_H

#include "character.h"
#include <QVector>

class ShielderCharacter : public Character
{
    Q_OBJECT

public:
    ShielderCharacter(const QString &name, int maxHealth = 150,
                      int normalDamage = 20, int shieldAmount = 50, int ultimateShield = 100);

    int getNormalDamage() const override { return normalDamage; }
    int getAbilityDamage() const override { return 0; }
    int getUltimateDamage() const override { return 0; }
    QString getAttackDescription(const QString &attackType) const override;

    void applyAbilityEffect(Character* target) override;
    void applyUltimateEffect(QVector<Character*> team) override;

    int getShieldAmount() const { return shieldAmount; }
    int getUltimateShield() const { return ultimateShield; }

private:
    int normalDamage;
    int shieldAmount;
    int ultimateShield;
};

#endif

#ifndef HEALERCHARACTER_H
#define HEALERCHARACTER_H

#include "character.h"
#include <QVector>

class HealerCharacter : public Character
{
    Q_OBJECT

public:
    HealerCharacter(const QString &name, int maxHealth = 130,
                    int normalDamage = 18, int abilityHeal = 30, int ultimateHeal = 80);

    int getNormalDamage() const override { return normalDamage; }
    int getAbilityDamage() const override { return 0; }
    int getUltimateDamage() const override { return 0; }
    QString getAttackDescription(const QString &attackType) const override;

    void applyAbilityEffect(Character* target) override;
    void applyUltimateEffect(QVector<Character*> team) override;

    int getAbilityHeal() const { return abilityHeal; }
    int getUltimateHeal() const { return ultimateHeal; }

private:
    int normalDamage;
    int abilityHeal;
    int ultimateHeal;
};

#endif

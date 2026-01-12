#ifndef PRIDEENEMY_H
#define PRIDEENEMY_H

#include "enemy.h"
#include <QVector>

class PrideEnemy : public Enemy
{
    Q_OBJECT

public:
    PrideEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;
    void setDamageImmunity(bool immune) override;
    void clearImmunity() override;

private:
    bool isProud;
    bool hasUsedAbilityThisTurn;
};

#endif

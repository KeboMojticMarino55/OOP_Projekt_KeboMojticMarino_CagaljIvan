#ifndef ENVYENEMY_H
#define ENVYENEMY_H

#include "enemy.h"
#include <QVector>

class EnvyEnemy : public Enemy
{
    Q_OBJECT

public:
    EnvyEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;
};

#endif

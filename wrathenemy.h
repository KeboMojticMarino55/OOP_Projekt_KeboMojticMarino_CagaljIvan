#ifndef WRATHENEMY_H
#define WRATHENEMY_H

#include "enemy.h"
#include <QVector>

class WrathEnemy : public Enemy
{
    Q_OBJECT

public:
    WrathEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;

private:
    int rageStacks;
};

#endif

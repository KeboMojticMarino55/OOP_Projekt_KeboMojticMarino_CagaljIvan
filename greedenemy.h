#ifndef GREEDENEMY_H
#define GREEDENEMY_H

#include "enemy.h"
#include <QVector>

class GreedEnemy : public Enemy
{
    Q_OBJECT

public:
    GreedEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;

private:
    int stolenGold;
};

#endif

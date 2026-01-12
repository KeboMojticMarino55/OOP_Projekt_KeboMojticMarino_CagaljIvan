#ifndef LUSTENEMY_H
#define LUSTENEMY_H

#include "enemy.h"
#include <QVector>

class LustEnemy : public Enemy
{
    Q_OBJECT

public:
    LustEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;
};

#endif

#ifndef SLOTHENEMY_H
#define SLOTHENEMY_H

#include "enemy.h"
#include <QVector>

class SlothEnemy : public Enemy
{
    Q_OBJECT

public:
    SlothEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;

private:
    bool hasSlept;
};

#endif

#ifndef GLUTTONYENEMY_H
#define GLUTTONYENEMY_H

#include "enemy.h"
#include <QVector>

class GluttonyEnemy : public Enemy
{
    Q_OBJECT

public:
    GluttonyEnemy();

    QString getNormalAttackDescription() const override;
    QString getAbilityDescription() const override;
    void performNormalAttack(Character* target) override;
    void performAbility(QVector<Character*> team) override;

private:
    int healOnAttack;
};

#endif

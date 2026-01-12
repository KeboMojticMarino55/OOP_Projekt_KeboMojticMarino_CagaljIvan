#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QString>
#include <QVector>
#include "character.h"

class Enemy : public QObject
{
    Q_OBJECT

public:
    Enemy(const QString &name, int maxHealth, int damage);
    virtual ~Enemy() = default;

    QString getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getDamage() const { return damage; }
    bool isAlive() const { return health > 0; }
    bool isImmune() const { return isImmuneToDamage; }
    virtual void setDamageImmunity(bool immune) { isImmuneToDamage = immune; }
    virtual void clearImmunity() { isImmuneToDamage = false; }

    void takeDamage(int amount);
    void reset();

    virtual QString getNormalAttackDescription() const = 0;
    virtual QString getAbilityDescription() const = 0;
    virtual void performNormalAttack(Character* target) = 0;
    virtual void performAbility(QVector<Character*> team) = 0;

signals:
    void healthChanged(int newHealth);
    void immunityChanged(bool isImmune);

protected:
    QString name;
    int maxHealth;
    int health;
    int damage;
    bool isImmuneToDamage;
};

#endif

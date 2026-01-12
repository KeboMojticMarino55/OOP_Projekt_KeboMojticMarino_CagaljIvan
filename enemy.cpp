#include "enemy.h"
#include "character.h"
#include <QDebug>

Enemy::Enemy(const QString &name, int maxHealth, int damage)
    : name(name), maxHealth(maxHealth), health(maxHealth), damage(damage), isImmuneToDamage(false)
{
}

void Enemy::takeDamage(int amount)
{
    try {
        if (isImmuneToDamage) {
            qDebug() << name << "is immune to damage!";
            emit immunityChanged(true);
            return;
        }

        if (amount < 0) {
            qWarning() << name << ": Negative damage amount:" << amount;
            amount = 0;
        }

        health -= amount;
        if (health < 0) health = 0;

        qDebug() << name << "took" << amount << "damage. Health:" << health;

        emit healthChanged(health);

    } catch (const std::exception& e) {
        qCritical() << "Error in Enemy::takeDamage for" << name << ":" << e.what();
    } catch (...) {
        qCritical() << "Unknown error in Enemy::takeDamage for" << name;
    }
}

void Enemy::reset()
{
    health = maxHealth;
    emit healthChanged(health);
}

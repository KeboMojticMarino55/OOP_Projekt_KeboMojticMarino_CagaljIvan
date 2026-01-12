#include "character.h"
#include <QDebug>

Character::Character(const QString &type, const QString &name, int maxHealth)
    : type(type), name(name), maxHealth(maxHealth), health(maxHealth), ultimateCharge(0.0f), damageMultiplier(100)
{
}

void Character::takeDamage(int damage)
{
    try {
        if (damage < 0) {
            qWarning() << name << ": Negative damage received:" << damage;
            damage = 0;
        }

        health -= damage;
        if (health < 0) health = 0;

        qDebug() << name << "took" << damage << "damage. Health:" << health;

        emit healthChanged(health);

    } catch (const std::exception& e) {
        qCritical() << "Error in Character::takeDamage for" << name << ":" << e.what();
    } catch (...) {
        qCritical() << "Unknown error in Character::takeDamage for" << name;
    }
}

void Character::heal(int amount)
{
    health += amount;
    if (health > maxHealth) health = maxHealth;
    emit healthChanged(health);
}

void Character::addUltimateCharge(float amount)
{
    ultimateCharge += amount;
    if (ultimateCharge > 100.0f) ultimateCharge = 100.0f;
    emit ultimateChanged(ultimateCharge);
}

void Character::resetUltimate()
{
    ultimateCharge = 0.0f;
    emit ultimateChanged(ultimateCharge);
}

void Character::addDamageMultiplier(int percentage)
{
    damageMultiplier += percentage;
    if (damageMultiplier < 0) damageMultiplier = 0;
    if (damageMultiplier > 300) damageMultiplier = 300;
    emit damageMultiplierChanged(damageMultiplier);

    qDebug() << name << "damage multiplier now:" << damageMultiplier << "%";
}

void Character::clearDamageMultiplier()
{
    if (damageMultiplier != 100) {
        damageMultiplier = 100;
        emit damageMultiplierChanged(damageMultiplier);
        qDebug() << name << "damage multiplier reset to 100%";
    }
}

void Character::reset()
{
    health = maxHealth;
    ultimateCharge = 0.0f;
    damageMultiplier = 100;
    emit healthChanged(health);
    emit ultimateChanged(ultimateCharge);
    emit damageMultiplierChanged(damageMultiplier);
}

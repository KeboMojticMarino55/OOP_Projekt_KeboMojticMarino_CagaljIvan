#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>
#include <QString>
#include <QVector>

class Character : public QObject
{
    Q_OBJECT

public:
    Character(const QString &type, const QString &name, int maxHealth);
    virtual ~Character() = default;

    QString getName() const { return name; }
    QString getType() const { return type; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    float getUltimateCharge() const { return ultimateCharge; }
    bool isAlive() const { return health > 0; }
    bool isUltimateReady() const { return ultimateCharge >= 100.0f; }

    virtual int getNormalDamage() const = 0;
    virtual int getAbilityDamage() const = 0;
    virtual int getUltimateDamage() const = 0;
    virtual QString getAttackDescription(const QString &attackType) const = 0;

    void takeDamage(int damage);
    void heal(int amount);
    void addUltimateCharge(float amount);
    void resetUltimate();
    void reset();
    int getDamageMultiplier() const { return damageMultiplier; }
    void addDamageMultiplier(int percentage);
    void clearDamageMultiplier();
    bool hasDamageBuff() const { return damageMultiplier > 100; }

    virtual void applyAbilityEffect(Character* target) { Q_UNUSED(target); }
    virtual void applyUltimateEffect(QVector<Character*> team) { Q_UNUSED(team); }

signals:
    void healthChanged(int newHealth);
    void ultimateChanged(float newCharge);
    void damageMultiplierChanged(int newMultiplier);

protected:
    QString type;
    QString name;
    int maxHealth;
    int health;
    float ultimateCharge;
    int damageMultiplier;
};

#endif

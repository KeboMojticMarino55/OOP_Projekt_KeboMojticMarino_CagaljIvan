#ifndef TEAM_H
#define TEAM_H

#include <QObject>
#include <QVector>
#include "character.h"

class Team : public QObject
{
    Q_OBJECT

public:
    Team();
    ~Team();

    bool addCharacter(Character *character);
    bool removeCharacter(Character *character);
    void clearTeam();

    QVector<Character*> getCharacters() const { return characters; }
    int getSize() const { return characters.size(); }
    bool isFull() const { return characters.size() >= 3; }
    bool isEmpty() const { return characters.isEmpty(); }
    bool contains(Character *character) const { return characters.contains(character); }

    Character* getCharacterAt(int index);
    Character* getNextAliveCharacter(int currentIndex);
    bool allDead() const;

    void resetAllCharacters();

signals:
    void teamChanged();

private:
    QVector<Character*> characters;
};

#endif

#include "team.h"

Team::Team()
{
}

Team::~Team()
{
}

bool Team::addCharacter(Character *character)
{
    if (characters.size() >= 3) {
        return false;
    }

    if (!characters.contains(character)) {
        characters.append(character);
        emit teamChanged();
        return true;
    }

    return false;
}

bool Team::removeCharacter(Character *character)
{
    if (characters.removeOne(character)) {
        emit teamChanged();
        return true;
    }
    return false;
}

void Team::clearTeam()
{
    characters.clear();
    emit teamChanged();
}

Character* Team::getCharacterAt(int index)
{
    if (index >= 0 && index < characters.size()) {
        return characters[index];
    }
    return nullptr;
}

Character* Team::getNextAliveCharacter(int currentIndex)
{
    if (characters.isEmpty()) return nullptr;

    for (int i = 1; i <= characters.size(); i++) {
        int nextIndex = (currentIndex + i) % characters.size();
        Character *character = characters[nextIndex];
        if (character->isAlive()) {
            return character;
        }
    }

    return nullptr;
}

bool Team::allDead() const
{
    for (Character *character : characters) {
        if (character->isAlive()) {
            return false;
        }
    }
    return true;
}

void Team::resetAllCharacters()
{
    for (Character *character : characters) {
        character->reset();
    }
}

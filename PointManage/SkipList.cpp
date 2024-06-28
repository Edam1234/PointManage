#include "SkipList.h"

SkipList::~SkipList()
{

    Node* current = Header;
    while (current) {
        Node* next = current->forward[0];
        delete current;
        current = next;
    }

}

SkipList::SkipList(int maxLevel, float probability)
    : maxLevel(maxLevel), Probability(probability), Level(0) {
    Header = new Node(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), maxLevel);
}

int SkipList::randomLevel()
{
    int lvl = 0;
    while ((rand() / double(RAND_MAX)) < Probability && lvl < maxLevel) {
        lvl++;
    }
    return lvl;
}

Node* SkipList::createNode(float longitude, float latitude, int level)
{
    return new Node(longitude, latitude, level);
}

bool SkipList::search(float longitude, float latitude)
{
    Node* current = Header;
    for (int i = Level; i >= 0; i--) {
        while (current->forward[i] && (current->forward[i]->Longitude < longitude ||
            (current->forward[i]->Longitude == longitude && current->forward[i]->Latitude < latitude))) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    return current && current->Longitude == longitude && current->Latitude == latitude;

}

void SkipList::insert(float longitude, float latitude)
{
    std::vector<Node*> update(maxLevel + 1);
    Node* current = Header;
    for (int i = Level; i >= 0; i--) {
        while (current->forward[i] && (current->forward[i]->Longitude < longitude ||
            (current->forward[i]->Longitude == longitude && current->forward[i]->Latitude < latitude))) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    int lvl = randomLevel();
    if (lvl > Level) {
        for (int i = Level + 1; i <= lvl; i++) {
            update[i] = Header;
        }
        Level = lvl;
    }

    Node* newNode = createNode(longitude, latitude, lvl);
    for (int i = 0; i <= lvl; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

void SkipList::SKdelete(float longitude, float latitude)
{
    std::vector<Node*> update(maxLevel + 1);
    Node* current = Header;
    for (int i = Level; i >= 0; i--) {
        while (current->forward[i] && (current->forward[i]->Longitude < longitude ||
            (current->forward[i]->Longitude == longitude && current->forward[i]->Latitude < latitude))) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current && current->Longitude == longitude && current->Latitude == latitude) {
        for (int i = 0; i <= Level; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        while (Level > 0 && Header->forward[Level] == nullptr) {
            Level--;
        }

        delete current;
    }
}

std::vector<std::pair<double, double>> SkipList::getAllNodes()
{
    std::vector<std::pair<double, double>> nodes;
    Node* current = Header->forward[0]; // 从第0层开始
    while (current) {
        nodes.emplace_back(current->Longitude, current->Latitude);
        current = current->forward[0];
    }
    return nodes;
}


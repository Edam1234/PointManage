#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

struct Node
{
    float Longitude;
    float Latitude;
    std::vector<Node*> forward;

    Node(float value, float value1, int level) : Longitude(value), Latitude(value1), forward(level + 1, nullptr) {}
};
class SkipList
{
public: 
    SkipList(int maxLevel, float probability);
    ~SkipList();
    int randomLevel();
    Node* createNode(float longitude, float latitude, int level);
    bool search(float longitude, float latitude);
    void insert(float longitude, float latitude);
    void SKdelete(float longitude, float latitude);
    std::vector<std::pair<double, double>> getAllNodes();
private:
    int maxLevel;//������
    float Probability;//���������ƽ����
    Node* Header;//ͷ���
    int Level;//����
};



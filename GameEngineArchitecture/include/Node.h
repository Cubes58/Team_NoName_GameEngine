#pragma once
#include <iostream>

class Node{

public:

	Node(float p_PosX, float p_PosY, float p_PosZ, int p_Row, int p_Col);
	std::pair <int, int> getID();

protected:

	int m_IdX;
	int m_IdY;

	float m_X;
	float m_Y;
	float m_Z;


};
#pragma once
#include <array>
#include <list>

class Node;

class Graph {


public:

	Graph(float x, float y, float z);

protected:

	static const int s_m_iCol = 100;
	static const int s_m_iRow = 100;
    std::array<std::array<Node*, s_m_iCol>, s_m_iRow> m_Graph;

};
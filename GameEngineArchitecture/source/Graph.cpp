#include "Graph.h"
#include "Node.h"

Graph::Graph(float x, float y, float z)
{
	for (int i = 0; i < s_m_iCol; i++)
	{
		for (int j = 0; j < s_m_iRow; j++)
		{
			m_Graph[j][i] = new Node(x, y, z);
		}
	}

}

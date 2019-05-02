
/**
@file BehaviourNode.h
@brief Creates a behavouir tree for the game AI
*/

#pragma once
#include <iostream>
#include <list>

class GraphNode;

/*! \class BehaviourNode
	\brief Interface class for the Behaviour Nodes
*/

class BehaviourNode {

public:

	/*!
	\brief Run the Node
	\return did it run or not
	*/

	virtual bool Run() = 0;
};

/*! \class CompositeNode
	\brief This class handles all the nodes that will be in the tree
*/

class CompositeNode : public BehaviourNode {
	//
private:
	std::list<BehaviourNode*> m_ChildrenList; //!< List of children nodes
public:
	/*!
	\brief get the children
	\return a list of children nodes
	*/
	const std::list<BehaviourNode*>& GetTheChildren() const { return m_ChildrenList; }
	/*!
	\brief adds
	\param p_NodesPosition sets Pos
	*/
	void AddTheChildren(BehaviourNode* p_Child) { m_ChildrenList.emplace_back(p_Child); } //!< Adds to the list of children
};

/*! \class Selector
	\brief This class is used to see which node will return true
*/

class Selector : public CompositeNode {

public:

	/*!
	\brief Run the Node
	\return did it run or not
	*/

	bool Run() override {

		for (BehaviourNode* BN : GetTheChildren())
		{
			if (BN->Run()) { return true; } //Run will return true as long as one child is working
		}
		return false;
	}

};

/*! \class Sequence
	\brief This class is the sequance of the nodes
*/

class Sequence : public CompositeNode {

public:
	/*!
	\brief Run the Node
	\return did it run or not
	*/

	bool Run() override {
		for (BehaviourNode* BN : GetTheChildren())
		{
			if (!BN->Run()) { return false; } //every child needs to work for this one
		}
		return true;
	}
};


/*! \class DistanceLeafNode
	\brief An example of a conditional leaf node
*/

class DistanceLeafNode : public BehaviourNode
{
private:
	float m_Distance; //!< Just rage for this example
public:
	/*!
	\brief Give me an float set method
	\param p_Float a float that tells us what the distance is
	*/
	void GiveMeFloat(float p_Float) { m_Distance = p_Float; } 

	/*!
	\brief Run the Node
	\return did it run or not
	*/
	bool Run() override {
		const static float m_Max = 100.0f;
		if (m_Distance <= m_Max) { //This part is the condition of the node
			std::cout << "Target Is Near" << std::endl;
			return true;
		}
		else return false;
	}
};

/*! \class ListEmptyLeafNode
	\brief checks to see if the list is empty
*/

class ListEmptyLeafNode : public BehaviourNode
{
private:
	std::list<GraphNode*> m_List;
public:
	/*!
	\brief Give me an List set method
	\param p_List Used to see if a list is empty
	*/
	void GiveMeList(std::list<GraphNode*> p_List) { m_List = p_List; }

	/*!
	\brief Run the Node
	\return did it run or not
	*/
	bool Run() override {
		if (m_List.empty()) { //This part is the condition of the node
			std::cout << "List is empty" << std::endl;
			return true;
		}
		else return false;
	}
};

/*! \class LeafNodeActionExample
	\brief An example of a conditional leaf node
*/

class LeafNodeActionExample : public BehaviourNode
{
private:

public:
	/*!
	\brief Run the Node
	\return did it run or not
	*/
	bool Run() override {
		std::cout << "AUGHHHHH!!!!!!" << std::endl;
		return true;
	}
};

//Inside a NPC 
//Sequance will be the root
//Example Sequance* root = new Sequance;
//Example Selector* selectorOne = new Selector;
//Selectors will be added to this Sequance each containing condition and action leaf nodes
//root->addTheChildren(selectorOne);

//LeafNodeRageExample * m_RageLeaf;
//in each update loop you can pass it a new Rage value
//m_rageLeaf->giveMeInt(m_currentNPCRage);
//next time run is called, it could return a diffrent value then before

//Last thing to do is to add it to the selector 
//selectorOne->addTheChildren(m_RageLeaf);
//The next action could be another condition or action they will only be hit if m_RageLeaf is true..... i think
//if one is false it would move onto the next selector in the sequance..... i think
//Each time a node is created in here it can re used, so when creating a new enemy with simlair
//properties you wont have to re use this code

//I hope this helps this can be confusing (i keep mixing up selector and sequance)




#pragma once
#include <iostream>
#include <list>

/*! Interface class for the Behaviour Nodes*/

class BehaviourNode {

public:
	virtual bool Run() = 0; //!< Run the Node
};

/*! This class handles all the nodes that will be in the tree*/

class CompositeNode : public BehaviourNode {
//
private:
	std::list<BehaviourNode*> m_ChildrenList; //!< List of children nodes
public:
	const std::list<BehaviourNode*>& GetTheChildren() const { return m_ChildrenList; } //!< Gets the list of children
	void addTheChildren(BehaviourNode* p_Child) { m_ChildrenList.emplace_back(p_Child);	} //!< Adds to the list of children
};

/*! This class is used to see which node will return true*/

class Selector : public CompositeNode {

public:
	bool Run() override {//!< Run the Node

		for (BehaviourNode* BN : GetTheChildren()) 
		{
			if (BN->Run()) { return true; } //Run will return true as long as one child is working
		}
		return false;
	}

};

/*! This class is the sequance of the nodes*/

class Sequence : public CompositeNode {

public:
	bool Run() override {//!< Run the Node
		for (BehaviourNode* BN : GetTheChildren())
		{
			if (!BN->Run()) { return false; } //every child needs to work for this one
		}
		return true;
	}
};

/*! An example of a conditional leaf node*/

class LeafNodeRageExample : public BehaviourNode
{
private : 

	int m_Rage; //!< Just rage for this example

public:

	void GiveMeInt(int p_Rage) { m_Rage = p_Rage; } //!< Stores new Value

	bool Run() override { //!< Run the Node
		if (m_Rage <= 100){ //This part will be the condition of the node
			return true;  
			std::cout << "Enemy Has Embrace the Void and it drove him Mad" << std::endl;
		}
		else return false;
		std::cout << "Enemy is calm..... for now" << std::endl;
	}
};

/*! An example of a action leaf node*/

class LeafNodeActionExample : public BehaviourNode
{
private:

	//int m_rage; //Just rage for this example

public:

	//void giveMeInt(int p_rage) { m_rage = p_rage; } //Stores new Value

	bool Run() override { //!< Run the Node
		
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
	



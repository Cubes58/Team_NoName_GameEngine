#define USE_GLFW 1
#ifdef USE_GLFW
#include "GLFW_EngineCore.h"
#endif

#include <memory>

#include "Game.h"
#include "DefaultGame.h"
#include "MemoryManager.h"

#include "Vector.h"
#include "HashMap.h"
#include "MyTestClass.h"

int main(int p_Argc, char* p_Argv[]) {
	MemoryManagerInstance;
	// Hash Map Test 1:
	unsigned int hasMapSize = 100;
	CC::HashMap<unsigned int, MyTestClass> myHashMap(hasMapSize);

	CC::HashNode<unsigned int, MyTestClass> hashNode;
	hashNode.m_Key = 2;
	myHashMap.Insert(hashNode);

	CC::HashNode<unsigned int, MyTestClass> hashNode2;
	hashNode2.m_Key = 1;
	myHashMap.Insert(hashNode2);

	CC::HashNode<unsigned int, MyTestClass> hashNode3;
	MyTestClass instance;
	instance.m_FloatNumber = 0.0f;
	instance.m_FloatNumber2 = 0.0f;
	instance.m_FloatNumber3 = 0.0f;
	instance.m_FloatNumber4 = 0.0f;
	instance.m_FloatNumber5 = 0.0f;
	instance.m_Number = 0;
	hashNode3.m_Key = 4;
	hashNode3.m_Value = instance;
	myHashMap.Insert(hashNode3);

	myHashMap.Remove(hashNode3);


	CC::HashNode<unsigned int, MyTestClass> *hashNodeGet;
	hashNodeGet = myHashMap.Get(hashNode.m_Key);
	std::cout << "HasNodeGet: " << hashNodeGet->m_Value << std::endl << std::endl;

	CC::HashNode<unsigned int, MyTestClass> *hashNodeGet2;
	hashNodeGet2 = myHashMap.Get(hashNode2.m_Key);
	std::cout << "HasNodeGet2: " << hashNodeGet2->m_Value << std::endl << std::endl;

	CC::HashNode<unsigned int, MyTestClass> *hashNodeGet3;
	hashNodeGet3 = myHashMap.Get(hashNode3.m_Key);
	std::cout << "HasNodeGet3: " << hashNodeGet3->m_Value << std::endl << std::endl;

	// Vector Test 1:
	unsigned int vectorSize = 10u;
	CC::Vector<unsigned int> myVector(vectorSize);

	vectorSize *= 2u;
	for(unsigned int i = 0u; i <= vectorSize; ++i) {
		myVector.PushBack(i);
	}

	myVector.Remove((unsigned int)(vectorSize / 2u));

	myVector.PushBack(255u);

	for(unsigned int i = 0u; i <= vectorSize; ++i) {
		if(myVector.At(i) != nullptr)
			std::cout << myVector[i] << "  \t";
	}

	myVector.PushBack(255u);
	std::cout << myVector[myVector.NumberOfElements() - 1] << std::endl;

	std::cout << std::endl << std::endl;
	if(true) {
		// Vector Test 2:
		unsigned int newVectorSize = 10u;
		CC::Vector<MyTestClass> myNewVector(newVectorSize);

		MyTestClass instance;
		for(unsigned int i = 0u; i < newVectorSize; ++i) {
			myNewVector.PushBack(instance);
		}

		myNewVector.Remove((unsigned int)(newVectorSize / 2u));

		myNewVector.PushBack(instance);

		for(unsigned int i = 0u; i < newVectorSize; ++i) {
			if(myNewVector.At(i) != nullptr)
				std::cout << myNewVector[i];
		}
	}
	// Vector Test 3:
	unsigned int vectorSize2 = 10u;
	CC::Vector<unsigned int> myVector2(vectorSize2);
	for(unsigned int i = 0u; i < vectorSize2; ++i) {
		myVector2.PushBack(i);
	}

	for(unsigned int i = 0u; i < vectorSize2; ++i) {
		if(myVector2.At(i) != nullptr)
			std::cout << myVector2[i] << "  \t";
	}
	std::cout << std::endl << std::endl;

	std::cout << std::endl << std::endl << std::endl << std::endl;
	system("pause");

	return 0;
}
/*
int main(int p_Argc, char* p_Argv[]) {
	MemoryManagerInstance;

	std::shared_ptr<IEngineCore> engineCore;

#ifdef USE_GLFW
	 engineCore = std::make_shared<GLFW_EngineCore>();
#endif

	if (!engineCore->InitWindow(800, 600, "GameEngine"))
		return -1;

	std::shared_ptr<Game> game = std::make_shared<DefaultGame>();
	engineCore->RunEngine(game);

	return 0;
}
*/
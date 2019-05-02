#pragma once

#include <iostream>

class MyTestClass {
public:
	unsigned int m_Number = 10000u;
	unsigned int m_OtherNumber = 10000u;
	float m_FloatNumber = 100.03f;
	float m_FloatNumber2 = 1000.03f;
	float m_FloatNumber3 = 1000000.03f;
	float m_FloatNumber4 = 1000.03f;
	float m_FloatNumber5 = 100.03f;

	bool operator<(const MyTestClass &p_Instance) const {
		p_Instance.m_Number < this->m_Number;
	}

	bool operator==(const MyTestClass &p_Other) const {
		return m_Number == p_Other.m_Number;
	}

	friend std::ostream &operator<<(std::ostream &p_OutputStream, const MyTestClass &p_Data);
};

std::ostream &operator<<(std::ostream &p_OutputStream, const MyTestClass &p_Data) {
	p_OutputStream << "m_Number = " << p_Data.m_Number << "\n"
		<< "m_OtherNumber = " << p_Data.m_OtherNumber << "\n"
		<< "m_FloatNumber = " << p_Data.m_FloatNumber << "\n"
		<< "m_FloatNumber2 = " << p_Data.m_FloatNumber2 << "\n"
		<< "m_FloatNumber3 = " << p_Data.m_FloatNumber3 << "\n"
		<< "m_FloatNumber4 = " << p_Data.m_FloatNumber4 << "\n"
		<< "m_FloatNumber5 = " << p_Data.m_FloatNumber5 << "\n" << std::endl;

	return p_OutputStream;
};
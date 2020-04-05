/*
Test Program for MyVector class and vector
Created by Juan Marcelo Portillo in Spring 2020.
*/

#include <vector>
#include <iostream>
#include <chrono>

#include "MyVector.h"

using namespace std;
using namespace std::chrono;

int main()
{
	// Objective of this Test:
	// Emplace(not push) 1 mill elements and measure how long it takes to do this 100 times, while also avoiding mem leaks.
	// Optional: Because the Algorithm to Resize the Capacity can be different, we can use Reserve on both to check purely on Element alloc

	const int test_count = 100;
	const int element_count = 1'000'000;
	using TestType = int;

	time_point<steady_clock> start_time;
	
	//milliseconds
	long long myvector_delta_time; 
	long long stlvector_delta_time; 

	start_time = high_resolution_clock::now();

	cout << "Test to emplace " << element_count << " elements for every test (" << test_count << " total tests)\n";
	//MyVector
	for (int i = 0; i < test_count; ++i)
	{
		MyVector<TestType> v;
		v.reserve(element_count);
		for (int j = 0; j < element_count; ++j)
			v.emplace_back(); //default constructor of the type we are emplacing
	}

	myvector_delta_time = duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count();
	
	cout << "MyVector took " << myvector_delta_time << " ms to finish the test.\n";


	//STL vector
	for (int i = 0; i < test_count; ++i)
	{
		vector<TestType> v;
		v.reserve(element_count);
		for (int j = 0; j < element_count; ++j)
			v.emplace_back(); //default constructor of the type we are emplacing
	}

	stlvector_delta_time = duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count();

	cout << "STL vector took " << stlvector_delta_time << " ms to finish the test.\n";

	//Final comparison!
	cout << "MyVector was " << ((double)stlvector_delta_time / (double)myvector_delta_time) << " times faster than STL vector in this test.\n";
}
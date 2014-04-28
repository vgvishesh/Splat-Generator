/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** main.cpp
** file to initialize the data and run the application
**
** Author: Vishesh Gupta
** -------------------------------------------------------------------------*/
#include<iostream>
#include "plyloader.h"

using namespace std;

void main()
{
	PLYModel MODEL("test_m_bin.ply");
	//MODEL.PLYSplatedModel("test_computed_ascii.ply",0.05);
	cout<<"\nModel Generated....\n";
	system("pause");
}



/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** plyloader_C_Binary.cpp
** This file contains the function definitions of the functions declared in the header file (plyloader.h)
**
** Author: Vishesh Gupta
** -------------------------------------------------------------------------*/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include<math.h>
#include <fstream>
#include <string>
#include "plyloader.h"


using namespace std;
using namespace glm;

PLYModel::PLYModel() {
}

struct data
{
	float _x, _y, _z;
    	float _nx, _ny, _nz;
    	unsigned char _r, _g, _b, _a;
	data(){
		 _x= _y=_z=_nx=_ny=_nz=_r=_g=_b=_a=0;
	}
	data(glm::vec3 position,glm::vec3 normal,glm::vec4 color)
	{
		_x= position.x;
		_y= position.y;
		_z= position.z;
		_nx=normal.x;
		_ny=normal.y;
		_nz=normal.z;
		_r=color.x;
		_g=color.y;
		_b=color.z;
		_a=color.w;
	}
};

struct index1
{
	int s1,s2,s3,s4;
	
	index1()
	{
		s1=s2=s3=s4=0;
	}
	index1(int a,int b,int c,int d)
	{
		s1=a;
		s2=b;
		s3=c;
		s4=d;	
	}
};


// Function to Read the binary .PLY file
PLYModel::PLYModel(const char* filename) 
{
    struct data Values;
	
    string line;
    string s1, s2;

    ifstream inputPly;
    inputPly.open(filename,ios::binary);
    if (!inputPly.is_open()) 
    {
    	cerr << "Couldn't open " << filename << '\n';
    	exit(1);
    }

	cout<<"\n File reading started";

    	getline(inputPly, line);
	while(line.compare("vertex")!=0)
	{
		getline(inputPly, line,' ');
		cout<<line<<"\n";
	}
	inputPly>>vertexCount; // 3rd line
	cout<<vertexCount;
	while(line.compare("end_header")!=0)
	{
		getline(inputPly,line);
		cout<<line<<"\n";
	}
	
	
	for (long int i = 0; i < vertexCount; i++) 
	{
        inputPly.read((char *)&Values,sizeof(Values));
		
		positions.push_back(vec3(Values._x, Values._y, Values._z));	
        normals.push_back(vec3(Values._nx, Values._ny, Values._nz));
		// for files having color information in range [0,255]
        colors.push_back(vec4((float)Values._r, (float)Values._g, (float)Values._b, (float)Values._a));
    }

	cout<<"\nFile read complete";
}

void RotateX(glm::vec3 &P, double ang)
{
	float y= P.y*cos(ang) - P.z*sin(ang);
	float z= P.y*sin(ang) + P.z*cos(ang);
	P.y=y;
	P.z=z;
}

void RotateY(glm::vec3 &P, double ang)
{
	float x= P.x*cos(ang) + P.z*sin(ang);
	float z= -1*P.x*sin(ang) + P.z*cos(ang);
	P.x=x;
	P.z=z;
}

void Translate(glm::vec3 &P,glm::vec3 V)
{
	P.x+=V.x;
	P.y+=V.y;
	P.z+=V.z;
}

void makeSplat(glm::vec3 &P, glm::vec3 pos,glm::vec3 nor)
{
	// enrter the rotation and translation code here
	float magNormal= sqrt(nor.x*nor.x + nor.y*nor.y + nor.z * nor.z);
	double theta = asin(nor.y/magNormal);
	double phi = atan(nor.x/nor.z);

	RotateX(P,theta);
	RotateY(P,phi);
	Translate(P,pos);
}


//Function to write the binary .PLY file...
void PLYModel :: PLYSplatedModel(const char *filename,float splatsize)
{
	FILE *outputPly;
	outputPly=fopen(filename,"wb");
	clock_t loop,begin=clock();
	
	//for now generating only ascii files.....
	fprintf(outputPly,"ply\n");
	fprintf(outputPly,"format binary_little_endian 1.0\n");
	fprintf(outputPly,"comment This contains a Splatted Point Cloud\n");
	fprintf(outputPly,"element vertex %d\n",4*vertexCount);
	fprintf(outputPly,"property float x\n");
	fprintf(outputPly,"property float y\n");
	fprintf(outputPly,"property float z\n");
	fprintf(outputPly,"property float nx\n");
	fprintf(outputPly,"property float ny\n");
	fprintf(outputPly,"property float nz\n");
	fprintf(outputPly,"property uchar red\n");
	fprintf(outputPly,"property uchar green\n");
	fprintf(outputPly,"property uchar blue\n");
	fprintf(outputPly,"property uchar alpha\n");
	fprintf(outputPly,"element face %d\n",vertexCount);
	fprintf(outputPly,"property list uchar int vertex_indices\n");
	fprintf(outputPly,"end_header\n");

	glm::vec3 a(splatsize/2,splatsize/2,0);
	glm::vec3 b(splatsize/2,-1*splatsize/2,0);
	glm::vec3 c(-1*splatsize/2,-1*splatsize/2,0);
	glm::vec3 d(-1*splatsize/2,splatsize/2,0);
	
	cout<<"\nMaking Splated PLY";
	loop=clock();

	for(long int i = 0,count=0; i < vertexCount ; i++, count++) 
	{
		glm::vec3 A(a),B(b),C(c),D(d);

		makeSplat(A,positions[i],normals[i]);
		makeSplat(B,positions[i],normals[i]);
		makeSplat(C,positions[i],normals[i]);
		makeSplat(D,positions[i],normals[i]);

		struct data aa(A,normals[i],colors[i]),
					bb(B,normals[i],colors[i]),
					cc(C,normals[i],colors[i]),
					dd(D,normals[i],colors[i]);
		//cout<<"\n"<<aa._r<<"\t"<<aa._g<<"\t"<<aa._b;

		fwrite(&aa,sizeof(aa),1,outputPly);
		fwrite(&bb,sizeof(bb),1,outputPly);
		fwrite(&cc,sizeof(cc),1,outputPly);
		fwrite(&dd,sizeof(dd),1,outputPly);

		if(count==1000000)
		{
			cout<<"\n"<<i<<"\tTime taken = "<<double(clock()-loop)/CLOCKS_PER_SEC;
			count=0;
			loop=clock();
		}
	}


	unsigned char sides=4;
	for(int i = 0; i < vertexCount; i++) 
	{
		int serial=i*4;
		struct index1 W(serial,serial+1,serial+2,serial+3);
		fwrite(&sides,sizeof(unsigned char),1,outputPly);
		fwrite(&W,sizeof(W),1,outputPly);
	}

	fclose(outputPly);

	cout<<"\nPLY completed";
	cout<<"\nTotal time taken = "<<double(clock()-begin)/CLOCKS_PER_SEC;
}

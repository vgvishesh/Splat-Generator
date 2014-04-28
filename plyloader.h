/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** plyloader.h
** This file contains the structure definition of the structure used to read, modify 
** the .ply point cloud file and again write back the splatted .ply file.
**
** Author: Vishesh Gupta
** -------------------------------------------------------------------------*/
#ifndef PLYLOADER_H
#define	PLYLOADER_H

#include <glm/glm.hpp>
#include <vector>

struct PLYModel {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;

    unsigned long int vertexCount; //number of vertices
    
	PLYModel();
    PLYModel(const char *filename);
	void PLYSplatedModel(const char *filename,float splatsize);
};


#endif	/* PLYLOADER_H */

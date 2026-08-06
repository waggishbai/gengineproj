#ifndef TINYOBJLOADER_IMPLEMENTATION

#define TINYOBJLOADER_IMPLEMENTATION
#include <objloader/tiny_obj_loader.h>

#endif

#include <iostream>
#include <vector>

#ifndef AUTOLOAD_OBJ_H
#define AUTOLOAD_OBJ_H

std::vector<float> autoloadObj(const char* objname, const char* mtlname) {
    using namespace tinyobj;
    using namespace std;

    attrib_t attrib;
    vector<shape_t> shapes;
    vector<material_t> mats;
    string err;
    string warn;

    if (mtlname == NULL) {
    	LoadObj(&attrib, &shapes, &mats, &err, &warn, objname);
    } else {
	LoadObj(&attrib, &shapes, &mats, &err, &warn, objname, mtlname);
    }


    if (warn != "") {
        cerr << "tinyobj warnings: " << warn << endl;
    }
    if (err != "") {
        cerr << "tinyobj errors: " << err << endl;
    }


    vector<float> output;

    for (int i = 0; i < shapes.size(); i++) {
        mesh_t mesh = shapes[i].mesh;

        for (int j = 0; j < mesh.indices.size(); j++) {
            int ind = mesh.indices[j].vertex_index;

            output.push_back(attrib.vertices[ind * 3 + 0]);
            output.push_back(attrib.vertices[ind * 3 + 1]);
            output.push_back(attrib.vertices[ind * 3 + 2]);

            ind = mesh.indices[j].normal_index;

            output.push_back(attrib.normals[ind * 3 + 0]);
            output.push_back(attrib.normals[ind * 3 + 1]);
            output.push_back(attrib.normals[ind * 3 + 2]);

            ind = mesh.indices[j].texcoord_index;

            output.push_back(attrib.texcoords[ind * 2 + 0]);
            output.push_back(attrib.texcoords[ind * 2 + 1]);

        }
    }
    return output;
}

#endif

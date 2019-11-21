#include "obj.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>

namespace renderframework { namespace vector {

    Obj::Obj( std::string objFile )
    {
    }

    Obj::~Obj()
    {
    }

    bool Obj::load() {
      std::ifstream f(mObjFile);
      if( !f.is_open() ) return false;

      auto curLine = 0u;
      std::string line;
      while( std::getline(f, line) ) {
        ++curLine;
        if( line.empty() ) continue;
        if( line.size() < 2 ) continue;
        if( line[0] == '#' ) continue;
        else if( line.substr(0, 2) == "vt" ) objTexCoord(line);
        else if( line.substr(0, 2) == "vn" ) objVertNormal(line);
        else if( line[0] == 'f' ) objFace(line);
        else if( line[0] == 'v' ) objVert(line);
        else std::cerr << "Obj::load: Ignoring unhandled entry at line " << curLine << " : " << mObjFile << std::endl;
      }

      /* Stuff that isn't handled here yet
        ...
        # Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement ( see below )
        vp 0.310000 3.210000 2.100000
        vp ...

        ...
        # Line element (see below)
        l 5 8 1 2 4 9
      */

      // At least for now we need to return equal size arrays
      // here, otherwise we'll crash or somesuch
      mNormals.resize(mVertices.size(), {0.f,0.f,0.f});
      mVertexColors.resize(mVertices.size(), {1.f,1.f,1.f,1.f});

      return true;
    }

    void Obj::objTexCoord(std::string line) {
      // # List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.
      // vt 0.500 1 [0]
    }

    void Obj::objVertNormal(std::string line) {
      // # List of vertex normals in (x,y,z) form; normals might not be unit vectors.
      // vn 0.707 0.000 0.707
    }

    void Obj::objFace(std::string line) {
      // # Polygonal face element (see below)
      // f 1 2 3
      // f 3/1 4/2 5/3
      // f 6/4/1 3/5/3 7/6/5
      // f 7//1 8//2 9//3
    }

    void Obj::objVert(std::string line) {
      // # List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.0.
      // v 0.123 0.234 0.345 1.0
      std::regex regex{R"([\s]+)"}; // split on space and comma
      std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
      std::vector<std::string> parts{it, {}};

      if( parts.size() < 4 ) return;
      auto x = std::stof(parts[1]);
      auto y = std::stof(parts[2]);
      auto z = std::stof(parts[3]);
      auto w = 1.0f;
      if( parts.size() == 5 ) w = std::stof(parts[4]);

      addVertex({ x, y, z });
    }
} }

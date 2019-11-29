#include "obj.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <array>
#include <set>

namespace renderframework { namespace vector {

    Obj::Obj( std::string objFile )
      : Vector(true)
      , mObjFile(objFile)
    {
      if( !load() ) throw std::runtime_error("Obj: Failed to load: " + objFile);
    }

    Obj::~Obj()
    {
    }

    bool Obj::load() {
      std::ifstream f(mObjFile);
      if( !f.is_open() ) return false;

      mIndices.clear();
      mVertNorms.clear();
      mVertCoords.clear();
      mVertTexCoords.clear();


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

      // Translate the separate obj indexes to the rf layout
      std::set<VertexDef> rfVerts;
      for( auto& i : mObjIndices ) {
        auto vertI = i[0];
        auto texI = i[1];
        auto normI = i[2];

        VertexDef vertex;
        // And don't forget .obj indices start at 1
        if( mVertCoords.size() >= vertI ) vertex.coord = mVertCoords[vertI - 1];
        if( mVertTexCoords.size() >= vertI ) vertex.texCoord = mVertTexCoords[texI - 1];
        if( mVertNorms.size() >= normI ) vertex.normal = mVertNorms[normI - 1];

        auto existingVert = std::find(mVertices.begin(), mVertices.end(), vertex);
        if( existingVert != mVertices.end() ) {
          mIndices.emplace_back( std::distance(mVertices.begin(), existingVert) );
        } else {
          mVertices.emplace_back( vertex );
          mIndices.emplace_back( mVertices.size() - 1 );
        }
      }

      return true;
    }

    void Obj::objTexCoord(std::string line) {
      // # List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.
      // vt 0.500 1 [0]
      std::regex regex{R"([\s]+)"}; // split on space
      std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
      std::vector<std::string> parts{it, {}};

      if( parts.size() < 2 ) return;
      parts.erase(parts.begin());
      auto u = std::stof(parts[0]);
      auto v = 0.f;
      // auto w = 0.f;
      if( parts.size() >= 2 ) v = std::stof(parts[1]);
      // if( parts.size() >= 3 ) w = std::stof(parts[2]);

      std::cerr << "Obj::load: Render Framework only supports u,v tex coords. w ignored" << std::endl;

      mVertTexCoords.push_back({u,v});
    }

    void Obj::objVertNormal(std::string line) {
      // # List of vertex normals in (x,y,z) form; normals might not be unit vectors.
      // vn 0.707 0.000 0.707
      std::regex regex{R"([\s]+)"}; // split on space
      std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
      std::vector<std::string> parts{it, {}};

      if( parts.size() < 4 ) return;
      auto x = std::stof(parts[1]);
      auto y = std::stof(parts[2]);
      auto z = std::stof(parts[3]);

      mVertNorms.push_back({x,y,z});
    }

    void Obj::objFace(std::string line) {
      // # Polygonal face element (see below)
      // vertIndex: f 1 2 3
      // vertIndex/texCoordIndex: f 3/1 4/2 5/3
      // vertIndex/texCoordIndex/vertNormIndex/: f 6/4/1 3/5/3 7/6/5
      // vertIndex//vertNormIndex: f 7//1 8//2 9//3

      std::regex regexSpace{R"([\s]+)"}; // split on space
      std::regex regexSlash{R"([/]+)"}; // split on slash

      std::sregex_token_iterator it{line.begin(), line.end(), regexSpace, -1};
      std::vector<std::string> parts{it, {}};

      if( parts.size() < 4 ) return;
      // Eat the f
      parts.erase(parts.begin());

      for( auto i = 0u; i < 3; ++i )
      {
        auto part = parts[i];
        std::sregex_token_iterator it2{part.begin(), part.end(), regexSlash, -1};
        std::vector<std::string> parts2{it2, {}};

        glm::vec<3,GLuint> face;
        face[0] = static_cast<GLuint>(std::stoul(parts2[0]));
        // If there's 3 parts it means all parameters were specified, else just the coord/normal
        if( parts2.size() == 3 ) {
          face[1] = static_cast<GLuint>(std::stoul(parts2[1]));
          face[2] = static_cast<GLuint>(std::stoul(parts2[2]));
        } else {
          face[2] = static_cast<GLuint>(std::stoul(parts2[1]));
        }

        mObjIndices.emplace_back(face);
      }
    }

    void Obj::objVert(std::string line) {
      // # List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.0.
      // v 0.123 0.234 0.345 1.0
      std::regex regex{R"([\s]+)"}; // split on space
      std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
      std::vector<std::string> parts{it, {}};

      if( parts.size() < 4 ) return;
      auto x = std::stof(parts[1]);
      auto y = std::stof(parts[2]);
      auto z = std::stof(parts[3]);
      // auto w = 1.0f;
      // if( parts.size() == 5 ) w = std::stof(parts[4]);

      //addVertex({ x, y, z });
      mVertCoords.push_back({x,y,z});
    }
} }

#ifndef BE_OBJ_LOADER_H_INCLUDED
#define BE_OBJ_LOADER_H_INCLUDED

// #include "src/math/vector3f.h"
// #include <vector>
// #include <map>
// #include "src/graphics/be_texture.h"

#include "kernel/be_log.h"
#include "filesystem/be_file.h"
#include "LinearMath/btVector3.h"
#include "utils/texturestore.h"
#include "math/vector3f.h"
#include "graphics/be_material.h"
#include "filesystem/be_filesystem.h"
#include "LinearMath/btTransform.h"

#include <unordered_map>

//Andy - to be renamed, probably will become BeEffect eventually
class Material
{
public:
	std::string m_imagePath;
	std::string m_imagePathBump;
	BeMaterial m_material;
	boost::shared_ptr<BeTexture2DResource> m_imageTexture2D;
	boost::shared_ptr<BeTexture2DResource> m_imageTexture2DBump;
	
	Material()
	{	
	}
	
	~Material()
	{
	}
};

struct Facematerial
{
	unsigned int faceindex_first;
	unsigned int faceindex_number;
	std::string material;
};

class BeObjLoader
{
	public:
		BeObjLoader( BeFilesystem& filesystem, BeFile& befile, const btVector3& scale, const btTransform& transform );
		~BeObjLoader();

		std::string m_filename;

		unsigned int getNumberVertices() const { return m_numberVertices; }
		unsigned int getNumberNormals() const { return m_numberNormals; }
		unsigned int getNumberTexCoor() const { return m_numberTexCoor; }
		unsigned int getNumberVIndices() const { return m_numberVIndices; }
// 		unsigned int m_numberTIndices const { return m_numberTIndices; }
// 		unsigned int m_numberNIndices const { return m_numberNIndices; }
		
		btScalar* gVertices;
		float* gNormals;
		float* gTexCoor;
		int* gVIndices;
		int* gTIndices;
		int* gNIndices;
		
		std::unordered_map<std::string, Material> matlist;
		typedef std::unordered_map <std::string, Material>::const_iterator matlist_iterator;
		matlist_iterator matit;

		std::vector<Facematerial> facematerials;
		void loadMaterialfile( BeFilesystem& filesystem, const std::string& mpath, const std::string& file );
		
	private:

		// FIXME check if all of these are needed
		unsigned int m_numberVertices;
		unsigned int m_numberNormals;
		unsigned int m_numberTexCoor;
		unsigned int m_numberVIndices;
		unsigned int m_numberTIndices;
		unsigned int m_numberNIndices;

		BeLogDebug m_logDebug;
};

#endif

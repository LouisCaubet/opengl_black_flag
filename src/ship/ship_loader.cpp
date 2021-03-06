#include "ship_loader.hpp"


#include <iostream>

using namespace cgp;

ShipLoader::ShipLoader() {

}

ShipLoader::~ShipLoader() {

}

void ShipLoader::loadObj() {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("assets/BlackPearl/black pearl retextured revised.obj", aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR - ASSIMP COULD NOT LOAD MODEL" << std::endl;
		return;
	}

	// Transform Assimp data structures into CGP data structures
	processMaterials(scene);

	processNode(scene->mRootNode, scene);

	drawMeshes(scene);

}

void ShipLoader::processNode(aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		convertMesh(mesh, scene); 
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void ShipLoader::convertMesh(aiMesh* m, const aiScene* scene) {

	int mtlId = m->mMaterialIndex;
	mesh& cgp_mesh = rawMeshes[mtlId];
	int n = cgp_mesh.position.size();

	for (unsigned int i = 0; i < m->mNumVertices; i++) {
		vec3 vertex;
		vertex.x = m->mVertices[i].x;
		vertex.y = m->mVertices[i].y;
		vertex.z = m->mVertices[i].z;
		cgp_mesh.position.push_back(vertex);

		

		if(m->HasNormals()) {
			vec3 normal;
			normal.x = m->mNormals[i].x;
			normal.y = m->mNormals[i].y;
			normal.z = m->mNormals[i].z;
			cgp_mesh.normal.push_back(normal);
		}


		
		if (m->HasTextureCoords(0)) {
			vec2 uv = { 0.0f, 0.0f };
			uv.x = m->mTextureCoords[0][i].x;
			uv.y = m->mTextureCoords[0][i].y;
			cgp_mesh.uv.push_back(uv);
		}

	
		
	}

	for (int i = 0; i < m->mNumFaces; i++) {
		aiFace face = m->mFaces[i];
		uint3 ids;
		
		
		if (face.mNumIndices == 2) {
			ids = { n+face.mIndices[0], n+face.mIndices[1], n+face.mIndices[0] };
		}
		else {
			ids = { n+face.mIndices[0], n+face.mIndices[1], n+face.mIndices[2] };
		}
		cgp_mesh.connectivity.push_back(ids);
	}

}

void ShipLoader::drawMeshes(const aiScene* scene) {
	for (int i = 0; i < scene->mNumMaterials; i++) {

		if (rawMeshes[i].position.size() == 0) continue;

		rawMeshes[i].fill_empty_field();
		mesh_drawable mesh;
		mesh.initialize(rawMeshes[i]);

		aiMaterial* mtl = scene->mMaterials[i];

		mesh.texture = textures[i];

		float alpha;
		mtl->Get(AI_MATKEY_OPACITY, alpha);
		mesh.shading.alpha = alpha;

		aiColor3D ambient(0.f, 0.f, 0.f);
		mtl->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		aiColor3D diffuse(0.f, 0.f, 0.f);
		mtl->Get(AI_MATKEY_COLOR_DIFFUSE, ambient);
		aiColor3D specular(0.f, 0.f, 0.f);
		mtl->Get(AI_MATKEY_COLOR_SPECULAR, ambient);

		mesh.shading.phong = { ambient.r + diffuse.r + specular.r, ambient.g + diffuse.g + specular.g, ambient.b + diffuse.b + specular.b };

		meshes.push_back(mesh);
	}
}

void ShipLoader::processMaterials(const aiScene* scene) {
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiString texture;
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
		std::string textureName = texture.C_Str();
		if (textureName != "") {
			textures.push_back(opengl_load_texture_image("assets/BlackPearl/" + textureName));
		}
		else {
			textures.push_back(opengl_load_texture_image("assets/placeholder.jpg"));
		}
		mesh mesh;
		rawMeshes.push_back(mesh);
	}
}
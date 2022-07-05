#include "Model.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


Model::Model(ObjectStructure* object)
    : modelObj(object)
{
	loadModel(modelObj->path);
    meshesAABB.push_back(createAABB());
}

Model::~Model()
{
    std::cout << "Model Deconstructor" << std::endl;

    for (int i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
    
    delete meshesAABB[0];
}

void Model::Draw(Shader& shader, GLenum mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->DrawTriangle(shader);
    }

    if (modelObj->AABB == true)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        meshesAABB[0]->DrawTriangle(shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    unsigned int importOptions = aiProcess_Triangulate
        | aiProcess_OptimizeMeshes
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_CalcTangentSpace;
    const aiScene* scene = import.ReadFile(path, importOptions);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Renderer* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    std::cout << "\nmName: " << mesh->mName.C_Str() << std::endl;


    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        xMin = t1.minValue(vector.x, xMin);
        xMax = t1.maxValue(vector.x, xMax);
        yMin = t1.minValue(vector.y, yMin);
        yMax = t1.maxValue(vector.y, yMax);
        zMin = t1.minValue(vector.z, zMin);
        zMax = t1.maxValue(vector.z, zMax);

        vertex.color.x = modelObj->modelColor.x;
        vertex.color.y = modelObj->modelColor.y;
        vertex.color.z = modelObj->modelColor.z;
        //std::cout << "COLOR " << glm::to_string(vertex.color) << std::endl;

        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
            //// tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.bitangent = vector;
        }
        else
            vertex.texUV = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    aabbVertices.push_back(xMin);
    aabbVertices.push_back(xMax);
    aabbVertices.push_back(yMin);
    aabbVertices.push_back(yMax);
    aabbVertices.push_back(zMin);
    aabbVertices.push_back(zMax);

    //std::cout << zMin << ' ' << zMax << std::endl;

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    //std::cout <<"Reflection Count: " << material->GetTextureCount(aiTextureType_REFLECTION) << std::endl;

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
    return new Renderer(vertices, indices, textures);
}

Renderer* Model::createAABB()
{       
    std::vector<Vertex> colliderVertices;
    std::vector<GLuint> colliderIndices
    {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3,
    };
    std::vector<Texture> colliderTextures;

    Vertex colliderVertex;
    glm::vec3 colliderVector;
    
    colliderVertex.position = glm::vec3(aabbVertices[0], aabbVertices[2], aabbVertices[5]);;
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[1], aabbVertices[2], aabbVertices[5]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[1], aabbVertices[3], aabbVertices[5]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[0], aabbVertices[3], aabbVertices[5]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[0], aabbVertices[2], aabbVertices[4]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[1], aabbVertices[2], aabbVertices[4]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[1], aabbVertices[3], aabbVertices[4]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    colliderVertex.position = glm::vec3(aabbVertices[0], aabbVertices[3], aabbVertices[4]);
    colliderVertex.color = glm::vec3(0.0f, 1.0f, 0.0f);
    colliderVertices.push_back(colliderVertex);

    return new Renderer(colliderVertices, colliderIndices, colliderTextures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    //std::cout << "Texture Count: " << mat->GetTextureCount(type) << std::endl;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
            {
                std::cout << "SAME PATH " << std::endl;
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture(str.C_Str(), typeName, i);
            
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}

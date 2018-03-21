#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	srand(time(NULL));
}

TerrainGenerator::~TerrainGenerator()
{
}

Geometry * TerrainGenerator::generateTerrain(int N)
{
	if (N < 2)
	{
		return NULL;
	}
	if (!(N % 2))
	{
		N += 1;
	}
	return createTerrain(N);
}

Geometry * TerrainGenerator::createTerrain(int N)
{
	mat.clear();
	Geometry* terrain = new Geometry();
    terrain->color = glm::vec3(0.6, 1, 0.7);
    terrain->terr = 1.0f;
	float i;
	float levels = (int)(log2(N) + 10) * N;
	float row;
	float col;
	bool alternateToRow;
	mat[0][0] = RANDOMIZE(MINHEIGHT, MAXHEIGHT);
	mat[0][N - 1] = RANDOMIZE(MINHEIGHT, MAXHEIGHT);
	mat[N - 1][0] = RANDOMIZE(MINHEIGHT, MAXHEIGHT);
	mat[N - 1][N - 1] = RANDOMIZE(MINHEIGHT, MAXHEIGHT);
	float blockSize;
	float halfBlockSize;
    
    float decrease = 1;
	for (i = 1; i < levels; i *= 2)
	{
		blockSize = (((float)N) - 1.0f) / i;
		halfBlockSize = blockSize / 2;
		for (row = 0; row <= ((float)(N - 1)) - halfBlockSize; row += blockSize)
		{
			for (col = 0; col <= ((float)(N - 1)) - halfBlockSize; col += blockSize)
			{
				mat[row + halfBlockSize][col + halfBlockSize] = MEAN4(mat[row][col], mat[row + blockSize][col], mat[row][col + blockSize], mat[row + blockSize][col + blockSize]) + PLUSMINUS(VARIANCE * decrease);;
			}
		}
		alternateToRow = true;
		for (row = 0; row <= ((float)N - 1); row += halfBlockSize)
		{
			if (alternateToRow)
			{
				for (col = halfBlockSize; col < ((float)N - 1); col += blockSize)
				{
					mat[row][col] = (row 
							? ((row < ((float)N - 1))
								? MEAN4(mat[row - halfBlockSize][col], mat[row + halfBlockSize][col], mat[row][col + halfBlockSize], mat[row][col - halfBlockSize])
								: MEAN3(mat[row - halfBlockSize][col], mat[row][col + halfBlockSize], mat[row][col - halfBlockSize]))
							: MEAN3(mat[row + halfBlockSize][col], mat[row][col + halfBlockSize], mat[row][col - halfBlockSize]))
						+ PLUSMINUS(VARIANCE * decrease);;
				}
				alternateToRow = !alternateToRow;
			}
			else
			{
				for (col = 0; col <= ((float)(N - 1)); col += blockSize)
				{
					mat[row][col] = (col
						? ((col < ((float)(N - 1)))
							? MEAN4(mat[row - halfBlockSize][col], mat[row + halfBlockSize][col], mat[row][col + halfBlockSize], mat[row][col - halfBlockSize])
							: MEAN3(mat[row - halfBlockSize][col], mat[row + halfBlockSize][col], mat[row][col - halfBlockSize]))
						: MEAN3(mat[row - halfBlockSize][col], mat[row + halfBlockSize][col], mat[row][col + halfBlockSize]))
						+ PLUSMINUS(VARIANCE * decrease);;
				}
				alternateToRow = !alternateToRow;
			}
		}
        decrease = decrease * RETAIN;
	}
	calculateVerticesAndIndices(terrain->getVertices(), terrain->getNormals(), terrain->getIndices(), N, levels);
    terrain->centerForTerrain();
	terrain->init();
	return terrain;
}


void TerrainGenerator::calculateVerticesAndIndices(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, int N, int levels)
{
	size_t currIndex;
	float row;
	float col;
	//float stepSize = (levels - 1) ? (float)(((float)N - 1) / (2.0f * ((float)levels - 1))) : N - 1;
	std::vector<float> index;
	index.reserve(mat.size());
	std::unordered_map<float, std::unordered_map<float, float>>::iterator end = mat.end();
	for (std::unordered_map<float, std::unordered_map<float, float>>::iterator it = mat.begin(); it != end; ++it)
	{
		index.push_back(it->first);
	}
	std::sort(index.begin(), index.end());
	size_t size = index.size() - 1;
	for (row = 0; row < size; ++row)
	{
		vertices.push_back(glm::vec3(0, mat[index[row]][index[0]], index[row]));
		vertices.push_back(glm::vec3(0, mat[index[row + 1]][index[0]], index[row + 1]));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		for (col = 0; col < size; ++col)
		{
			vertices.push_back(glm::vec3(index[col + 1], mat[index[row]][index[col + 1]], index[row]));
			vertices.push_back(glm::vec3(index[col + 1], mat[index[row + 1]][index[col + 1]], index[row + 1]));

			currIndex = vertices.size() - 1;

			normals.push_back(glm::cross(vertices[currIndex - 1] - vertices[currIndex - 2], vertices[currIndex - 3] - vertices[currIndex - 2]));
			normals.push_back(glm::cross(vertices[currIndex] - vertices[currIndex - 2], vertices[currIndex - 1] - vertices[currIndex - 2]));

			indices.push_back(currIndex - 3);
			indices.push_back(currIndex - 2);
			indices.push_back(currIndex - 1);
			indices.push_back(currIndex - 1);
			indices.push_back(currIndex - 2);
			indices.push_back(currIndex);
		}
	}
	/*for (row = 0; row <= (((float)N - 1) - stepSize); row += stepSize)
	{
		vertices.push_back(glm::vec3(0, mat[row][0], row));
		vertices.push_back(glm::vec3(0, mat[row + stepSize][0], row + stepSize));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		for (col = 0; col <= (((float)(N - 1)) - stepSize); col += stepSize)
		{
			vertices.push_back(glm::vec3(col + stepSize, mat[row][col + stepSize], row));
			vertices.push_back(glm::vec3(col + stepSize, mat[row + stepSize][col + stepSize], row + stepSize));

			currIndex = vertices.size() - 1;

			normals.push_back(glm::cross(vertices[currIndex - 1] - vertices[currIndex - 2], vertices[currIndex - 3] - vertices[currIndex - 2]));
			normals.push_back(glm::cross(vertices[currIndex] - vertices[currIndex - 2], vertices[currIndex - 1] - vertices[currIndex - 2]));

			indices.push_back(currIndex - 3);
			indices.push_back(currIndex - 2);
			indices.push_back(currIndex - 1);
			indices.push_back(currIndex - 1);
			indices.push_back(currIndex - 2);
			indices.push_back(currIndex);
		}
	}*/
}

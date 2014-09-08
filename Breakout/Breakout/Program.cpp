#include "Graphics\GraphicsManager.h"
#include "Storage\FileManager.h"

GraphicsManager *m_GraphicsManager;

int main(int argc, char** argv)
{
	//m_GraphicsManager = new GraphicsManager();
	//m_GraphicsManager->InitWindow(600, 400);

	ModelData* test = FileManager().LoadFile<ModelData>("sphere.obj");

	system("pause");

	return 0;
}
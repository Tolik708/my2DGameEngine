#pragma once

namespace myScene
{
	GameObject *boxes;

	//meshes
	mesh mesh1;
	mesh mesh2;
	mesh mesh3;
	mesh mesh4;
	mesh mesh5;
	mesh mesh6;
	mesh mesh7;

	void Meshes()
	{
		mesh1.meshIs = line;
		mesh1.vertexAmm = 5;
		mesh1.vertesies.reserve(mesh1.vertexAmm);
		mesh1.vertesies.push_back({-1, -1});
		mesh1.vertesies.push_back({1, -1});
		mesh1.vertesies.push_back({1, 1});
		mesh1.vertesies.push_back({-1, 1});
		mesh1.vertesies.push_back({-1, -1});
		
		mesh2.meshIs = circle;
		mesh2.pathToTexture = "C:\\proggraming\\gameArt\\test1.bmp";
		mesh2.genMesh();
		
		mesh3.meshIs = polygon;
		mesh3.vertexAmm = 4;
		mesh3.vertesies.reserve(mesh3.vertexAmm);
		mesh3.vertesies.push_back({-1, -1});
		mesh3.vertesies.push_back({1, -1});
		mesh3.vertesies.push_back({1, 1});
		mesh3.vertesies.push_back({-1, 1});
		mesh3.pathToTexture = "C:\\proggraming\\gameArt\\test2.bmp";
		mesh3.genMesh();
		
		mesh4.meshIs = polygon;
		mesh4.vertexAmm = 6;
		mesh4.vertesies.reserve(mesh4.vertexAmm);
		mesh4.vertesies.push_back({2, 1});
		mesh4.vertesies.push_back({3, 2});
		mesh4.vertesies.push_back({3, 3});
		mesh4.vertesies.push_back({2, 4});
		mesh4.vertesies.push_back({1, 3});
		mesh4.vertesies.push_back({1, 2});
		mesh4.pathToTexture = "C:\\proggraming\\gameArt\\test2.bmp";
		mesh4.genMesh();
		
		mesh5.meshIs = polygon;
		mesh5.vertexAmm = 5;
		mesh5.vertesies.reserve(mesh5.vertexAmm);
		mesh5.vertesies.push_back({1, 1});
		mesh5.vertesies.push_back({3, 1});
		mesh5.vertesies.push_back({3, 3});
		mesh5.vertesies.push_back({2, 2});
		mesh5.vertesies.push_back({1, 3});
		mesh5.pathToTexture = "C:\\proggraming\\gameArt\\test1.bmp";
		mesh5.genMesh();
		
		mesh6.meshIs = box;
		mesh6.genMesh();
		
		mesh7.meshIs = fastTriangle;
		mesh7.vertexAmm = 3;
		mesh7.vertesies.reserve(mesh7.vertexAmm);
		mesh7.vertesies.push_back({1, 1});
		mesh7.vertesies.push_back({2, 2});
		mesh7.vertesies.push_back({1, 2});
		mesh7.tConects.push_back({{0, 1, 2}});
		mesh7.genMesh();
	}
	
	void Scene()
	{
		boxes = new GameObject[7];
		boxes[0].position = {250, 250};
		boxes[0].myMesh = mesh1;
		boxes[0].SetSize({30, 30});
		
		boxes[1].position = {100, 100};
		boxes[1].myMesh = mesh2;
		boxes[1].SetSize({90, 90});
		
		boxes[2].position = {300, 300};
		boxes[2].myMesh = mesh3;
		boxes[2].SetSize({90, 90});
		
		boxes[3].position = {250, 400};
		boxes[3].myMesh = mesh4;
		boxes[3].myMesh.myColor = {255, 0, 0, 255};
		boxes[3].SetSize({30, 30});
		
		boxes[4].position = {100, 400};
		boxes[4].myMesh = mesh5;
		boxes[4].myMesh.myColor = {0, 255, 0, 255};
		boxes[4].SetSize({50, 50});
		
		boxes[5].position = {300, 100};
		boxes[5].myMesh = mesh6;
		boxes[5].myMesh.myColor = {0, 255, 0, 255};
		boxes[5].SetSize({30, 30});
		
		boxes[6].position = {250, 250};
		boxes[6].myMesh = mesh7;
		boxes[6].myMesh.myColor = {0, 255, 0, 255};
		boxes[6].SetSize({20, 20});
	}

	void Update()
	{
		//boxes[1].rotate(1);
		//boxes[2].rotate(1);
		//boxes[3].rotate(1);
		//boxes[4].rotate(1);

		if (Input::mouse[SDL_BUTTON_LEFT])
		{
			boxes[0].LookAt(Input::mousePos(), 45);
			boxes[1].position = Input::mousePos();
		}
		if (Input::keysDown[SDLK_r])
		{
			boxes[2].SetSize({50, 50});
		}
		if (Input::keysDown[SDLK_y])
		{
			boxes[2].SetSize({20, 20});
			for (int i = 0; i < 7; i++)
			{
				boxes[i].SetColor(new Color(true));
			}
		}
	}
}
#pragma once

namespace myScene
{
	GameObject boxes[6];
	
	//meshes
	mesh mesh1;
	mesh mesh2;
	mesh mesh3;
	mesh mesh4;
	mesh mesh5;
	mesh mesh6;

	void Meshes()
	{
		mesh1.meshIs = line;
		mesh1.vertexAmm = 5;
		mesh1.sicknes = 3;
		mesh1.vertesies.reserve(mesh1.vertexAmm);
		mesh1.vertesies.push_back({1, 1});
		mesh1.vertesies.push_back({2, 1});
		mesh1.vertesies.push_back({2, 2});
		mesh1.vertesies.push_back({1, 2});
		mesh1.vertesies.push_back({1, 1});
		
		mesh2.meshIs = circle;
		mesh2.pathToTexture = "C:\\proggraming\\gameArt\\test1.bmp";
		
		mesh3.meshIs = polygon;
		mesh3.vertexAmm = 3;
		mesh3.vertesies.reserve(mesh3.vertexAmm);
		mesh3.vertesies.push_back({-1, -1});
		mesh3.vertesies.push_back({1, -1});
		mesh3.vertesies.push_back({1, 1});
		//mesh3.vertesies.push_back({-1, 1});
		mesh3.pathToTexture = "C:\\proggraming\\gameArt\\test2.bmp";
		
		mesh4.meshIs = polygon;
		mesh4.vertexAmm = 3;
		mesh4.vertesies.reserve(mesh4.vertexAmm);
		mesh4.vertesies.push_back({1, 1});
		mesh4.vertesies.push_back({3, 1});
		mesh4.vertesies.push_back({-1, 3});
		//mesh4.vertesies.push_back({2, 4});
		//mesh4.vertesies.push_back({1, 3});
		//mesh4.vertesies.push_back({1, 2});
		mesh4.pathToTexture = "C:\\proggraming\\gameArt\\test2.bmp";
		
		mesh5.meshIs = polygon;
		mesh5.vertexAmm = 3;
		mesh5.vertesies.reserve(mesh5.vertexAmm);
		mesh5.vertesies.push_back({1, 1});
		mesh5.vertesies.push_back({3, 1});
		mesh5.vertesies.push_back({3, 3});
		//mesh5.vertesies.push_back({2, 2});
		//mesh5.vertesies.push_back({1, 3});
		mesh5.pathToTexture = "C:\\proggraming\\gameArt\\test1.bmp";
		
		mesh6.meshIs = box;
	}
	
	void Scene()
	{
		boxes[0].position = {250, 250};
		boxes[0].myMesh = &mesh1;
		boxes[0].SetSize(vec2(90, 90));
		boxes[0].layer = 2;
		
		boxes[1].position = {100, 100};
		boxes[1].myMesh = &mesh2;
		boxes[1].SetSize({100, 80});
		
		boxes[2].position = {300, 300};
		boxes[2].myMesh = &mesh3;
		//boxes[2].SetSize({10, 10});
		
		boxes[3].position = {250, 400};
		boxes[3].myMesh = &mesh4;
		boxes[3].SetSize({5, 5});
		std::cout << "yay" << std::endl;
		
		boxes[4].position = {100, 400};
		boxes[4].myMesh = &mesh5;
		//boxes[4].SetSize({50, 50});
		
		boxes[5].position = {300, 100};
		boxes[5].myMesh = &mesh6;
		boxes[5].SetSize({30, 30});
	}

	void Update()
	{
		boxes[1].SetFastSize(vec2(boxes[1].size.x + Input::GetAxisInt(WheelX), boxes[1].size.y + Input::GetAxisInt(WheelX)));
		boxes[2].rotate(Input::GetAxisInt(Vertical));
		if (Input::keys[MouseLeft])
		{
			boxes[0].LookAt(Input::GetAxisVec2(MousePos), 45);
			boxes[3].position = Input::GetAxisVec2(MousePos);
		}
		if (Input::keysDown[SDLK_r])
		{
			boxes[2].SetFastSize(vec2(50, 50));
		}
		if (Input::keysDown[SDLK_y])
		{
			boxes[2].SetFastSize(vec2(20, 20));
			for (int i = 0; i < 6; i++)
			{
				boxes[i].SetColor(new Color(true));
			}
		}
	}
}
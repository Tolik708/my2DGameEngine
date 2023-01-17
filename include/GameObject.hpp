#pragma once

namespace myMain
{
	struct GameObject {
		//transform
		vec2 position = vec2(0, 0);
		vec2 size = vec2(1, 1);
		vec2 meshSize = vec2(1, 1);
		float rotation = 0;
		
		//mesh/draw
		bool active = true;
		int layer = 0;
		mesh* myMesh;
		
		void draw()
		{
			if (myMesh->myTexture != nullptr)
			{
				SDL_Rect Pos;
				Pos.x = position.x - (size.x/2);
				Pos.y = position.y - (size.y/2);
				Pos.w = size.x;
				Pos.h = size.y;
				SDL_RenderCopyEx(rend, myMesh->myTexture, nullptr, &Pos, rotation, NULL, SDL_FLIP_NONE);
			}
		}
		void SetColor(Color newColor)
		{
			myMesh->myColor = newColor;
			myMesh->changeColor();
		}
		void SetColor(uint8_t toR, uint8_t toG, uint8_t toB, uint8_t toA)
		{
			myMesh->myColor = {toR, toG, toB, toA};
			myMesh->changeColor();
		}
		
		////////////////size//////////////
		void SetFastSize(vec2 newSize)
		{
			size = newSize;
		}
		void SetSize(vec2 newSize)
		{
			if (newSize.x != size.x && newSize.y != size.y)
			{
				switch (myMesh->meshIs)
				{
					case line:
					case polygon:
					{
						for (int i = 0; i < myMesh->vertexAmm; i++)
							myMesh->vertesies[i] = {myMesh->vertesies[i].x / size.x * newSize.x, myMesh->vertesies[i].y / size.y * newSize.y};
						break;
					}
					case circle:
					{
						myMesh->radius = vec2(newSize.x/2, newSize.y/2);
						break;
					}
					case box:
					{
						myMesh->bSize = newSize;
						break;
					}
				}
				meshSize = size = newSize;
				myMesh->genMesh();
			}
		}
		
		////////////////rotation///////////
		void SetRotation(float degree)
		{
			rotate((360 - rotation) + degree);
		}
		void rotate(float degree)
		{
			float rotInRad = degree*toRadians;
			rotation += rotInRad/toRadians;
			if (rotation > 360)
				rotation -= 360;
		}
		void LookAt(vec2 targ, float addDegree)
		{
			SetRotation((atan2(targ.y - position.y, targ.x - position.x)/toRadians) + addDegree);
		}
		void LookAt(vec2 targ)
		{
			SetRotation(atan2(targ.y - position.y, targ.x - position.x)/toRadians);
		}
	};
	
	bool layerSortGameObject(GameObject obj1, GameObject obj2)
	{
		return (obj1.layer < obj2.layer);
	}
}
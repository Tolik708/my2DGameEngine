#pragma once

namespace myMain
{
	enum meshType
	{
		line=0, circle=1, elipse=1, polygon=2, box=3
	};
	
	enum lineType
	{
		basic=0, bresenham=0, GuptaSproll=1
	};
	
	SDL_Surface* resizeImage(int nW, int nH, SDL_Surface* surfaceFrom)
	{
		//init 4 new arrays for 4 chanels
		uint8_t** newR = new uint8_t*[nH];
		uint8_t** newG = new uint8_t*[nH];
		uint8_t** newB = new uint8_t*[nH];
		uint8_t** newA = new uint8_t*[nH];
		for (int i = 0; i < nH; i++)
		{
			newR[i] = new uint8_t[nW];
			newG[i] = new uint8_t[nW];
			newB[i] = new uint8_t[nW];
			newA[i] = new uint8_t[nW];
		}
		
		uint8_t* pixelsFrom = (uint8_t*) surfaceFrom->pixels;
		
		//calculate ratio
		float ratioX = static_cast<float>(surfaceFrom->w-1)/static_cast<float>(nW-1);
		float ratioY = static_cast<float>(surfaceFrom->h-1)/static_cast<float>(nH-1);
		
		//calculate new colors
		for (int y = 0; y < nH; y++)
		{
			for (int x = 0; x < nW; x++)
			{
				float tempX = x*ratioX;
				float tempY = y*ratioY;
				
				float weightX = tempX-floor(tempX);
				float weightY = tempY-floor(tempY);
				
				int fx = static_cast<int>(floor(tempX)*surfaceFrom->pitch); //floor x
				int fy = static_cast<int>(floor(tempY)*surfaceFrom->format->BytesPerPixel); //floor y
				int cx = static_cast<int>(ceil(tempX)*surfaceFrom->pitch); //ceil x
				int cy = static_cast<int>(ceil(tempY)*surfaceFrom->format->BytesPerPixel); //ceil y
				
				uint8_t x1y1 = 0;
				uint8_t x2y1 = 0;
				uint8_t x1y2 = 0;
				uint8_t x2y2 = 0;
				
				//red
				x1y1 = pixelsFrom[fx + fy+2];
				x2y1 = pixelsFrom[cx + fy+2];
				x1y2 = pixelsFrom[fx + cy+2];
				x2y2 = pixelsFrom[cx + cy+2];
				newR[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
				
				//green
				x1y1 = pixelsFrom[fx + fy+1];
				x2y1 = pixelsFrom[cx + fy+1];
				x1y2 = pixelsFrom[fx + cy+1];
				x2y2 = pixelsFrom[cx + cy+1];
				newG[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
				
				//blue
				x1y1 = pixelsFrom[fx + fy];
				x2y1 = pixelsFrom[cx + fy];
				x1y2 = pixelsFrom[fx + cy];
				x2y2 = pixelsFrom[cx + cy];
				newB[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
				
				//alpha
				x1y1 = pixelsFrom[fx + fy+3];
				x2y1 = pixelsFrom[cx + fy+3];
				x1y2 = pixelsFrom[fx + cy+3];
				x2y2 = pixelsFrom[cx + cy+3];
				newA[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
			}
		}
		
		//creating new surface
		SDL_Surface* newSurface = SDL_CreateRGBSurface(0, nW, nH, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		SDL_LockSurface(newSurface);
		uint8_t* pixels = (uint8_t*)newSurface->pixels;
		
		//filling newSurface
		for (int y = 0; y < nH; y++)
		{
			for (int x = 0; x < nW; x++)
			{
				int tx = x*newSurface->pitch; //x
				int ty = y*newSurface->format->BytesPerPixel; //y
				
				pixels[tx+ty+2] = newR[y][x]; //r
				pixels[tx+ty+1] = newG[y][x]; //g
				pixels[tx+ty] = newB[y][x]; //b
				pixels[tx+ty+3] = newA[y][x]; //a
			}
		}
		
		SDL_UnlockSurface(newSurface);
		return newSurface;
	}
	SDL_Point getTextureSize(SDL_Texture *texture) 
	{
		SDL_Point size;
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		return size;
	}
	
	//structs
	struct Color
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;
		Color (uint8_t myR, uint8_t myG, uint8_t myB, uint8_t myA)
		{
			r = myR;
			g = myG;
			b = myB;
			a = myA;
		}
		Color(bool random)
		{
			if (random)
			{
				r = randomRange(10, 254);
				g = randomRange(10, 254);
				b = randomRange(10, 254);
				a = 255;
			}
		}
	};
	
	Color colorLerp(Color num1, Color num2, float t)
	{
		Color result = {static_cast<uint8_t>(myRound(lerp(num1.r, num2.r, t))),
						static_cast<uint8_t>(myRound(lerp(num1.g, num2.g, t))),
						static_cast<uint8_t>(myRound(lerp(num1.b, num2.b, t))),
						static_cast<uint8_t>(myRound(lerp(num1.a, num2.a, t)))};
		return result;
	}

	struct mesh
	{
		//general
		Color myColor = {255, 0, 0, 255};
		const char *pathToTexture;
		SDL_Texture* myTexture;
		std::vector<vec2> vertesies;
		int vertexAmm;
		int height;
		int width;
		int currLowestX;
		int currLowestY;
		meshType meshIs;
		
		SDL_Surface* tempSurface;
		uint8_t* pixels;
		
		SDL_Surface* textureToLoad;
		uint8_t* pixelsToLoad;
		
		uint8_t* pixelsOfSprite;
		
		//line
		std::vector<vec2> lConects;
		lineType lineIs = basic;
		int sicknes = 1;
		
		//circle/elipse
		vec2 radius = vec2(1, 1);
		
		//polygon
		bool selfIntersecting = false;

		//boxes
		vec2 bSize = {1, 1};
		
		//fast triangles
		std::vector<std::vector<int>> tConects;
		
		void genMesh()
		{
			switch(meshIs)
			{
				case line:
				{
					//creating connections for line
					for (int i = 0; i < static_cast<int>(vertesies.size()) - 1; i++)
					{
						lConects.push_back({static_cast<float>(i), static_cast<float>(i+1)});
					}
					
					//creating texture
					createBounds();
					
					/* if statment there for easier calculations
					here we are appending width and height of texture for sicknes */
					if (sicknes%2 == 0)
					{
						width += sicknes;
						height += sicknes;
					}
					else
					{
						width += sicknes-1;
						height += sicknes-1;
					}
					
					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();
					
					if (lineIs == basic)
					{
						for (int i = 0; i < vertexAmm-1; i++)
						{
							//allign all vertesies to 0, 0, then add offset of sicknes
							int x0 = vertesies[i].x - currLowestX + static_cast<int>(ceil(sicknes/2));
							int y0 = vertesies[i].y - currLowestY + static_cast<int>(ceil(sicknes/2));
							int x1 = vertesies[i+1].x - currLowestX + static_cast<int>(ceil(sicknes/2));
							int y1 = vertesies[i+1].y - currLowestY + static_cast<int>(ceil(sicknes/2));
							
							if (abs(x1-x0) > abs(y1-y0))
							{
								if (x0 > x1)
									bresenhamLow(x1, y1, x0, y0, sicknes);
								else
									bresenhamLow(x0, y0, x1, y1, sicknes);
							}
							else
							{
								if (y0 > y1)
									bresenhamHeigh(x1, y1, x0, y0, sicknes);
								else
									bresenhamHeigh(x0, y0, x1, y1, sicknes);
							}
						}
					}
					createTexture();
					break;
				}
				case circle:
				{
					if (radius.x == radius.y)
						height = width = radius.x*2;
					else
					{
						width = radius.x*2;
						height = radius.y*2;
					}
					
					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();

					if (radius.x == radius.y)
					{
						for (int i = 0; i < radius.y; i++)
						{
							for (int u = 0; u < radius.x; u++)
							{
								if ((pow(i - radius.x, 2) + pow(u - radius.x, 2) - radius.x*radius.x) <= 0)
								{
									int j = u*tempSurface->pitch; //x
									int k = i*tempSurface->format->BytesPerPixel; //y
									
									int h = (radius.x-1-u+radius.x)*tempSurface->pitch; //x2
									int g = (radius.x-1-i+radius.x)*tempSurface->format->BytesPerPixel; //y2
									
									if (pathToTexture == nullptr)
									{
										//top left
										pixelsOfSprite[j + k] = pixels[j + k] = myColor.r; //r
										pixelsOfSprite[j + k+1] = pixels[j + k+1] = myColor.g; //g
										pixelsOfSprite[j + k+2] = pixels[j + k+2] = myColor.b; //b
										pixelsOfSprite[j + k+3] = pixels[j + k+3] = myColor.a; //a
										
										//top right
										pixelsOfSprite[h + k] = pixels[h + k] = myColor.r; //r
										pixelsOfSprite[h + k+1] = pixels[h + k+1] = myColor.g; //g
										pixelsOfSprite[h + k+2] = pixels[h + k+2] = myColor.b; //b
										pixelsOfSprite[h + k+3] = pixels[h + k+3] = myColor.a; //a 
										
										//low left
										pixelsOfSprite[j + g] = pixels[j + g] = myColor.r; //r
										pixelsOfSprite[j + g+1] = pixels[j + g+1] = myColor.g; //g
										pixelsOfSprite[j + g+2] = pixels[j + g+2] = myColor.b; //b
										pixelsOfSprite[j + g+3] = pixels[j + g+3] = myColor.a; //a
										
										//low right
										pixelsOfSprite[h + g] = pixels[h + g] = myColor.r; //r
										pixelsOfSprite[h + g+1] = pixels[h + g+1] = myColor.g; //g
										pixelsOfSprite[h + g+2] = pixels[h + g+2] = myColor.b; //b
										pixelsOfSprite[h + g+3] = pixels[h + g+3] = myColor.a; //a
									}
									else
									{
										int tj = u*textureToLoad->pitch; //tx
										int tk = i*textureToLoad->format->BytesPerPixel; //ty
										
										int th = (radius.x-1-u+radius.x)*textureToLoad->pitch; //tx2
										int tg = (radius.x-1-i+radius.x)*textureToLoad->format->BytesPerPixel; //ty2
										
										//top left
										pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[tj + tk+2]; //r
										pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[tj + tk+1]; //g
										pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[tj + tk]; //b
										pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[tj + tk+3]; //a
										
										//top right
										pixelsOfSprite[j + k] = pixels[h + k] = pixelsToLoad[th + tk+2]; //r
										pixelsOfSprite[j + k+1] = pixels[h + k+1] = pixelsToLoad[th + tk+1]; //g
										pixelsOfSprite[j + k+2] = pixels[h + k+2] = pixelsToLoad[th + tk]; //b
										pixelsOfSprite[j + k+3] = pixels[h + k+3] = pixelsToLoad[th + tk+3]; //a 
										
										//low left
										pixelsOfSprite[j + k] = pixels[j + g] = pixelsToLoad[tj + tg+2]; //r
										pixelsOfSprite[j + k+1] = pixels[j + g+1] = pixelsToLoad[tj + tg+1]; //g
										pixelsOfSprite[j + k+2] = pixels[j + g+2] = pixelsToLoad[tj + tg]; //b
										pixelsOfSprite[j + k+3] = pixels[j + g+3] = pixelsToLoad[tj + tg+3]; //a
										
										//low right
										pixelsOfSprite[j + k] = pixels[h + g] = pixelsToLoad[th + tg+2];//r
										pixelsOfSprite[j + k+1] = pixels[h + g+1] = pixelsToLoad[th + tg+1]; //g
										pixelsOfSprite[j + k+2] = pixels[h + g+2] = pixelsToLoad[th + tg]; //b
										pixelsOfSprite[j + k+3] = pixels[h + g+3] = pixelsToLoad[th + tg+3]; //a
									}
								}
							}
						}
					}
					else
					{
						for (int i = 0; i < radius.y; i++)
						{
							for (int u = 0; u < radius.x; u++)
							{
								if ((pow(u-radius.x, 2)/pow(radius.x, 2)) + (pow(i-radius.y, 2)/pow(radius.y, 2)) <= 1)
								{
									int j = u*tempSurface->pitch; //x
									int k = i*tempSurface->format->BytesPerPixel; //y
									
									int h = (radius.x-1-u+radius.x)*tempSurface->pitch; //x2
									int g = (radius.y-1-i+radius.y)*tempSurface->format->BytesPerPixel; //y2
									
									if (pathToTexture == nullptr)
									{
										//top left
										pixelsOfSprite[j + k] = pixels[j + k] = myColor.r; //r
										pixelsOfSprite[j + k+1] = pixels[j + k+1] = myColor.g; //g
										pixelsOfSprite[j + k+2] = pixels[j + k+2] = myColor.b; //b
										pixelsOfSprite[j + k+3] = pixels[j + k+3] = myColor.a; //a
										
										//top right
										pixelsOfSprite[h + k] = pixels[h + k] = myColor.r; //r
										pixelsOfSprite[h + k+1] = pixels[h + k+1] = myColor.g; //g
										pixelsOfSprite[h + k+2] = pixels[h + k+2] = myColor.b; //b
										pixelsOfSprite[h + k+3] = pixels[h + k+3] = myColor.a; //a 
										
										//low left
										pixelsOfSprite[j + g] = pixels[j + g] = myColor.r; //r
										pixelsOfSprite[j + g+1] = pixels[j + g+1] = myColor.g; //g
										pixelsOfSprite[j + g+2] = pixels[j + g+2] = myColor.b; //b
										pixelsOfSprite[j + g+3] = pixels[j + g+3] = myColor.a; //a
										
										//low right
										pixelsOfSprite[h + g] = pixels[h + g] = myColor.r; //r
										pixelsOfSprite[h + g+1] = pixels[h + g+1] = myColor.g; //g
										pixelsOfSprite[h + g+2] = pixels[h + g+2] = myColor.b; //b
										pixelsOfSprite[h + g+3] = pixels[h + g+3] = myColor.a; //a
									}
									else
									{
										int tj = u*textureToLoad->pitch; //tx
										int tk = i*textureToLoad->format->BytesPerPixel; //ty
										
										int th = (radius.x-1-u+radius.x)*textureToLoad->pitch; //tx2
										int tg = (radius.y-1-i+radius.y)*textureToLoad->format->BytesPerPixel; //ty2
										
										//top left
										pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[tj + tk+2]; //r
										pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[tj + tk+1]; //g
										pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[tj + tk]; //b
										pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[tj + tk+3]; //a
										
										//top right
										pixelsOfSprite[h + k] = pixels[h + k] = pixelsToLoad[th + tk+2]; //r
										pixelsOfSprite[h + k+1] = pixels[h + k+1] = pixelsToLoad[th + tk+1]; //g
										pixelsOfSprite[h + k+2] = pixels[h + k+2] = pixelsToLoad[th + tk]; //b
										pixelsOfSprite[h + k+3] = pixels[h + k+3] = pixelsToLoad[th + tk+3]; //a 
										
										//low left
										pixelsOfSprite[j + g] = pixels[j + g] = pixelsToLoad[tj + tg+2]; //r
										pixelsOfSprite[j + g+1] = pixels[j + g+1] = pixelsToLoad[tj + tg+1]; //g
										pixelsOfSprite[j + g+2] = pixels[j + g+2] = pixelsToLoad[tj + tg]; //b
										pixelsOfSprite[j + g+3] = pixels[j + g+3] = pixelsToLoad[tj + tg+3]; //a
										
										//low right
										pixelsOfSprite[h + g] = pixels[h + g] = pixelsToLoad[th + tg+2];//r
										pixelsOfSprite[h + g+1] = pixels[h + g+1] = pixelsToLoad[th + tg+1]; //g
										pixelsOfSprite[h + g+2] = pixels[h + g+2] = pixelsToLoad[th + tg]; //b
										pixelsOfSprite[h + g+3] = pixels[h + g+3] = pixelsToLoad[th + tg+3]; //a
									}
								}
							}
						}
					}
					createTexture();
					break;
				}
				case polygon:
				{
					//TODO: triangulazing polygons
					createBounds();
					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();
					
					if (selfIntersecting)
					{
						//line points variables
						int x1, y1;
						int x2, y2;
						int x3, y3;
						int x4, y4;
						int detA;
						float u, v;
						//if object have no texture
						/* loop through every pixel and check it's intersection with
						every edge of polygon */
						for (int x = 0; x < width; x++)
						{
							for (int y = 0; y < height; y++)
							{
								int count = 0;
								for (int i = 0; i < vertexAmm; i++)
								{
									x1 = x + currLowestX;
									y1 = y + currLowestY;
									x2 = width + currLowestX;
									y2 = y1;
									x3 = vertesies[i].x;
									y3 = vertesies[i].y;
									x4 = vertesies[(i+1)%vertexAmm].x;
									y4 = vertesies[(i+1)%vertexAmm].y;
									
									detA = (x1-x2)*(y4-y3) - (x4-x3)*(y1-y2);
									
									if (detA != 0)
									{
										u = static_cast<float>((y4-y3)*(x1-x3) + (x3-x4)*(y1-y3)) / static_cast<float>(detA);
										v = static_cast<float>((y2-y1)*(x1-x3) + (x1-x2)*(y1-y3)) / static_cast<float>(detA);
										
										if((u > 0.0) && (u < 1.0) && (v > 0.0) && (v < 1.0))
										{
											count++;
										}
									}
								}
								//if count of intersections are odd, pixel is inside polygon and we draw it
								if (count % 2)
								{
									int j = x*tempSurface->pitch; //x
									int k = y*tempSurface->format->BytesPerPixel; //y
									
									if (pathToTexture == nullptr)
									{
										pixelsOfSprite[j + k] = pixels[j + k] = myColor.r; //r
										pixelsOfSprite[j + k+1] = pixels[j + k+1] = myColor.g; //g
										pixelsOfSprite[j + k+2] = pixels[j + k+2] = myColor.b; //b
										pixelsOfSprite[j + k+3] = pixels[j + k+3] = myColor.a; //a
									}
									else
									{
										int tj = clampInt(x, 0, textureToLoad->w-1)*textureToLoad->pitch; //tx
										int tk = clampInt(y, 0, textureToLoad->h-1)*textureToLoad->format->BytesPerPixel; //ty
										
										pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[tj + tk+2]; //r
										pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[tj + tk+1]; //g
										pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[tj + tk]; //b
										pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[tj + tk+3]; //a
									}
								}
							}
						}
					}
					else
					{
						vec2 Points[3] = {vertesies[0], vertesies[1], vertesies[2]};
						drawFastTriangle(Points);
					}
					
					createTexture();
					break;
				}
				case box:
				{
					width = bSize.x;
					height = bSize.y;
					
					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();
					
					for (int x = 0; x < bSize.x; x++)
					{
						for (int y = 0; y < bSize.y; y++)
						{
							int j = x*tempSurface->pitch; //x
							int k = y*tempSurface->format->BytesPerPixel; //y
							
							if (pathToTexture == nullptr)
							{
								pixelsOfSprite[j + k] = pixels[j + k] = myColor.r; //r
								pixelsOfSprite[j + k+1] = pixels[j + k+1] = myColor.g; //g
								pixelsOfSprite[j + k+2] = pixels[j + k+2] = myColor.b; //b
								pixelsOfSprite[j + k+3] = pixels[j + k+3] = myColor.a; //a
							}
							else
							{
								int tj = clampInt(x, 0, textureToLoad->w-1)*textureToLoad->pitch; //tx
								int tk = clampInt(y, 0, textureToLoad->h-1)*textureToLoad->format->BytesPerPixel; //ty
								
								pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[tj + tk+2]; //r
								pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[tj + tk+1]; //g
								pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[tj + tk]; //b
								pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[tj + tk+3]; //a
							}
						}
					}
					createTexture();
					break;
				}
			}
		}
		
		void drawFastTriangle(vec2 points[3])
		{
			//creating edges for triangle
			std::vector<vec2> edgePoints;
			int allCount = 0;
			for (int u = 0; u < 3; u++)
			{
				int x1 = points[u].x - currLowestX;
				int y1 = points[u].y - currLowestY;
				int x2 = points[(u+1)%3].x - currLowestX;
				int y2 = points[(u+1)%3].y - currLowestY;
				if (y1 != y2)
				{
					if (y1 > y2)
					{
						std::swap(x1, x2);
						std::swap(y1, y2);
					}
					
					int dx = x2 - x1;
					int dy = y2 - y1;
					
					for (int ty = y1; ty < y2; ty++)
					{
						int tx = x1 + dx * (ty - y1) / dy;
						edgePoints.push_back(vec2(static_cast<float>(tx), static_cast<float>(ty)));
						allCount++;
					}
				}
			}
			//sorting coords that we get for easy filling
			vec2 *edgePoints2 = new vec2[allCount];
			std::sort(edgePoints.begin(), edgePoints.end(), YSortVec2);
			for (int i = 0; i < static_cast<int>(edgePoints.size()); i++)
			{
				edgePoints2[i] = edgePoints[i];
				std::cout << edgePoints[i].x << " " << edgePoints[i].y << std::endl;
			}
			std::cout << "real" << std::endl;
			//fill triangle
			for (int u = 0; u < allCount-1; u += 2)
			{
				std::cout << static_cast<float>(edgePoints2[u].x) << " " << static_cast<float>(edgePoints2[u+1].x) << " " << static_cast<float>(edgePoints2[u].y) << std::endl;
				
				int x1 = edgePoints2[u].x;
				int x2 = edgePoints2[u+1].x;
				int y = edgePoints2[u].y;
				
				//for knowing that we are going from smallest to heighest
				if (x1 > x2)
					std::swap(x1, x2);
				
				std::cout << x1 << " " << x2 << " " << y << std::endl;
				
				for (int x = x1; x < x2; x++)
				{
					//drawing pixels
					int j = x * tempSurface->pitch; //x
					int k = y * tempSurface->format->BytesPerPixel; //y
					
					if (pathToTexture == nullptr)
					{
						pixelsOfSprite[j + k] = pixels[j + k] = myColor.r; //r
						pixelsOfSprite[j + k+1] = pixels[j + k+1] = myColor.g; //g
						pixelsOfSprite[j + k+2] = pixels[j + k+2] = myColor.b; //b
						pixelsOfSprite[j + k+3] = pixels[j + k+3] = myColor.a; //a
					}
					else
					{
						int tj = x * textureToLoad->pitch; //tx
						int tk = y * textureToLoad->format->BytesPerPixel; //ty
						
						pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[tj + tk+2];
						pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[tj + tk+1];
						pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[tj + tk];
						pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[tj + tk+3];
					}
				}
				std::cout << "next" << std::endl;
			}
			std::cout << "nuxt" << std::endl;
		}
		
		//line drawing algorithm brethenham's
		void bresenhamLow(int x0, int y0, int x1, int y1, int sickness)
		{
			int dx = x1-x0;
			int dy = y1-y0;
			int yi = 1;
			
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			
			int D = (2*dy)-dx;
			int y = y0;
			
			for (int x = x0; x < x1; x++)
			{
				//draw pixels
				int additional = 0;
				for (int i = 0; i < sickness; i++)
				{
					int sx = x*tempSurface->pitch; //x
					int sy;  //y
					if (i%2 == 0)
						sy = (y-additional)*tempSurface->format->BytesPerPixel;
					else
						sy = (y+additional)*tempSurface->format->BytesPerPixel;
					
					if (pathToTexture == nullptr)
					{
						pixelsOfSprite[sx + sy] = pixels[sx + sy] = myColor.r; //r
						pixelsOfSprite[sx + sy+1] = pixels[sx + sy+1] = myColor.g; //g
						pixelsOfSprite[sx + sy+2] = pixels[sx + sy+2] = myColor.b; //b
						pixelsOfSprite[sx + sy+3] = pixels[sx + sy+3] = myColor.a; //a
					}
					else
					{
						int tsx = x*textureToLoad->pitch; //tx
						int tsy; //ty
						
						if (i%2 == 0)
							tsy = (y-additional)*tempSurface->format->BytesPerPixel;
						else
							tsy = (y+additional)*tempSurface->format->BytesPerPixel;
						
						pixelsOfSprite[sx + sy] = pixels[sx + sy] = pixelsToLoad[tsx + tsy+2];
						pixelsOfSprite[sx + sy+1] = pixels[sx + sy+1] = pixelsToLoad[tsx + tsy+1];
						pixelsOfSprite[sx + sy+2] = pixels[sx + sy+2] = pixelsToLoad[tsx + tsy];
						pixelsOfSprite[sx + sy+3] = pixels[sx + sy+3] = pixelsToLoad[tsx + tsy+3];
					}
					
					if (i%2 == 0)
						additional++;
				}
				
				if (D > 0)
				{
					y = y+yi;
					D = D + (2*(dy-dx));
				}
				else
					D = D + 2*dy;
			}
		}

		void bresenhamHeigh(int x0, int y0, int x1, int y1, int sickness)
		{
			int dx = x1-x0;
			int dy = y1-y0;
			int xi = 1;
			
			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			
			int D = (2*dx)-dy;
			int x = x0;
			
			for (int y = y0; y < y1; y++)
			{
				//drawing pixels
				int additional = 0;
				for (int i = 0; i < sickness; i++)
				{
					int sx; //x
					int sy = y*tempSurface->format->BytesPerPixel; //y
					if (i%2 == 0)
						sx = (x-additional)*tempSurface->pitch;
					else
						sx = (x+additional)*tempSurface->pitch;
					
					if (pathToTexture == nullptr)
					{
						pixelsOfSprite[sx + sy] = pixels[sx + sy] = myColor.r; //r
						pixelsOfSprite[sx + sy+1] = pixels[sx + sy+1] = myColor.g; //g
						pixelsOfSprite[sx + sy+2] = pixels[sx + sy+2] = myColor.b; //b
						pixelsOfSprite[sx + sy+3] = pixels[sx + sy+3] = myColor.a; //a
					}
					else
					{
						int tsx; //tx
						int tsy = y*textureToLoad->format->BytesPerPixel; //ty
						
						if (i%2 == 0)
							tsx = (x-additional)*tempSurface->pitch;
						else
							tsx = (x+additional)*tempSurface->pitch;
						
						pixelsOfSprite[sx + sy] = pixels[sx + sy] = pixelsToLoad[tsx + tsy+2];
						pixelsOfSprite[sx + sy+1] = pixels[sx + sy+1] = pixelsToLoad[tsx + tsy+1];
						pixelsOfSprite[sx + sy+2] = pixels[sx + sy+2] = pixelsToLoad[tsx + tsy];
						pixelsOfSprite[sx + sy+3] = pixels[sx + sy+3] = pixelsToLoad[tsx + tsy+3];
					}
					if (i%2 == 0)
						additional++;
				}
				
				if (D > 0)
				{
					x = x+xi;
					D = D + (2*(dx-dy));
				}
				else
					D = D + 2*dx;
			}
		}

		//functions
		void changeColor()
		{
			if (pathToTexture == nullptr)
			{
				initTempSurface();
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						int j = x*tempSurface->pitch; //x
						int k = y*tempSurface->format->BytesPerPixel; //y
						
						pixels[j + k] = myColor.r;
						pixels[j + k+1] = myColor.g;
						pixels[j + k+2] = myColor.b;
						pixels[j + k+3] = pixelsOfSprite[j + k+3];
					}
				}
				createTexture();
			}
		}
		
		void createBounds()
		{
			int lowestX = 9999;
			int lowestY = 9999;
			int hiegestX = -9999;
			int hiegestY = -9999;
			for (int i = 0; i < vertexAmm; i++)
			{
				if (lowestX > vertesies[i].x)
					lowestX = vertesies[i].x;
				if (lowestY > vertesies[i].y)
					lowestY = vertesies[i].y;
				if (hiegestX < vertesies[i].x)
					hiegestX = vertesies[i].x;
				if (hiegestY < vertesies[i].y)
					hiegestY = vertesies[i].y;
			}
			currLowestX = lowestX;
			currLowestY = lowestY;
			lowestX--;
			lowestY--;
			hiegestX++;
			hiegestY++;
			height = hiegestY - lowestY;
			width = hiegestX - lowestX;
		}
		
		void initTempSurface()
		{
			tempSurface = SDL_CreateRGBSurface(0, height, width, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
			SDL_LockSurface(tempSurface);
			pixels = (uint8_t*)tempSurface->pixels;
			
			//init pixels of sprite array
			int sizeOfArray = height*tempSurface->pitch;
			pixelsOfSprite = new uint8_t[sizeOfArray];
			for (int i = 0; i < sizeOfArray; i++)
			{
				pixelsOfSprite[i] = 0;
			}
		}
		
		void initTextureSurface()
		{
			SDL_Surface* tempImage = SDL_LoadBMP(pathToTexture);
			
			if (tempImage->w != width || tempImage->h != height)
				textureToLoad = resizeImage(width, height, tempImage);
			else
				textureToLoad = tempImage;
			
			SDL_FreeSurface(tempImage);
			
			pixelsToLoad = (uint8_t*) textureToLoad->pixels;
			
			if (textureToLoad == nullptr)
				SDL_Log("something wrong with texture loading %s\n", SDL_GetError());
		}
		
		void createTexture()
		{
			if (textureToLoad != nullptr)
				SDL_FreeSurface(textureToLoad);

			std::cout << "lux" << std::endl;
			SDL_UnlockSurface(tempSurface);
			myTexture = SDL_CreateTextureFromSurface(rend, tempSurface);
			SDL_FreeSurface(tempSurface);
		}
		
		
	};
	
	//TODOs
	//TODO: check if polygon is self intersecting
	//TODO: change line drawing function inside drawing fast triangles
}
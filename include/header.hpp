#pragma once
namespace Debug
{
	//TODO: Logs
}

//TODO: separate files
namespace myMain
{
	float toRadians = 0.0174532925;
	
	enum meshType
	{
		line, circle, polygon, box, fastTriangle
	};
	
	//smooth functions
	float lerp(float a, float b, float t)
	{
		return a * (1.0 - t) + (b * t);
	}
	float smoothstep(float a, float b, float t)
	{
		float t1 = t*t;
		float t2 = 1 - (1-t)*(1-t);
		return lerp(a, b, lerp(t1, t2, t));
	}

	//round
	int myRound(float n)
	{
		return floor(n+0.5);
	}
	float myRound(float n, int u)
	{
		return floor((n+0.5)*(10*u))/(10*u);
	}
	float myRound(float n, int u, bool i)
	{
		if (i == false)
			return floor(n*(10*u))/(10*u);
		return floor((n+0.5)*(10*u))/(10*u);
	}
	//vector2
	struct vec2 {
		float x = 0;
		float y = 0;
		vec2(float x1, float y1)
		{
			x = x1;
			y = y1;
		}
		
		vec2 operator + (vec2 const &secondVec) const
		{
			return vec2(x + secondVec.x, y + secondVec.y);
		}
		vec2 operator - (vec2 const &secondVec) const
		{
			return vec2(x - secondVec.x, y - secondVec.y);
		}
		vec2 operator / (vec2 const &secondVec) const
		{
			return vec2(x / secondVec.x, y / secondVec.y);
		}
		vec2 operator * (vec2 const &secondVec) const
		{
			return vec2(x * secondVec.x, y * secondVec.y);
		}
	};

	vec2 vec2Lerp(vec2 num1, vec2 num2, float t)
	{
		vec2 result = {lerp(num1.x, num2.x, t), lerp(num1.y, num2.y, t)};
		return result;
	}

	//math
	int randomRange(int min, int max) //range : [min, max]
	{
	   return min + rand() % (( max + 1 ) - min);
	}
	template<typename T>
	T clampInt(T num, T min, T max)
	{
		if (num > max)
			return max;
		else if (num < min)
			return min;
		
		return num;
	}
	int** bilinearResize2DArray(int sX, int sY, int** arrayFrom, int sizeOfArrayFromX, int sizeOfArrayFromY)
	{
		float ratioX = static_cast<float>(sizeOfArrayFromY-1)/static_cast<float>(sX-1);
		float ratioY = static_cast<float>(sizeOfArrayFromX-1)/static_cast<float>(sY-1);
		
		int** arrayTo = new int*[sY];
		for (int h = 0; h < sY; h++)
			arrayTo[h] = new int[sX];
		
		for (int y = 0; y < sY; y++)
		{
			for (int x = 0; x < sX; x++)
			{
				float tempX = x*ratioX;
				float tempY = y*ratioY;
				
				float weightX = tempX-floor(tempX);
				float weightY = tempY-floor(tempY);

				int x1y1 = arrayFrom[static_cast<int>(floor(tempY))][static_cast<int>(floor(tempX))];
				int x2y1 = arrayFrom[static_cast<int>(floor(tempY))][static_cast<int>(ceil(tempX))];
				int x1y2 = arrayFrom[static_cast<int>(ceil(tempY))][static_cast<int>(floor(tempX))];
				int x2y2 = arrayFrom[static_cast<int>(ceil(tempY))][static_cast<int>(ceil(tempX))];
				
				arrayTo[y][x] = myRound(lerp(lerp(x1y1, x2y1, weightX), lerp(x1y2, x2y2, weightX), weightY));
			}
		}
		return arrayTo;
	}
	bool YSortVec2 (vec2 num1, vec2 num2)
	{
		return (num1.y < num2.y);
	}
	
	SDL_Surface* resizeImage(int nH, int nW, SDL_Surface* surfaceFrom)
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
		SDL_Surface* newSurface = SDL_CreateRGBSurface(0, nH, nW, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
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
		meshType meshIs;
		
		SDL_Surface* tempSurface;
		uint8_t* pixels;
		
		SDL_Surface* textureToLoad;
		uint8_t* pixelsToLoad;
		
		uint8_t* pixelsOfSprite;
		
		//line
		std::vector<vec2> lConects;
		
		//circle
		int radius = 1;

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
					//TODO: make own line algorithm
					//creating connections for line
					for (int i = 0; i < static_cast<int>(vertesies.size()) - 1; i++)
					{
						lConects.push_back({static_cast<float>(i), static_cast<float>(i+1)});
					}
					break;
				}
				case circle:
				{
					height = width = radius*2;
					
					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();
					
					pixelsOfSprite = new uint8_t[height*tempSurface->pitch];
					
					for (int i = 0; i < radius; i++)
					{
						for (int u = 0; u < radius; u++)
						{
							if ((pow(i - radius, 2) + pow(u - radius, 2) - radius*radius) <= 0)
							{
								
								int j = u*tempSurface->pitch; //x
								int k = i*tempSurface->format->BytesPerPixel; //y
								
								int h = (radius-1-u+radius)*tempSurface->pitch; //x2
								int g = (radius-1-i+radius)*tempSurface->format->BytesPerPixel; //y2
								
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
									int tj = clampInt(u, 0, textureToLoad->w-1)*textureToLoad->pitch; //tx
									int tk = clampInt(i, 0, textureToLoad->h-1)*textureToLoad->format->BytesPerPixel; //ty
									
									int th = (radius-1-clampInt(u, 0, textureToLoad->w-1)+radius)*textureToLoad->pitch; //tx2
									int tg = (radius-1-clampInt(i, 0, textureToLoad->h-1)+radius)*textureToLoad->format->BytesPerPixel; //ty2
									
									//top left
									pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[th + tk+2]; //r
									pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[th + tk+1]; //g
									pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[th + tk]; //b
									pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[th + tk+3]; //a
									
									//top right
									pixelsOfSprite[j + k] = pixels[h + k] = pixelsToLoad[th + tk+2]; //r
									pixelsOfSprite[j + k+1] = pixels[h + k+1] = pixelsToLoad[th + tk+1]; //g
									pixelsOfSprite[j + k+2] = pixels[h + k+2] = pixelsToLoad[th + tk]; //b
									pixelsOfSprite[j + k+3] = pixels[h + k+3] = pixelsToLoad[th + tk+3]; //a 
									
									//low left
									pixelsOfSprite[j + k] = pixels[j + g] = pixelsToLoad[tj + tg+2]; //r
									pixelsOfSprite[j + k+1] = pixels[j + g+1] = pixelsToLoad[tj + tg+1]; //g
									pixelsOfSprite[j + k+2] = pixels[j + g+2] = pixelsToLoad[tj + tg]; //b
									pixelsOfSprite[j + k+3] = pixels[j + g+3] = pixelsToLoad[th + tk+3]; //a
									
									//low right
									pixelsOfSprite[j + k] = pixels[h + g] = pixelsToLoad[th + tg+2];//r
									pixelsOfSprite[j + k+1] = pixels[h + g+1] = pixelsToLoad[th + tg+1]; //g
									pixelsOfSprite[j + k+2] = pixels[h + g+2] = pixelsToLoad[th + tg]; //b
									pixelsOfSprite[j + k+3] = pixels[h + g+3] = pixelsToLoad[th + tk+3]; //a
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
					//creating bounds
					int lowestX = 9999;
					int lowestY = 9999;
					int hiegestX = -9999;
					int hiegestY = -9999;
					for (int i = 0; i < static_cast<int>(vertesies.size()); i++)
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
					lowestX--;
					lowestY--;
					hiegestX++;
					hiegestY++;
					height = hiegestY - lowestY;
					width = hiegestX - lowestX;

					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();
					
					pixelsOfSprite = new uint8_t[height*tempSurface->pitch];
					
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
								x1 = x + lowestX;
								y1 = y + lowestY;
								x2 = width + lowestX;
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
					
					pixelsOfSprite = new uint8_t[height*tempSurface->pitch];
					
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
				case fastTriangle:
				{
					//creating bounds
					int lowestX = 9999;
					int lowestY = 9999;
					int hiegestX = -9999;
					int hiegestY = -9999;
					for (int i = 0; i < static_cast<int>(vertesies.size()); i++)
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
					lowestX--;
					lowestY--;
					hiegestX++;
					hiegestY++;
					height = hiegestY - lowestY;
					width = hiegestX - lowestX;
					
					initTempSurface();
					
					if (pathToTexture != nullptr)
						initTextureSurface();
					
					pixelsOfSprite = new uint8_t[height*tempSurface->pitch];
					
					for (int i = 0; i < static_cast<int>(tConects.size()); i++)
					{
						//creating edges for triangle
						std::vector<vec2> edgePoints;
						for (int u = 0; u < 3; u++)
						{
							int x1 = vertesies[tConects[i][u]].x - (width-2);
							int y1 = vertesies[tConects[i][u]].y - (height-2);
							int x2 = vertesies[tConects[i][(u+1)%3]].x - (width-2);
							int y2 = vertesies[tConects[i][(u+1)%3]].y - (height-2);
							if (y1 != y2)
							{
								if (y1 > y2)
								{
									std::swap(x1, x2);
									std::swap(y1, y2);
								}
								
								int dx = x2 - x1;
								int dy = y2 - y1;

								float m = 0;
								if (dx != 0)
									m = dy / dx;
								
								int count = 0;
								for (int ty = y1; ty < y2; ty++)
								{
									int tx = round(x1 + (m * count));
									edgePoints.push_back({static_cast<float>(tx), static_cast<float>(ty)});
									count++;
								}
							}
						}
						//sorting coords that we get for easy filling
						std::sort(edgePoints.begin(), edgePoints.end(), YSortVec2);
						//fill triangle
						for (int u = 0; u < static_cast<int>(edgePoints.size())-1; u += 2)
						{
							int x1 = edgePoints[u].x;
							int x2 = edgePoints[u+1].x;
							int y = edgePoints[u].y;
							
							//for knowing that we are going from smallest to heighest
							if (x1 > x2)
								std::swap(x1, x2);
							
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
									int tj = clampInt(x, 0, textureToLoad->w-1) * textureToLoad->pitch; //tx
									int tk = clampInt(y, 0, textureToLoad->h-1) * textureToLoad->format->BytesPerPixel; //ty
									
									pixelsOfSprite[j + k] = pixels[j + k] = pixelsToLoad[tj + tk+2];
									pixelsOfSprite[j + k+1] = pixels[j + k+1] = pixelsToLoad[tj + tk+1];
									pixelsOfSprite[j + k+2] = pixels[j + k+2] = pixelsToLoad[tj + tk];
									pixelsOfSprite[j + k+3] = pixels[j + k+3] = pixelsToLoad[tj + tk+3];
								}
							}
						}
					}
					createTexture();
					break;
				}
			}
			//TODO: implement elipsoids
		}

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
		
		//functions
		void initTempSurface()
		{
			tempSurface = SDL_CreateRGBSurface(0, height, width, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
			SDL_LockSurface(tempSurface);
			pixels = (uint8_t*)tempSurface->pixels;
		}
		
		void initTextureSurface()
		{
			SDL_Surface* tempImage = SDL_LoadBMP(pathToTexture);
			if (tempImage->w != width || tempImage->h != height)
				textureToLoad = resizeImage(width, height, tempImage);
			else
				textureToLoad = tempImage;
			pixelsToLoad = (uint8_t*) textureToLoad->pixels;
			if (textureToLoad == nullptr)
				SDL_Log("something wrong with texture loading %s\n", SDL_GetError());
		}
		
		void createTexture()
		{
			if (textureToLoad != nullptr)
				SDL_FreeSurface(textureToLoad);

			SDL_UnlockSurface(tempSurface);
			myTexture = SDL_CreateTextureFromSurface(rend, tempSurface);	
			SDL_FreeSurface(tempSurface);
		}
	};



	struct GameObject {
		//transform
		vec2 position = {250, 250};
		vec2 size = {1, 1};
		float rotation = 0;
		bool active = true;
		
		//mesh/draw
		mesh myMesh;
		
		void draw()
		{
			if (myMesh.meshIs == line)
			{
				SDL_SetRenderDrawColor(rend, myMesh.myColor.r, myMesh.myColor.g, myMesh.myColor.b, myMesh.myColor.a);
				for (int i = 0; i < static_cast<int>(myMesh.lConects.size()); i++)
				{
					SDL_RenderDrawLine(rend, myMesh.vertesies[myMesh.lConects[i].x].x + position.x, myMesh.vertesies[myMesh.lConects[i].x].y + position.y,
											myMesh.vertesies[myMesh.lConects[i].y].x + position.x, myMesh.vertesies[myMesh.lConects[i].y].y + position.y);
				}
			}
			else
			{
				SDL_Rect Pos;
				Pos.x = position.x - (size.x/2);
				Pos.y = position.y - (size.y/2);
				Pos.w = size.x;
				Pos.h = size.y;
				SDL_RenderCopyEx(rend, myMesh.myTexture, nullptr, &Pos, rotation, NULL, SDL_FLIP_NONE);
			}
		}
		void SetColor(Color newColor)
		{
			myMesh.myColor = newColor;
			myMesh.changeColor();
		}
		void SetColor(uint8_t toR, uint8_t toG, uint8_t toB, uint8_t toA)
		{
			myMesh.myColor = {toR, toG, toB, toA};
			myMesh.changeColor();
		}
		
		////////////////size//////////////
		void SetSize(vec2 newSize)
		{
			if (newSize.x != size.x || newSize.y != size.y)
			{
				switch (myMesh.meshIs)
				{
					case line:
					case fastTriangle:
					case polygon:
					{
						for (int i = 0; i < myMesh.vertexAmm; i++)
						{
							myMesh.vertesies[i] = {myMesh.vertesies[i].x / size.x * newSize.x, myMesh.vertesies[i].y / size.y * newSize.y};
						}
						myMesh.genMesh();
						size = newSize;
						break;
					}
					case circle:
					{
						myMesh.radius = round(newSize.x/2);
						myMesh.genMesh();
						size = {round(newSize.x), round(newSize.x)};
						break;
					}
					case box:
					{
						myMesh.bSize = newSize;
						size = newSize;
						myMesh.genMesh();
						break;
					}
				}
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
			
			else if (myMesh.meshIs == line)
			{
				for (int i = 0; i < myMesh.vertexAmm; i++)
				{
					float y = cos(rotInRad);
					float u = sin(rotInRad);
					vec2 result = {(myMesh.vertesies[i].x*y - myMesh.vertesies[i].y*u), (myMesh.vertesies[i].x*u + myMesh.vertesies[i].y*y)};
					myMesh.vertesies[i] = result;
				}
			}
		}
		void LookAt(vec2 targ, float addDegree)
		{
			rotate((360 - rotation) + (atan2(targ.y - position.y, targ.x - position.x)/toRadians) + addDegree);
		}
	};
}

namespace Input
{
	bool keysUp[322];
	bool keysDown[322];
	bool keys[322];
	bool newKeys[322];
	bool newMouse[3];
	bool mouse[3];
	bool mouseDown[3];
	bool mouseUp[3];
	
	void events()
	{	
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
			
			else if (event.type == SDL_KEYDOWN)
				newKeys[event.key.keysym.sym] = true;
			
			else if (event.type == SDL_KEYUP)
				newKeys[event.key.keysym.sym] = false;
			
			else if (event.type == SDL_MOUSEBUTTONDOWN)
				newMouse[event.button.button] = true;
			
			else if (event.type == SDL_MOUSEBUTTONUP)
				newMouse[event.button.button] = false;
		}
		for (int i = 0; i < 322; i++)
		{
			keysDown[i] = false;
			keysUp[i] = false;
			
			if (keys[i] == false && newKeys[i] == true)
				keysDown[i] = true;
			if (keys[i] == true && newKeys[i] == false)
				keysUp[i] = true;
			
			keys[i] = newKeys[i];
		}
		for (int i = 0; i < 3; i++)
		{
			mouseDown[i] = false;
			mouseUp[i] = false;
			
			if (mouse[i] == false && newMouse[i] == true)
				mouseDown[i] = true;
			if (mouse[i] == true && newMouse[i] == false)
				mouseUp[i] = true;
			
			mouse[i] = newMouse[i];
		}
	}
	vec2 mousePos()
	{
		int x;
		int y;
		SDL_GetMouseState(&x, &y);
		vec2 result = {(float) x, (float) y};
		return result;
	}
	
	//TODO: mouse wheel input
	
	//TODO: mouse delta input
	
	//TODO: wasd like in unity
}
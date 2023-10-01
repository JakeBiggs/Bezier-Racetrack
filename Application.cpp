	#include "stdafx.h"
	#include "Application.h"
	#include "Bezier.h"
	using namespace std;

	// Constructor -- initialise application-specific data here
	Application::Application()
	{
		m_track.push_back(Bezier(Vector2(400, 50), Vector2(600, 50), Vector2(700, 100), Vector2(700, 300)));
		m_track.push_back(Bezier(Vector2(700, 300), Vector2(700, 400), Vector2(700, 500), Vector2(600, 500)));
		m_track.push_back(Bezier(Vector2(600, 500), Vector2(400, 500), Vector2(100, 600), Vector2(100, 500)));
		m_track.push_back(Bezier(Vector2(100, 500), Vector2(100, 400), Vector2(200, 400), Vector2(300, 300)));
		m_track.push_back(Bezier(Vector2(300, 300), Vector2(400, 200), Vector2(100, 200), Vector2(100, 150)));
		m_track.push_back(Bezier(Vector2(100, 150), Vector2(100, 100), Vector2(200, 50), Vector2(400, 50)));
	}

	// Process a single event
	void Application::processEvent(const SDL_Event &ev)
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			m_quit = true;
			break;

		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_ESCAPE)
			{
				m_quit = true;
			}
			break;
		}
	}

	// Render the scene
	void Application::render()
	{
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);

		SDL_SetRenderDrawColor(m_renderer, 128, 255, 128, 255);
		for (const auto& segment : m_track)
		{
			segment.draw(m_renderer);
		}

		drawCarOnTrack(m_carPosition);
		updateCarPosition();
	}

	// Draw a car on the track
	// position is given in terms of the Bezier curves making up the track:
	// the first curve runs from 0.0f to 1.0f, the second one from 1.0f to 2.0f, and so on.
	// So for example a position of 1.2f means 0.2 along the second curve (the curve at index 1).
	void Application::drawCarOnTrack(float position)
	{
		// TODO
		int trackInt = static_cast<int>(position);
		float t = fmodf(position, 1.0f);
		//cin >> trackInt;
		
		if (trackInt >= 0 && trackInt < m_track.size()) {
			const Bezier& curve = m_track[trackInt];
			Vector2 p0 = curve.p0;
			Vector2 p1 = curve.p1;
			Vector2 p2 = curve.p2;
			Vector2 p3 = curve.p3;
			Vector2 curvePos = curve.cubicBezier(p0, p1, p2, p3, t);//Calculate car's position on curve
			
			float carAngle = findTangentAngle(findTangentVector(p0, p1, p2, p3, t));
			
			drawCar(curvePos, carAngle);
		}
	}

	Vector2 Application::findTangentVector(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t) {
		Vector2 tanPos;
		tanPos.x = 3 * pow((1 - t), 2) * (p1.x - p0.x) + 6 * (1 - t) * t * (p2.x - p1.x) + 3 * (t * t) * (p3.x - p2.x);
		tanPos.y = 3 * pow((1 - t), 2) * (p1.y - p0.y) + 6 * (1 - t) * t * (p2.y - p1.y) + 3 * (t * t) * (p3.y - p2.y);
		return tanPos;
	}

	float Application::findTangentAngle(Vector2 position) {
		float angleRadians = atan2f(position.y, position.x);
		float angleDegrees = angleRadians * (180/ 3.14159265358979323846f);
		return angleDegrees+90;
	}

	// Called on each frame to advance the car position variable
	void Application::updateCarPosition()
	{
		int trackInt = static_cast<int>(m_carPosition);
		float t = fmodf(m_carPosition, 1.0f);


		m_carPosition = fmodf(m_carPosition, m_track.size());
		
		//const Bezier& curve = m_track[trackInt];
		//ector2 tangent = findTangentVector(curve.p0, curve.p1, curve.p2, curve.p3, t);

		m_carPosition += 0.01f;
		
		
		/*
		//Target Speed in units per frame (maybe adjust)
		float targetSpeed = 0.01f; 

		//Current Track number
		int trackID = static_cast<int>(m_carPosition);

		m_carPosition = fmodf(m_carPosition, m_track.size());

		//Position within current curve
		float t = fmodf(m_carPosition, 1.0f);
		
		if (trackID >= 0 && trackID < m_track.size()) {
			//Bezier container for current curve
			const Bezier& curve = m_track[trackID];

			//Find Tangent at current point (value of t)
			Vector2 tangent = findTangentVector(curve.p0, curve.p1, curve.p2, curve.p3, t);

			//Calculating the length of the tangent vector
			float currentSpeed = tangent.magnitude();

			//Scaling factor to match the desired speed value
			float scalingFactor = targetSpeed / currentSpeed;

			m_totalDistance += targetSpeed;
			float deltaT = m_totalDistance / curve.curveLength;

			// Increment the car position
			m_carPosition += deltaT;
			// Wrap the car position to be in the range 0 to m_track.size()
			m_carPosition = fmodf(m_carPosition, m_track.size());
		}
		else {
			m_carPosition += 0.01f;
		}
		*/
	}

	//----------------------------------------------------------------------------

	// Initialise the required parts of the SDL library
	// Return true if initialisation is successful, or false if initialisation fails
	bool Application::initSDL()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			return false;
		}

		m_window = SDL_CreateWindow("COMP270", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, c_windowWidth, c_windowHeight, SDL_WINDOW_SHOWN);
		if (m_window == nullptr)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			return false;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (m_renderer == nullptr)
		{
			std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			return false;
		}

		m_carSprite = loadTexture("car.png");
		if (m_carSprite == nullptr)
		{
			return false;
		}

		return true;
	}

	// Shutdown the SDL library
	void Application::shutdownSDL()
	{
		if (m_carSprite)
		{
			SDL_DestroyTexture(m_carSprite);
			m_carSprite = nullptr;
		}

		if (m_renderer)
		{
			SDL_DestroyRenderer(m_renderer);
			m_renderer = nullptr;
		}

		if (m_window)
		{
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}

		SDL_Quit();
	}

	// Load a texture
	SDL_Texture* Application::loadTexture(const std::string& path)
	{
		// Load the surface from the specified image file
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (!loadedSurface)
		{
			std::cout << "IMG_Load(" << path << ") error: " << IMG_GetError() << std::endl;
			return nullptr;
		}
		else
		{
			//Create texture from surface pixels
			SDL_Texture* newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
			if (!newTexture)
			{
				std::cout << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);

			return newTexture;
		}
	}

	// Draw a car sprite, centred at the given position, with the given rotation
	void Application::drawCar(const Vector2& position, float angleDegrees)
	{
		const float size = 50;
		SDL_FRect rect;
		rect.x = position.x - size / 2;
		rect.y = position.y - size / 2;
		rect.w = rect.h = size;

		SDL_RenderCopyExF(m_renderer, m_carSprite, nullptr, &rect, angleDegrees, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	//----------------------------------------------------------------------------

	// Run the application
	// Return true if the application finishes successfully, or false if an error occurs
	bool Application::run()
	{
		// Initialisation
		if (!initSDL())
			return false;

		// Main loop
		m_quit = false;
		while (!m_quit)
		{
			// Process events
			SDL_Event ev;
			while (SDL_PollEvent(&ev))
			{
				processEvent(ev);
			}

			// Render
			render();
			SDL_RenderPresent(m_renderer);
		}

		// Shutdown
		shutdownSDL();
		return true;
	}

	//----------------------------------------------------------------------------

	// Application entry point
	int main(int, char**)
	{
		Application application;
		if (application.run())
			return 0;
		else
			return 1;
	}

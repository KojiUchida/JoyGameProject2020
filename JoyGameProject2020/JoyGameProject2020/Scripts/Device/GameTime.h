#pragma once

class GameTime {

private:
	GameTime();
	~GameTime();
	GameTime(const GameTime&) = delete;
	void operator=(const GameTime&) = delete;

public:
	static GameTime& Instance();
	void Init();
	void Update();

	static double CurrentTime();
	static double ElapsedTime();
	static double DeltaTime();
	static double FPS();
	static double UnscaledDeltaTime();
};
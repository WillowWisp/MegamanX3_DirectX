#pragma once
#include"Scene.h"
class SceneManager
{
private:
	static SceneManager *instance;

	Scene *currentScene;
public:
	SceneManager();
	Scene * getScene();
	static SceneManager *getInstance();

	void Update();
	void ReplaceScene(Scene *scene);
	~SceneManager();
};


#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
	currentScene = nullptr;
}

Scene * SceneManager::getScene()
{
	return currentScene;
}

SceneManager * SceneManager::getInstance()
{
	if (!instance)
		instance = new SceneManager();
	return instance;
}


void SceneManager::Update()
{
	currentScene->Update();
}

void SceneManager::ReplaceScene(Scene * scene)
{
	delete currentScene;
	currentScene = scene;
}

SceneManager::~SceneManager()
{
}

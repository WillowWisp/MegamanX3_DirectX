#pragma once

class Scene
{
protected:

public:
	Scene();
	//ham khoi tao scene
	virtual void Start();

	//ham ve scene
	virtual void Render();

	//ham update
	virtual void Update();
	~Scene();
};


#pragma once

enum class SceneType
{
	None,
	Exit,
	Menu,
	Game,
	Shop,
	Summary,
	Selection,
	Settings,
	History,
	About,
};


class Scene
{
	SceneType nextScene = SceneType::None;
public:
	virtual ~Scene() = default;			// 确保通过父类指针销毁子类对象时，子类对象中资源被完全清理
	virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	void SetNextScene(SceneType const& t)
	{
		nextScene = t;
	}
	SceneType GetNextScene() const 
	{
		return nextScene;
	}
};
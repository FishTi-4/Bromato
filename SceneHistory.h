#pragma once

#include "context.h"
class SceneHistory : public Scene
{
	std::vector<std::string> saveFileList;
	int saveIndex = -1;
	RuntimeData previewdata; //选中的save的数据
	int previewIndex = -1;

	int maxSize = 10; //文件最大数量

	Button btn{ {1450,950},{300,100},"返回主菜单" };

	void RefreshSaveList();
public:
	SceneHistory();
	void Input() override;
	void Update() override;
	void Draw() override;
};

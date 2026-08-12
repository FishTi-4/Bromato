// 环境，写代码加上

#pragma once

#include <raylib.h>
#include <raymath.h>
#include <memory>

#include "AssetsManager.h"
#include "Attribute.h"
#include "WeaponsManager.h"
#include "ItemManager.h"
#include "CharacterManager.h"
#include "MapManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "RuntimeData.h"
#include "Entity.h"
#include "EntityPool.h"
#include "Scene.h"
#include "Button.h"
#include "AudioManager.h"
#include "ChineseFontHandler.h"

extern AssetsManager assets;
extern WeaponsManager weapons;
extern ItemManager items;
extern CharacterManager characters;
extern MapManager maps;
extern EnemyManager enemies;
extern RuntimeData data;
extern AudioManager audio;
extern Font fontCN;
extern ChineseFontHandler chineseFontHandler;

const int LOGICAL_WIDTH = 1920;
const int LOGICAL_HEIGHT = 1080;

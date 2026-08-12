#include "SceneGame.h"

/*
玩家部分
负责人：雪梨
实现函数：PlayerUpdate
实现功能：玩家的 无敌时间 移动 边界（不要让材质出去一半）方向（单位化） 朝向(looking)
*/
void SceneGame::PlayerUpdate()
{
	//更新无敌时间
	player.UpdateInvincibleTimer();

	//移动速度
	player.SetSpeed(data.attribute.speedMul * 300);

	// 移动方向
	Vector2 dir = { 0, 0 };
	if (IsKeyDown(KEY_W)) dir.y -= 1;
	if (IsKeyDown(KEY_S)) dir.y += 1;
	if (IsKeyDown(KEY_A)) dir.x -= 1;
	if (IsKeyDown(KEY_D)) dir.x += 1;

	player.SetDirection(dir);		//单位化
	player.UpdatePosition();		//更新位置

	//边界限制
	Vector2 pos = player.GetPosition();
	if (pos.x < PLAYER_RADIUS) pos.x = PLAYER_RADIUS;
	if (pos.x > MAP_WIDTH - PLAYER_RADIUS) pos.x = MAP_WIDTH - PLAYER_RADIUS;	//确保玩家形状不会卡出地图外（x方向上）
	if (pos.y < PLAYER_RADIUS) pos.y = PLAYER_RADIUS;
	if (pos.y > MAP_HEIGHT - PLAYER_RADIUS) pos.y = MAP_HEIGHT - PLAYER_RADIUS;	//确保玩家形状不会卡出地图外（y方向上）
	player.SetPosition(pos);

	//朝向
	if (IsKeyDown(KEY_A)) player.SetLooking(0);
	if (IsKeyDown(KEY_D)) player.SetLooking(1);
}
// ----------------------------------------------------------------------------------------------------


/*
尝试开枪部分
负责人：咸鱼
实现函数：
Enemy* FindEnemy(float radius)			尝试找到某个武器攻击范围内最近的敌人
float CalculateWeaponDamage(WeaponName) 对某一把武器，计算他开出的子弹的伤害
void TryShot(WeaponName)				对某一把武器，尝试开枪
void ProcessAllWeapon()					处理所有武器
实现功能：
对于每把武器尝试开枪
找到最近敌人
计算子弹伤害（武器基础伤害 + 武器等级 * 武器升级伤害）* (1+伤害加成)
处理武器冷却（攻速效果）
生成子弹申请 playerBulletSpawnRequests
*/
Enemy* SceneGame::FindEnemy(float radius)
{
	float d = LLONG_MAX;
	auto& temp = enemyPool.GetEntities();			//咸鱼：注意看一下头文件，我把这个放到成员函数了，不然用不了类的成员函数
	Enemy* res = nullptr;

	auto pos = player.GetPosition();
	for (auto& x : temp) {
		if (!x.IsActive()) continue;
		float dis = pow(x.GetPosition().x - pos.x, 2) + pow(x.GetPosition().y - pos.y, 2);
		if (dis >= radius * radius) continue;

		if (dis <= d) {
			d = dis;
			res = &x;
		}
	}
	return res;
}

float SceneGame::CalculateWeaponDamage(WeaponName weaponName)
{
	const Weapon& weapon = weapons.GetWeapon(weaponName);
	return  (weapon.damage + data.weaponLevel[Idx(weaponName)] * weapon.upgradeDamage) * data.attribute.damageMul;
}

Vector2 SceneGame::RotateVector(Vector2 v, float angle)
{
	angle *= DEG2RAD;
	float c = cosf(angle);
	float s = sinf(angle);

	return {
		v.x * c - v.y * s,
		v.x * s + v.y * c
	};
}

void SceneGame::TryShot(WeaponName weaponName)
{
	// 尝试找到敌人，如果没找到直接返回
	// 计算武器伤害
	// 构造子弹生成申请 BulletSpawnRequest
	// 分别设置好子弹生成申请对象的参数：生成位置 移动方向 速度 半径 伤害 材质
	// 将子弹生成申请压入 vector
	// 重制该武器的冷却时间 weaponShootTimers[Idx(weaponName)]
	// 计算公式：实际冷却时间 = 武器冷却时间 / data.attribute.hasteMul


	auto target = FindEnemy(weapons.GetWeapon(weaponName).radius);
	if (target == nullptr)	return;

	auto val = CalculateWeaponDamage(weaponName);
	auto& w = weapons.GetWeapon(weaponName);

	Vector2 dire = { target->GetPosition().x - player.GetPosition().x, target->GetPosition().y - player.GetPosition().y };

	BulletSpawnRequest cur{ player.GetPosition(), dire, w.bulletSpeed, w.bulletRadius, val, w.bulletTextureName };

	if (weaponName == WeaponName::Pistol) audio.PlaySFX(SFXType::PistolShot);
	if (weaponName == WeaponName::Shotgun) audio.PlaySFX(SFXType::ShotgunShot);
	if (weaponName == WeaponName::Scout) audio.PlaySFX(SFXType::ScoutShot);
	if (weaponName == WeaponName::Smg) audio.PlaySFX(SFXType::SmgShot);
	playerBulletSpawnRequests.push_back(cur);
	if (weaponName == WeaponName::Shotgun)
	{
		cur.direction = RotateVector(cur.direction, 25);
		playerBulletSpawnRequests.push_back(cur);
		cur.direction = RotateVector(cur.direction, 25);
		playerBulletSpawnRequests.push_back(cur);
		cur.direction = RotateVector(cur.direction, -75);
		playerBulletSpawnRequests.push_back(cur);
		cur.direction = RotateVector(cur.direction, -25);
		playerBulletSpawnRequests.push_back(cur);
	}
	weaponShootTimers[static_cast<int>(weaponName)] = weapons.GetWeapon(weaponName).cooldown / data.attribute.hasteMul;

	return;
}

void SceneGame::ProcessAllWeapon()
{
	// 使用 Idx(WeaponName::Count) 从 0 开始一直枚举到 Idx(WeaponName::Count) - 1
	// 尝试使用每一把枪开枪
		// 更新这把枪的冷却时间
		// 如果枪冷却时间小于 0，则尝试用这把枪开枪

	for (int i = 0; i < Idx(WeaponName::Count); i++)
	{
		weaponShootTimers[i] -= data.dt;
		if (weaponShootTimers[i] <= 0) TryShot((WeaponName)i);
	}
}

// --------------------------------------------------------------------------------------------------------------

/*
敌人部分
负责人：呆猫
实现函数：
void UpdateEnemy()
实现功能：
尝试生成敌人
敌人更新
*/

// 更新 enemySpawnTimer 
// 敌人生成
	// 如果 计时器到点了 生成一个敌人 重制计时器时间为 ENEMY_SPAWN_INTERVAL
	//	随机生成四种敌人中的一种
	//  使用 enemyPool.Create() 来获得白板敌人，然后设置好敌人的各种参数
	//  敌人生成在地图边缘位置
// 处理敌人更新
	// 获取 &enemyPool.GetEntities() 并遍历
	// 对于每个敌人 方向朝向玩家 可以使用Vector2Subtract()函数
	// 对于每个敌人 更新注视方向 (enemy.SetLooking(0/1))
	// 对于每个敌人 UpdateShootTimer() 并 检查是否 CanShoot()
		// 如果 canshoot 使用 ResetShootTimer() 重制冷却 并 向enemyBulletSpawnRequests 压入子弹生成申请
	// 使用 enemyPool.UpdatePosition() 更新所有敌人的移动
// ---------------------------------------------------------------------------------------------------------------

static double pe_distance(Vector2 player, Vector2 enemy) {
	return (enemy.x - player.x) * (enemy.x - player.x) + (enemy.y - player.y) * (enemy.y - player.y);
}

static Vector2 pe_direction(Vector2 player, Vector2 enemy) {
	return std::move(Vector2Normalize(Vector2(player.x - enemy.x, player.y - enemy.y)));
}

static Vector2 pe_direction(Vector2 player, Vector2 enemy, EnemyName name) {
	if (name == EnemyName::Ranged) {
		if (pe_distance(player, enemy) < 100000) {
			return std::move(Vector2Normalize(Vector2(enemy.x - player.x, enemy.y - player.y)));
		}
		else if (pe_distance(player, enemy) == 100000) {
			return std::move(Vector2(0, 0));
		}
	}
	return std::move(Vector2Normalize(Vector2(player.x - enemy.x, player.y - enemy.y)));
}

class Logistic {
public:
	Logistic() {
		l = 50;
		k = -0.3;
		x0 = 9;
	}

	Logistic(double l, double k, double x0) {
		this->l = l;
		this->k = k;
		this->x0 = x0;
	}

	int f(double x) {
		return l / (1 + exp(k * (x - x0)));
	}
private:
	double l;
	double k;
	double x0;
};

void SceneGame::EnemyUpdate() {
	Logistic logistic = Logistic(50 + data.wave * 3, -0.3, 5);
	enemySpawnTimer -= data.dt;
	auto& enemy_pool = enemyPool.GetEntities();
	Vector2 player_position = player.GetPosition();
	if (enemySpawnTimer <= 0) {
		enemySpawnTimer = ENEMY_SPAWN_INTERVAL;
		int current_number = enemyPool.GetActiveCount();
		if (current_number < 1000) {
			int rand = GetRandomValue(1, logistic.f(data.wave));
			while (rand > current_number) {

				Vector2 new_enemy_positon = std::move(Vector2(GetRandomValue(0, MAP_WIDTH - 1), GetRandomValue(0, MAP_HEIGHT - 1)));
				while (pe_distance(player_position, new_enemy_positon) < 40000) {
					new_enemy_positon.x = GetRandomValue(0, MAP_WIDTH - 1);
					new_enemy_positon.y = GetRandomValue(0, MAP_HEIGHT - 1);
				}
				EnemyName pick;
				int r = GetRandomValue(1, data.wave <= 2 ? 90 : 100);	//5波以前不会随机到91-100，就不会刷boss
				if (r <= 30) pick = EnemyName::Normal1;		//slow----30%
				else if (r <= 55) pick = EnemyName::Normal2;		//fast-----25%
				else if (r <= 75) pick = EnemyName::Ranged;			//remote----20%
				else if (r <= 90) pick = EnemyName::Charge;			//charge----15%
				else              pick = EnemyName::Boss;			//boss-----10%

				enemySpawnRequests.push_back(std::move(EnemySpawnRequest{
					/*std::vector<EnemyName>{EnemyName::Normal1,EnemyName::Charge,EnemyName::Normal2,EnemyName::Ranged,EnemyName::Boss}[GetRandomValue(0,4)]*/
					pick,
					new_enemy_positon
					}))
					;
				current_number++;
				if (current_number == 1000) {
					break;
				}
				rand = GetRandomValue(1, logistic.f(data.wave));
			}
		}
	}

	for (Enemy& enemy : enemy_pool) {
		if (!enemy.IsActive()) {
			continue;
		}
		Enemy enemy_temp = enemies.GetEnemy(enemy.GetName());
		Vector2 direction = Vector2Normalize(enemy.GetDirection()) * enemy.GetSpeed() + 
			pe_direction(player_position, enemy.GetPosition(), enemy.GetName()) * enemy_temp.GetSpeed() / 2.0;
		double speed = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction = Vector2Normalize(direction);
		if (speed > enemy_temp.GetSpeed()) {
			speed = enemy_temp.GetSpeed();
		}
		//Vector2 direction = pe_direction(player_position, enemy.GetPosition());
		enemy.SetSpeed(speed);
		enemy.SetDirection(direction);
		enemy.SetLooking(pe_direction(player_position, enemy.GetPosition()).x > 0);
		enemy.UpdateShootTimer();
		if (enemy.CanShoot()) {
			enemy.ResetShootTimer();
			enemyBulletSpawnRequests.push_back(std::move(
				BulletSpawnRequest(
					enemy.GetPosition(),
					pe_direction(player_position, enemy.GetPosition()),
					enemy.GetBulletSpeed(),
					enemy.GetBulletRadius(),
					enemy.GetBulletDamage(),
					TextureName::Bullet
				)
			));
		}
	}
	enemyPool.UpdatePosition();

	for (auto i : enemySpawnRequests) {
		Enemy* ptr = enemyPool.Create();
		Enemy enemy = enemies.GetEnemy(i.name);
		ptr->SetBulletDamage(enemy.GetBulletDamage() * (1 + 0.1 * data.wave));
		ptr->SetBulletRadius(enemy.GetBulletRadius());
		ptr->SetBulletSpeed(enemy.GetBulletSpeed());
		ptr->SetDamage(enemy.GetDamage() * (1 + 0.1 * data.wave));
		ptr->SetDirection(pe_direction(player.GetPosition(), i.position));
		ptr->SetHP(enemy.GetHP() * (1 + 0.1 * data.wave));
		ptr->SetLooking(ptr->GetDirection().x > 0);
		ptr->SetName(enemy.GetName());
		ptr->SetPosition(i.position);
		ptr->SetRadius(enemy.GetRadius());
		ptr->SetShootInterval(enemy.GetShootInterval());
		ptr->SetSpeed(enemy.GetSpeed());
		ptr->SetTextureName(enemy.GetTextureName());
		ptr->ResetShootTimer();
	}
	enemySpawnRequests.clear();
}

/*
子弹部分
负责人：黑蛋
实现函数：
void HandleBulletSpawnRequests(BulletPool& pool, std::vector<BulletSpawnRequest>& requests)
void BulletUpdate()
实现功能：
处理敌我子弹生成申请
更新子弹移动
*/

void SceneGame::HandleBulletSpawnRequests(BulletPool& pool, std::vector<BulletSpawnRequest>& requests)
{
	// 遍历每一个申请
	for (auto& req : requests) {
		// 对于每一个申请：
		// 从子弹池申请一个子弹
		Bullet* bullet = pool.Create();

		if (!bullet) continue;

		// 设置这个子弹的属性
		bullet->SetPosition(req.position);
		bullet->SetDirection(req.direction);
		bullet->SetSpeed(req.speed);
		bullet->SetRadius(req.radius);
		bullet->SetDamage(req.damage);
		bullet->SetTextureName(req.texture);
	}

	// 清空申请vector
	requests.clear();
}

/*
每一帧统一更新场上所有处于激活状态的子弹，负责子弹的移动、边界判定回收
*/
void SceneGame::BulletUpdate()
{
	HandleBulletSpawnRequests(enemyBulletPool, enemyBulletSpawnRequests);
	HandleBulletSpawnRequests(playerBulletPool, playerBulletSpawnRequests);
	std::vector<Bullet>& player_bulletPool = playerBulletPool.GetEntities();
	for (auto& bullet : player_bulletPool) {
		if (bullet.IsActive()) {
			// 调用子弹对象池的 UpdatePosition() 函数更新子弹位置
			bullet.UpdatePosition();
			Vector2 pos = bullet.GetPosition();
			if (pos.x < 0 || pos.x > MAP_WIDTH || pos.y < 0 || pos.y > MAP_HEIGHT) {
				bullet.Delete();
			}
		}
	}

	std::vector<Bullet>& enemyBullets = enemyBulletPool.GetEntities();
	for (auto& bullet : enemyBullets)
	{
		if (bullet.IsActive())
		{
			// 调用子弹对象池的 UpdatePosition() 函数更新子弹位置
			bullet.UpdatePosition();
			Vector2 pos = bullet.GetPosition();
			if (pos.x < 0 || pos.x > MAP_WIDTH || pos.y < 0 || pos.y > MAP_HEIGHT)
			{
				bullet.Delete();
			}
		}
	}
}
// ---------------------------------------------------------------------------------------------

/*
碰撞部分
负责人：蒟蒻
实现函数：
void SceneGame::CollisionUpdate()
实现功能：
处理三种碰撞 把伤害压到 damageRequests 里
玩家敌人 子弹玩家 子弹敌人
统一处理伤害
统一处理死亡
子弹消失
*/
void SceneGame::CollisionUpdate()
{
	damageRequests.clear();//上一帧先清一下

	Vector2 playerpos = player.GetPosition();
	float playerRadius = player.GetRadius();//玩家半径

	//敌人玩家，玩家受击
	if (player.IsActive() && !player.IsInvincible())//玩家无敌帧过了，也活着
	{
		for (auto& enemy : enemyPool.GetEntities())
		{
			if (!enemy.IsActive())continue;//死的跳了
			if (CheckCollisionCircles(playerpos, playerRadius, enemy.GetPosition(), enemy.GetRadius()))
			{
				damageRequests.push_back({ &player,nullptr,enemy.GetDamage() });
				audio.PlaySFX(SFXType::Attack);
				break;
			}
		}
	}

	//子弹玩家，玩家受击
	if (player.IsActive() && !player.IsInvincible())
	{
		for (auto& bullet : enemyBulletPool.GetEntities())
		{
			if (!bullet.IsActive()) continue;
			if (CheckCollisionCircles(playerpos, playerRadius,
				bullet.GetPosition(), bullet.GetRadius()))
			{
				damageRequests.push_back({ &player, nullptr, bullet.GetDamage() });
				bullet.Delete();  // 子弹命中后消失
				audio.PlaySFX(SFXType::BulletImpactBody);
				break;
			}
		}
	}

	//子弹敌人，敌人受击
	for (auto& bullet : playerBulletPool.GetEntities())
	{
		if (!bullet.IsActive())continue;
		for (auto& enemy : enemyPool.GetEntities())
		{
			if (!enemy.IsActive())continue;
			if (CheckCollisionCircles(bullet.GetPosition(), 
				bullet.GetRadius(), enemy.GetPosition(), enemy.GetRadius()))
			{
				Enemy enemy_temp = enemies.GetEnemy(enemy.GetName());
				Vector2 new_direction = Vector2Normalize(enemy.GetDirection()) * enemy.GetSpeed() 
					+ Vector2Normalize(enemy.GetPosition() - bullet.GetPosition()) * enemy_temp.GetSpeed() * 3;
				double new_speed = sqrt(new_direction.x * new_direction.x + new_direction.y * new_direction.y);
				new_direction = Vector2Normalize(new_direction);
				enemy.SetDirection(new_direction);
				enemy.SetSpeed(new_speed);
				damageRequests.push_back({ nullptr,&enemy,bullet.GetDamage() });
				audio.PlaySFX(SFXType::Hit);
				bullet.Delete();
				break;
			}
		}
	}
	//处理全部的扣血
	for (auto& dam : damageRequests)
	{
		if (dam.playerTarget)
			dam.playerTarget->TakeDamage(dam.damage / data.attribute.defenceMul);//扣玩家
		if (dam.enemyTarget)
		{
			dam.enemyTarget->TakeDamage(dam.damage);
			damageTexts.Add((int)dam.damage, dam.enemyTarget->GetPosition());
		}
	}
	//敌人死亡
	for (auto& enemy : enemyPool.GetEntities())
	{
		if (!enemy.IsActive())continue;//死的跳过
		if (enemy.GetHP() <= 0)
		{
			enemy.Delete();
			Coin* coin = coinPool.Create();//这里
			if (coin)//池子里是否有金币，但基本都有应该
			{
				coin->SetPosition(enemy.GetPosition());
				coin->SetRadius(20);
				coin->SetTextureName(TextureName::Coin);
			}
			data.enemyKilled++;
		}
	}
	//捡金币//这里
	for (auto& coin : coinPool.GetEntities())
	{
		if (!coin.IsActive())continue;
		if (CheckCollisionCircles(playerpos, playerRadius,
			coin.GetPosition(), coin.GetRadius()))
		{
			coin.Delete();
			data.coin += 20;//这里改成100能达到战斗爽
		}
	}
	//玩家寄了
	if (player.GetHP() <= 0)
		data.isDead = true;
}
//-----------------------------------------------------------------------------------------------------



/*
初始化部分 外部控制函数 检测游戏结束 绘制
负责人：夜游
*/

void SceneGame::Input()
{
	if (IsKeyPressed(KEY_L)) player.TakeDamage(10);
	if (!isPaused && pauseButton.Update())
	{
		isPaused = 1;
		audio.PauseBGM();
	}
	if (isPaused && resumeButton.Update())
	{
		isPaused = 0;
		audio.ResumeBGM();
	}
	if (isPaused && endGameButton.Update())
	{
		player.SetHP(-1);
		isPaused = 0;
	}
}

SceneGame::SceneGame()
{
	Initialize();
}


void SceneGame::Initialize()
{
	switch (data.map)
	{
	case Map::Mp1: audio.PlayBGM(BGMTrack::Map1); break;
	case Map::Mp2: audio.PlayBGM(BGMTrack::Map2); break;
	case Map::Mp3: audio.PlayBGM(BGMTrack::Map3); break;
	case Map::Mp4: audio.PlayBGM(BGMTrack::Map4); break;
	}

	// 每次进入场景重新计算玩家属性
	data.attribute = Attribute{};
	const Attribute& characterAttribute = characters.GetCharacter(data.character);
	const Attribute& mapAttribute = maps.GetMap(data.map);
	data.attribute.hpMul += characterAttribute.hpMul + mapAttribute.hpMul;
	data.attribute.hpAdd += characterAttribute.hpAdd + mapAttribute.hpAdd;
	data.attribute.defenceMul += characterAttribute.defenceMul + mapAttribute.defenceMul;
	data.attribute.damageMul += characterAttribute.damageMul + mapAttribute.damageMul;
	data.attribute.hasteMul += characterAttribute.hasteMul + mapAttribute.hasteMul;
	data.attribute.speedMul += characterAttribute.speedMul + mapAttribute.speedMul;
	for (int i = 0; i < Idx(ItemName::Count); i++)
	{
		const Attribute& itemAttribute = items.GetItem((ItemName)i);
		int count = data.itemCount[i];
		data.attribute.hpMul += itemAttribute.hpMul * count;
		data.attribute.hpAdd += itemAttribute.hpAdd * count;
		data.attribute.defenceMul += itemAttribute.defenceMul * count;
		data.attribute.damageMul += itemAttribute.damageMul * count;
		data.attribute.hasteMul += itemAttribute.hasteMul * count;	
		data.attribute.speedMul += itemAttribute.speedMul * count;
	}
	data.attribute.textureName = characterAttribute.textureName;
	player.SetActive(1);
	player.SetPosition({ MAP_WIDTH / 2,MAP_HEIGHT / 2 });
	player.SetRadius(PLAYER_RADIUS);
	player.SetSpeed(data.attribute.speedMul);
	player.SetHP(data.attribute.hpAdd * data.attribute.hpMul);
	player.SetMaxHP(data.attribute.hpAdd * data.attribute.hpMul);
	player.SetTextureName(characters.GetCharacter(data.character).textureName);
	data.isDead = 0;
	data.waveOver = 0;
	data.wave++;
	data.timer = 0;
	for (int i = 0; i < Idx(WeaponName::Count); i++) weaponShootTimers[i] = 0;

	// 摄像机设置
	camera.offset = { LOGICAL_WIDTH / 2,LOGICAL_HEIGHT / 2 };
	camera.target = player.GetPosition();
	camera.rotation = 0;
	camera.zoom = 1;
}

void SceneGame::Update()
{
	if (isPaused) return;
	data.timer += data.dt;
	data.totalTimer += data.dt;
	PlayerUpdate();
	ProcessAllWeapon();
	EnemyUpdate();
	BulletUpdate();
	CollisionUpdate();
	CheckEnd();
}

void SceneGame::CheckEnd()
{
	if (data.timer > ROUND_TIME)
	{
		data.timer = 0;
		//audio.StopBGM();
		SetNextScene(SceneType::Shop);
	}
	if (player.GetHP() > 0) return;
	player.Delete();
	data.isDead = 1;
	//audio.StopBGM();
	SetNextScene(SceneType::Summary);
}

static void DrawGameIcon(TextureName textureName, Rectangle area)
{
	const Texture2D& texture = assets.GetTexture(textureName);
	Rectangle source = { 0,0,(float)texture.width,(float)texture.height };
	DrawTexturePro(texture, source, area, { 0,0 }, 0, WHITE);
}

void SceneGame::Draw()
{
	// -------------------摄像机------------------------------------
	// 摄像机跟踪
	camera.target = player.GetPosition();
	// 摄像机不能拍到地图外面，得拉回来
	if (camera.target.x - LOGICAL_WIDTH / 2 < 0) camera.target.x = LOGICAL_WIDTH / 2;
	if (camera.target.x + LOGICAL_WIDTH / 2 > MAP_WIDTH) camera.target.x = MAP_WIDTH - LOGICAL_WIDTH / 2;
	if (camera.target.y - LOGICAL_HEIGHT / 2 < 0) camera.target.y = LOGICAL_HEIGHT / 2;
	if (camera.target.y + LOGICAL_HEIGHT / 2 > MAP_HEIGHT) camera.target.y = MAP_HEIGHT - LOGICAL_HEIGHT / 2;
	BeginMode2D(camera);

	// ---------------------地图 --------------------------------------------------------------
	const Texture2D& mapTexture = assets.GetTexture(maps.GetMap(data.map).textureName);
	Rectangle src = { 0,0,(float)mapTexture.width,(float)mapTexture.height };
	Rectangle dest = { 0,0,MAP_WIDTH,MAP_HEIGHT };
	DrawTexturePro(mapTexture, src, dest, { 0,0 }, 0, WHITE);

	// ---------------------- 玩家 --------------------------------------------------
	Color color;
	if (player.IsInvincible() && ((int)(player.GetInvincibleTimer() * 20) % 2)) color = RED;
	else color = WHITE;
	const Texture2D& playertexture = assets.GetTexture(player.GetTextureName());
	float radius = player.GetRadius();
	Rectangle source = { 0,0,player.GetLooking() ? playertexture.width : -playertexture.width,playertexture.height };
	Rectangle destination = { player.GetPosition().x - radius,player.GetPosition().y - radius,radius + radius,radius + radius };
	DrawTexturePro(playertexture, source, destination, { 0,0 }, 0, color);

	// --------------------- 武器 -----------------------------------------

	for (int i = 0; i < Idx(WeaponName::Count); i++)
	{
		const Texture2D& text = assets.GetTexture(weapons.GetWeapon((WeaponName)i).textureName);
		Rectangle src = { 0,0,(float)text.width,(float)text.height };
		Rectangle dst = { player.GetPosition().x - 70 + i * 36,player.GetPosition().y - radius - 42,32,32 };
		DrawTexturePro(text, src, dst, { 0,0 }, 0, weaponShootTimers[i] > 0 ? GRAY : WHITE);
	}

	// -------------------- 实体 ---------------------------------------------
	enemyPool.Draw();
	playerBulletPool.Draw();
	enemyBulletPool.Draw();
	coinPool.Draw();
	damageTexts.Draw();

	EndMode2D();

	// ----------------------------- UI --------------------------------------------
	float hpRatio = 0;
	if (player.GetMaxHP() > 0) hpRatio = player.GetHP() / player.GetMaxHP();
	DrawRectangle(40, 35, 420, 36, Color{ 45,45,45,220 });
	DrawRectangle(44, 39, (int)(412 * hpRatio), 28, Color{ 205,55,55,255 });
	DrawTextEx(fontCN, TextFormat("生命: %.0f / %.0f", player.GetHP(), player.GetMaxHP()), { 52,40 }, 24, 1, WHITE);

	Rectangle killedPanel = { 40,90,180,60 };
	DrawRectangleRounded(killedPanel, 0.25, 8, Color{ 35,40,50,220 });
	DrawRectangleRoundedLinesEx(killedPanel, 0.25, 8, 2, Color{ 120,130,145,220 });
	DrawGameIcon(TextureName::Slow, { 50,99,42,42 });
	DrawTextEx(fontCN, TextFormat("%d", data.enemyKilled), { 110,103 }, 32, 1, WHITE);

	Rectangle coinPanel = { 240,90,180,60 };
	DrawRectangleRounded(coinPanel, 0.25, 8, Color{ 35,40,50,220 });
	DrawRectangleRoundedLinesEx(coinPanel, 0.25, 8, 2, Color{ 214,158,62,220 });
	DrawGameIcon(TextureName::Coin, { 250,99,42,42 });
	DrawTextEx(fontCN, TextFormat("%d", data.coin), { 310,103 }, 32, 1, GOLD);

	Rectangle wavePanel = { 440,90,180,60 };
	DrawRectangleRounded(wavePanel, 0.25, 8, Color{ 35,40,50,220 });
	DrawRectangleRoundedLinesEx(wavePanel, 0.25, 8, 2, Color{ 98,216,155,220 });
	DrawTextEx(fontCN, TextFormat("第 %d 波", data.wave), { 475,103 }, 30, 1, WHITE);

	float remainingTime = ROUND_TIME - data.timer;
	if (remainingTime < 0) remainingTime = 0;
	Color timerColor = remainingTime <= 5 ? RED : WHITE;
	Rectangle timerPanel = { LOGICAL_WIDTH / 2 - 130,25,260,95 };
	DrawRectangleRounded(timerPanel, 0.25, 8, Color{ 25,30,40,230 });
	DrawRectangleRoundedLinesEx(timerPanel, 0.25, 8, 3, timerColor);
	const char* timerText = TextFormat("%.1f", remainingTime);
	DrawTextEx(fontCN, "剩余时间", { 915,32 }, 20, 1, LIGHTGRAY);
	DrawTextEx(fontCN, timerText, { 930,58 }, 48, 1, timerColor);

	if (isPaused)
	{
		DrawRectangle(0, 0, MAP_WIDTH, MAP_HEIGHT, Fade(BLACK, 0.5));
		const char* text = "游戏已暂停";
		int fontSize = 70;
		int textWidth = MeasureTextEx(fontCN, text, fontSize, 1).x;
		DrawTextEx(fontCN, text, { (float)(LOGICAL_WIDTH - textWidth) / 2,(float)(LOGICAL_HEIGHT - fontSize) / 2 }, fontSize, 1, WHITE);
		resumeButton.Draw();
		endGameButton.Draw();
	}
	if (hpRatio < 0.3)
	{
		DrawRectangle(0, 0, MAP_WIDTH, MAP_HEIGHT, Fade(RED, 0.3 - hpRatio));
	}
	if (!isPaused) pauseButton.Draw();
}

void SceneGame::DamageText::Add(int damage, Vector2 position)
{
	Text text
	{
		fontCN,
		std::to_string(damage),
		position,
		position,
		(float)GetRandomValue(-15,15),
		(float)(30 + GetRandomValue(-4,4))
	};

	deleteTimes.push_back(GetTime() + 0.4);
	texts.push_back(text);
}

void SceneGame::DamageText::Draw()
{
	double currentTime = GetTime();
	while (!deleteTimes.empty())
	{
		if (deleteTimes.front() <= currentTime)
		{
			deleteTimes.pop_front();
			texts.pop_front();
		}
		else break;
	}

	for (auto& text : texts)
		DrawTextPro(text.font, text.text.c_str(), text.position, { 0,0 }, text.rotation, text.size, 2, { 204,22,22,225 });
}

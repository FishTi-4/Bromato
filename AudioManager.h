#pragma once

#include <raylib.h>
#include <unordered_map>

enum class BGMTrack
{
	Background,
	Map1,
	Map2,
	Map3,
	Map4,
	Count
};

enum class SFXType
{
	Attack,
	Hit,
	Push,
	Shoot,
	Buy,
	Select,
	AxeSwing,
	KnifeSwing,
	PistolShot,
	SmgShot,
	ShotgunShot,
	ScoutShot,
	BulletImpactBody,
	Count
};

class AudioManager
{
public:
	void Init();
	void Update();
	void Shutdown();

	void PlayBGM(BGMTrack track);
	void StopBGM();
	void PauseBGM();
	void ResumeBGM();
	BGMTrack GetCurrentBGM() const;

	void PlaySFX(SFXType type);

private:
	std::unordered_map<BGMTrack, Music> bgmMap;
	std::unordered_map<SFXType, Sound> sfxMap;
	BGMTrack currentBGM = BGMTrack::Background;
	bool playing = false;
};

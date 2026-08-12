#include "AudioManager.h"
#include "context.h"

void AudioManager::Init()
{
	InitAudioDevice();

	sfxMap[SFXType::Attack] = LoadSound("assets/sounds/attack.wav");
	sfxMap[SFXType::Hit] = LoadSound("assets/sounds/hit.wav");
	sfxMap[SFXType::Push] = LoadSound("assets/sounds/push.wav");
	sfxMap[SFXType::Shoot] = LoadSound("assets/sounds/shoot.wav");
	sfxMap[SFXType::Buy] = LoadSound("assets/sounds/buy.wav");
	sfxMap[SFXType::Select] = LoadSound("assets/sounds/select.wav");
	sfxMap[SFXType::AxeSwing] = LoadSound("assets/sounds/axe_swing.wav");
	sfxMap[SFXType::KnifeSwing] = LoadSound("assets/sounds/knife_swing.wav");
	sfxMap[SFXType::PistolShot] = LoadSound("assets/sounds/pistol_shot.wav");
	sfxMap[SFXType::SmgShot] = LoadSound("assets/sounds/smg_shot.wav");
	sfxMap[SFXType::ShotgunShot] = LoadSound("assets/sounds/shotgun_shot.wav");
	sfxMap[SFXType::ScoutShot] = LoadSound("assets/sounds/scout_shot.wav");
	sfxMap[SFXType::BulletImpactBody] = LoadSound("assets/sounds/bullet_impact_body.wav");

	bgmMap[BGMTrack::Background] = LoadMusicStream("assets/sounds/bg.wav");
	bgmMap[BGMTrack::Map1] = LoadMusicStream("assets/BGM/Map1.mp3");
	bgmMap[BGMTrack::Map2] = LoadMusicStream("assets/BGM/Map2.mp3");
	bgmMap[BGMTrack::Map3] = LoadMusicStream("assets/BGM/Map3.mp3");
	bgmMap[BGMTrack::Map4] = LoadMusicStream("assets/BGM/Map4.mp3");
}

void AudioManager::Update()
{
	if (playing)
	{
		Music& music = bgmMap[currentBGM];
		SetMusicVolume(music, data.volume / 100.f);
		UpdateMusicStream(music);
	}
}

void AudioManager::Shutdown()
{
	StopBGM();
	for (auto& pair : bgmMap) UnloadMusicStream(pair.second);
	for (auto& pair : sfxMap) UnloadSound(pair.second);
	CloseAudioDevice();
}

void AudioManager::PlayBGM(BGMTrack track)
{
	if (playing && currentBGM == track) return;
	if (playing) StopMusicStream(bgmMap[currentBGM]);
	currentBGM = track;
	PlayMusicStream(bgmMap[currentBGM]);
	playing = true;
}

void AudioManager::StopBGM()
{
	if (playing) StopMusicStream(bgmMap[currentBGM]);
	playing = false;
}

void AudioManager::PauseBGM()
{
	if (playing) PauseMusicStream(bgmMap[currentBGM]);
}

void AudioManager::ResumeBGM()
{
	if (playing) ResumeMusicStream(bgmMap[currentBGM]);
}

void AudioManager::PlaySFX(SFXType type)
{
	SetSoundVolume(sfxMap[type], data.volume / 100.f);
	PlaySound(sfxMap[type]);
}

BGMTrack AudioManager::GetCurrentBGM() const { return currentBGM; }

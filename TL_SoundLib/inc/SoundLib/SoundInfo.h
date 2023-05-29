#pragma once
#ifndef __SOUNDINFO
#define __SOUNDINFO

#include "Math\TL_Math.h"

#ifdef SOUND_EXPORTING
#define SOUND_DLL_DECLSPEC __declspec(dllexport)//COMMON_API
#define SOUND_DLL_STRUCT __declspec(dllexport)//COMMON_API
#else
#define SOUND_DLL_DECLSPEC __declspec(dllimport)//COMMON_API
#define SOUND_DLL_STRUCT
#endif

namespace TL_Sound
{
	class SoundSource;
	class Channel;
	class Sound;
	class Effect;
	class ISoundSystem;
	class SoundListener;
	class ReverbZones;

	enum class eFileType
	{
		WAV = 0
		, BANK = 1
	};

	enum class eEffectType
	{
		UNKNOWN = 0
		, MIXER = 1
		, OSCILLATOR = 2
		, LOWPASS = 3
		, ITLOWPASS = 4
		, HIGHPASS = 5
		, ECHO = 6
		, FADER = 7
		, FLANGE = 8
		, DISTORTION = 9
		, NORMALIZE = 10
		, LIMITER = 11
		, PARAMEQ = 12
		, PITCHSHIFT = 13
		, CHORUS = 14
		, VSTPLUGIN = 15
		, WINAMPPLUGIN = 16
		, ITECHO = 17
		, COMPRESSOR = 18
		, SFXREVERB = 19
		, LOWPASS_SIMPLE = 20
		, DELAY = 21
		, TREMOLO = 22
		, LADSPAPLUGIN = 23
		, SEND = 24
		, RETURN = 25
		, HIGHPASS_SIMPLE = 26
		, PAN = 27
		, THREE_EQ = 28
		, FFT = 29
		, LOUDNESS_METER = 30
		, ENVELOPEFOLLOWER = 31
		, CONVOLUTIONREVERB = 32
		, CHANNELMIX = 33
		, TRANSCEIVER = 34
		, OBJECTPAN = 35
		, MULTIBAND_EQ = 36
		, MAX = 37
	};

	enum class eAudioRolloffMode
	{
		Linear = 0x00200000
		, Custom = 0x04000000
	};
	
	enum class eReverbPreset
	{
		PRESET_OFF                
		, PRESET_GENERIC
		, PRESET_PADDEDCELL
		, PRESET_ROOM
		, PRESET_BATHROOM
		, PRESET_LIVINGROOM
		, PRESET_STONEROOM
		, PRESET_AUDITORIUM
		, PRESET_CONCERTHALL
		, PRESET_CAVE
		, PRESET_ARENA
		, PRESET_HANGAR
		, PRESET_CARPETTEDHALLWAY
		, PRESET_HALLWAY
		, PRESET_STONECORRIDOR
		, PRESET_ALLEY
		, PRESET_FOREST
		, PRESET_CITY
		, PRESET_MOUNTAINS
		, PRESET_QUARRY
		, PRESET_PLAIN
		, PRESET_PARKINGLOT
		, PRESET_SEWERPIPE
		, PRESET_UNDERWATER
	};

	struct ListenerAttributes 
	{
		TL_Math::Vector3 _position;
		TL_Math::Vector3 _velocity;
		TL_Math::Vector3 _forward;
		TL_Math::Vector3 _up;
	};

	struct ReverbAttributes
	{
		TL_Math::Vector3 _position;
		float _minDistance;
		float _maxDistance;
	};

	struct ReverbProperties
	{
		ReverbProperties()
			: _decayTime(1500.f)
			, _earlyDelay(7.f)
			, _lateDelay(11.f)
			, _hfReference(5000.f)
			, _hfDecayRatio(50.f)
			, _diffusion(50.f)
			, _density(100.f)
			, _lowShelfFrequency(250.f)
			, _lowShelfGain(0.f)
			, _highCut(200000.f)
			, _earlyLateMix(50.f)
			, _wetLevel(-6.f)
		{}
		ReverbProperties(float dacayTime, float earlyDalay, float lateDelay, float hfReference, float hfDecayRatio, float diffusion
			, float density, float lowShelfFrequency, float lowShelfGain, float highCut, float earlyLateMix, float wetLevel)
			: _decayTime(dacayTime)
			, _earlyDelay(earlyDalay)
			, _lateDelay(lateDelay)
			, _hfReference(hfReference)
			, _hfDecayRatio(hfDecayRatio)
			, _diffusion(diffusion)
			, _density(density)
			, _lowShelfFrequency(lowShelfFrequency)
			, _lowShelfGain(lowShelfGain)
			, _highCut(highCut)
			, _earlyLateMix(earlyLateMix)
			, _wetLevel(wetLevel)
		{}
		float _decayTime;			// [   0, 20000 ]
		float _earlyDelay;			// [   0,   300 ]
		float _lateDelay;			// [   0,   100 ]
		float _hfReference;			// [  20, 20000 ]
		float _hfDecayRatio;		// [  10,   100 ]
		float _diffusion;			// [  10,   100 ]
		float _density;				// [   0,   100 ]
		float _lowShelfFrequency;	// [  20,  1000 ]
		float _lowShelfGain;		// [ -36,    12 ]
		float _highCut;				// [   0, 20000 ]
		float _earlyLateMix;		// [   0,   100 ]	
		float _wetLevel;			// [ -80,    20 ]
	};
}
#endif
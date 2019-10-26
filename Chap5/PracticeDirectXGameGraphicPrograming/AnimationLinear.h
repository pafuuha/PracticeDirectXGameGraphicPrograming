#pragma once
class AnimationLinear
{
public:
	AnimationLinear();
	~AnimationLinear();

	void CreateKey(int numKey);

	void SetKeyValue(int index, float key, float value);

	void SetDuration(DWORD duration);

	void SetLoopFlag(DWORD isLoop);

	void SetStarTime(DWORD start);

	bool GetValue(DWORD time, float* pValue, bool *pIsEnd);

protected:
	float GetFraction(DWORD time);

	int GetBeginIndex(float fraction);

private:
	int m_numKey;
	float* m_keys;
	float* m_values;
	bool m_isLoop;
	DWORD m_startTime;
	DWORD m_duration;
};


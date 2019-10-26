#include "stdafx.h"
#include "AnimationLinear.h"


AnimationLinear::AnimationLinear()
	: m_numKey(0), m_keys(0), m_values(0), m_isLoop(TRUE), m_startTime(0), m_duration(0)
{
}


AnimationLinear::~AnimationLinear()
{
	if (m_keys != 0) {
		delete[] m_keys;
	}

	if (m_values != 0) {
		delete[] m_values;
	}
}

void AnimationLinear::CreateKey(int numKey)
{

	// �L�[�t���[�����̐ݒ�
	m_numKey = numKey;

	// �Â��f�[�^���폜
	if (m_keys != 0) {
		delete[] m_keys;
	}

	if (m_values != 0) {
		delete[] m_values;
	}

	// �V�����z��̐���
	m_keys = new float[m_numKey];
	m_values = new float[m_numKey];
}

void AnimationLinear::SetKeyValue(int index, float key, float value)
{
	// �͈͊O�����O
	if ((index < 0) || (index >= m_numKey)) 
		return;
	

	m_keys[index] = key;
	m_values[index] = value;
}

void AnimationLinear::SetDuration(DWORD duration)
{
	m_duration = duration;
}

void AnimationLinear::SetLoopFlag(DWORD isLoop)
{
	m_isLoop = isLoop;
}

void AnimationLinear::SetStarTime(DWORD start)
{
	m_startTime = start;
}

bool AnimationLinear::GetValue(DWORD time, float * pValue, bool * pIsEnd)
{
	if (pValue == 0) 
		return false;
	
	if (pIsEnd == 0) 
		return false;
	
	if (m_numKey <= 0) 
		return false;
	
	//
	// �A�j���[�V�������I��������
	//
	// �I������
	DWORD endTime = m_startTime + m_duration;

	if (!m_isLoop) {
		if (time < m_startTime) {
			*pValue = m_values[0];
			*pIsEnd = true;
			return true;
		}
		else if (time > endTime) {
			*pValue = m_values[m_numKey - 1];
			*pIsEnd = true;
			return true;
		}
	}

	//
	// �A�j���[�V�����̌v�Z
	//
	int beginIndex;
	int endIndex;
	float fraction;
	float slope;

	// ����
	fraction = GetFraction(time);

	beginIndex = GetBeginIndex(fraction);
	endIndex = beginIndex + 1;

	if (beginIndex < 0) {
		*pValue = m_values[0];
	}
	else if (beginIndex >= m_numKey - 1) {
		*pValue = m_values[m_numKey - 1];
	}
	else {
		float diffValue = m_values[endIndex] - m_values[beginIndex];
		float diffTime = m_keys[endIndex] - m_keys[beginIndex];
		// �X��
		slope = diffValue / diffTime;

		float fPastFromPrev = fraction - m_keys[beginIndex];
		*pValue = slope * fPastFromPrev * fPastFromPrev + m_values[beginIndex];
	}

	*pIsEnd = false;

	return true;
}

float AnimationLinear::GetFraction(DWORD time)
{
	DWORD diffTime;
	float fraction;

	if (m_duration == 0) {
		fraction = 1.0f;
	}
	else {
		if (time > m_startTime) {
			diffTime = time - m_startTime;
			diffTime = diffTime % m_duration;
			fraction = (float)diffTime / (float)m_duration;
			fraction = fraction - (int)fraction;
		}
		else {
			diffTime = m_startTime - time;
			diffTime = diffTime % m_duration;
			fraction = (float)diffTime / (float)m_duration;
			fraction = 1.0f - (fraction - (int)fraction);

		}
	}


	return fraction;
}

int AnimationLinear::GetBeginIndex(float fraction)
{
	if (fraction < m_keys[0])
		return -1;

	int index = 0;
	for (int i = 0; i < m_numKey; i++) {
		if (m_keys[i] <= fraction) {
			index = i;
		}
		else {
			break;
		}
	}

	return index;
}

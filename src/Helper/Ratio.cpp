#include "Ratio.hpp"

#include "Debugging/Log.hpp"

Ratio::Ratio(unsigned int numerator, unsigned int denominator)
	: m_numerator(numerator), m_denominator(denominator),
	  m_bMustRecomputeAspectRatio(false), m_bMustRecomputeNumAndDenom(false)
{
	recomputeAspectRatio();
}

Ratio::Ratio(float fRatio)
	: m_ratio(fRatio),
	  m_bMustRecomputeAspectRatio(false), m_bMustRecomputeNumAndDenom(false)
{
	recomputeNumAndDenom();
}

void Ratio::set(float newRatio) {
	m_ratio = newRatio;
	m_bMustRecomputeNumAndDenom = true;
}

void Ratio::set(unsigned int num, unsigned int denom) {
	m_numerator = num;
	m_denominator = denom;
	m_bMustRecomputeAspectRatio = true;
}

void Ratio::set(const Ratio& ratio) {
	m_numerator = ratio.numerator();
	m_denominator = ratio.denominator();
	m_ratio = (float)ratio;
	m_bMustRecomputeAspectRatio = false;
	m_bMustRecomputeNumAndDenom = false;
}

void Ratio::operator =(const Ratio& ratio) {
	set(ratio);
}

Ratio::operator float() const {
	if (m_bMustRecomputeAspectRatio)
		recomputeAspectRatio();
	return m_ratio;
}

unsigned int Ratio::numerator() const {
	if (m_bMustRecomputeNumAndDenom)
		recomputeNumAndDenom();
	return m_numerator;
}
unsigned int Ratio::denominator() const {
	if (m_bMustRecomputeNumAndDenom)
		recomputeNumAndDenom();
	return m_denominator;
}

unsigned int* Ratio::getNumeratorPtr() {
	if (m_bMustRecomputeNumAndDenom)
		recomputeNumAndDenom();
	m_bMustRecomputeAspectRatio = true;
	return &m_numerator;
}

unsigned int* Ratio::getDenominatorPtr() {
	if (m_bMustRecomputeNumAndDenom)
		recomputeNumAndDenom();
	m_bMustRecomputeAspectRatio = true;
	return &m_denominator;
}

void Ratio::recomputeAspectRatio() const {
	*(float*)& m_ratio = (float)m_numerator / m_denominator; // we need to mark this method as const so that getNumerator() etc. can be marked as const, and therefore be used when a ratio is passed by const reference
	*(bool*)& m_bMustRecomputeAspectRatio = false;
}

void Ratio::recomputeNumAndDenom() const {// we need to mark this method as const so that getNumerator() etc. can be marked as const, and therefore be used when a ratio is passed by const reference
	// following this article : https://begriffs.com/pdf/dec2frac.pdf
	float precision = 0.00001f;
	float n = 1;
	float Z_n = m_ratio;
	int D_n_minus_one = 0;
	int D_n = 1;
	int N_n = round(m_ratio * D_n);
	while (n < 5 && abs(m_ratio - (float)D_n / N_n) > precision) {
		n++;
		float tmp = Z_n - floor(Z_n);
		if (abs(tmp) > precision) {
			Z_n = 1.0f / (tmp);
			int tmpD_n = D_n;
			D_n = D_n * floor(Z_n) + D_n_minus_one;
			D_n_minus_one = tmpD_n;
			N_n = round(m_ratio * D_n);
		}
		else {
			break;
		}
	}
	*(unsigned int*)& m_numerator = N_n;
	*(unsigned int*)& m_denominator = D_n;
	*(bool*)& m_bMustRecomputeNumAndDenom = false;
	//spdlog::info("{} is approx {} / {}", m_ratio, m_numerator, m_denominator);
}
#pragma once

struct Ratio {
public:
	Ratio(unsigned int numerator, unsigned int denominator);
	Ratio(float fRatio);

	void set(float newRatio);
	void set(unsigned int num, unsigned int denom);
	void set(const Ratio& ratio);
	void operator =(const Ratio& ratio);

	operator float() const;
	unsigned int numerator() const;
	unsigned int denominator() const;

	unsigned int* getNumeratorPtr();
	unsigned int* getDenominatorPtr();

private:
	void recomputeAspectRatio() const; // we need to mark these methods as const so that getNumerator() etc. can be marked as const,
	void recomputeNumAndDenom() const; // and therefore be used when a ratio is passed by const reference
private:
	unsigned int m_numerator;
	unsigned int m_denominator;
	float m_ratio;

	bool m_bMustRecomputeAspectRatio;
	bool m_bMustRecomputeNumAndDenom;
};
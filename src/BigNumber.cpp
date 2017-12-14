#include "BigNumber.h"

#include <cmath>
#include <cstdio>

const uint32_t BigNumber::INTERAL_BUFFER_SIZE = 100;
const uint32_t BigNumber::MAX_SEGMENT_VALUE = 1000;
const std::vector<const char*> BigNumber::SEGMENT_STRINGS_LONG = 
	{ ""
	, "thousand"
	, "million"
	, "billion"
	, "trillion"
	, "quadrillion"
	, "quintillion"
	, "sextillion"
	, "septillion"
	, "octillion"
	, "nonillion"
	, "decillion"
	, "undecillion"
	, "duodecillion"
	, "tredecillion"
	, "quatturodecillion"
	, "quindecillion"
	, "sexdecillion"
	, "septendecillion"
	, "octodecillion"
	, "novemdecillion"
};
const std::vector<const char*> BigNumber::SEGMENT_STRINGS_SHORT =
{ ""
, "thou."
, "M"
, "B"
, "T"
, "q"
, "Q"
, "s"
, "S"
, "O"
, "N"
, "d"
, "UD"
, "DD"
, "TD"
, "qD"
, "QD"
, "sD"
, "SD"
, "OD"
, "ND"
};

const uint32_t BigNumber::SEGMENT_COUNT = SEGMENT_STRINGS_LONG.size();

void BigNumber::normalise()
{
	for (uint32_t index = 0; index < _segments.size() - 1; ++index)
	{
		if (_segments[index] >= MAX_SEGMENT_VALUE)
		{
			_segments[index + 1] += ((int)_segments[index] / MAX_SEGMENT_VALUE);
			_segments[index] = ((int)_segments[index] % MAX_SEGMENT_VALUE);
		}
	}
}

void BigNumber::normaliseDecimals()
{
	BigNumberValue decimalValue; 
	_segments[0] = round(_segments[0]);;
	for (uint32_t index = 1; index < _segments.size(); ++index)
	{
		decimalValue = _segments[index] - floor(_segments[index]);
		if (decimalValue != 0)
		{
			_segments[index - 1] += (decimalValue * MAX_SEGMENT_VALUE);
			_segments[index] = floor(_segments[index]);
		}
	}
}

const uint32_t BigNumber::topIndex() const
{
	for (int32_t index = _segments.size() - 1; index >=0; --index)
	{
		if (_segments[index] != 0)
		{
			return index;
		}
	}
	return 0;
}

BigNumber BigNumber::MAX_BIG_NUMBER()
{
	BigNumber result;
	for (uint32_t index = 0; index < result._segments.size(); ++index)
	{
		result._segments[index] = 999;
	}
	return result;
}

BigNumber::BigNumber()
	: _segments( std::vector<BigNumberValue>(SEGMENT_COUNT))
{
}


BigNumber::BigNumber(int64_t value)
	: _segments(std::vector<BigNumberValue>(SEGMENT_COUNT))
{
	for (uint32_t index = 0; index < _segments.size(); ++index)
	{
		_segments[index] = value % MAX_SEGMENT_VALUE;
		value /= MAX_SEGMENT_VALUE;
	}
}

BigNumber BigNumber::operator+(const BigNumber & other) const
{
	return BigNumber(*this) += other;
}

BigNumber & BigNumber::operator+=(const BigNumber & other)
{
	for (uint32_t index = 0; index < _segments.size(); ++index)
	{
		_segments[index] += other._segments[index];
	}
	normalise();
	return *this;
}

BigNumber BigNumber::operator-(const BigNumber & other) const
{
	return BigNumber(*this)-=other;
}

BigNumber & BigNumber::operator-=(const BigNumber & other)
{
	if (*this < other)
		return BigNumber();
	for (uint32_t index = 0; index < _segments.size(); ++index)
	{
		_segments[index] -= other._segments[index];
		if (_segments[index] < 0)
		{
			_segments[index] += MAX_SEGMENT_VALUE;
			if (index + 1 < topIndex())
			{
				--_segments[index + 1];
			}
		}
	}
	normalise();
	return *this;
}

BigNumber BigNumber::operator*(const BigNumberValue & scalar) const
{
	return BigNumber(*this) *= scalar;
}

BigNumber & BigNumber::operator*=(const BigNumberValue & scalar)
{
	for (uint32_t index = 0; index < _segments.size(); ++index)
	{
		_segments[index] *= scalar;
	}
	normaliseDecimals();
	normalise();
	return *this;
}


bool BigNumber::operator>(const BigNumber & other) const
{
	uint32_t topIndex = this->topIndex();
	if (topIndex > other.topIndex())
		return true;

	for (int32_t index = topIndex; index >= 0; --index)
	{
		if (_segments[index] > other._segments[index])
			return true;
	}
	return false;
}

bool BigNumber::operator>=(const BigNumber & other) const
{
	return !(*this<other);
}

bool BigNumber::operator<(const BigNumber & other) const
{
	return other>*this;
}

bool BigNumber::operator<=(const BigNumber & other) const
{
	return !(*this>other);
}

bool BigNumber::operator==(const BigNumber & other) const
{
	uint32_t topIndex = topIndex;
	if (topIndex != other.topIndex())
		return false;

	for (uint32_t index = 0; index <= topIndex; ++index)
	{
		if (_segments[index] != other._segments[index])
			return false;
	}
	return true;
}

bool BigNumber::operator!=(const BigNumber & other) const
{
	return !(*this == other);
}

BigNumberValue BigNumber::getValue(const uint32_t & index) const
{
	if(index>=0 && index<=SEGMENT_COUNT)
		return _segments[index];
	return 0.0;
}

std::string BigNumber::toStringE(const bool & rightAlign) const
{
	char buffer[INTERAL_BUFFER_SIZE];
	char * writer = buffer;
	uint32_t writtenBytes = 0;
	uint32_t topIndex = this->topIndex();
	for (int32_t index = topIndex; index >=0 ;--index)
	{
		writtenBytes = sprintf(writer
								, index != topIndex ? "%03.0lf%s": "%1.0lf%s"
								, _segments[index]
								, index != 0 ? "," : "");
		writer += writtenBytes;
	}
	if (rightAlign)
	{
		std::string aligner("%"+std::to_string(SEGMENT_COUNT * 4 + 3)+"s");
		char alignedBuffer[INTERAL_BUFFER_SIZE];
		sprintf(alignedBuffer, aligner.c_str(), buffer);
		return std::string(alignedBuffer);
	}
	return std::string(buffer);
}

std::string BigNumber::toString(const bool & longVersion) const
{
	char buffer[INTERAL_BUFFER_SIZE];
	uint32_t topIndex = this->topIndex();
	const char * segmentString = longVersion ? SEGMENT_STRINGS_LONG[topIndex] : SEGMENT_STRINGS_SHORT[topIndex];
	if (topIndex > 1)
		sprintf(buffer, "%1.0lf %s", _segments[topIndex], segmentString);
	else if(topIndex == 1)
		sprintf(buffer, "%1.0lf,%03.0lf",_segments[1], _segments[0]);
	else 
		sprintf(buffer, "%1.0lf", _segments[0]);
	return std::string(buffer);
}

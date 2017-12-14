#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H
#include <string>
#include <vector>


//treated as ints but we will still use decimal values for multiplication with constants < 1.0
typedef double BigNumberValue;

class BigNumber
{
public:
	static const uint32_t MAX_SEGMENT_VALUE;
	static const std::vector<const char*> SEGMENT_STRINGS_LONG;
	static const std::vector<const char*> SEGMENT_STRINGS_SHORT;
	static const uint32_t SEGMENT_COUNT;

	static BigNumber MAX_BIG_NUMBER();

	BigNumber();
	BigNumber(int64_t value);

	BigNumber operator+(const BigNumber & other) const;
	BigNumber & operator+=(const BigNumber & other);

	BigNumber operator-(const BigNumber & other) const;
	BigNumber & operator-=(const BigNumber & other);

	BigNumber operator*(const BigNumberValue & scalar) const;
	BigNumber & operator*=(const BigNumberValue & scalar);

	bool operator>(const BigNumber & other) const;
	bool operator>=(const BigNumber & other) const;
	bool operator<(const BigNumber & other) const;
	bool operator<=(const BigNumber & other) const;
	bool operator==(const BigNumber & other) const;
	bool operator!=(const BigNumber & other) const;

	BigNumberValue getValue(const uint32_t & index) const;

	std::string toStringE(const bool & rightAlign = false) const;

	std::string toString(const bool & longVersion = true) const;

private:
	
	//reduces segment values to numbers under the max value and increments higher index segments accordingly
	void normalise();
	
	// due to division/multiplication with a scalar, some values will require normalisation
	void normaliseDecimals();

	//returns index of right-most(in the vector) non-null segment
	const uint32_t topIndex() const;

	std::vector<BigNumberValue> _segments;

	static const uint32_t INTERAL_BUFFER_SIZE;
};


#endif
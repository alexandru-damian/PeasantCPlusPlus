#include "BigNumber.h"
#include <stdio.h>

#define PRINT_BIG(number) printf("%s = %s = %s\n", number.toString(false).c_str(),number.toString().c_str(), number.toStringE().c_str())
int main(int argc, const char * argv[])
{
	BigNumber big(350256123);
	BigNumber copy(big);
	copy += (big+big);


	PRINT_BIG(big);
	PRINT_BIG(copy);

	BigNumber testMultiply(1000);
	PRINT_BIG((testMultiply * 0));
	PRINT_BIG((testMultiply * 1));
	PRINT_BIG((testMultiply * 5000000));
	PRINT_BIG((testMultiply * 0.5));
	PRINT_BIG((big * 0.5));
	PRINT_BIG((copy * 5));
	PRINT_BIG(BigNumber::MAX_BIG_NUMBER());
	PRINT_BIG((BigNumber::MAX_BIG_NUMBER()*20));

	PRINT_BIG((BigNumber::MAX_BIG_NUMBER() -1));
	PRINT_BIG((BigNumber::MAX_BIG_NUMBER() - BigNumber::MAX_BIG_NUMBER()));
	return 0;
}
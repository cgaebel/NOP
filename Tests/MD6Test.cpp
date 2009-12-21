#include "../IHash.h"
#include "../IHash.cpp"
#include "../HMD6.h"
#include "../HMD6.cpp"

SUITE(MD6Test)
{
	struct MD6Fixture
	{
		HMD6 context;
		std::string toHash;
	};

	TEST_FIXTURE(MD6Fixture, HelloWorld)
	{
		toHash = "Hello world!";

		CHECK_EQUAL("39200e5197b11a1229f2df7d2e453c3b4d8a8d2238b5c83c1d89077be85e95616d80f2ab1b557992d7a6ac2232089fdcb271a0d28d8e099960d8b216c81d9ad2",
					context.GetHashAndReset((const BYTE*)toHash.c_str(), toHash.length()));
	}

	TEST_FIXTURE(MD6Fixture, HelloWorld2)
	{
		// Note the difference in capitalization.
		toHash = "hello World!";

		CHECK_EQUAL("483389cb30ee0a12fbac484e316e26647f10c29bf80ab51153cf0bd0705e367bfc04d5e30653ede4492c69d6c4647dce6d3778fe9e466cbd32681e0c8305eb42",
					context.GetHashAndReset((const BYTE*)toHash.c_str(), toHash.length()));
	}

	TEST_FIXTURE(MD6Fixture, HashWithSalt)
	{
		toHash = "Hello world!";
		const unsigned char* salt = (const unsigned char*)"arglebargle";

		HMD6 temp(512, salt, strlen((const char*)salt));

		CHECK_EQUAL("cdf5440e36cbec1fac005df6cb28f5155c56ca5295b4b661b9baff0043fb470e7c8102806ebb61a648a466cd6a5c3e695c9fc59e6d96e8742b2c2a1e354059f3",
					temp.GetHashAndReset((const BYTE*)toHash.c_str(), toHash.length()));
	}
}
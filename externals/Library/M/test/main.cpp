#include "String.h"
#include "Md5Hash.h"

#include <stdio.h>

int main() {

	M::String str("my text");
	printf("%s\n", str.ToCString());
	printf("cap: %zu\n", str.Capacity());
	printf("len: %zu\n", str.Length());

	str.Reserve(128);
	printf("%s\n", str.ToCString());
	printf("cap: %zu\n", str.Capacity());
	printf("len: %zu\n", str.Length());

	M::String str2;
	str2 = "this is the start of some long text that will be assigned to";
	str2 += "my mstring which is a custom implementation similar to std::string";
	str2 += "but designed to be more consistent with my coding style";
	str2 += "it's also way faster!";
	printf("%s\n", str2.ToCString());
	printf("cap: %zu\n", str2.Capacity());
	printf("len: %zu\n", str2.Length());

	str2 = "new assignment";
	printf("%s\n", str2.ToCString());
	printf("cap: %zu\n", str2.Capacity());
	printf("len: %zu\n", str2.Length());

	printf("\nmd5 test\n");
	M::Md5Hash orig("098f6bcd4621d373cade4e832627b4f6");
	M::Md5Hash test;
	test.Update("test", 4);
	test.Final();
	printf("Orig Hash: %s\n", orig.ToString().ToCString());
	printf("Test Hash: %s\n", test.ToString().ToCString());


	return 0;
}

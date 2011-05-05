#include "String.hxx"
#include "Md5Hash.hxx"

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

	printf("\nerase test\n");
	M::String erase("0123456789");
	printf("starts with: %s\n", erase.ToCString());
	erase.Erase(0,0);
	printf("erase(0,0):  %s\n", erase.ToCString());
	erase.Erase(0,1);
	printf("erase(0,1):  %s\n", erase.ToCString());
	erase.Erase(0,1);
	printf("erase(0,1):  %s\n", erase.ToCString());
	erase.Erase(3,1);
	printf("erase(3,1):  %s\n", erase.ToCString());
	erase.Erase(10,1);
	printf("erase(10,1): %s\n", erase.ToCString());
	erase.Erase(1,10);
	printf("erase(1,10): %s\n", erase.ToCString());
	erase.Erase(4);
	printf("erase(4):    %s\n", erase.ToCString());
	erase.Erase();
	printf("erase():     %s\n", erase.ToCString());

	printf("\nFastCompare4 Test (true, then false, false)\n");
	M::String fast("0123456789");
	printf("[%s]\n", (fast.FastCompare4('0', '1', '2', '3')) ? "true" : "false");
	printf("[%s]\n", (fast.FastCompare4('0', '1', '2', '4')) ? "true" : "false");
	printf("[%s]\n", (fast.FastCompare4('4', '3', '2', '1')) ? "true" : "false");

	printf("\nmore string test\n");
	M::String ct;
	ct += '2';
	ct += '0';
	ct += '0';
	printf("Should be 200: [%s]\n", ct.ToCString());

	return 0;
}

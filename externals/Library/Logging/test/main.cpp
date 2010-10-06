#include "Logging.h"

#include <stdio.h>
#include <vector>

void a();
void b();
void c();
void d();
void e();
void f();
void g();
void h();
void i();

void a() {
	b();
}

void b() {
	c();
}

void c() {
	d();
}

void d() {
	e();
}

void e() {
	f();
}

void f() {
	g();
}

void g() {
	h();
}

void h() {
	i();
}

void i() {
	Logging::Debug("Where am I?");
}

int main() {
	
	Logging::SetLogLevel(Logging::LEVEL_DEBUG);
	Logging::Debug("Testing Debug");
	Logging::Info("Testing Info");
	Logging::Warn("Testing Warn");
	Logging::Error("Testing Error");
	Logging::Critical("Testing Critical");
	printf("\n");

	Logging::SetLogLevel(Logging::LEVEL_WARN);
	Logging::Debug("Testing Debug");
	Logging::Info("Testing Info");
	Logging::Warn("Testing Warn");
	Logging::Error("Testing Error");
	Logging::Critical("Testing Critical");
	printf("\n");

	Logging::SetLogLevel(Logging::LEVEL_CRITICAL);
	Logging::Debug("Testing Debug", 123);
	Logging::Info("Testing Info");
	Logging::Warn("Testing Warn");
	Logging::Error("Testing Error");
	Logging::Critical("Testing Critical");
	printf("\n");

	printf("Current logging level: %s\n",
	       Logging::GetLogLevel().ToString().ToCString());

	Logging::SetLogLevel(Logging::LEVEL_DEBUG);
	//Logging::SetStackDump(true);
	a();

	return 0;
}

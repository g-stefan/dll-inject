// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2008-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

#include <stdio.h>
#include <string.h>

#include <stdexcept>
#include <string>
#include <vector>

#include <XYO/DllInject/Application.hpp>

// XYO::Win also declares an Application class, so refer to this project's
// class through an alias instead of `using namespace XYO::DllInject`.
using String = XYO::Encoding::String;
using Application = XYO::DllInject::Application;

static void check(bool value, const char *expression, int line) {
	if (!value) {
		char message[1024];
		snprintf(message, sizeof(message), "line %d: %s", line, expression);
		throw std::runtime_error(message);
	};
};

#define CHECK(x) check((x), #x, __LINE__)

// Parse `arguments` (without the implicit program name, which is added here).
static bool parse(const std::vector<const char *> &argv, Application::Arguments &arguments) {
	std::vector<char *> cmdS;
	cmdS.push_back((char *)"dll-inject");
	for (const char *argument : argv) {
		cmdS.push_back((char *)argument);
	};
	cmdS.push_back(nullptr);
	return Application::parseArguments((int)cmdS.size() - 1, cmdS.data(), arguments);
};

static std::string commandLine(const std::vector<const char *> &argv) {
	std::vector<char *> cmdS;
	for (const char *argument : argv) {
		cmdS.push_back((char *)argument);
	};
	cmdS.push_back(nullptr);
	String retV = Application::buildCommandLine((int)cmdS.size() - 1, cmdS.data(), 0);
	return std::string(retV.value());
};

static void testInvalid() {
	Application::Arguments arguments;

	// No arguments.
	CHECK(!parse({}, arguments));

	// A flag, but no dll and no application.
	CHECK(!parse({"--wait"}, arguments));
	CHECK(!parse({"--hidden"}, arguments));
	CHECK(!parse({"--realtime"}, arguments));
	CHECK(!parse({"--hidden-realtime"}, arguments));
	CHECK(!parse({"--wait-hidden-realtime"}, arguments));

	// A dll, but no application.
	CHECK(!parse({"my.dll"}, arguments));
	CHECK(!parse({"--wait", "my.dll"}, arguments));

	// An unknown option must not be mistaken for a dll path.
	CHECK(!parse({"--unknown", "my.dll", "app.exe"}, arguments));
};

static void testInformational() {
	Application::Arguments arguments;

	CHECK(parse({"--license"}, arguments));
	CHECK(arguments.action == Application::Action::ShowLicense);

	CHECK(parse({"--version"}, arguments));
	CHECK(arguments.action == Application::Action::ShowVersion);

	CHECK(parse({"--help"}, arguments));
	CHECK(arguments.action == Application::Action::ShowUsage);

	CHECK(parse({"-h"}, arguments));
	CHECK(arguments.action == Application::Action::ShowUsage);

	CHECK(parse({"-?"}, arguments));
	CHECK(arguments.action == Application::Action::ShowUsage);
};

static void testFlags() {
	Application::Arguments arguments;

	// No flag: just dll and application.
	CHECK(parse({"my.dll", "app.exe"}, arguments));
	CHECK(arguments.action == Application::Action::Run);
	CHECK(!arguments.doWait);
	CHECK(!arguments.doHidden);
	CHECK(!arguments.doRealtime);
	CHECK(strcmp(arguments.dllToInject, "my.dll") == 0);
	CHECK(strcmp(arguments.applicationToExecute.value(), "\"app.exe\"") == 0);

	CHECK(parse({"--wait", "my.dll", "app.exe"}, arguments));
	CHECK(arguments.doWait && !arguments.doHidden && !arguments.doRealtime);

	CHECK(parse({"--hidden", "my.dll", "app.exe"}, arguments));
	CHECK(!arguments.doWait && arguments.doHidden && !arguments.doRealtime);

	CHECK(parse({"--realtime", "my.dll", "app.exe"}, arguments));
	CHECK(!arguments.doWait && !arguments.doHidden && arguments.doRealtime);

	CHECK(parse({"--hidden-realtime", "my.dll", "app.exe"}, arguments));
	CHECK(!arguments.doWait && arguments.doHidden && arguments.doRealtime);

	CHECK(parse({"--wait-hidden-realtime", "my.dll", "app.exe"}, arguments));
	CHECK(arguments.doWait && arguments.doHidden && arguments.doRealtime);
};

static void testCommandLine() {
	Application::Arguments arguments;

	// A single application token: quoted, no trailing space (regression).
	CHECK(commandLine({"app.exe"}) == "\"app.exe\"");

	// Several tokens: each quoted, separated by exactly one space.
	CHECK(commandLine({"app.exe", "-a", "-b"}) == "\"app.exe\" \"-a\" \"-b\"");

	// A path with spaces is kept as one quoted token.
	CHECK(commandLine({"C:\\Program Files\\app.exe", "one two"}) == "\"C:\\Program Files\\app.exe\" \"one two\"");

	// No tokens: empty command line.
	CHECK(commandLine({}) == "");

	// The application command line produced by parseArguments carries the
	// application and every following argument, and nothing from before it.
	CHECK(parse({"--wait", "my.dll", "app.exe", "--option", "value"}, arguments));
	CHECK(strcmp(arguments.applicationToExecute.value(), "\"app.exe\" \"--option\" \"value\"") == 0);
};

static void test() {
	Application::initMemory();

	testInvalid();
	testInformational();
	testFlags();
	testCommandLine();

	printf("Done.\r\n");
};

int main(int cmdN, char *cmdS[]) {

	try {
		test();
		return 0;
	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};

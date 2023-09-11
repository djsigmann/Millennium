#pragma once
#include <utils/cout/logger.hpp>

#include <chrono>
#include <string>
#include <iostream>

std::vector<std::string> output_log = {};
std::vector<std::string> socket_log = {};

std::string output_console::get_time()
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::stringstream ss;
	ss << std::put_time(&*std::localtime(&time), "%H:%M:%S");
	return std::format("[{}]", ss.str());
}

void output_console::log(std::string type, const std::string& message)
{
	std::lock_guard<std::mutex> lock(logMutex);

	output_log.push_back(std::format("{}{}{}", get_time(), type, message));

	fileStream << get_time() << type << message << std::endl;
	std::cout << get_time() << type << message << std::endl;
}

void output_console::log_socket(std::string val) {
	socket_log.push_back(std::format("{} {}", get_time(), val));
}

output_console::output_console() {
	fileStream.open("millennium.log", std::ios::trunc);
}
output_console::~output_console() {
	fileStream.close();
}

void output_console::log(std::string val) {
	log(" [info] ", val);
}

void output_console::log_hook(std::string val) {
	log(" [hook] ", val);
}
void output_console::err(std::string val) {
	log(" [fail] ", val);
}
void output_console::warn(std::string val) {
	log(" [warn] ", val);
}
void output_console::succ(std::string val) {
	log(" [okay] ", val);
}
void output_console::imp(std::string val) {
	log(" [+] ", val);
}

void output_console::log_patch(std::string type, std::string what_patched, std::string regex) 
{
	std::lock_guard<std::mutex> lock(logMutex);

	const auto message = std::format("[{}] match -> [windowName: '{}'][Regex: '{}']", type, what_patched, regex);

	output_log.push_back(message);
	fileStream << get_time() << message << std::endl;
	std::cout << get_time() << message << std::endl;
}
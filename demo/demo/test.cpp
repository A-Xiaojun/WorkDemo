#include <iostream>
#include <filesystem>
#include <functional>
#include"FSWatcher.hpp"

int main() {
	CFSEasyWatcher watcher;

	// 注册回调函数
	//std::string watch_id = "example_watch";
	FS::path file_path = "config.txt";
	EventCallback cb = []() {
		//这里放读取配置的逻辑
		std::cout << "文件被修改了！" << std::endl;
	};

	if (watcher.add(file_path, cb)) {
		std::cout << "监控注册成功！" << std::endl;
	}
	else {
		std::cout << "监控注册失败！" << std::endl;
	}

	// 模拟文件修改
	//std::this_thread::sleep_for(std::chrono::seconds(5));

	// 运行监控循环
	while (true) {
		watcher.loop_once();
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
//
//int main() {
//	FS::path file_path = "config.txt";
//	std::error_code ec;
//	auto absolute_path = FS::absolute(file_path, ec);
//	if (ec) {
//		std::cerr << "Error converting to absolute path: " << ec.message() << std::endl;
//		return 1;
//	}
//
//	auto canonical_path = FS::weakly_canonical(absolute_path, ec);
//	if (ec) {
//		std::cerr << "Error canonicalizing path: " << ec.message() << std::endl;
//		return 1;
//	}
//
//	std::cout << "原始路径: " << file_path << std::endl;
//	std::cout << "绝对路径: " << absolute_path << std::endl;
//	std::cout << "规范化路径: " << canonical_path << std::endl;
//}
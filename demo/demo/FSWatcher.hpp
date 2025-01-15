#pragma once
#include <iostream>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>
#include <chrono>

namespace FS = std::filesystem;

// 日志宏
//#define LOGB(level, msg, ...) std::cout << level << ": " << msg << " " << __VA_ARGS__ << std::endl

// 回调函数类型
using EventCallback = std::function<void()>;
using EventCallback2 = std::function<void(const int64_t& modifytime_millionseconds)>;


// 文件监控器类
class CFSEasyWatcher {
public:
	bool add(const FS::path& path, const EventCallback& cb, const std::string& watch_id = "");
	bool add2(const FS::path& path, const EventCallback2& cb, const std::string& watch_id = "");
	void loop_once();
	bool remove(const std::string& watch_id);
	void clear();

private:

	// 监控项结构体
	struct IDEventCB {
		EventCallback2 cb;
		std::string watch_id;
	};

	struct FileItem
	{
		int64_t last_write_time = 0;
		std::list<IDEventCB> cblist;
	};

	std::shared_mutex smtx_;
	std::map < FS::path, FileItem > file2cb_;
};
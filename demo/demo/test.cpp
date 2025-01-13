#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>

namespace fs = std::filesystem;

// ������
class Config {
public:
	Config(const std::string& filename) : filename_(filename) {
		loadConfig();
	}

	void loadConfig() {
		std::ifstream file(filename_);
		if (!file.is_open()) {
			std::cerr << "�޷��������ļ�: " << filename_ << std::endl;
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				config_[key] = value;
			}
		}
		file.close();
		last_modified_ = fs::last_write_time(filename_);
	}

	std::string get(const std::string& key) const {
		auto it = config_.find(key);
		if (it != config_.end()) {
			return it->second;
		}
		return "";
	}

	bool checkAndReload() {
		auto current_modified = fs::last_write_time(filename_);
		if (current_modified != last_modified_) {
			loadConfig();
			last_modified_ = current_modified;
			return true;
		}
		return false;
	}

private:
	std::string filename_;
	std::unordered_map<std::string, std::string> config_;
	fs::file_time_type last_modified_;
};

int main() {
	Config config("config.txt");

	while (true) {
		if (config.checkAndReload()) {
			std::cout << "�����ļ��Ѹ��£�" << std::endl;
			std::cout << "interval: " << config.get("interval") << std::endl;
			std::cout << "enabled: " << config.get("enabled") << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(5)); // ÿ5����һ��
	}

	return 0;
}
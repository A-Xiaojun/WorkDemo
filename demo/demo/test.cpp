#include <iostream>
#include <filesystem>
#include <functional>
#include"FSWatcher.hpp"

int main() {
	CFSEasyWatcher watcher;

	// ע��ص�����
	//std::string watch_id = "example_watch";
	FS::path file_path = "config.txt";
	EventCallback cb = []() {
		//����Ŷ�ȡ���õ��߼�
		std::cout << "�ļ����޸��ˣ�" << std::endl;
	};

	if (watcher.add(file_path, cb)) {
		std::cout << "���ע��ɹ���" << std::endl;
	}
	else {
		std::cout << "���ע��ʧ�ܣ�" << std::endl;
	}

	// ģ���ļ��޸�
	//std::this_thread::sleep_for(std::chrono::seconds(5));

	// ���м��ѭ��
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
//	std::cout << "ԭʼ·��: " << file_path << std::endl;
//	std::cout << "����·��: " << absolute_path << std::endl;
//	std::cout << "�淶��·��: " << canonical_path << std::endl;
//}
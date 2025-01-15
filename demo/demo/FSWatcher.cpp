#include"FSWatcher.hpp"
#include<shared_mutex>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <thread>
#include <chrono>

bool CFSEasyWatcher::add(const FS::path& path, const EventCallback& cb, const std::string& watch_id) {
	auto __cb = [cb](const int64_t&) { cb(); };
	return this->add2(path, __cb, watch_id);
}

bool CFSEasyWatcher::add2(const FS::path& path, const EventCallback2& cb, const std::string& watch_id) {
	std::unique_lock<std::shared_mutex> lck(smtx_);
	std::error_code ec;

	auto _path = FS::weakly_canonical(FS::absolute(path, ec), ec);
	if (!_path.empty()) {
		if (!watch_id.empty()) {
			auto iter = file2cb_.find(_path);
			if (iter != file2cb_.end()) {
				auto result = std::find_if(iter->second.cblist.begin(), iter->second.cblist.end(), [&](const IDEventCB& e) {
					return e.watch_id == watch_id;
				});

				if (result != iter->second.cblist.end())
					return false;
			}
		}

		IDEventCB evcb;
		evcb.cb = cb;
		evcb.watch_id = watch_id;
		file2cb_[_path].last_write_time = FS::last_write_time(_path, ec).time_since_epoch().count();
		file2cb_[_path].cblist.emplace_back(std::move(evcb));
		return true;
	}
	return false;
}


void CFSEasyWatcher::loop_once() {
	std::shared_lock<std::shared_mutex> lck(smtx_);
	std::list<std::pair<EventCallback2, int64_t>> cblist2;

	for (auto&[filep, item] : file2cb_) {
		std::error_code ec;
		auto file_mtime = FS::last_write_time(filep, ec).time_since_epoch().count();
		if (!ec) {
			if (file_mtime != item.last_write_time) {
				for (auto& subiter : item.cblist) {
					//LOGB("Info", "File Changed. >> last_write_time:%" PRId64 " >> %s", file_mtime, filep.string().c_str());
					cblist2.push_back(std::make_pair(subiter.cb, file_mtime));
				}
				item.last_write_time = file_mtime;
			}
		}
	}
	lck.unlock();

	for (auto& pr : cblist2) {
#if defined(_WIN32)
		pr.second -= int64_t(11644473600LL) * 1000 * 1000 * 10; // 100 ns
		pr.second /= 10'000;                                    // million
#endif
		pr.first(pr.second);
	}
}

bool CFSEasyWatcher::remove(const std::string& watch_id)
{
	if (watch_id.empty())
		return false;

	std::unique_lock<std::shared_mutex> lck(smtx_);

	for (auto iter = file2cb_.begin(); iter != file2cb_.end(); ++iter)
	{
		auto subiter = std::find_if(iter->second.cblist.begin(), iter->second.cblist.end(),
			[&](auto& e) { return e.watch_id == watch_id; });

		if (subiter != iter->second.cblist.end())
		{
			iter->second.cblist.erase(subiter);
			if (iter->second.cblist.empty())
				file2cb_.erase(iter);

			return true;
		}
	}
	return false;
}

void CFSEasyWatcher::clear()
{
	std::unique_lock<std::shared_mutex> lck(smtx_);
	file2cb_.clear();
}
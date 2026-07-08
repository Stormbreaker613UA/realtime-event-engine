// file_storage.cpp
#include "storage/istorage.hpp"  
#include "core/event.hpp"

#include <fstream>
#include <iomanip>
#include <mutex>

namespace storage {

class FileStorage : public IStorage {
public:
	explicit FileStorage(const std::string& path = "events.log") : path_(path) {}
	~FileStorage() override = default;

	bool save_event(const core::Event& ev) override
	{
		std::lock_guard<std::mutex> lk(m_);
		std::ofstream ofs(path_, std::ios::app | std::ios::binary);
		if (!ofs) return false;
		// write simple text line: id type size
		ofs << ev.id << ' ' << ev.type << ' ' << ev.payload.size() << '\n';
		return static_cast<bool>(ofs);
	}

private:
	std::string path_;
	std::mutex m_;
};

} // namespace storage

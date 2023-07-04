#include <vfs/providers/local_filesystem_provider.hpp>

#include <vfs/path.hpp>

#include <filesystem>
#include <fstream>

namespace vfs {
	local_filesystem_provider::local_filesystem_provider(const char *target_path)
	: m_target_path{target_path}
	{}

	provider_capabilites local_filesystem_provider::get_capabilities() {
		provider_capabilites caps;
		caps.supports_reads = true;
		caps.supports_writes = true;
		return caps;
	}

	bool local_filesystem_provider::file_exists(const path &path) const {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();
		std::error_code error;
		bool exists = std::filesystem::exists(fs_path, error);
		if (error) {
			return false;
		} else {
			return exists;
		}
	}

	bool local_filesystem_provider::is_file_writable(const path &path) const {
		return true;
	}

	bool local_filesystem_provider::is_file_readable(const path &path) const {
		return true;
	}

	std::size_t local_filesystem_provider::get_file_size(const path &path) const {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();
		std::error_code error;
		auto size = std::filesystem::file_size(fs_path, error);
		if (error) {
			return 0;
		} else {
			return size;
		}
	}

	void local_filesystem_provider::read_file_contents(const path &path, std::vector<std::byte> &data) const {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();

		std::error_code error;
		const auto size = std::filesystem::file_size(fs_path, error);
		if (error) {
			data.resize(0);
			return;
		}

		if (auto stream = std::ifstream(fs_path)) {
			data.resize(size);
			stream.read(reinterpret_cast<char*>(data.data()), size);
		} else {
			data.resize(0);
			return;
		}
	}

	void local_filesystem_provider::write_file_contents(const path &path, const std::vector<std::byte> &data) {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();

		if (auto stream = std::ofstream(fs_path)) {
			stream.write(reinterpret_cast<const char*>(data.data()), data.size());
			return;
		} else {
			return;
		}
	}
}

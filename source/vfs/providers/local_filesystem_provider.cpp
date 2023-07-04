#include <vfs/providers/local_filesystem_provider.hpp>

#include <vfs/path.hpp>

#include <filesystem>
#include <fstream>

namespace vfs {
	local_filesystem_provider::local_filesystem_provider(const char *target_path)
	: m_target_path{target_path}
	{}

	local_filesystem_provider::~local_filesystem_provider()
	{}

	provider_capabilites local_filesystem_provider::get_capabilities() {
		provider_capabilites caps;
		caps.supports_reads = true;
		caps.supports_writes = true;
		return caps;
	}

	file_exists_result local_filesystem_provider::file_exists(const path &path) const {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();
		std::error_code error;
		bool exists = std::filesystem::exists(fs_path, error);
		if (error) {
			return {false, file_exists_status::ERROR};
		} else {
			return {exists, file_exists_status::SUCCESS};
		}
	}

	is_file_writable_result local_filesystem_provider::is_file_writable(const path &) const {
		return {true, is_file_writable_status::SUCCESS};
	}

	is_file_readable_result local_filesystem_provider::is_file_readable(const path &) const {
		return {true, is_file_readable_status::SUCCESS};
	}

	get_file_size_result local_filesystem_provider::get_file_size(const path &path) const {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();
		std::error_code error;
		auto size = std::filesystem::file_size(fs_path, error);
		if (error) {
			return {0, get_file_size_status::OTHER_ERROR};
		} else {
			return {size, get_file_size_status::SUCCESS};
		}
	}

	read_file_contents_result local_filesystem_provider::read_file_contents(const path &path, std::vector<std::byte> &data) const {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();

		std::error_code error;
		const auto size = std::filesystem::file_size(fs_path, error);
		if (error) {
			data.resize(0);
			return {read_file_contents_status::FAILED_TO_GET_FILE_SIZE};
		}

		if (auto stream = std::ifstream(fs_path)) {
			data.resize(size);
			stream.read(reinterpret_cast<char*>(data.data()), size);
			return {read_file_contents_status::SUCCESS};
		} else {
			data.resize(0);
			return {read_file_contents_status::OTHER_ERROR};
		}
	}

	write_file_contents_result local_filesystem_provider::write_file_contents(const path &path, const std::vector<std::byte> &data) {
		std::filesystem::path fs_path = m_target_path;
		fs_path /= path.to_string();

		if (auto stream = std::ofstream(fs_path)) {
			stream.write(reinterpret_cast<const char*>(data.data()), data.size());
			return {write_file_contents_status::SUCCESS};
		} else {
			return {write_file_contents_status::ERROR};
		}
	}
}

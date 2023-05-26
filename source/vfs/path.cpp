#include <vfs/path.hpp>
#include <cstring>

namespace vfs {
	path::path() {
		m_data = nullptr;
	}
	path::~path() {
		if (m_data != nullptr) {
			delete[] m_data;
		}
	}

	path::path(const path& path) {
		set(path.m_data);
	}
	path::path(path&& path) {
		m_data = path.m_data;
		path.m_data = nullptr;
	}

	path& path::operator=(const path& path) {
		set(path.m_data);
		return *this;
	}
	path& path::operator=(path&& path) {
		m_data = path.m_data;
		path.m_data = nullptr;
		return *this;
	}

	void path::set(const char* string) {
		delete[] m_data;

		if (string != nullptr) {
			const auto len = std::strlen(string);
			m_data = new char[len+1];
			std::strncpy(m_data, string, len);
			m_data[len] = '\0';
		} else {
			m_data = nullptr;
		}
	}
	const char* path::to_string() const {
		return m_data;
	}

	bool path::is_empty() const {
		return m_data == nullptr || std::strlen(m_data) == 0;
	}
	bool path::is_directory() const {
		if (m_data == nullptr) {
			return false;
		}

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);
		if (len >= sep_len) {
			return std::strncmp(&m_data[len-sep_len], s_separator, sep_len) == 0;
		} else {
			return false;
		}
	}
	const char* path::get_basename() const {
		return nullptr;
	}
	const char* path::get_dirname() const {
		if (m_data == nullptr) {
			return nullptr;
		}

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);

		std::size_t last_sep = len;

		// find last separator
		for (char* sep = &m_data[len-1]; sep != m_data; --sep) {
			if (sep+sep_len <= m_data+len && std::strncmp(sep, s_separator, sep_len) == 0) {
				last_sep = sep-m_data;
				break;
			}
		}

		if (last_sep == len) {
			return nullptr;
		}

		std::size_t second_last_sep = len;

		// find second-to-last separator
		for (char* sep = &m_data[last_sep-1]; sep >= m_data; --sep) {
			if (sep+sep_len <= m_data+len && std::strncmp(sep, s_separator, sep_len) == 0) {
				second_last_sep = sep-m_data;
				break;
			}
		}

		if (second_last_sep == len) {
			return nullptr;
		}

		// allocate result str
		char* result = new char[last_sep-second_last_sep];
		std::strncpy(result, &m_data[second_last_sep+1], last_sep-second_last_sep-1);
		result[last_sep-second_last_sep-1] = '\0';

		return result;
	}
	const char* path::get_extension() const {
		if (m_data == nullptr) {
			return nullptr;
		}

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);
		for (char* dot = &m_data[len-1]; dot != m_data; --dot) {
			if (*dot == '.') {
				return dot+1;
			} else if (dot+sep_len <= m_data+len && std::strncmp(dot, s_separator, sep_len) == 0) {
				return nullptr;
			}
		}

		return nullptr;
	}

	void path::append_dir(const char* dirname) {
		const auto sep_len = std::strlen(s_separator);
		const auto dirname_len = std::strlen(dirname);
		const bool need_pre_separator = ! is_directory();
		bool need_post_deparator = false;

		const auto len = std::strlen(dirname);
		const auto sep_len = std::strlen(s_separator);
		if (len >= sep_len) {
			return std::strncmp(&m_data[len-sep_len], s_separator, sep_len) == 0;
		} else {
			return false;
		}
	}
	void path::append_filename(const char* filename) {
		(void)filename;
	}
}

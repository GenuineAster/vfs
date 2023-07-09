#include <vfs/path.hpp>
#include <cstring>
#include <utility>


namespace vfs {
	path::path() {
		m_data = nullptr;
	}

	path::path(const char *string) {
		m_data = nullptr;
		set(string);
	}

	path::~path() {
		if (m_data != nullptr) {
			delete[] m_data;
		}
	}

	path::path(const path &path) {
		set(path.m_data);
	}

	path::path(path &&path) {
		m_data = path.m_data;
		path.m_data = nullptr;
	}

	path &path::operator=(const path &path) {
		set(path.m_data);
		return *this;
	}

	path &path::operator=(path &&path) {
		m_data = path.m_data;
		path.m_data = nullptr;
		return *this;
	}

	void path::set(const char *string) {
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

	const char *path::to_string() const {
		return m_data;
	}

	std::size_t path::get_length() const {
		if (is_empty()) {
			return 0;
		} else {
			return std::strlen(m_data);
		}
	}

	bool path::is_empty() const {
		return m_data == nullptr || std::strlen(m_data) == 0;
	}

	bool path::s_is_root(const char *path) {
		if (path == nullptr) {
			return false;
		}

		const auto len = std::strlen(path);
		const auto sep_len = std::strlen(s_separator);
		if (len >= sep_len) {
			return std::strncmp(path, s_separator, sep_len) == 0;
		} else {
			return false;
		}
	}

	bool path::is_root() const {
		return s_is_root(m_data);
	}

	bool path::s_is_dir(const char *path) {
		if (path == nullptr) {
			return false;
		}

		const auto len = std::strlen(path);
		const auto sep_len = std::strlen(s_separator);
		if (len >= sep_len) {
			return std::strncmp(&path[len-sep_len], s_separator, sep_len) == 0;
		} else {
			return false;
		}
	}

	bool path::is_directory() const {
		return s_is_dir(m_data);
	}

	const char *path::get_basename() const {
		if (m_data == nullptr) {
			return nullptr;
		}

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);

		std::size_t last_sep = len;

		// find last separator
		for (char *sep = &m_data[len-1]; sep != m_data; --sep) {
			if (sep+sep_len <= m_data+len && std::strncmp(sep, s_separator, sep_len) == 0) {
				last_sep = sep-m_data;
				break;
			}
		}

		if (last_sep == len) {
			return nullptr;
		}

		// allocate result str
		char *result = new char[len-last_sep+1];
		std::strncpy(result, &m_data[last_sep+1], len-last_sep);
		result[len-last_sep] = '\0';

		return result;
	}

	const char *path::get_dirname() const {
		if (m_data == nullptr) {
			return nullptr;
		}

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);

		std::size_t last_sep = len;

		// find last separator
		for (char *sep = &m_data[len-1]; sep != m_data; --sep) {
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
		for (char *sep = &m_data[last_sep-1]; sep >= m_data; --sep) {
			if (sep+sep_len <= m_data+len && std::strncmp(sep, s_separator, sep_len) == 0) {
				second_last_sep = sep-m_data;
				break;
			}
		}

		if (second_last_sep == len) {
			return nullptr;
		}

		// allocate result str
		char *result = new char[last_sep-second_last_sep];
		std::strncpy(result, &m_data[second_last_sep+1], last_sep-second_last_sep-1);
		result[last_sep-second_last_sep-1] = '\0';

		return result;
	}

	const char *path::get_extension() const {
		if (m_data == nullptr) {
			return nullptr;
		}

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);
		for (char *dot = &m_data[len-1]; dot != m_data; --dot) {
			if (*dot == '.') {
				return dot+1;
			} else if (dot+sep_len <= m_data+len && std::strncmp(dot, s_separator, sep_len) == 0) {
				return nullptr;
			}
		}

		return nullptr;
	}

	void path::append_dir(const char *dirname) {
		const bool needs_sep_lhs = ! is_directory() && ! s_is_root(dirname);
		const bool trunc_sep_dirname = is_directory() && s_is_root(dirname);
		const bool needs_sep_rhs = ! s_is_dir(dirname);

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);
		const auto dir_len = std::strlen(dirname);

		auto new_len = len + dir_len;
		if (needs_sep_lhs) {
			new_len += sep_len;
		} else if (trunc_sep_dirname) {
			new_len -= sep_len;
		}
		if (needs_sep_rhs) {
			new_len += sep_len;
		}

		char *result = new char[new_len+1];
		char *out = result;

		std::strncpy(out, m_data, len);
		out += len;

		if (needs_sep_lhs) {
			std::strncpy(out, s_separator, sep_len);
			out += sep_len;
		}

		if (trunc_sep_dirname) {
			std::strncpy(out, dirname+sep_len, dir_len-sep_len);
			out += dir_len-sep_len;
		} else {
			std::strncpy(out, dirname, dir_len);
			out += dir_len;
		}

		if (needs_sep_rhs) {
			std::strncpy(out, s_separator, sep_len);
			out += sep_len;
		}

		out[0] = '\0';

		if (m_data != nullptr) {
			delete[] m_data;
		}

		m_data = result;
	}

	void path::append_filename(const char *filename) {
		const bool needs_sep_lhs = ! is_directory() && ! s_is_root(filename);

		const auto len = std::strlen(m_data);
		const auto sep_len = std::strlen(s_separator);
		const auto dir_len = std::strlen(filename);

		auto new_len = len;
		if (needs_sep_lhs) {
			new_len += sep_len;
		}

		char *result = new char[new_len+1];
		char *out = result;

		std::strncpy(out, m_data, len);
		out += len;

		if (needs_sep_lhs) {
			std::strncpy(out, s_separator, sep_len);
			out += sep_len;
		}

		std::strncpy(out, filename, dir_len);
		out += dir_len;
		out[0] = '\0';

		if (m_data != nullptr) {
			delete[] m_data;
		}

		m_data = result;
	}

	relative_result path::get_relative_to(const path &root) const {
		if (root.is_root() && ! this->is_empty()) {
			const std::size_t root_length = root.get_length();
			const std::size_t this_length = this->get_length();
			// the root must be shorter than the full path we want to make relative
			if (root_length <= this_length) {
				// the path we want to make relative must start with the same characters as the root
				if (std::strncmp(this->m_data, root.m_data, root_length) == 0) {
					// this is only valid if the root is a directory
					// we can verify this either by checking that the root ends with a separator
					// or by checking if the path we want to make relative starts with the root, immediately
					// followed by a separator.
					const auto sep_len = std::strlen(s_separator);
					if (root.is_directory()) {
						path relative;
						relative.set(m_data + root_length);
						return {true, root, std::move(relative)};
					} else if ((this_length >= root_length+sep_len && std::strncmp(this->m_data+root_length, s_separator, sep_len) == 0)) {
						path relative;
						relative.set(m_data + root_length + sep_len);
						return {true, root, std::move(relative)};
					} else {
						return {false, {}, {}};
					}
				} else {
					return {false, {}, {}};
				}
			} else {
				return {false, {}, {}};
			}
		} else {
			return {false, {}, {}};
		}
	}
}

#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/path_fwd.hpp>

#include <cstddef>

namespace vfs {
	struct relative_result;

	class VFS_EXPORT path final {
	public:
		static constexpr char s_separator[2] = "/";

		path();
		~path();

		path(const path &path);
		path(path &&path);

		path &operator=(const path &path);
		path &operator=(path&& path);

		void set(const char *string);
		const char *to_string() const;

		std::size_t get_length() const;

		bool is_empty() const;
		bool is_directory() const;
		bool is_root() const;
		const char *get_basename() const;
		const char *get_dirname() const;
		const char *get_extension() const;

		void append_dir(const char *dirname);
		void append_filename(const char *filename);

		relative_result get_relative_to(const path &other) const;

	private:
		static bool s_is_root(const char *path);
		static bool s_is_dir(const char *path);

		char *m_data = nullptr;
	};

	struct VFS_EXPORT relative_result {
		bool had_common_root;
		path root;
		path relative;
	};
}

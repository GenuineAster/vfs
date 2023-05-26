#include <vfs/vfs_export.hpp>

namespace vfs {
	class VFS_EXPORT path final {
	public:
		static constexpr char s_separator[2] = "/";

		path();
		~path();

		path(const path& path);
		path(path&& path);

		path& operator=(const path& path);
		path& operator=(path&& path);

		void set(const char* string);
		const char* to_string() const;

		bool is_empty() const;
		bool is_directory() const;
		const char* get_basename() const;
		const char* get_dirname() const;
		const char* get_extension() const;

		void append_dir(const char* dirname);
		void append_filename(const char* filename);

	private:
		char* m_data = nullptr;
	};
}

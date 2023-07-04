#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/virtual_file_system_fwd.hpp>

#include <vfs/path.hpp>
#include <vfs/provider_fwd.hpp>
#include <vfs/file_operation_results.hpp>

#include <cstddef>
#include <vector>

namespace vfs {
	using mount_handle = std::uint64_t;

	class VFS_EXPORT virtual_file_system {
	public:
		static constexpr mount_handle c_invalid_mount_handle = 0;

		virtual_file_system();
		~virtual_file_system();

		// mount operations
		mount_handle mount(provider &provider, const path &path);
		provider *get_mount_provider(mount_handle handle);
		const provider *get_mount_provider(mount_handle handle) const;
		path get_mount_location(mount_handle handle) const;
		mount_handle get_mount_handle_for_location(const path &path) const;
		void unmount(mount_handle handle);

		// file operations
		file_exists_result file_exists(const path &path) const;
		is_file_writable_result is_file_writable(const path &path) const;
		is_file_readable_result is_file_readable(const path &path) const;
		get_file_size_result get_file_size(const path &path) const;
		read_file_contents_result read_file_contents(const path &path, std::vector<std::byte> &data) const;
		write_file_contents_result write_file_contents(const path &path, const std::vector<std::byte> &data);

	private:
		struct mount_data {
			provider *provider = nullptr;
			path mount_point;
			mount_handle handle;
		};

		struct resolved_mount {
			const mount_data *mount = nullptr;
			path local_path;
		};

		resolved_mount resolve_mount(const path &path) const;

		std::vector<mount_data> m_mounts;
		mount_handle m_mount_handle_counter = 0;
	};
}

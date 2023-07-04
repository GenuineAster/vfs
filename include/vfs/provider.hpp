#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/provider_fwd.hpp>
#include <vfs/path_fwd.hpp>
#include <vfs/file_operation_results.hpp>

#include <cstddef>
#include <vector>

namespace vfs {
	struct VFS_EXPORT provider_capabilites {
		bool supports_writes;
		bool supports_reads;
	};

	class VFS_EXPORT provider {
	public:
		virtual ~provider() = default;

		virtual provider_capabilites get_capabilities() = 0;
		virtual file_exists_result file_exists(const path &path) const = 0;
		virtual is_file_writable_result is_file_writable(const path &path) const = 0;
		virtual is_file_readable_result is_file_readable(const path &path) const = 0;
		virtual get_file_size_result get_file_size(const path &path) const = 0;
		virtual read_file_contents_result read_file_contents(const path &path, std::vector<std::byte> &data) const = 0;
		virtual write_file_contents_result write_file_contents(const path &path, const std::vector<std::byte> &data) = 0;
	};
}

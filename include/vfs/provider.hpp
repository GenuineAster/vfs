#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/provider_fwd.hpp>
#include <vfs/path_fwd.hpp>

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
		virtual bool file_exists(const path &path) const = 0;
		virtual bool is_file_writable(const path &path) const = 0;
		virtual bool is_file_readable(const path &path) const = 0;
		virtual std::size_t get_file_size(const path &path) const = 0;
		virtual void read_file_contents(const path &path, std::vector<std::byte> &data) const = 0;
		virtual void write_file_contents(const path &path, const std::vector<std::byte> &data) = 0;
	};
}

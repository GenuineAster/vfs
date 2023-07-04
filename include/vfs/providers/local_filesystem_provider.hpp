#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/provider.hpp>

#include <string>

namespace vfs {
	class local_filesystem_provider final : public provider {
	public:
		local_filesystem_provider(const char *target_path);
		virtual ~local_filesystem_provider();

		virtual provider_capabilites get_capabilities() override;
		virtual file_exists_result file_exists(const path &path) const override;
		virtual is_file_writable_result is_file_writable(const path &path) const override;
		virtual is_file_readable_result is_file_readable(const path &path) const override;
		virtual get_file_size_result get_file_size(const path &path) const override;
		virtual read_file_contents_result read_file_contents(const path &path, std::vector<std::byte> &data) const override;
		virtual write_file_contents_result write_file_contents(const path &path, const std::vector<std::byte> &data) override;

	private:
		std::string m_target_path;
	};
}

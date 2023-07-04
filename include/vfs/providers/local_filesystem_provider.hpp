#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/provider.hpp>

#include <string>

namespace vfs {
	class local_filesystem_provider final : public provider {
	public:
		local_filesystem_provider(const char *target_path);

		virtual provider_capabilites get_capabilities() override;
		virtual bool file_exists(const path &path) const override;
		virtual bool is_file_writable(const path &path) const override;
		virtual bool is_file_readable(const path &path) const override;
		virtual std::size_t get_file_size(const path &path) const override;
		virtual void read_file_contents(const path &path, std::vector<std::byte> &data) const override;
		virtual void write_file_contents(const path &path, const std::vector<std::byte> &data) override;

	private:
		std::string m_target_path;
	};
}

#include <vfs/virtual_file_system.hpp>

#include <vfs/provider.hpp>

namespace vfs {
	virtual_file_system::virtual_file_system() {

	}
	virtual_file_system::~virtual_file_system() {

	}

	mount_handle virtual_file_system::mount(provider &provider, const path &path) {
		mount_data mnt;
		mnt.handle = ++m_mount_handle_counter;
		mnt.provider = &provider;
		mnt.mount_point = path;
		m_mounts.emplace_back(std::move(mnt));
		return m_mount_handle_counter;
	}

	provider *virtual_file_system::get_mount_provider(mount_handle handle) {
		auto itr = std::find_if(m_mounts.begin(), m_mounts.end(), [handle](const auto &val) {
			return val.handle == handle;
		});

		if (itr != m_mounts.end()) {
			return itr->provider;
		} else {
			return nullptr;
		}
	}

	const provider *virtual_file_system::get_mount_provider(mount_handle handle) const {
		auto itr = std::find_if(m_mounts.begin(), m_mounts.end(), [handle](const auto &val) {
			return val.handle == handle;
		});

		if (itr != m_mounts.end()) {
			return itr->provider;
		} else {
			return nullptr;
		}
	}

	path virtual_file_system::get_mount_location(mount_handle handle) const {
		auto itr = std::find_if(m_mounts.begin(), m_mounts.end(), [handle](const auto &val) {
			return val.handle == handle;
		});

		if (itr != m_mounts.end()) {
			return itr->mount_point;
		} else {
			// TODO: better error handling
			return path();
		}
	}

	mount_handle virtual_file_system::get_mount_handle_for_location(const path &path) const {
		if (auto resolved = resolve_mount(path); resolved.mount) {
			return resolved.mount->handle;
		} else {
			return c_invalid_mount_handle;
		}
	}

	void virtual_file_system::unmount(mount_handle handle) {
		auto itr = std::find_if(m_mounts.begin(), m_mounts.end(), [handle](const auto &val) {
			return val.handle == handle;
		});

		if (itr != m_mounts.end()) {
			m_mounts.erase(itr);
		}
	}

	file_exists_result virtual_file_system::file_exists(const path &path) const {
		if (auto resolved = resolve_mount(path); resolved.mount != nullptr) {
			return resolved.mount->provider->file_exists(resolved.local_path);
		} else {
			return {false, file_exists_status::NO_PROVIDER};
		}
	}

	is_file_writable_result virtual_file_system::is_file_writable(const path &path) const {
		if (auto resolved = resolve_mount(path); resolved.mount != nullptr) {
			return resolved.mount->provider->is_file_writable(resolved.local_path);
		} else {
			return {false, is_file_writable_status::NO_PROVIDER};
		}
	}

	is_file_readable_result virtual_file_system::is_file_readable(const path &path) const {
		if (auto resolved = resolve_mount(path); resolved.mount != nullptr) {
			return resolved.mount->provider->is_file_readable(resolved.local_path);
		} else {
			return {false, is_file_readable_status::NO_PROVIDER};
		}
	}

	get_file_size_result virtual_file_system::get_file_size(const path &path) const {
		if (auto resolved = resolve_mount(path); resolved.mount != nullptr) {
			return resolved.mount->provider->get_file_size(resolved.local_path);
		} else {
			return {0, get_file_size_status::NO_PROVIDER};
		}
	}

	read_file_contents_result virtual_file_system::read_file_contents(const path &path, std::vector<std::byte> &data) const {
		if (auto resolved = resolve_mount(path); resolved.mount != nullptr) {
			return resolved.mount->provider->read_file_contents(resolved.local_path, data);
		} else {
			return {read_file_contents_status::NO_PROVIDER};
		}
	}

	write_file_contents_result virtual_file_system::write_file_contents(const path &path, const std::vector<std::byte> &data) {
		if (auto resolved = resolve_mount(path); resolved.mount != nullptr) {
			return resolved.mount->provider->write_file_contents(resolved.local_path, data);
		} else {
			return {write_file_contents_status::NO_PROVIDER};
		}
	}

	virtual_file_system::resolved_mount virtual_file_system::resolve_mount(const path &path) const {
		relative_result best_fit_path;
		const mount_data *best_fit_mount = nullptr;
		for (auto &mount : m_mounts) {
			if (auto relative = path.get_relative_to(mount.mount_point); relative.had_common_root) {
				if (relative.root.get_length() > best_fit_path.root.get_length()) {
					best_fit_path = relative;
					best_fit_mount = &mount;
				}
			}
		}

		resolved_mount result;
		result.local_path = best_fit_path.relative;
		result.mount = best_fit_mount;
		return result;
	}
}

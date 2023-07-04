#pragma once

#include <vfs/vfs_export.hpp>

#include <cstddef>

namespace vfs {
	enum class file_exists_status {
		SUCCESS,
		NO_PROVIDER,
		ERROR
	};

	struct VFS_EXPORT file_exists_result {
		bool exists;
		file_exists_status status;
	};

	enum class is_file_writable_status {
		SUCCESS,
		NO_PROVIDER,
		ERROR
	};

	struct VFS_EXPORT is_file_writable_result {
		bool writable;
		is_file_writable_status status;
	};

	enum class is_file_readable_status {
		SUCCESS,
		NO_PROVIDER,
		ERROR
	};

	struct VFS_EXPORT is_file_readable_result {
		bool readable;
		is_file_readable_status status;
	};

	enum class get_file_size_status {
		SUCCESS,
		FILE_NOT_FOUND,
		NO_PROVIDER,
		OTHER_ERROR
	};

	struct VFS_EXPORT get_file_size_result {
		std::size_t file_size;
		get_file_size_status status;
	};

	enum class read_file_contents_status {
		SUCCESS,
		FAILED_TO_GET_FILE_SIZE,
		NO_PROVIDER,
		OTHER_ERROR
	};

	struct VFS_EXPORT read_file_contents_result {
		read_file_contents_status status;
	};

	enum class write_file_contents_status {
		SUCCESS,
		NO_PROVIDER,
		ERROR
	};

	struct VFS_EXPORT write_file_contents_result {
		write_file_contents_status status;
	};
}

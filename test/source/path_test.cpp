#include <vfs/path.hpp>

#include <catch2/catch.hpp>

#include <string>
#include <cstring>

TEST_CASE( "Directory name is correct", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo.ext");
	const char* dirname = path.get_dirname();
	REQUIRE( dirname != nullptr);
	std::string dirname_str = dirname;
	REQUIRE( dirname_str == "baz" );
}

TEST_CASE( "Directory name is correct consecutive separators", "[path]" ) {
	vfs::path path("/foobar/bar/baz//boo.ext");
	const char* dirname = path.get_dirname();
	REQUIRE( dirname != nullptr);
	std::string dirname_str = dirname;
	REQUIRE( dirname_str == "" );
}


TEST_CASE( "Basename is correct", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo.ext");
	const char* basename = path.get_basename();
	REQUIRE( basename != nullptr);
	std::string basename_str = basename;
	REQUIRE( basename_str == "boo.ext" );
}

TEST_CASE( "Basename is correct consecutive separators", "[path]" ) {
	vfs::path path("/foobar/bar/baz//boo.ext");
	const char* basename = path.get_basename();
	REQUIRE( basename != nullptr);
	std::string basename_str = basename;
	REQUIRE( basename_str == "boo.ext" );
}

TEST_CASE( "Ext is correct", "[path]" ) {
	vfs::path path("/foobar/bar/baz//boo.ext");
	const char* ext = path.get_extension();
	REQUIRE( ext != nullptr);
	std::string ext_str = ext;
	REQUIRE( ext_str == "ext" );
}

TEST_CASE( "Empty directory name", "[path]" ) {
	vfs::path path("//foo");
	const char* dirname = path.get_dirname();
	REQUIRE( dirname != nullptr);
	std::string dirname_str = dirname;
	REQUIRE( dirname_str == "" );

	path.set("//");
	dirname = path.get_dirname();
	REQUIRE( dirname != nullptr);
	dirname_str = dirname;
	REQUIRE( dirname_str == "" );

	path.set("/");
	dirname = path.get_dirname();
	REQUIRE( dirname == nullptr);
}

TEST_CASE( "Append dir to filename", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo.ext");
	path.append_dir("foobar");
	std::string ext_str = path.to_string();
	REQUIRE( ext_str == "/foobar/bar/baz/boo.ext/foobar/" );
}

TEST_CASE( "Append dir to dir", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo/");
	path.append_dir("foobar");
	std::string ext_str = path.to_string();
	REQUIRE( ext_str == "/foobar/bar/baz/boo/foobar/" );
}

TEST_CASE( "Append dir with trailing separator", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo");
	path.append_dir("foobar/");
	std::string ext_str = path.to_string();
	REQUIRE( ext_str == "/foobar/bar/baz/boo/foobar/" );
}

TEST_CASE( "Append dir with trailing separator to dir", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo/");
	path.append_dir("foobar/");
	std::string ext_str = path.to_string();
	REQUIRE( ext_str == "/foobar/bar/baz/boo/foobar/" );
}

TEST_CASE( "Append dir with double separators", "[path]" ) {
	vfs::path path("/foobar/bar/baz/boo/");
	path.append_dir("/foobar/");
	std::string ext_str = path.to_string();
	REQUIRE( ext_str == "/foobar/bar/baz/boo/foobar/" );
}

TEST_CASE( "Get relative dir", "[path]" ) {
	vfs::path root("/foobar/");

	vfs::path full("/foobar/barfoo");

	const auto rel = full.get_relative_to(root);
	REQUIRE(rel.had_common_root);
	REQUIRE(std::string{rel.root.to_string()} == std::string{root.to_string()});
	REQUIRE(std::string{rel.relative.to_string()} == "barfoo");
}

TEST_CASE( "Get relative dir no trailing separator", "[path]" ) {
	vfs::path root("/foobar");

	vfs::path full("/foobar/barfoo");

	const auto rel = full.get_relative_to(root);
	REQUIRE(rel.had_common_root);
	REQUIRE(std::string{rel.root.to_string()} == std::string{root.to_string()});
	REQUIRE(std::string{rel.relative.to_string()} == "barfoo");
}

TEST_CASE( "Get relative dir no common root", "[path]" ) {
	vfs::path root("/foobar");

	vfs::path full("/foobarbar/barfoo");

	const auto rel = full.get_relative_to(root);
	REQUIRE(rel.had_common_root == false);
}

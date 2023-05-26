#include <vfs/path.hpp>

#include <catch2/catch.hpp>

#include <string>
#include <cstring>

TEST_CASE( "Directory name is correct", "[path]" ) {
	vfs::path path;
	path.set("/foobar/bar/baz/boo.ext");
	const char* dirname = path.get_dirname();
	REQUIRE( dirname != nullptr);
	std::string dirname_str = dirname;
	REQUIRE( dirname_str == "baz" );
}

TEST_CASE( "Directory name is correct2", "[path]" ) {
	vfs::path path;
	path.set("/foobar/bar/baz//boo.ext");
	const char* dirname = path.get_dirname();
	REQUIRE( dirname != nullptr);
	std::string dirname_str = dirname;
	REQUIRE( dirname_str == "" );
}


TEST_CASE( "Empty directory name", "[path]" ) {
	vfs::path path;
	path.set("//foo");
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

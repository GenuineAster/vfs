# vfs

🚧 This project is very new! Perhaps it will never reach a state of maturity, it does not yet reach all of its goals, and is not guaranteed to work.

VFS is a Virtual File System, allowing users to abstract different data sources behind a single file system interface by mounting various providers to paths on the file system, much like how the file system works on Linux. It also allows the user to transparently add new data sources to their application without needing other systems in their code to know about it.


## Why use a virtual file system?
For example, in development you could be reading data files from a local source, as well as configuration. You might create a virtual file system with the following mounts:
```
/config/ -> local_file_system(~/.config/my_app)
/data/ -> local_file_system(./dev_data/)
```

But in production, perhaps your data comes from HTTP, or from within an archive file. Without having to change any paths or data access, you can just reconfigure your mounts to be:
```
/config/ -> local_file_system(~/.config/my_app)
/data/ -> http(https://foo.bar/app_data/)

or

/data/ -> zip_fs(./data.zip)
```

Or, on another operating system, perhaps your configuration is in a different place
```
/config/ -> local_file_system($APPDATA/../Local/my_app)
```

In these scenarios, all non-VFS-init code can just access "/config/foo" or "/data/bar/baz" and always get the data they expect.

One can also use VFS to transparently add new data sources, or override others, at run-time. For example, if the application user downloads a new extension for your program, you may want to override part of the data to the new data provided by the extension. So, for example:

```
/data/ -> local_file_system(./data/)
/data/ui -> local_file_system(./extension/ui_overhaul)
```

Or, perhaps the user wants to load their own projects into your software, then you can transparently add their data to the Virtual File System and load it like any other application data.

```
/data/ -> local_file_system(./data/)
/user/project -> local_file_system($user_data)
```

# Building and installing

See the [BUILDING](BUILDING.md) document.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

See the [LICENSE](LICENSE.md) document.

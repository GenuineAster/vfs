# VFS style guide

## `#include` patterns

* In a header, always have the export header included as the very first thing, in its own block.
* In a source file, always include the corresponding header as the very first thing, in its own block.
* In a test file, always include the thing you're testing as the very first thing, in its own block.
* Each set of includes from a different library dependency should go into its own block, with each block separated by a single empty line.
* Always put project-local local includes first.
* Always put STL includes last.
* Order blocks in between by "distance" from the current project. i.e internal libraries should come first, external libraries last.
* Never use a local path to include a file, always use the root include path.
* In library code, always include with `<>`.

### Examples:

`include/vfs/FooBar.hpp`:
```c++
#pragma once

#include <vfs/vfs_export.hpp>

#include <vfs/FileA.hpp>
#include <vfs/FileB.hpp>
#include <vfs/FileC.hpp>

#include <external/FileA.hpp>

#include <memory>
```

`source/vfs/FooBar.cpp`:
```c++
#include <vfs/FooBar.hpp>

#include <vfs/FileA.hpp>
#include <vfs/FileB.hpp>
#include <vfs/FileC.hpp>
```

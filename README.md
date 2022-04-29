Google Summer of Code 2022 Coding exercise
==========================================

## Goal: Implement a very word frequency counter

Your task is to provide the missing implementation for functions and structures
in wordstat.c.  The basic usage might look like this:

```
WordStat *ws = WordStatNew(" ", false);

if (!ws) {
    /* error */
}

WordStatAddText(ws, "My simple sentence for my GSoC");

char *word = WordStatGetMostFrequent(ws, &num_occur);

assert(word);
assert(strcmp(word, "my") == 0);

assert(WordStatGetTotal(ws) == 6);

free(word);
WordStatFree(ws);
```


## What's provided:

In this repository is a basic structure without the implementation and source
code for a binary with few tests included.  Build system used in this project is
[meson](https://mesonbuild.com).  Files that are included in this repository
are:

- **`meson.build`** -- configuration for the build
- **`meson_options.txt`** -- meson build option definitions
- **`README.md`** -- this file
- **`main.c`** -- binary using the WordStat implementation with various tests
- **`wordstat.h`** -- public header file for callers
- **`wordstat.c`** -- file prepared for your implementation

The only file you should need to touch is `wordstat.c`.  The rest is provided to
ease testing of your implementation.


## Compilation and testing:

In order to compile this project you first need to configure meson in a separate
directory, most commonly this is called `build`:

```
meson build
```

Now you can compile the project using `ninja` either directly from the directory
or with `-C path/to/directory`:

```
ninja -C build
```

Once the build is complete you will have a binary called `wordstat` in the build
directory which you can run to test your implementation:

```
build/wordstat
```

Or you can combine these steps by running:

```
ninja -C build test
```

although due to the size of the code and simplicity all tests show up as only
one test, but you will get the output in case the test suite fails.


## Test design

There are few tests in the `main.c` file which will run when you execute the
binary.  This is provided to you so that you can write your implementation using
test driven development if that's what you like.  By default the binary will
stop after first failed test allowing you to progress at your own pace.  If,
however, that is not what you prefer in this situation and you would rather the
tests to continue after non-critical failures, you can reconfigure your project
build with the option `run_all_tests` like so:

```
meson -Drun_all_tests=true --reconfigure build
```


## Implementation

The design of `WordStat` structure is completely up to you and is invisible to
callers (in our case the main binary with the tests).  You can introduce new
internal functions, macros or whatever you like, but all of it should be
contained inside `wordstat.c`.  You are allowed to use additional includes, but
should not add any build dependency.  The only non-standard library you are
allowed to use is [glib](https://wiki.gnome.org/Projects/GLib) but using it is
not required in the slightest.

The implementation and algorithms are completely up to you.  If you allocate any
memory, do not forget to free it once no longer needed.

### APIs

APIs are slightly documented in comments above them, but we might've of course
forgot something.  If you are unsure, then:

- try to read the comments again
- check the variable types thoroughly
- see how it is being used in the tests
- feel free to ask if you feel like anything is missing

There are no minus points for asking questions, communication is encouraged.

### Caveats

For simplicity's sake it is not defined which word is the most frequent and
should be returned in case there are two words that have the same number of
occurrences.  Hence the tests do not even test for this option.

Feel free to ask any questions.

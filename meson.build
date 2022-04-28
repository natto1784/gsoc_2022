project('wordstat', 'c',
  default_options: [
    'c_std=gnu11',
    'warning_level=3',
    'werror=true',
  ],
)

source = [
  'main.c',
  'wordstat.c',
]

header = [
  'wordstat.h',
]

deps = [
  dependency('glib-2.0'),
]

cc_flags = [
  '-Wall',
  '-O0',
  '-D_GNU_SOURCE',
]

if get_option('run_all_tests')
    cc_flags += [ '-DRUN_ALL_TESTS' ]
endif

e = executable('wordstat',
  source + header,
  dependencies: deps,
  c_args: cc_flags,
)

test('Main test suite', e)

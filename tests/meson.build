tests_deps = [
    penrose_dep,
    dependency('fmt'),
    dependency('catch2-with-main')
]

tests_src = [
    'TestsMain.cpp',

    # TESTS
    'EngineTests.cpp',
]

tests_exe = executable('penrose-tests', tests_src, dependencies: tests_deps, include_directories: incdir)

test('All Tests', tests_exe)

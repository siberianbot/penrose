tests_deps = [
    penrose_dep,
    dependency('catch2-with-main')
]

tests_src = [
    'TestsMain.cpp',

    # TESTS
    'ComplexTests.cpp',
    'EngineTests.cpp',
]

tests_exe = executable(
    'penrose-tests',
    tests_src,
    dependencies: tests_deps,
    include_directories: incdir
)

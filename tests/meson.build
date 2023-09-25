tests_deps = [
    penrose_dep,
    dependency('catch2-with-main')
]

tests_src = [
    'src/Main.cpp',

    # ECS
    'src/ECS/TestCountdownSystem.cpp',
    'src/ECS/TestSurfaceResizeSystem.cpp',

    # Unit Tests
    'src/UnitTests/Rendering/PipelineInfo.cpp',

    # Integration Tests
    'src/IntTests/ComplexScenes/OrbitalCamera.cpp',
    'src/IntTests/ComplexScenes/RotatingCamera.cpp',
    'src/IntTests/Engine/EngineStartStop.cpp',
    'src/IntTests/Engine/EngineStartWaitStop.cpp',
    'src/IntTests/Rendering/SwapchainResize.cpp'
]

tests_exe = executable(
    'penrose-tests',
    tests_src,
    dependencies: tests_deps,
    include_directories: incdir
)

tests_workdir = meson.project_source_root()

test('All unit tests', tests_exe, args: ['[engine-unit-test]'], workdir: tests_workdir)
test('All integration tests', tests_exe, args: ['[engine-int-test]'], workdir: tests_workdir)

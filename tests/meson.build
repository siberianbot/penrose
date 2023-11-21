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
    'src/UnitTests/Math/NumericFuncs.cpp',
    'src/UnitTests/Rendering/PipelineInfo.cpp',

    # Integration Tests
    'src/IntegrationTests/ComplexScenes/LottaObjects.cpp',
    'src/IntegrationTests/ComplexScenes/OrbitalCamera.cpp',
    'src/IntegrationTests/ComplexScenes/Projection.cpp',
    'src/IntegrationTests/ComplexScenes/RotatingCamera.cpp',
    'src/IntegrationTests/Engine/EngineStartStop.cpp',
    'src/IntegrationTests/Engine/EngineStartWaitStop.cpp',
    'src/IntegrationTests/Rendering/SwapchainResize.cpp',

    # Interactive Tests
    'src/InteractiveTests/Input/ImGuiInputHook.cpp',
    'src/InteractiveTests/Input/InputReceiving.cpp',
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

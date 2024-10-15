project(
  'OpenGL Tuto',
  'c',
  version: '0.1.0',
  default_options: ['c_std=c17'],
)

project_sources = []

project_dependencies = [
  dependency('glfw3'),
]

subdir('src')

inc = include_directories('include')
#lib = static_library('cglm')

executable(
  'open-tuto',
  project_sources,
  include_directories: inc,
  dependencies: project_dependencies,
  #link_with: lib,
)

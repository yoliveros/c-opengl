project(
  'OpenGL Tuto',
  'c',
  version: '0.1.0',
  default_options: ['c_std=c17'],
)

project_sources = []

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: false)

project_dependencies = [
  dependency('glfw3'),
  m_dep
]


subdir('src')
subdir('shaders')
subdir('textures')

inc = include_directories('include')

executable(
  'open-tuto',
  project_sources,
  include_directories: inc,
  dependencies: project_dependencies,
)

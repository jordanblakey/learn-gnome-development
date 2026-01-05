libatwaita_basics_sources = [
  'main.c',
  'libatwaita-basics-application.c',
  'libatwaita-basics-window.c',
]

libatwaita_basics_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1', version: '>= 1.4'),
]

libatwaita_basics_sources += gnome.compile_resources('libatwaita-basics-resources',
  'libatwaita-basics.gresource.xml',
  c_name: 'libatwaita_basics'
)

executable('libatwaita-basics', libatwaita_basics_sources,
  dependencies: libatwaita_basics_deps,
       install: true,
)

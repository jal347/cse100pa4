inc = include_directories('.')

actor_graph = library('actor_graph', sources: ['ActorGraph.cpp'])
actor_graph_dep = declare_dependency(include_directories: inc,
  link_with: actor_graph)

pathfinder_exe = executable('pathfinder.cpp.executable',
    sources: ['pathfinder.cpp'],
    dependencies : [actor_graph_dep],
    install : true)

linkpredictor_exe = executable('linkpredictor.cpp.executable',
    sources: ['linkpredictor.cpp'],
    dependencies : [actor_graph_dep],
    install : true)

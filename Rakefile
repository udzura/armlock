require "bundler/gem_tasks"
require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("armlock") do |ext|
  ext.lib_dir = "lib/armlock"
end

task :default => [:clobber, :compile, :spec]

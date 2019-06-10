# -*- ruby -*-

require 'rubygems'

tries = 0
begin
  require 'hoe'
  require 'minitest'
  require 'rake/extensiontask'
rescue LoadError
  tries += 1
  Gem.install 'hoe'
  Gem.install 'rake-compiler'
  Gem.install 'hoe-gemspec'
  Gem.install 'hoe-git'
  Gem.install 'minitest'
  raise unless tries < 10
  retry
end

Hoe.plugin :minitest
Hoe.plugin :gemspec # `gem install hoe-gemspec`
Hoe.plugin :git     # `gem install hoe-git`

HOE = Hoe.spec 'myhidapi' do
  developer('Aaron Patterson', 'tenderlove@ruby-lang.org')
  license "MIT"
  self.readme_file   = 'README.md'
  self.history_file  = 'CHANGELOG.md'
  self.extra_rdoc_files  = FileList['*.md']

  self.spec_extras = {
    :extensions => ["ext/myhidapi/extconf.rb"],
    :required_ruby_version => '>= 2.3.0'
  }
end

Rake::ExtensionTask.new("myhidapi", HOE.spec) do |ext|
end

# vim: syntax=ruby

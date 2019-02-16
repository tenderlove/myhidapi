# -*- ruby -*-

require 'rubygems'
require 'hoe'

Hoe.plugin :minitest
Hoe.plugin :gemspec # `gem install hoe-gemspec`
Hoe.plugin :git     # `gem install hoe-git`

HOE = Hoe.spec 'gel' do
  developer('Aaron Patterson', 'tenderlove@ruby-lang.org')
  license "MIT"
  self.readme_file   = 'README.md'
  self.history_file  = 'CHANGELOG.md'
  self.extra_rdoc_files  = FileList['*.md']
end

require 'rake/extensiontask'
Rake::ExtensionTask.new("gel", HOE.spec) do |ext|
end

# vim: syntax=ruby

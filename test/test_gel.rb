require 'minitest/autorun'
require 'gel'

class TestGel < MiniTest::Test
  def test_it_works
    enum = Gel::Enumerate.new 0x0, 0x0
  end
end

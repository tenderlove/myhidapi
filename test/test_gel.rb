require 'minitest/autorun'
require 'gel'

class TestGel < MiniTest::Test
  def test_it_works
    devices = Gel.enumerate 0x0, 0x0
    p devices.find { |dev| dev.product_string == "ErgoDox EZ" }
  end
end

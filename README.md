# MyHIDAPI

* https://github.com/tenderlove/myhidapi

## DESCRIPTION:

This is a small wrapper around HIDAPI.  I couldn't get other HIDAPI wrappers to
work, so I wrote this one.  I'm using it to communicate with my keyboard, so
it really only supports enough of the HIDAPI to do that.

## FEATURES/PROBLEMS:

* Incomplete
* No tests
* Seems to work (for me)

## SYNOPSIS:

```ruby
 devices = MyHIDAPI.enumerate 0x0, 0x0
 dev = devices.find { |dev| dev.product_string == "ErgoDox EZ" }

 handle = dev.open

 while !handle
   p "retry"
   handle = dev.open
 end

 p handle

 buf = [0x0, 0x3, 35]
 loop do
   break if handle.write buf.pack('C*')
 end
 puts "done writing"

 buf = handle.read_timeout 1, 500
 p buf
 ```

## REQUIREMENTS:

This depends on libhidapi.

## INSTALL:

* Install hidapi.  On MacOS that is `brew install hidapi`

## LICENSE:

(The MIT License)

Copyright (c) 2019 Aaron Patterson

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

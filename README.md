# DFPlayMini

This is an attempt at writing a somewhat-useable library for the DFPlayer Mini MP3-player (https://dfrobot.com/index.php?route=product/product&product_id=1121)

This code is very much a work-in-progress at the moment. **Also, the documentation for this module is inaccurate!** The functionality described in the official documentation either doesn't work or doesn't work as described, so a lot of this stuff is based on experimentation and very likely does not work as expected.

The Chinese documentation seems to indicate that they have a firmware-version 0x10, whereas my module responds with a firmware-version of 0x05, so that may part of the issue.

That said, the code is more-or-less useable for basic playback of the supported files, it just may or may not be reliable.
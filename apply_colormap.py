#!/usr/bin/env python

from __future__ import division

import numpy as np
import piio
import sys
import os.path
from matplotlib import cm

if __name__ == '__main__':
    if '-h' in sys.argv:
        print "Usage: {} input=- output=- map=magma min=input.min() max=input.max()".format(os.path.basename(sys.argv[0]))
        sys.exit(0)

    in_image = piio.read(sys.argv[1] if len(sys.argv) > 1 else '-')[:,:,0]
    vmin = float(sys.argv[4]) if len(sys.argv) > 4 else in_image.min()
    vmax = float(sys.argv[5]) if len(sys.argv) > 5 else in_image.max()
    in_image = (in_image - vmin) / (vmax - vmin)
    cmap = cm.get_cmap(sys.argv[3] if len(sys.argv) > 3 else 'magma')
    out_image = cmap(in_image)[:,:,:3]

    piio.write(sys.argv[2] if len(sys.argv) > 2 else 'TIFF:-', out_image)

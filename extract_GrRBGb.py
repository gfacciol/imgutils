#!/usr/bin/env python

from __future__ import division

import numpy as np

pattern = [np.array([[3, 2],
                     [1, 0]]),
           np.array([[2, 3],
                     [0, 1]]),
           np.array([[1, 0],
                     [3, 2]]),
           np.array([[0, 1],
                     [2, 3]])]

def extract_GrRBGb(in_image, phase):
    if len(in_image.shape) != 2:
        assert(len(in_image.shape)) == 3
        assert(in_image.shape[2] == 1)
        in_image = in_image[:, :, 0]

    w, h = in_image.shape

    out_image = np.zeros((w // 2, h // 2, 4), dtype = in_image.dtype)
    for i in xrange(2):
        for j in xrange(2):
            chan = pattern[phase][i, j]
            out_image[:, :, chan] = in_image[i::2, j::2]
    return out_image

if __name__ == '__main__':
    import piio
    import sys
    import os.path
    if '-h' in sys.argv:
        print "Usage: {} [phase [input [output]]]".format(os.path.basename(sys.argv[0]))
        sys.exit(1)

    phase = int(sys.argv[1]) if len(sys.argv) > 1 else 2

    in_image = piio.read(sys.argv[2] if len(sys.argv) > 2 else '-')
    out_image = extract_GrRBGb(in_image, phase)

    piio.write(sys.argv[3] if len(sys.argv) > 3 else '-', out_image)

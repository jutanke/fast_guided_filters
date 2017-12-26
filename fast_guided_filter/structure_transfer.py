import numpy as np
import cv2

def boxfilter(I, r):
    """ unnormalized boxfilter
        I: input image
        r: size of extension into all directions
    """
    r = r * 2 + 1
    imDst = cv2.boxfilter(I, -1, (r, r), (-1, -1), False, cv2.BORDER_CONSTANT)
    return imDst

def structureTransference1C(I, p, r, eps, s):
    """ transfers the structure of one image onto the other
        I guidance image
        p filtering input image
        r local window radius
        eps regularization parameter
        s subsampling ratio
    """
    q = None
    I = I.astype('float32') / 255

    cols, rows = I.shape
    I_sub = cv2.resize(I, (int(cols/s), int(rows/s)))


    return q;

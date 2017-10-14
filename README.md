# Fast Guided Filters

Headers-only OpenCV3 implementation of the ![Fast Guided Filter](https://arxiv.org/pdf/1505.00996.pdf)

![smila_animation](https://user-images.githubusercontent.com/831215/31575901-bb16c344-b0f0-11e7-9c08-d73e99ee0379.gif)

The animation starts at the original image and then increases the subsampling size by 4 every round: Subsampling sizes: [1, 4, 8, 12, 16, 20]

## Samples

Bluring an image with radius 4, epsilon 0.2^2 and sumsampling-size 4:

<div>
<img width='300px' style='float=left' src='https://user-images.githubusercontent.com/831215/31560087-472a3b34-b053-11e7-931d-a18b2dc5a4ee.png'/>
<img width='300px' src='https://user-images.githubusercontent.com/831215/31560078-43150e84-b053-11e7-9abb-8535fec5ed7d.png'>
</div>

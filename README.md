# pang
Generates square png files in one of 4 channel modes.

## Testing
`$ pangs input.foo` will generate all modes to current dir. You can change it with `$ pangs input.foo /some/output/path`.

Clean things up with: `$ pangs clean` or `$ pangs clean /some/output/path`

## Example
Files made out of mp3:
* grayscale `-g`
<img src="example/g.png" width="50%" height="50%"/>

* grayscale + alpha `-ga`
<img src="example/ga.png" width="50%" height="50%"/>

* rgb (default)
<img src="example/rgb.png" width="50%" height="50%"/>

* rgb + alpha `-a`
<img src="example/rgba.png" width="50%" height="50%"/>

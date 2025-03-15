# Hap Codec for Adobe CC on Apple Silicon

This is fork of https://github.com/disguise-one/hap-encoder-adobe-cc

Main focus of this repo: working plugins build for Apple Silicon platform.  
Installer can be downloaded [here](https://github.com/exscriber/Hap-Adobe-CC-Apple/releases).

## What is HAP

HAP is a collection of high-performance codecs optimised for playback of multiple layers of video.

HAP prioritises decode-speed, efficient upload to GPUs and GPU-side decoding to enable the highest amount of video content to be played back at once on modern hardware.

Please see
[http://hap.video](http://hap.video)
for details.

Exporter plugins are provided for
- Adobe Media Encoder
- Adobe Premiere Pro
- Adobe After Effects

Please see [LICENSE](LICENSE) for the licenses of this plugin and the components used to create it.

## Compatibility

macOS 11+  
This codec has been tested on macOS 15 Sequoia with Adobe CC 2024.

## Usage

### Adobe Media Encoder and Adobe Premiere Pro

After installation, the encoders will be available as the 'HAP Video' format when exporting in Adobe Media Encoder or Adobe Premiere

![HAP format](doc/user_guide/format-option.png)

After choosing the format, codec options may be chosen.

![HAP codec options](doc/user_guide/codec-options.png)

<!-- Default presets are supplied and are available in Adobe Media Encoder.

![HAP presets](doc/user_guide/media-encoder-presets.png) -->

Movies that are encoded with the plugin are exported into .mov files.

### Adobe After Effects

The HAP codecs may be selected by choosing 'Quicktime HAP Format' on an output module.

### Choosing the right codec for the job: Hap, Hap Alpha, Hap Q

There are four different flavors of HAP to choose from when encoding your clips.

 codec       | properties
 ----------- | --------------------------------------------------------------------------------
 Hap         | lowest data-rate and reasonable image quality                                    
 Hap Alpha   | same image quality as Hap, and supports an Alpha channel                         
 Hap Q       | improved image quality, at the expense of larger file sizes                      

### Codec parameters
For Hap and Hap Alpha codecs render time can be reduced with Quality-Fast option. It uses fast and simple algorithm, but with reduced image quality.

- Fast suggested for draft renders, last-minute notebook renders, etc...
- Normal is default option for general renders

![Hap quality option](doc/user_guide/codec-quality.png)

An optional specified number of chunks size may be specified to optimize for ultra high resolution video on a particular hardware system. This setting should typically only be used if you are reaching a CPU performance bottleneck during playback. As a general guide, for HD footage or smaller you can set the chunk size to 1 and for 4k or larger footage the number of chunks should never exceed the number of CPU cores on the computer used for playback.

![HAP chunk counts](doc/user_guide/chunk-counts.png)

At present, 'auto' corresponds to choosing 1 chunk per texture; this may change in the future.

## Development

Please see the instructions for the Codec Foundation upon which these plugins are based:
[README-DEV.md](README-DEV.md)

## Credits

Principal contributors to this plugin are

-  Greg Bakker (gbakker@gmail.com)
-  Richard Sykes
-  [Tom Butterworth](http://kriss.cx/tom)
-  [Nick Zinovenko](https://github.com/exscriber)

Development of this plugin was sponsored by
 - [disguise](http://disguise.one), makers of the disguise show production software and hardware.
 - [10bitFX](http://notch.one), creators of the Notch VFX software

The Hap codec was developed by Tom Butterworth with the support of [VIDVOX](https://vidvox.net).

Many thanks to Tom Butterworth, David Lublin, Nick Wilkinson, Ruben Garcia and the disguise QA team for their assistance throughout development of this plugin.

node-oculus is a simple node.js addon for interfacing with the Oculus Rift. 

Goals: 
	Create a simple node interface to the Oculus Rift.
	Provide a clean start point for complex node based oculus projects to evolve from.

Based on:
	[Minimal Oculus Application](https://developer.oculusvr.com/wiki/index.php?title=Minimal_Oculus_Application&status=1)
	-
	[node-hmd](https://github.com/Geocent/node-hmd)



Dependencies:
	node-gyp if not already availible in your distribution.

Installation:
	1. Pull the repository.
	2. cd to repository directory.
	4. node-gyp rebuild


Usage:
```javascript
// 1. Create oculus object:
var oculus = nodeOculus.createDeviceVR();

// 2. Discover sensor:
oculus.discoverSensor();

// 3. Get device info:
var deviceInfo = oculus.getDeviceInfo();

// 4. Poll orientation data:
var quat = oculus.getOrientationQuat();

// 5. Feel responsibility to make something cool:
...

``` 

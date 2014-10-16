var nodeOculus = require('../build/Release/nodeOculus');

var oculus = nodeOculus.createOculus();

if (oculus.discoverSensor()) {
  console.log(oculus.getDeviceInfo());

  console.log('about to log some orientations');

  // logOrientation();
  logTrackingData();
}

function logOrientation() {
	var quat = new Float32Array(4);
	console.log(oculus.getOrientationQuat(quat));
	setTimeout(logOrientation, 500);
}

function logTrackingData() {
	console.log(oculus.getTrackingData());
	setTimeout(logTrackingData, 500);
}

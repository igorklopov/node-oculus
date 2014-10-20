var nodeOculus = require('../build/Release/nodeOculus');

var oculus = nodeOculus.createOculus();

if (oculus.discoverSensor()) {
  // logOrientation();
  // logTrackingData();
  printDeviceInfo(oculus.getDeviceInfo());
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

function printDeviceInfo(deviceInfo) {
	console.log('---- DEVICE INFO ----');
	logProp('displayDeviceName');
	logProp('displayId');
	logProp('lensSeperationDistance');
	logProp('eyeToScreenDistance');
	logProp('screenCenter');
	logProp('interpupillaryDistance');
	logProp('screenSize');
	logProp('resolution');
	logProp('distortion');

	function logProp(prop) {
		if (Object.prototype.toString.call(deviceInfo[prop]) === '[object Array]') {
			for (var i=0; i<deviceInfo[prop].length; i++) {
				console.log(prop + '[' + i + ']: ' + deviceInfo[prop][i]);
			}
		} else {
			console.log(prop + ': ' + deviceInfo[prop]);
		}
	}
}
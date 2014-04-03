var nodeOculus = require('../build/Release/nodeOculus');

var oculus = nodeOculus.createOculus();

if (oculus.discoverSensor()) {
  console.log(oculus.getDeviceInfo());

  var quat = new Float32Array(4);
  for (var i=0; i<10; ++i) {
    console.log(oculus.getOrientationQuat(quat));
  }
}

oculus.destroyResources();

console.log("Test complete.\n");

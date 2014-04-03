var nodeOculus = require('../build/Release/nodeOculus');

var oculus = nodeOculus.createOculus();

if (oculus.discoverSensor()) {
  console.log(oculus.getDeviceInfo());

  for (var i=0; i<10; ++i) {
    console.log(oculus.getOrientationQuat());
  }
}

oculus.destroyResources();

console.log("Test complete.\n");

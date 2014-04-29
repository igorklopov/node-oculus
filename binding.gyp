{
  "targets": [
    {
      "target_name": "nodeOculus",

      "include_dirs": [
        "src/ovr",

        "src/ovr/LibOVR/Src",
        "src/ovr/LibOVR/Include"
      ],

	  "type": "static_library",
	  
      "sources" : [
		"src/ovr/LibOVR/Src/Kernel/OVR_Timer.cpp",
      	"src/ovr/3rdParty/TinyXml/tinyxml2.cpp",
		"src/ovr/LibOVR/Src/OVR_Device.h",
        "src/ovr/LibOVR/Src/OVR_DeviceHandle.cpp",
        "src/ovr/LibOVR/Src/OVR_DeviceImpl.cpp",
        "src/ovr/LibOVR/Src/OVR_JSON.cpp",
        "src/ovr/LibOVR/Src/OVR_LatencyTestImpl.cpp",
        "src/ovr/LibOVR/Src/OVR_Profile.cpp",
        "src/ovr/LibOVR/Src/OVR_SensorFilter.cpp",
        "src/ovr/LibOVR/Src/OVR_SensorFusion.cpp",
        "src/ovr/LibOVR/Src/OVR_SensorImpl.cpp",
        "src/ovr/LibOVR/Src/OVR_ThreadCommandQueue.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_Alg.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_Allocator.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_Atomic.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_File.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_FileFILE.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_Log.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_Math.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_RefCount.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_Std.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_String.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_String_FormatUtil.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_String_PathUtil.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_SysFile.cpp",
        "src/ovr/LibOVR/Src/Kernel/OVR_System.cpp",
        
        "src/ovr/LibOVR/Src/Kernel/OVR_UTF8Util.cpp",
        "src/ovr/LibOVR/Src/Util/Util_LatencyTest.cpp",
        "src/ovr/LibOVR/Src/Util/Util_Render_Stereo.cpp",
		"nodeOculus.cc",
        "Device.cc"
      ],

      "conditions" : [
        ['OS=="linux"', {
            "sources": [
              "src/ovr/LibOVR/Src/OVR_Linux_HIDDevice.cpp",
              "src/ovr/LibOVR/Src/OVR_Linux_SensorDevice.cpp",
              "src/ovr/LibOVR/Src/OVR_Linux_DeviceManager.cpp",
              "src/ovr/LibOVR/Src/OVR_Linux_HMDDevice.cpp",
              "src/ovr/LibOVR/Src/Kernel/OVR_ThreadsPthread.cpp"
            ]
        }],
        ['OS=="mac"', {
            "sources": [
              "src/ovr/LibOVR/Src/OVR_OSX_HIDDevice.cpp",
              "src/ovr/LibOVR/Src/OVR_OSX_SensorDevice.cpp",
              "src/ovr/LibOVR/Src/OVR_OSX_DeviceManager.cpp",
              "src/ovr/LibOVR/Src/OVR_OSX_HMDDevice.cpp",
              "src/ovr/LibOVR/Src/Kernel/OVR_ThreadsPthread.cpp"
            ],
            "link_settings": {
              "libraries": [
                "ApplicationServices.framework",
                "CoreGraphics.framework",
                "IOKit.framework"
              ],
            },
        }],
        ['OS=="win"', {
          "sources": [
          ]
        }],
      ]
    }
  ]
}

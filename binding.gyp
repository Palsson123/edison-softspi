{
  "targets": [
    {
      "target_name": "EdisonSoftSpi",
      "sources": [ "EdisonSoftSpi.cpp", "softspi.cpp" ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ],
}

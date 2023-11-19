{
  "targets": [
    {
        "target_name": "ptr",
        "sources": [ "ptr.cpp" ], 
        "include_dirs" : ["<!(node -e \"require('nan')\")"]
    }
  ]
}

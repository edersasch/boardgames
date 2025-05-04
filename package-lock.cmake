CPMDeclarePackage(googletest
  GITHUB_REPOSITORY   google/googletest
  VERSION             1.16.0
  OPTIONS             "INSTALL_GTEST OFF"
)
CPMDeclarePackage(nlohmann_json
  NAME                nlohmann_json
  GITHUB_REPOSITORY   nlohmann/json
  VERSION             3.4.0
  DOWNLOAD_ONLY       true
)
CPMDeclarePackage(martinus_robin-hood-hashing
  NAME                martinus_robin-hood-hashing
  GITHUB_REPOSITORY   martinus/robin-hood-hashing
  GIT_TAG             3.11.5
  DOWNLOAD_ONLY       true
)
CPMDeclarePackage(boost-ext_sml
  NAME                boost-ext_sml
  GITHUB_REPOSITORY   boost-ext/sml
  VERSION             1.1.4
  DOWNLOAD_ONLY       true
)

meta:
	ADDON_NAME = ofxCeresSolver
	ADDON_DESCRIPTION = An openFrameworks wrapper for Eigen.
	ADDON_AUTHOR = Yuya Hanai
	ADDON_URL = https://github.com/hanasaan/ofxCeresSolver

common:
	ADDON_DEFINES =
	ADDON_CFLAGS = -O3 -Wno-strict-aliasing -DGLM_FORCE_UNRESTRICTED_GENTYPE

	# Exclude includes and source.
	ADDON_SOURCES_EXCLUDE = libs/eigen3/include/%
	ADDON_INCLUDES_EXCLUDE = libs/eigen3/include/%
	ADDON_INCLUDES_EXCLUDE += libs/ceres/include/%

	# Manually add the includes and source.
	ADDON_INCLUDES = libs/eigen3/include
	ADDON_INCLUDES += libs/ceres/include
	ADDON_INCLUDES += src

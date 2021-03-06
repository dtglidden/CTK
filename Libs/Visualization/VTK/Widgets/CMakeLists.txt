project(CTKVisualizationVTKWidgets)

#
# See CTK/CMake/ctkMacroBuildLib.cmake for details
#

set(KIT_export_directive "CTK_VISUALIZATION_VTK_WIDGETS_EXPORT")

#
# Add the libraries QtTesting
#
if(CTK_USE_QTTESTING)
  find_package(QtTesting REQUIRED)
  include_directories(
    ${QtTesting_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR} # For ctkConfig.h
    )
endif()

# Source files
set(KIT_SRCS
  ctkVTKAbstractMatrixWidget.cpp
  ctkVTKAbstractMatrixWidget.h
  ctkVTKAbstractMatrixWidget_p.h
  ctkVTKAbstractView.cpp
  ctkVTKAbstractView.h
  ctkVTKAbstractView_p.h
  ctkVTKDataSetModel.cpp
  ctkVTKDataSetModel.h
  ctkVTKDataSetArrayComboBox.cpp
  ctkVTKDataSetArrayComboBox.h
  ctkVTKMagnifyView.cpp
  ctkVTKMagnifyView.h
  ctkVTKMagnifyView_p.h
  ctkVTKMatrixWidget.cpp
  ctkVTKMatrixWidget.h
  ctkVTKPropertyWidget.cpp
  ctkVTKPropertyWidget.h
  ctkVTKRenderView.cpp
  ctkVTKRenderView.h
  ctkVTKRenderView_p.h
  ctkVTKScalarBarWidget.cpp
  ctkVTKScalarBarWidget.h
  ctkVTKScalarsToColorsUtils.cpp
  ctkVTKScalarsToColorsUtils.h
  ctkVTKSliceView.cpp
  ctkVTKSliceView.h
  ctkVTKSliceView_p.h
  ctkVTKSurfaceMaterialPropertyWidget.cpp
  ctkVTKSurfaceMaterialPropertyWidget.h
  ctkVTKTextPropertyWidget.cpp
  ctkVTKTextPropertyWidget.h
  ctkVTKThresholdWidget.cpp
  ctkVTKThresholdWidget.h
  ctkVTKThumbnailView.cpp
  ctkVTKThumbnailView.h
  ctkVTKWidgetsUtils.cpp
  ctkVTKWidgetsUtils.h
  )

# Headers that should run through moc
set(KIT_MOC_SRCS
  ctkVTKAbstractMatrixWidget_p.h
  ctkVTKAbstractView.h
  ctkVTKAbstractView_p.h
  ctkVTKDataSetArrayComboBox.h
  ctkVTKDataSetModel.h
  ctkVTKMagnifyView.h
  ctkVTKMagnifyView_p.h
  ctkVTKMatrixWidget.h
  ctkVTKPropertyWidget.h
  ctkVTKRenderView.h
  ctkVTKRenderView_p.h
  ctkVTKScalarBarWidget.h
  ctkVTKSliceView.h
  ctkVTKSliceView_p.h
  ctkVTKSurfaceMaterialPropertyWidget.h
  ctkVTKTextPropertyWidget.h
  ctkVTKThresholdWidget.h
  ctkVTKThumbnailView.h
  )

# UI files
set(KIT_UI_FORMS
  Resources/UI/ctkVTKScalarBarWidget.ui
  Resources/UI/ctkVTKTextPropertyWidget.ui
  Resources/UI/ctkVTKPropertyWidget.ui
  Resources/UI/ctkVTKThresholdWidget.ui
)

# Resources
set(KIT_resources
  Resources/ctkVTKWidgets.qrc
)

# Set VTK_LIBRARIES variable
set(VTK_LIBRARIES
  QVTK
  )

if(CTK_LIB_Visualization/VTK/Widgets_USE_TRANSFER_FUNCTION_CHARTS)
  set(CTK_USE_CHARTS 1)
endif()

IF (${CTK_USE_CHARTS})
  set(KIT_SRCS
      ctkVTKChartView.cpp
      ctkVTKChartView.h
      ctkVTKVolumePropertyWidget.cpp
      ctkVTKVolumePropertyWidget.h
      ctkVTKScalarsToColorsView.cpp
      ctkVTKScalarsToColorsView.h
      ctkVTKScalarsToColorsWidget.cpp
      ctkVTKScalarsToColorsWidget.h
      ${KIT_SRCS})
  set(KIT_MOC_SRCS
      ctkVTKChartView.h
      ctkVTKVolumePropertyWidget.h
      ctkVTKScalarsToColorsView.h
      ctkVTKScalarsToColorsWidget.h
      ${KIT_MOC_SRCS})
  set(KIT_UI_FORMS
      Resources/UI/ctkVTKVolumePropertyWidget.ui
      Resources/UI/ctkVTKScalarsToColorsWidget.ui
      ${KIT_UI_FORMS})
  set(VTK_LIBRARIES
      vtkCharts
      ${VTK_LIBRARIES})
  add_definitions(-DCTK_USE_CHARTS)
endif()

# Target libraries - See CMake/ctkFunctionGetTargetLibraries.cmake
# The following macro will read the target libraries from the file 'target_libraries.cmake'
ctkFunctionGetTargetLibraries(KIT_target_libraries)

# If we use QtTessting, we add all the dependencies
if(CTK_USE_QTTESTING)
  list(APPEND KIT_SRCS
    ctkVTKRenderViewEventPlayer.cpp
    ctkVTKRenderViewEventPlayer.h
    ctkVTKRenderViewEventTranslator.cpp
    ctkVTKRenderViewEventTranslator.h
    )
  list(APPEND KIT_MOC_SRCS
    ctkVTKRenderViewEventPlayer.h
    ctkVTKRenderViewEventTranslator.h
)
  list(APPEND KIT_target_libraries QtTesting)
endif()


ctkMacroBuildLib(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${KIT_export_directive}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_FORMS ${KIT_UI_FORMS}
  TARGET_LIBRARIES ${KIT_target_libraries}
  RESOURCES ${KIT_resources}
  LIBRARY_TYPE ${CTK_LIBRARY_MODE}
  )

if(CTK_WRAP_PYTHONQT_LIGHT)
  ctkMacroBuildLibWrapper(
    TARGET ${PROJECT_NAME}
    SRCS ${KIT_SRCS}
    WRAPPER_LIBRARY_TYPE ${CTK_LIBRARY_MODE}
    )
endif()

# Plugins
if(CTK_BUILD_QTDESIGNER_PLUGINS)
  add_subdirectory(Plugins)
endif()

# Testing
if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

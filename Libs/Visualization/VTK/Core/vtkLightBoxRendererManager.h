#ifndef __vtkLightBoxRendererManager_h
#define __vtkLightBoxRendererManager_h

#include <vtkObject.h>

#include "CTKVisualizationVTKCoreExport.h"

class vtkRenderWindow;
class vtkRenderer;
class vtkImageData;
class vtkCamera;

class CTK_VISUALIZATION_VTK_CORE_EXPORT vtkLightBoxRendererManager : public vtkObject
{
  vtkTypeRevisionMacro(vtkLightBoxRendererManager,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);
  static vtkLightBoxRendererManager *New();

  void Initialize(vtkRenderWindow* renderWindow);

  bool IsInitialized();

  /// Get associated RenderWindow
  vtkRenderWindow* GetRenderWindow();

  /// Set image data
  void SetImageData(vtkImageData* newImageData);

  /// Get active camera
  /// Note that the same camera is used with all the renderWindowItem
  vtkCamera* GetActiveCamera();

  /// Set actice camera
  void SetActiveCamera(vtkCamera* newActiveCamera);

  /// Reset cameras associated with all renderWindowItem
  void ResetCamera();
  
  /// Return number of underlying render window Item
  /// \note In the current implementation
  int GetRenderWindowItemCount();
  
  /// Get a reference to the associated vtkRenderer(s) identified by its \a id
  vtkRenderer* GetRenderer(int id);
  
  /// Get a reference to the associated vtkRenderer(s) given its position in the grid
  /// \sa GetRenderer(int)
  vtkRenderer* GetRenderer(int rowId, int columnId);
  
  /// The layout type determines how the image slices should be displayed
  /// within the different render view items.
  /// \sa SetRenderWindowLayout() GetRenderWindowLayoutType()
  enum RenderWindowLayoutType{LeftRightTopBottom = 0, LeftRightBottomTop};
  
  /// Get current layout type
  int GetRenderWindowLayoutType() const;
  
  /// Set current \a layoutType
  void SetRenderWindowLayoutType(int layoutType);

  /// Split the current vtkRenderWindow in \a rowCount per \a columnCount grid
  void SetRenderWindowLayout(int rowCount, int columnCount);

  /// Highlight / Unhighlight a render view item given its \a id
  void SetHighlightedById(int id, bool highlighted);

  ///  Highlight / Unhighlight a render view item given its position in the grid
  /// \sa setHighlighted(int, bool)
  void SetHighlighted(int rowId, int columnId, bool highlighted);
  
  /// Convenient function allowing to compute the renderWindowItemId
  /// given \a rowId and \a columnId.
  /// The following formula is used: ColumnCount * rowId + columnId
  /// \note The \a rowCount and \a columnCount correspond to the one set
  /// using setRenderWindowLayout.
  inline int ComputeRenderWindowItemId(int rowId, int columnId);
  
  /// Set corner annotation \a text
  void SetCornerAnnotationText(const std::string& text);

  /// Get current corner annotation
  const std::string GetCornerAnnotationText()const;

  /// Set background color
  void SetBackgroundColor(const double newBackgroundColor[3]);

  /// Get background color
  double* GetBackgroundColor()const;

  /// Set color level
  void SetColorLevel(double colorLevel);

  /// Get color level
  double GetColorLevel()const;

  /// Set color window
  void SetColorWindow(double colorWindow);

  /// Get color window
  double GetColorWindow()const;

  /// Set color Window and color level
  void SetColorWindowAndLevel(double colorWindow, double colorLevel);
  
protected:

  vtkLightBoxRendererManager();
  ~vtkLightBoxRendererManager();
  
private:
  vtkLightBoxRendererManager(const vtkLightBoxRendererManager&); // Not implemented.
  void operator=(const vtkLightBoxRendererManager&);             // Not implemented.
  
//BTX
  class vtkInternal;
  vtkInternal* Internal;
//ETX

};

#endif
